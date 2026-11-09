#include "CampusGraph.h"
#include "DepartmentTree.h"
#include "FacilityHashTable.h"
#include "Menu.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

namespace
{
std::string runMenu(
    const std::string& inputText,
    CampusGraph& graph,
    FacilityHashTable& facilities,
    DepartmentTree& departments)
{
    std::istringstream input(inputText);
    std::ostringstream output;
    std::streambuf* previousInput = std::cin.rdbuf(input.rdbuf());
    std::streambuf* previousOutput = std::cout.rdbuf(output.rdbuf());

    displayMenu(graph, facilities, departments);

    std::cin.rdbuf(previousInput);
    std::cout.rdbuf(previousOutput);
    return output.str();
}

bool contains(const std::string& text, const std::string& expected)
{
    return text.find(expected) != std::string::npos;
}

void testAllMenuSections()
{
    CampusGraph graph;
    graph.addBuilding("Library");
    graph.addBuilding("Student Union");
    graph.addBuilding("Science Building");
    graph.addBuilding("Engineering Building");
    graph.addBuilding("Observatory");
    graph.addConnection("Library", "Student Union");
    graph.addConnection("Student Union", "Science Building");
    graph.addConnection("Science Building", "Engineering Building");

    FacilityHashTable facilities;
    assert(facilities.insert(
        Facility{"F001", "Main Reading Room", "Library", "Library"}) ==
        FacilityInsertResult::Inserted);
    assert(facilities.insert(
        Facility{"F002", "Tutoring Center", "Library", "Tutoring"}) ==
        FacilityInsertResult::Inserted);

    DepartmentTree departments;
    std::ostringstream setupOutput;
    std::streambuf* previousOutput = std::cout.rdbuf(setupOutput.rdbuf());
    assert(departments.addDepartment("University", ""));
    assert(departments.addDepartment("College of Science", "University"));
    assert(departments.addDepartment("Physics", "College of Science"));
    std::cout.rdbuf(previousOutput);

    const std::string output = runMenu(
        "invalid\n"
        "1\n"
        "1\n"
        "2\n"
        "library\n"
        "3\n"
        "Observatory\n"
        "4\n"
        "Library\n"
        "Engineering Building\n"
        "5\n"
        "6\n"
        "0\n"
        "2\n"
        "1\n"
        "2\n"
        "\n"
        "f001\n"
        "2\n"
        "F999\n"
        "2\n"
        "F12\n"
        "3\n"
        "0\n"
        "3\n"
        "1\n"
        "2\n"
        "physics\n"
        "2\n"
        "Missing Department\n"
        "3\n"
        "Applied Mathematics\n"
        "College of Science\n"
        "1\n"
        "4\n"
        "5\n"
        "6\n"
        "7\n"
        "0\n"
        "0\n",
        graph,
        facilities,
        departments);

    assert(contains(output, "Invalid choice. Enter a number from 0 to 3."));
    assert(contains(output, "Campus Graph:"));
    assert(contains(output, "Reachable buildings: 4 / 5"));
    assert(contains(output, "DFS starting from Observatory:"));
    assert(contains(output, "Total connections: 3"));
    assert(contains(output, "Connected Components: 2"));
    assert(contains(output, "Engineering Building: 1"));
    assert(contains(output, "ID: F001 | Name: Main Reading Room"));
    assert(contains(output, "Total facilities: 2"));
    assert(contains(output, "No facility found with ID F999."));
    assert(contains(output, "Invalid facility ID format."));
    assert(contains(output, "Facilities: 2"));
    assert(contains(output, "Department found: Physics (parent: College of Science)"));
    assert(contains(output, "Department not found."));
    assert(contains(output, "Department added: Applied Mathematics"));
    assert(contains(output, "University\n    College of Science"));
    assert(contains(output, "Total departments: 4"));
    assert(departments.searchDepartment("applied mathematics") != 0);
    assert(graph.getBuildingCount() == 5);
    assert(graph.getConnectionCount() == 3);
    assert(facilities.size() == 2);
}

void testEndOfInputExitsMenu()
{
    CampusGraph graph;
    FacilityHashTable facilities;
    DepartmentTree departments;

    const std::string output = runMenu(
        "",
        graph,
        facilities,
        departments);

    assert(contains(output, "Smart Campus Navigation and Facility Analytics"));
}
}

int main()
{
    testAllMenuSections();
    testEndOfInputExitsMenu();
    std::cout << "Menu tests passed.\n";
    return 0;
}
