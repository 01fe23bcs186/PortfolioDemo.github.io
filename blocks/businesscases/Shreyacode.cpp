//1a.Unemployment rate calculation using Array
#include <iostream>
#include <algorithm>

using namespace std;

double calculateUnemploymentRate(const int statusList[], int totalPeople) {
    int unemployedPeople = count(statusList, statusList + totalPeople, 0);

    if (totalPeople == 0) {
        return 0.0;
    }

    double unemploymentRate = (static_cast<double>(unemployedPeople) / totalPeople) * 100;
    return unemploymentRate;
}

int main() {
    int totalPeople;

    // Get the number of people from the user
    cout << "Enter the total number of people: ";
    cin >> totalPeople;

    // Create an array to hold the employment status
    int *employmentStatus = new int[totalPeople];

    // Get the employment status from the user
    cout << "Enter the employment status (1 for employed, 0 for unemployed) for each person:\n";
    for (int i = 0; i < totalPeople; ++i) {
        cin >> employmentStatus[i];
    }

    double unemploymentRate = calculateUnemploymentRate(employmentStatus, totalPeople);
    cout << "Unemployment Rate: " << unemploymentRate << "%" << endl;

    // Free the allocated memory
    delete[] employmentStatus;

    return 0;
}

//1b.Unemployment rate calculation using hash table
#include <iostream>
#include <unordered_map>

using namespace std;

double calculateUnemploymentRate(const unordered_map<int, int>& statusMap) {
    int totalPeople = statusMap.size();
    int unemployedPeople = 0;

    for (const auto& pair : statusMap) {
        if (pair.second == 0) {
            unemployedPeople++;
        }
    }

    if (totalPeople == 0) {
        return 0.0;
    }

    double unemploymentRate = (static_cast<double>(unemployedPeople) / totalPeople) * 100;
    return unemploymentRate;
}

int main() {
    unordered_map<int, int> employmentStatus;
    int totalPeople;

    // Get the number of people from the user
    cout << "Enter the total number of people: ";
    cin >> totalPeople;

    // Get the employment status from the user
    cout << "Enter the employment status (1 for employed, 0 for unemployed) for each person:\n";
    for (int i = 0; i < totalPeople; ++i) {
        int id, status;
        cout << "Person " << i + 1 << " - ID: ";
        cin >> id;
        cout << "Employment Status: ";
        cin >> status;
        employmentStatus[id] = status;
    }

    double unemploymentRate = calculateUnemploymentRate(employmentStatus);
    cout << "Unemployment Rate: " << unemploymentRate << "%" << endl;

    return 0;
}

//2a.Job matching code for kmp
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
        bool matchFound = false; // Flag to track if any job matches
        cout << "Job Seeker " << i + 1 << " (" << jobSeekersSkills[i] << ") matches:\n";
        for (int j = 0; j < m; j++) {
            if (KMPSearch(jobSeekersSkills[i], jobRequirements[j])) {
                cout << "  - Job " << j + 1 << " (" << jobRequirements[j] << ")\n";
                matchFound = true; // A match is found
            }
        }
        if (!matchFound) {
            cout << "  No job matches Job Seeker " << i + 1 << "'s skills.\n"; // Display message if no match is found
        }
    }

    return 0;
}
//2b.Job matching code for rabin karp
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
    cout << "Enter number of job seekers: " << endl;
    cin >> n;
    cin.ignore();
    vector<string> jobSeekersSkills(n);
    for (int i = 0; i < n; i++) {
        getline(cin, jobSeekersSkills[i]);
    }

    cout << "Enter number of job requirements: " << endl;
    cin >> m;
    cin.ignore();
    vector<string> jobRequirements(m);
    for (int i = 0; i < m; i++) {
        getline(cin, jobRequirements[i]);
    }

    bool foundMatch = false;
    for (int i = 0; i < n; i++) {
        bool matchFoundForCurrentSeeker = false;
        for (int j = 0; j < m; j++) {
            if (rabinKarp(jobSeekersSkills[i], jobRequirements[j])) {
                cout << "Job Seeker " << i + 1 << " matches Job " << j + 1 << endl;
                matchFoundForCurrentSeeker = true;
                foundMatch = true;
            }
        }
        if (!matchFoundForCurrentSeeker) {
            cout << "Job Seeker " << i + 1 << " has no matching jobs." << endl;
        }
    }

    if (!foundMatch) {
        cout << "No job matches any job seeker's skills." << endl;
    }

    return 0;
}
