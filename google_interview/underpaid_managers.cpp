/**
Given a company tree, calculate how many managers are paid less than
the average salary of their direct and indirect employees?

For example, consider the following:
A($100)
|
+- B($100)
+- C($200)
   |
   +- D($60)

Here there are two managers, A and C. A should be counted since the
average salary of their employees is ($100 + $200 + $60) / 3 = $120,
which is more than $100. C should not be counted because their salary
is $200, which is more than the average salary of their employees,
that is $60.

Follow-up question: Now that you’ve identified underpaid managers, we
want to make sure that they are paid fairly. Given an org tree, find
out the minimum amount of extra salary needed to make sure that no one
is underpaid.

This problem asks you to count managers whose salary is lower than the
average salary of all their direct and indirect subordinates. A natural
solution is a postorder DFS that returns the total salary and employee
count for each subtree, then checks whether the current manager is
underpaid. The follow-up extends this tree aggregation idea to compute
the minimum additional salary needed so that every manager is paid fairly.
 */

#include <iostream>
#include <string>
#include <vector>

// Employee node
struct Employee {
    std::string name;
    double salary;
    sdt::vector<Employee*> subordinates;

    Employee(std::string& n, double s) : name(n), salary(s) {}
};
// Helper struct to pass data up the tree
struct SubtreeInfo {
    double total_salary;
    uint32_t count;
};
// Count underpaid managers
SubtreeInfo dfs_count(Employee* node, uint32_t underpaid_count) {
    if (!node) return {0x0, 0};
    double sub_salary_sum = 0.0;
    uint32_t sub_count = 0;
    // Post-order traversal - process all subordinates first
    for (Employee* sub : node->subordinates) {
        SubtreeInfo info = dfs_count(sub, underpaid_count);
        sub_salary_sum += info.total_salary;
        sub_count += info.count;
    }
    // If the employee has subordinates, they are a manager
    if (sub_count > 0) {
        const double average_salary = sub_salary_sum / sub_count;
        if (node->salary < average_salary) {
            ++underpaid_count;
        }
    }
    // Return the sum and count including the CURRENT node
    return {sub_salary_sum + node->salary, sub_count + 1};
}

uint32_t count_underpaid_managers(Employee* root) {
    uint32_t count = 0;
    dfs_count(root, count);
    return count;
}

SubtreeInfo dfs_extra_salary(Employee* node, double& total_extra_salary) {
    if (!node) return {0.0, 0};

    double sub_salary_sum = 0.0;
    uint32_t sub_count = 0;

    for (Employee* sub : node->subordinates) {
        SubtreeInfo info = dfs_extra_salary(sub, total_extra_salary);
        sub_salary_sum += info.total_salary;
        sub_count += info.count;
    }

    double cur_salary = node->salary;

    if (sub_count > 0) {
        const double average_salary = sub_salary_sum / sub_count;
        if (cur_salary > average_salary) {
            // Calculate how much we need to bump this manager's salary
            const double difference = average_salary - cur_salary;
            total_extra_salary += difference;
        }
    }

    return {sub_salary_sum + current_salary, sub_count + 1};
}

double getMinimumExtraSalary(Employee* root) {
    double totalExtraSalary = 0.0;
    dfsExtraSalary(root, totalExtraSalary);
    return totalExtraSalary;
}

// ==========================================
// Test Driver
// ==========================================
int main() {
    // Constructing the tree from the example:
    //       A($100)
    //       /     \
    //  B($100)   C($200)
    //               |
    //            D($60)

    Employee* A = new Employee("A", 100.0);
    Employee* B = new Employee("B", 100.0);
    Employee* C = new Employee("C", 200.0);
    Employee* D = new Employee("D", 60.0);

    A->subordinates.push_back(B);
    A->subordinates.push_back(C);
    C->subordinates.push_back(D);

    // Test Part 1
    int underpaidCount = countUnderpaidManagers(A);
    cout << "Underpaid Managers Count: " << underpaidCount << "\n"; 
    // Expected: 1 (Manager A)

    // Test Part 2
    double extraSalary = getMinimumExtraSalary(A);
    cout << "Minimum Extra Salary Needed: $" << extraSalary << "\n";
    // Expected: $20 (A's salary needs to go from 100 to 120)

    // Clean up memory
    delete A; delete B; delete C; delete D;

    return 0;
}
