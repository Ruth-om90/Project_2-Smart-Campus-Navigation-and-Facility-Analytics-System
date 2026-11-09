
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "CampusGraph.h"
#include "Facility.h"
#include "FacilityHashTable.h"
#include "DepartmentTree.h"
#include "Menu.h"

using namespace std;

int main() {
    CampusGraph graph;
    FacilityHashTable facilities;
    DepartmentTree departments;

    if (!graph.loadBuildings("data/buildings.txt")) {
        cout << "Error loading buildings." << endl;
        return 1;
    }

    if (!graph.loadConnections("data/connections.txt")) {
        cout << "Error loading connections." << endl;
        return 1;
    }

    vector<string> buildingNames;
    ifstream buildingFile("data/buildings.txt");
    string buildingName;

    if (!buildingFile) {
        cout << "Error opening building file." << endl;
        return 1;
    }

    while (getline(buildingFile, buildingName)) {
        buildingName = trimWhitespace(buildingName);

        if (!buildingName.empty() && buildingName[0] != '#') {
            buildingNames.push_back(buildingName);
        }
    }

    FacilityLoadSummary summary = loadFacilities(
        "data/facilities.txt",
        buildingNames,
        facilities,
        cout
    );

    cout << "Facilities loaded: " << summary.accepted << endl;
    cout << "Facilities rejected: " << summary.rejected << endl;

    departments.loadFromFile("data/departments.txt");

    cout << "Welcome to the Smart Campus Navigation and Facility Analytics System!" << endl;

    displayMenu(graph, facilities, departments);

    cout << "Thank you for using the system!" << endl;

    return 0;
}

