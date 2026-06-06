/**
Here we have an event log file produced by a Friends service, e.g.:

1648385616 Alice and Bob become friends
1648305678 Charlie and Dan become friends
1648366171 Bob and Charlie become friends
1648366237 Alice and Erin become friends
...
Given a list of all users and the logs above, implement a function that returns the earliest timestamp at which every user became reachable from every other user via friendship connections (i.e., when the social graph first became fully connected).
If the graph never becomes fully connected, return an appropriate sentinel (e.g., None / -1).

You may assume a helper that parses the log:

def log_parser(log_file: str) -> Generator[tuple[int, str, str, str], None, None]:
    with open(log_file, 'r') as f:
        for line in f:
            columns = line.split(' ')
            timestamp = int(columns[0])
            src = columns[1]
            dst = columns[3]
            event = columns[4]         # e.g., "friends"
            yield (timestamp, src, dst, event)
Constraints / notes:

Each line indicates an undirected friendship between two users at the given timestamp.
Timestamps may be out of order in the file; your algorithm should use chronological order.
Input includes the full set of users (some may appear only in the users list before any edges).
Return a single integer timestamp (or sentinel), not the sequence of edges.
From friendship logs, find the earliest timestamp when the undirected graph over all users becomes connected.
If it never becomes connected, return a sentinel.
Use Union-Find: sort edges by timestamp, union endpoints, track component count; when it hits 1, return that timestamp.
 */

typedef struct
{
    int timestamp;
    int u;
    int v;
} Edge;

// Find with path compression
int find(int parent[], int x)
{
    if (parent[x] != x)
        parent[x] = find(parent, parent[x]);

    return parent[x];
}

// Union by rank
int unite(int parent[], int rank[],
          int a, int b)
{
    const int rootA = find(parent, a);
    const int rootB = find(parent, b);

    if (rootA == rootB)
        return 0; // already connected

    if (rank[rootA] < rank[rootB])
    {
        parent[rootA] = rootB;
    }
    else if (rank[rootA] > rank[rootB])
    {
        parent[rootB] = rootA;
    }
    else
    {
        parent[rootB] = rootA;
        rank[rootA]++;
    }

    return 1; // merge occurred
}

// Sorting events
int compareEdges(const void *a, const void *b)
{
    const Edge *e1 = a;
    const Edge *e2 = b;

    if (e1->timestamp < e2->timestamp)
        return -1;

    if (e1->timestamp > e2->timestamp)
        return 1;

    return 0;
}

// Main algo
int earliestFullyConnected(
        int numUsers,
        Edge edges[],
        int numEdges)
{
    qsort(edges,
          numEdges,
          sizeof(Edge),
          compareEdges);

    int *parent =
        malloc(numUsers * sizeof(int));

    int *rank =
        calloc(numUsers, sizeof(int));

    for (int i = 0; i < numUsers; i++)
        parent[i] = i;

    int components = numUsers;

    for (int i = 0; i < numEdges; i++)
    {
        if (unite(parent,
                  rank,
                  edges[i].u,
                  edges[i].v))
        {
            components--;

            if (components == 1)
            {
                int answer =
                    edges[i].timestamp;

                free(parent);
                free(rank);

                return answer;
            }
        }
    }

    free(parent);
    free(rank);

    return -1;
}