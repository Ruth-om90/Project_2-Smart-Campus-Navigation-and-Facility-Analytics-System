
#include "../include/CampusGraph.h"
#include <iostream>

using namespace std;

int main() {
    CampusGraph campus;

    campus.addBuilding("Library");
    campus.addBuilding("Student Union");
    campus.addBuilding("Science Building");
    campus.addBuilding("Engineering Building");
    campus.addBuilding("Business Building");
    campus.addBuilding("Gym");
    campus.addBuilding("Observatory");

    campus.addConnection("Library", "Student Union");
    campus.addConnection("Library", "Business Building");
    campus.addConnection("Student Union", "Science Building");
    campus.addConnection("Student Union", "Gym");
    campus.addConnection("Science Building", "Engineering Building");
    campus.addConnection("Business Building", "Gym");

    campus.displayGraph();

    cout << "\nTesting BFS from Library:\n";
    campus.BFS("Library");

    cout << "\nTesting BFS from Observatory:\n";
    campus.BFS("Observatory");

    cout << "\nTesting an unknown building:\n";
    campus.BFS("Unknown Building");

    return 0;
}
