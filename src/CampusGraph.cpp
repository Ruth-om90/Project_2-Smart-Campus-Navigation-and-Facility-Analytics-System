
#include "../include/CampusGraph.h"
#include <algorithm>
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

void CampusGraph::shortestPath(const string& start, const string& destination) const {
    int startIndex = findBuilding(start);
    int destinationIndex = findBuilding(destination);

    if (startIndex == -1 || destinationIndex == -1) {
        cout << "Building not found." << endl;
        return;
    }

    if (startIndex == destinationIndex) {
        cout << "\nShortest Path:\n";
        cout << buildings[startIndex] << endl;
        cout << "Total connections: 0" << endl;
        return;
    }

    vector<bool> visited(buildings.size(), false);
    vector<int> parent(buildings.size(), -1);
    queue<int> q;

    visited[startIndex] = true;
    q.push(startIndex);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        if (current == destinationIndex) {
            break;
        }

        for (int neighbor : adjacencyList[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = current;
                q.push(neighbor);
            }
        }
    }

    if (!visited[destinationIndex]) {
        cout << "No path exists between these buildings." << endl;
        return;
    }

    vector<int> path;
    int current = destinationIndex;

    while (current != -1) {
        path.push_back(current);
        current = parent[current];
    }

    reverse(path.begin(), path.end());

    cout << "\nShortest Path:\n";

    for (int index : path) {
        cout << buildings[index] << endl;
    }

    cout << "Total connections: " << path.size() - 1 << endl;
}

void CampusGraph::connectivityReport() const {
    cout << "\nCampus Connectivity Report:\n";

    cout << "Total Buildings: " << getBuildingCount() << endl;
    cout << "Total Connections: " << getConnectionCount() << endl;

    int highestDegree = 0;

    for (int i = 0; i < buildings.size(); i++) {
        if (adjacencyList[i].size() > highestDegree) {
            highestDegree = adjacencyList[i].size();
        }
    }

    cout << "\nHighest Degree Buildings:\n";

    for (int i = 0; i < buildings.size(); i++) {
        if (adjacencyList[i].size() == highestDegree) {
            cout << buildings[i] << " (" << highestDegree << ")" << endl;
        }
    }

    cout << "\nIsolated Buildings:\n";

    bool foundIsolated = false;

    for (int i = 0; i < buildings.size(); i++) {
        if (adjacencyList[i].empty()) {
            cout << buildings[i] << endl;
            foundIsolated = true;
        }
    }

    if (!foundIsolated) {
        cout << "None" << endl;
    }

    vector<bool> visited(buildings.size(), false);
    int components = 0;

    for (int i = 0; i < buildings.size(); i++) {
        if (!visited[i]) {
            components++;

            queue<int> q;
            q.push(i);
            visited[i] = true;

            while (!q.empty()) {
                int current = q.front();
                q.pop();

                for (int neighbor : adjacencyList[current]) {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
        }
    }

    cout << "\nConnected Components: " << components << endl;
}
