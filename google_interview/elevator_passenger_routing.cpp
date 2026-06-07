/**
You are given a 2-D grid representing two elevators (left is elevator A, right is elevator B).
Each cell may contain a digit representing the number of people waiting on that floor for that elevator.

Example grid:
[A] [B]

|1 2 3|
|4 5 6|
|7 8 #|
[C] [D]

Legend:
- Column A = left elevator
- Column B = right elevator
- Numbers = number of people waiting
- # = no service / broken panel
- Floors go from bottom to top

You are also given a query describing a request:
7#3   → going to the 7th floor with 3 people

Task
Given the grid and a request in the format:
<target-floor>#<people-count>

Write a function that determines:
- Which elevator (A or B) should serve the request
- Whether that elevator has enough capacity
- Return the total number of people currently waiting along the path to the target floor

You may assume:
- Floor numbers correspond to rows in the grid
- Elevators serve straight vertical paths
- If a floor contains #, that elevator cannot serve that floor
- If both elevators can serve the floor, choose the one with fewer total waiting people

Given a grid where each column represents an elevator and each cell shows people waiting (or # for blocked floors), determine:
- Which elevator can reach the target floor
- Total number of people waiting on its path 
- If both can reach, choose the one with fewer total waiting people
- The request format F#N means N people want to go to floor F.
 */

#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Result
{
    char elevator;
    bool canServe;
    bool enoughCapacity;
    int waitingPeople;
};

int pathLoad(const vector<vector<string>>& grid,
             int elevator,
             int targetFloor)
{
    // check if the target floor is blocked
    if (grid[targetFloor][elevator] == "#")
        return -1; // blocked

    int load = 0;

    for (int floor = 0; floor <= targetFloor; ++floor)
    {
        // skip floor if elevator doesn't serve this floor
        if (grid[floor][elevator] == "#")
            continue;
        
        load += stoi(grid[floor][elevator]);
    }

    return load;
}

Result chooseElevator(
    const vector<vector<string>>& grid,
    const string& request,
    int capacity)
{
    const size_t pos = request.find('#');
    const int targetFloor = stoi(request.substr(0, pos));
    const int groupSize = stoi(request.substr(pos + 1));
    const int floorIndex = targetFloor - 1;

    // Elevator A
    const int loadA = pathLoad(grid, 0, floorIndex);
    const bool validA = (loadA != -1);

    // Elevator B
    const int loadB = pathLoad(grid, 1, floorIndex);
    const bool validB = (loadB != -1);

    Result res;

    if (!validA && !validB)
    {
        res.canServe = false;
        return res;
    }

    if (validA && (!validB || loadA <= loadB))
    {
        res.elevator = 'A';
        res.waitingPeople = loadA;
    }
    else
    {
        res.elevator = 'B';
        res.waitingPeople = loadB;
    }

    res.canServe = true;
    res.enoughCapacity = (capacity - res.waitingPeople >= groupSize);

    return res;
}
