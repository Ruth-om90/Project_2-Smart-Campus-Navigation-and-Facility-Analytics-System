/*
 * File: department_test.cpp
 * Purpose: Tests the DepartmentTree module.
 * Author: Member C
 */

#include "DepartmentTree.h"

#include <iostream>

using namespace std;

int main()
{
    DepartmentTree tree;

    cout << "\n===== LOADING DEPARTMENTS =====\n";

    tree.loadFromFile("data/departments.txt");

    cout << "\n===== HIERARCHY =====\n";
    tree.displayHierarchy();

    cout << "\n===== PREORDER =====\n";
    tree.preorderTraversal();

    cout << "\n===== POSTORDER =====\n";
    tree.postorderTraversal();

    cout << "\n===== LEVEL ORDER =====\n";
    tree.levelOrderTraversal();

    cout << "\n===== STATISTICS =====\n";
    tree.displayStatistics();

    cout << "\n===== SEARCH TEST =====\n";

    DepartmentNode* result =
        tree.searchDepartment("computer science");

    if (result != nullptr)
    {
        cout << "Found: " << result->name << "\n";
    }
    else
    {
        cout << "Department not found.\n";
    }

    return 0;
}
