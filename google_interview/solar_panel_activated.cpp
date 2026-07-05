/**
10 solar panel activated
11 low battery warning
12 tire one: low air pressure
13 solar panel activated
14 low battery warning
21 solar panel activated
35 solar panel activated

This problem presents a time-ordered stream of alert events such
as "solar panel activated", "low battery warning", and
"tire one: low air pressure". The key idea is to process the
events in sequence and use a simple data structure, often a
hash map or array-based grouping, to track repeated alerts,
counts, or transitions. Depending on the exact task, sorting
by timestamp and scanning once is usually enough to extract the 
final answer.
 */

class AlertTracker {
private:
    // Maps the alert msgs to a list of timestamps when it occurred
    std::unordered_map<std::string, std::vector<uint32_t>> alert_history;
public:
    void process_event(const std::string& event) {
        if (event.empty()) return;
        std::stringstream ss(event);
        uint32_t timestamp;
        std:string msg;
        // Extract the timestamp
        if (ss >> timestamp) {
            // Extract the alert message
            std::getline(ss, msg);
            // Trim the leading whitespace
            const size_t first_char = msg.find_first_not_off(" \t");
            if (first_char != std::string::npos) {
                msg = msg.substr(first_char);
            }
            // Record the timestamp under this specific alert
            alert_history[msg].push_back(timestamp);
        }
    }

    // Print a summary of counts and occurrences
    void print_summary() const {
        std::cout << "--- System Alert Summary ---\n\n";
        for (const auto& pair : alert_history) {
            const std::string& alert_name = pair.first;
            const std::vector<int>& timestamps = pair.second;

            std::cout << "Alert: [" << alert_name << "]\n";
            std::cout << "Count: " << timestamps.size() << "\n";
            std::cout << "Timestamps: ";
            
            for (size_t i = 0; i < timestamps.size(); ++i) {
                std::cout << timestamps[i] << (i < timestamps.size() - 1 ? ", " : "");
            }
            std::cout << "\n\n";
        }
    }
};

// Demonstration using the provided example data
int main() {
    AlertTracker tracker;

    // Simulating an incoming stream of alerts
    std::vector<std::string> stream = {
        "10 solar panel activated",
        "11 low battery warning",
        "12 tire one: low air pressure",
        "13 solar panel activated",
        "14 low battery warning",
        "21 solar panel activated",
        "35 solar panel activated"
    };

    // Process the stream
    for (const std::string& log : stream) {
        tracker.process_event(log);
    }

    // Output the results
    tracker.print_summary();

    return 0;
}
