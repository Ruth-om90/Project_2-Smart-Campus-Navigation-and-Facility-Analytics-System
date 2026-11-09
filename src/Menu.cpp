#include "Menu.h"

#include "Facility.h"

#include <iostream>
#include <sstream>
#include <string>

namespace
{
bool readMenuChoice(
    const std::string& prompt,
    int minimum,
    int maximum,
    int& choice)
{
    std::cout << prompt;

    std::string line;
    if (!std::getline(std::cin, line))
    {
        return false;
    }


    std::istringstream input(line);
    int parsedChoice;
    char extraCharacter;

    if (!(input >> parsedChoice) ||
        (input >> extraCharacter) ||
        parsedChoice < minimum ||
        parsedChoice > maximum)
    {
        std::cout << "Invalid choice. Enter a number from "
                  << minimum << " to " << maximum << ".\n";
        return true;
    }

    choice = parsedChoice;
    return true;
}

bool readRequiredLine(const std::string& prompt, std::string& value)
{
    while (true)
    {
        std::cout << prompt;
        if (!std::getline(std::cin, value))
        {
            return false;
        }

        value = trimWhitespace(value);
        if (!value.empty())
        {
            return true;
        }

        std::cout << "Input cannot be empty.\n";
    }
}

void displayCampusMenu(CampusGraph& graph)
{
    while (true)
    {
        std::cout << "\nCampus Navigation\n"
                  << "1. Display campus graph\n"
                  << "2. Breadth-first search (BFS)\n"
                  << "3. Depth-first search (DFS)\n"
                  << "4. Find shortest path\n"
                  << "5. Display connectivity report\n"
                  << "6. Display building degrees\n"
                  << "0. Return to main menu\n";

        int choice = -1;
        if (!readMenuChoice("Choice: ", 0, 6, choice))
        {
            return;
        }

        switch (choice)
        {
        case 0:
            return;
        case 1:
            graph.displayGraph();
            break;
        case 2:
        case 3:
        {
            std::string building;
            if (!readRequiredLine("Starting building: ", building))
            {
                return;
            }

            if (choice == 2)
            {
                graph.BFS(building);
            }
            else
            {
                graph.DFS(building);
            }
            break;
        }
        case 4:
        {
            std::string start;
            std::string destination;
            if (!readRequiredLine("Starting building: ", start) ||
                !readRequiredLine("Destination building: ", destination))
            {
                return;
            }

            graph.shortestPath(start, destination);
            break;
        }
        case 5:
            graph.connectivityReport();
            break;
        case 6:
            graph.displayDegrees();
            break;
        default:
            break;
        }
    }
}

void displayFacilityMenu(const FacilityHashTable& facilities)
{
    while (true)
    {
        std::cout << "\nFacility Management\n"
                  << "1. List all facilities\n"
                  << "2. Look up a facility by ID\n"
                  << "3. Display hash table statistics\n"
                  << "0. Return to main menu\n";

        int choice = -1;
        if (!readMenuChoice("Choice: ", 0, 3, choice))
        {
            return;
        }

        switch (choice)
        {
        case 0:
            return;
        case 1:
            printFacilities(facilities, std::cout);
            break;
        case 2:
        {
            std::string id;
            if (!readRequiredLine("Facility ID: ", id))
            {
                return;
            }

            const FacilityLookupResult result = facilities.lookup(id);
            if (result.status == FacilityLookupStatus::Found &&
                result.facility != 0)
            {
                std::cout << "ID: " << result.facility->id
                          << " | Name: " << result.facility->name
                          << " | Building: " << result.facility->building
                          << " | Category: " << result.facility->category
                          << '\n';
            }
            else if (result.status == FacilityLookupStatus::InvalidFormat)
            {
                std::cout << "Invalid facility ID format. Use F followed "
                             "by exactly three digits.\n";
            }
            else if (result.status == FacilityLookupStatus::NotFound)
            {
                std::cout << "No facility found with ID "
                          << result.normalizedId << ".\n";
            }
            else
            {
                std::cout << "Error: facility lookup returned an "
                             "inconsistent result.\n";
            }
            break;
        }
        case 3:
            std::cout << "Facilities: " << facilities.size()
                      << "\nHash table capacity: " << facilities.capacity()
                      << "\nCollisions: " << facilities.collisionCount()
                      << '\n';
            break;
        default:
            break;
        }
    }
}

void displayDepartmentMenu(DepartmentTree& departments)
{
    while (true)
    {
        std::cout << "\nDepartment Organization\n"
                  << "1. Display department hierarchy\n"
                  << "2. Search for a department\n"
                  << "3. Add a department\n"
                  << "4. Preorder traversal\n"
                  << "5. Postorder traversal\n"
                  << "6. Level-order traversal\n"
                  << "7. Display department statistics\n"
                  << "0. Return to main menu\n";

        int choice = -1;
        if (!readMenuChoice("Choice: ", 0, 7, choice))
        {
            return;
        }

        switch (choice)
        {
        case 0:
            return;
        case 1:
            departments.displayHierarchy();
            break;
        case 2:
        {
            std::string name;
            if (!readRequiredLine("Department name: ", name))
            {
                return;
            }

            const DepartmentNode* department =
                departments.searchDepartment(name);
            if (department == 0)
            {
                std::cout << "Department not found.\n";
            }
            else
            {
                std::cout << "Department found: " << department->name;
                if (department->parent != 0)
                {
                    std::cout << " (parent: "
                              << department->parent->name << ')';
                }
                else
                {
                    std::cout << " (root department)";
                }
                std::cout << '\n';
            }
            break;
        }
        case 3:
        {
            std::string name;
            if (!readRequiredLine("New department name: ", name))
            {
                return;
            }

            std::string parent;
            if (!departments.isEmpty() &&
                !readRequiredLine("Parent department name: ", parent))
            {
                return;
            }

            departments.addDepartment(name, parent);
            break;
        }
        case 4:
            departments.preorderTraversal();
            break;
        case 5:
            departments.postorderTraversal();
            break;
        case 6:
            departments.levelOrderTraversal();
            break;
        case 7:
            departments.displayStatistics();
            break;
        default:
            break;
        }
    }
}
}

void displayMenu(
    CampusGraph& graph,
    FacilityHashTable& facilities,
    DepartmentTree& departments)
{
    while (true)
    {
        std::cout << "\nSmart Campus Navigation and Facility Analytics\n"
                  << "1. Campus navigation\n"
                  << "2. Facility management\n"
                  << "3. Department organization\n"
                  << "0. Exit\n";

        int choice = -1;
        if (!readMenuChoice("Choice: ", 0, 3, choice))
        {
            return;
        }

        switch (choice)
        {
        case 0:
            return;
        case 1:
            displayCampusMenu(graph);
            break;
        case 2:
            displayFacilityMenu(facilities);
            break;
        case 3:
            displayDepartmentMenu(departments);
            break;
        default:
            break;
        }
    }
}
