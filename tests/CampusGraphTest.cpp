
#include "../include/CampusGraph.h"
#include <iostream>

using namespace std;

int main() {
    CampusGraph campus;

    if (!campus.loadBuildings("data/buildings.txt")) {
        return 1;
    }

    if (!campus.loadConnections("data/connections.txt")) {
        return 1;
    }

    campus.displayGraph();

    cout << "\nTotal Buildings: "
         << campus.getBuildingCount() << endl;

    cout << "Total Connections: "
         << campus.getConnectionCount() << endl;

    campus.displayDegrees();

    cout << "\nTesting BFS from Library:\n";
    campus.BFS("Library");

    cout << "\nTesting BFS from Observatory:\n";
    campus.BFS("Observatory");

    cout << "\nTesting an unknown building:\n";
    campus.BFS("Unknown Building");

    cout << "\nTesting case-insensitive search:\n";
    campus.BFS("library");

    cout << "\nTesting whitespace in building names:\n";
    campus.BFS("  Library  ");

    cout << "\nTesting duplicate building:\n";
    campus.addBuilding("Library");
    cout << "Total Buildings: "
         << campus.getBuildingCount() << endl;

    cout << "\nTesting duplicate connection:\n";
    campus.addConnection("Library", "Student Union");
    cout << "Total Connections: "
         << campus.getConnectionCount() << endl;

    cout << "\nTesting invalid connection:\n";
    campus.addConnection("Library", "Unknown Building");
    cout << "Total Connections: "
         << campus.getConnectionCount() << endl;

    return 0;
}

