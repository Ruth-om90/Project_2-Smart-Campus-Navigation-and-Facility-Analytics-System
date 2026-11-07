
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
    
    cout << "\nTesting shortest path:\n";
    campus.shortestPath("Library", "Engineering Building");

    cout << "\nTesting same start and destination:\n";
    campus.shortestPath("Library", "Library");

    cout << "\nTesting disconnected buildings:\n";
    campus.shortestPath("Library", "Observatory");

    cout << "\nTesting unknown destination:\n";
    campus.shortestPath("Library", "Unknown Building");

    cout << "\nTesting case-insensitive shortest path:\n";
    campus.shortestPath("library", "GYM");
    cout << "\nTesting Connectivity Report:\n";
    campus.connectivityReport();
    return 0;
}

