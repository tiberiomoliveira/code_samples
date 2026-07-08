/**
You are given a list of log messages, each associated with some source
file that emitted them.

struct LogMessage {
    string source_file;
    string message;
};

Your goal is to truncate the list down to size max_log_messages. That is,
you want to return a subset of the original list. However, you want to
perform the truncation in a fair way.

For the sake of this problem, our "fair" truncation algorithm is as follows:
Let X be the max log messages maintained per source file.

If the source file has emitted more than X messages, truncate the log messages
for that source file to X messages.
If the source file has emitted X or fewer messages, do not truncate the log
messages for that source file.

This problem asks you to fairly truncate a list of log messages by source file.
The key idea is to count how many messages each source file emitted, then cap
each source at X messages: keep only the first X messages for sources with more
than X entries, and keep all messages for sources with X or fewer entries.
A hash map for counting and a stable pass to rebuild the output are the main tools.
 */

struct LogMessage {
    std::string source_file;
    std::string message;
};

std::vector<LogMessage> truncate_logs(
        const std::vector<LogMessage>& logs,
        uint32_t max_log_msgs) {
    // If the total logs are already within the limit, return them as-is
    if (logs.size() <= max_log_msgs) return logs;
    // Count the occurrences of each source file
    std::unordered_map<std::string, uint32_t> msg_counts;
    for (const auto& log : logs) {
        ++msg_counts[log.source_file];
    }
    // Binary search to find the optimal baseline threshold X
    uint32_t low = 0;
    uint32_t high = max_log_msgs;
    uint32_t best_X = 0;
    while (low <= high) {
        const uint32_t mid = low + (high - low) / 2;
        uint32_t cur_total = 0;
        // Calculate how many logs we would keep if we capped at 'mid'
        for (const auto& pair : msg_counts) {
            cur_total += std::min(pair.second, mid);
        }
        if (cur_total <= max_log_msgs) {
            // This X is valid, but we might be able to go higher
            best_X = mid;
            low = mid + 1;
        } else {
            // This X exceeds our max, scale down
            high = mid - 1;
        }
    }
    // Calculate exact quotas per file based on 'best_X'
    std::unordered_map<std::string, uint32_t> quotas;
    uint32_t total_assigned = 0;
    for (const auto& pair : msg_counts) {
        quotas[pair.first] = std::min(pair.second, best_X);
        total_assigned += quotas[pair.first];
    }
    // Distribute any remainder to strictly 'hit max_log_msgs'
    // We only give remainders to files that actually have more
    // logs than 'best_X'
    uint32_t remainder = max_log_msgs - total_assigned;
    for (const auto& pair : msg_counts) {
        if (remainder == 0) break;
        if (pair.second > best_X) {
            ++quotas[pair.first];
            --remainder;
        }
    }
    // Stable pass to rebuild the output list
    std::unordered_map<std::string, uint32_t> cur_counts;
    std::vector<LogMessage> result;
    result.reserve(max_log_msgs);
    for (const auto& log : logs) {
        if (cur_counts[log.source_file] < quotas[log.source_file]) {
            result.push_back(log);
            ++cur_counts[log.source_file];
        }
    }
    return results;
}
