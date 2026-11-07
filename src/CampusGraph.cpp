
#include "../include/CampusGraph.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <cctype>

using namespace std;

CampusGraph::CampusGraph() {
}

int CampusGraph::findBuilding(const string& name) const {
    auto normalize = [](const string& text) {
        size_t start = text.find_first_not_of(" \t\r\n");
        if (start == string::npos) {
            return string("");
        }

        size_t end = text.find_last_not_of(" \t\r\n");
        string result = text.substr(start, end - start + 1);

        for (char& c : result) {
            c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
        }

        return result;
    };

    string target = normalize(name);

    for (int i = 0; i < static_cast<int>(buildings.size()); i++) {
        if (normalize(buildings[i]) == target) {
            return i;
        }
    }

    return -1;
}

void CampusGraph::addBuilding(const string& name) {
    if (name.empty() || findBuilding(name) != -1) {
        return;
    }

    buildings.push_back(name);
    adjacencyList.push_back(vector<int>());
}

void CampusGraph::addConnection(const string& buildingA,
                                const string& buildingB) {
    int a = findBuilding(buildingA);
    int b = findBuilding(buildingB);

    if (a == -1 || b == -1 || a == b) {
        return;
    }

    for (int neighbor : adjacencyList[a]) {
        if (neighbor == b) {
            return;
        }
    }

    adjacencyList[a].push_back(b);
    adjacencyList[b].push_back(a);
}

void CampusGraph::displayGraph() const {
    cout << "\nCampus Graph:\n";

    for (int i = 0; i < static_cast<int>(buildings.size()); i++) {
        cout << buildings[i] << ": ";

        if (adjacencyList[i].empty()) {
            cout << "(no connections)";
        } else {
            for (int j = 0;
                 j < static_cast<int>(adjacencyList[i].size()); j++) {
                if (j > 0) {
                    cout << ", ";
                }
                cout << buildings[adjacencyList[i][j]];
            }
        }

        cout << endl;
    }
}

void CampusGraph::BFS(const string& start) const {
    int startIndex = findBuilding(start);

    if (startIndex == -1) {
        cout << "Building not found." << endl;
        return;
    }

    vector<bool> visited(buildings.size(), false);
    vector<int> distance(buildings.size(), -1);
    queue<int> q;

    visited[startIndex] = true;
    distance[startIndex] = 0;
    q.push(startIndex);

    int reachableCount = 0;

    cout << "\nBFS starting from "
         << buildings[startIndex] << ":\n";

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        cout << buildings[current]
             << " - Distance: "
             << distance[current] << endl;

        reachableCount++;

        for (int neighbor : adjacencyList[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                distance[neighbor] = distance[current] + 1;
                q.push(neighbor);
            }
        }
    }

    cout << "Reachable buildings: "
         << reachableCount << " / "
         << buildings.size() << endl;
}

void CampusGraph::DFS(const string& start) const {
    int startIndex = findBuilding(start);

    if (startIndex == -1) {
        cout << "Building not found." << endl;
        return;
    }

    vector<bool> visited(buildings.size(), false);
    stack<int> s;

    s.push(startIndex);

    cout << "\nDFS starting from "
         << buildings[startIndex] << ":\n";

    while (!s.empty()) {
        int current = s.top();
        s.pop();

        if (visited[current]) {
            continue;
        }

        visited[current] = true;
        cout << buildings[current] << endl;

        for (int i = static_cast<int>(adjacencyList[current].size()) - 1;
             i >= 0; i--) {
            int neighbor = adjacencyList[current][i];

            if (!visited[neighbor]) {
                s.push(neighbor);
            }
        }
    }
}

int CampusGraph::getBuildingCount() const {
    return buildings.size();
}

int CampusGraph::getConnectionCount() const {
    int total = 0;

    for (int i = 0; i < adjacencyList.size(); i++) {
        total += adjacencyList[i].size();
    }

    return total / 2;
}

void CampusGraph::displayDegrees() const {
    cout << "\nBuilding Degrees:\n";

    for (int i = 0; i < buildings.size(); i++) {
        cout << buildings[i] << ": "
             << adjacencyList[i].size() << endl;
    }
}

bool CampusGraph::loadBuildings(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Could not open " << filename << endl;
        return false;
    }

    string name;

    while (getline(file, name)) {
        if (name.empty() || name[0] == '#') {
            continue;
        }

        addBuilding(name);
    }

    file.close();
    return true;
}

bool CampusGraph::loadConnections(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Could not open " << filename << endl;
        return false;
    }

    string line;

    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        stringstream ss(line);
        string buildingA;
        string buildingB;

        getline(ss, buildingA, '|');
        getline(ss, buildingB);

        addConnection(buildingA, buildingB);
    }

    file.close();
    return true;
}
