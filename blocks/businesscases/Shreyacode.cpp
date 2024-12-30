//Unemployment rate calculation using dijkstra's
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

struct Edge {
    int to;
    int weight;
};

void dijkstra(int nodes, vector<vector<Edge>> &graph, int start, vector<int> &dist) {
    dist.assign(nodes, INT_MAX);
    dist[start] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        int currentDist = pq.top().first;
        int currentNode = pq.top().second;
        pq.pop();

        if (currentDist > dist[currentNode]) continue;

        for (const Edge &edge : graph[currentNode]) {
            int newDist = currentDist + edge.weight;
            if (newDist < dist[edge.to]) {
                dist[edge.to] = newDist;
                pq.push({newDist, edge.to});
            }
        }
    }
}

int main() {
    int nodes, edges, start;
    cin >> nodes >> edges;

    vector<vector<Edge>> graph(nodes);
    for (int i = 0; i < edges; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
        graph[v].push_back({u, w}); // Assuming undirected graph
    }

    cin >> start;

    vector<int> dist;
    dijkstra(nodes, graph, start, dist);

    cout << "Shortest distances from node " << start << ":\n";
    for (int i = 0; i < nodes; i++) {
        cout << "To node " << i << ": " << (dist[i] == INT_MAX ? -1 : dist[i]) << endl;
    }

    return 0;
}
//Unemployment rate calculation using bellmanford algorithm
#include <iostream>
#include <vector>
#include <climits>
using namespace std;

struct Edge {
    int from, to, weight;
};

bool bellmanFord(int nodes, int edges, vector<Edge> &graph, int start, vector<int> &dist) {
    dist.assign(nodes, INT_MAX);
    dist[start] = 0;

    for (int i = 1; i < nodes; i++) {
        for (const Edge &edge : graph) {
            if (dist[edge.from] != INT_MAX && dist[edge.from] + edge.weight < dist[edge.to]) {
                dist[edge.to] = dist[edge.from] + edge.weight;
            }
        }
    }

    // Check for negative-weight cycles
    for (const Edge &edge : graph) {
        if (dist[edge.from] != INT_MAX && dist[edge.from] + edge.weight < dist[edge.to]) {
            return false; // Negative cycle detected
        }
    }

    return true;
}

int main() {
    int nodes, edges, start;
    cin >> nodes >> edges;

    vector<Edge> graph(edges);
    for (int i = 0; i < edges; i++) {
        cin >> graph[i].from >> graph[i].to >> graph[i].weight;
    }

    cin >> start;

    vector<int> dist;
    if (bellmanFord(nodes, edges, graph, start, dist)) {
        cout << "Shortest distances from node " << start << ":\n";
        for (int i = 0; i < nodes; i++) {
            cout << "To node " << i << ": " << (dist[i] == INT_MAX ? -1 : dist[i]) << endl;
        }
    } else {
        cout << "Negative-weight cycle detected.\n";
    }

    return 0;
}
//Job matching code for kmp
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Function to compute the LPS (Longest Prefix Suffix) array
void computeLPSArray(const string &pattern, vector<int> &lps) {
    int m = pattern.size();
    lps[0] = 0; // LPS[0] is always 0
    int len = 0, i = 1;

    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

// KMP Search function
bool KMPSearch(const string &text, const string &pattern) {
    int n = text.size();
    int m = pattern.size();

    vector<int> lps(m, 0);
    computeLPSArray(pattern, lps);

    int i = 0, j = 0; // i for text, j for pattern
    while (i < n) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }

        if (j == m) {
            return true; // Pattern found
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    return false; // Pattern not found
}

int main() {
    int n, m;
    cout << "Enter the number of job seekers: ";
    cin >> n;
    cin.ignore();

    vector<string> jobSeekersSkills(n);
    cout << "Enter skills of job seekers (one per line):\n";
    for (int i = 0; i < n; i++) {
        getline(cin, jobSeekersSkills[i]);
    }

    cout << "Enter the number of job requirements: ";
    cin >> m;
    cin.ignore();

    vector<string> jobRequirements(m);
    cout << "Enter job requirements (one per line):\n";
    for (int i = 0; i < m; i++) {
        getline(cin, jobRequirements[i]);
    }

    cout << "\nMatching Results:\n";
    for (int i = 0; i < n; i++) {
        cout << "Job Seeker " << i + 1 << " (" << jobSeekersSkills[i] << ") matches:\n";
        for (int j = 0; j < m; j++) {
            if (KMPSearch(jobSeekersSkills[i], jobRequirements[j])) {
                cout << "  - Job " << j + 1 << " (" << jobRequirements[j] << ")\n";
            }
        }
    }

    return 0;
}
//Job matching code for rabin karp
#include <iostream>
#include <vector>
#include <string>
using namespace std;

const int PRIME = 101;

long long calculateHash(const string &str, int length) {
    long long hash = 0;
    for (int i = 0; i < length; i++) {
        hash = (hash * 256 + str[i]) % PRIME;
    }
    return hash;
}

bool rabinKarp(const string &text, const string &pattern) {
    int n = text.size();
    int m = pattern.size();

    if (m > n) return false;

    long long patternHash = calculateHash(pattern, m);
    long long textHash = calculateHash(text, m);

    long long highestPower = 1;
    for (int i = 0; i < m - 1; i++) {
        highestPower = (highestPower * 256) % PRIME;
    }

    for (int i = 0; i <= n - m; i++) {
        if (patternHash == textHash && text.substr(i, m) == pattern) {
            return true;
        }

        if (i < n - m) {
            textHash = (256 * (textHash - text[i] * highestPower) + text[i + m]) % PRIME;
            if (textHash < 0) textHash += PRIME;
        }
    }

    return false;
}

int main() {
    int n, m;
    cin >> n;
    cin.ignore();
    vector<string> jobSeekersSkills(n);
    for (int i = 0; i < n; i++) {
        getline(cin, jobSeekersSkills[i]);
    }

    cin >> m;
    cin.ignore();
    vector<string> jobRequirements(m);
    for (int i = 0; i < m; i++) {
        getline(cin, jobRequirements[i]);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (rabinKarp(jobSeekersSkills[i], jobRequirements[j])) {
                cout << "Job Seeker " << i + 1 << " matches Job " << j + 1 << endl;
            }
        }
    }

    return 0;
}


