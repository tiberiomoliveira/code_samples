/**
Query: use the query to match the key.
Key: label of each data slice.
Value: the data itself.

1. Compute the similarity between query and key.

query [1 2 3 ...]
key [4 5 6 ...]
score

The problem describes a simple query-key-value setup and asks for the similarity
score between the query and the key. In practice, this points to a vector-style
matching step, often using dot product or another similarity metric to measure
how well the query aligns with each key. The score is then used as the basis for
selecting or weighting the associated value. The key idea is to identify the
similarity computation correctly from the prompt and handle the sequence alignment
carefully.
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <stdexcept>
#include <string>

// A struct to represent our Key-Value pair
struct DataSlice {
    std::string label;           // The Key's label or identifier
    std::vector<double> key;     // The Key vector
    std::string value;           // The Value (data itself)
};

class QKVSearch {
public:
    /**
     * Computes the dot product similarity score between a query and a key.
     */
    static double computeSimilarity(const std::vector<double>& query, const std::vector<double>& key) {
        if (query.size() != key.size()) {
            throw std::invalid_argument("Dimension mismatch: Query and Key must be the same size.");
        }
        
        // std::inner_product efficiently computes the dot product of two ranges.
        // It multiplies corresponding elements and sums them up starting from an initial sum of 0.0.
        return std::inner_product(query.begin(), query.end(), key.begin(), 0.0);
    }

    /**
     * Finds the best matching value for a given query from a list of data slices.
     */
    static std::string retrieveBestValue(const std::vector<double>& query, const std::vector<DataSlice>& database) {
        if (database.empty()) {
            return "Database is empty.";
        }

        double max_score = -std::numeric_limits<double>::infinity();
        std::string best_value = "";
        std::string best_label = "";

        for (const auto& slice : database) {
            double current_score = computeSimilarity(query, slice.key);
            
            // Keep track of the highest scoring key-value pair
            if (current_score > max_score) {
                max_score = current_score;
                best_value = slice.value;
                best_label = slice.label;
            }
        }

        std::cout << "Matched Label: " << best_label << " with Score: " << max_score << "\n";
        return best_value;
    }
};

// Demonstration
int main() {
    // Define a query vector
    std::vector<double> query = {1.0, 2.0, 3.0};

    // Define our database of Key-Value data slices
    std::vector<DataSlice> database = {
        {"Slice A", {4.0, 5.0, 6.0}, "Data for Slice A (e.g., Image A pixels or Text A)"},
        {"Slice B", {1.0, 2.0, 3.0}, "Data for Slice B (e.g., Image B pixels or Text B)"}, // Exact match
        {"Slice C", {-1.0, 0.0, 1.0}, "Data for Slice C (e.g., Image C pixels or Text C)"}
    };

    try {
        std::string result = QKVSearch::retrieveBestValue(query, database);
        std::cout << "Retrieved Value: " << result << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
