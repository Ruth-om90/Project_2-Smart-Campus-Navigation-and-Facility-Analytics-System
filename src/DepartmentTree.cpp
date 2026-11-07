
/*
 * File: DepartmentTree.cpp
 * Purpose: Implements the department general tree.
 * Author: Member C
 */

#include "DepartmentTree.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <cctype>
#include <algorithm>

using namespace std;

// Removes whitespace from the beginning and end of a string.
static string trim(const string& text)
{
    size_t start = 0;
    size_t end = text.size();

    while (start < end &&
           isspace(static_cast<unsigned char>(text[start])))
    {
        start++;
    }

    while (end > start &&
           isspace(static_cast<unsigned char>(text[end - 1])))
    {
        end--;
    }

    return text.substr(start, end - start);
}

// Converts a string to uppercase for case-insensitive comparison.
static string normalize(const string& text)
{
    string result = trim(text);

    for (char& character : result)
    {
        character = static_cast<char>(
            toupper(static_cast<unsigned char>(character)));
    }

    return result;
}

// Creates a department node.
DepartmentNode::DepartmentNode(
    const string& departmentName,
    DepartmentNode* parentNode)
{
    name = departmentName;
    parent = parentNode;
}

// Creates an empty department tree.
DepartmentTree::DepartmentTree()
{
    root = nullptr;
}

// Releases all tree nodes.
DepartmentTree::~DepartmentTree()
{
    deleteTree(root);
}

// Deletes a node and all its children recursively.
void DepartmentTree::deleteTree(DepartmentNode* current)
{
    if (current == nullptr)
    {
        return;
    }

    for (DepartmentNode* child : current->children)
    {
        deleteTree(child);
    }

    delete current;
}

// Checks whether the tree is empty.
bool DepartmentTree::isEmpty() const
{
    return root == nullptr;
}

// Searches recursively for a department.
DepartmentNode* DepartmentTree::findDepartment(
    DepartmentNode* current,
    const string& departmentName) const
{
    if (current == nullptr)
    {
        return nullptr;
    }

    if (normalize(current->name) == normalize(departmentName))
    {
        return current;
    }

    for (DepartmentNode* child : current->children)
    {
        DepartmentNode* result =
            findDepartment(child, departmentName);

        if (result != nullptr)
        {
            return result;
        }
    }

    return nullptr;
}

// Searches for a department by name.
DepartmentNode* DepartmentTree::searchDepartment(
    const string& departmentName) const
{
    if (trim(departmentName).empty())
    {
        return nullptr;
    }

    return findDepartment(root, departmentName);
}

// Adds a department to the tree.
bool DepartmentTree::addDepartment(
    const string& departmentName,
    const string& parentName)
{
    string cleanName = trim(departmentName);
    string cleanParent = trim(parentName);

    // Department names cannot be empty.
    if (cleanName.empty())
    {
        cout << "Error: Department name cannot be blank.\n";
        return false;
    }

    // Reject duplicate department names.
    if (searchDepartment(cleanName) != nullptr)
    {
        cout << "Error: Department already exists: "
             << cleanName << "\n";
        return false;
    }

    // The first department becomes the root.
    if (root == nullptr)
    {
        root = new DepartmentNode(cleanName);

        cout << "Root department added: "
             << cleanName << "\n";

        return true;
    }

    // Existing trees require a parent.
    if (cleanParent.empty())
    {
        cout << "Error: A parent department is required.\n";
        return false;
    }

    // Find the requested parent.
    DepartmentNode* parentNode =
        searchDepartment(cleanParent);

    if (parentNode == nullptr)
    {
        cout << "Error: Unknown parent department: "
             << cleanParent << "\n";

        return false;
    }

    // Create and attach the new department.
    DepartmentNode* newNode =
        new DepartmentNode(cleanName, parentNode);

    parentNode->children.push_back(newNode);

    cout << "Department added: "
         << cleanName << "\n";

    return true;
}

// Loads departments from a text file.
void DepartmentTree::loadFromFile(const string& filename)
{
    ifstream inputFile(filename);

    int accepted = 0;
    int rejected = 0;
    int lineNumber = 0;

    if (!inputFile.is_open())
    {
        cout << "Error: Cannot open file: "
             << filename << "\n";

        cout << filename
             << ": 0 accepted, 0 rejected\n";

        return;
    }

    string line;

    while (getline(inputFile, line))
    {
        lineNumber++;

        // Remove whitespace, including Windows carriage returns.
        line = trim(line);

        // Ignore blank lines.
        if (line.empty())
        {
            continue;
        }

        // Ignore comment lines.
        if (line[0] == '#')
        {
            continue;
        }

        // Split the line using the | delimiter.
        string departmentName;
        string parentName;
        string extraField;

        stringstream stream(line);

        if (!getline(stream, departmentName, '|') ||
            !getline(stream, parentName, '|'))
        {
            cout << "Warning: " << filename
                 << ", line " << lineNumber
                 << ": Incorrect field count.\n";

            rejected++;
            continue;
        }

        // Detect extra fields.
        if (getline(stream, extraField, '|'))
        {
            cout << "Warning: " << filename
                 << ", line " << lineNumber
                 << ": Too many fields.\n";

            rejected++;
            continue;
        }

        departmentName = trim(departmentName);
        parentName = trim(parentName);

        // Reject empty required fields.
        if (departmentName.empty() || parentName.empty())
        {
            cout << "Warning: " << filename
                 << ", line " << lineNumber
                 << ": Empty required field.\n";

            rejected++;
            continue;
        }

        // The first accepted record must be the root.
        if (root == nullptr)
        {
            if (normalize(parentName) != "NONE")
            {
                cout << "Warning: " << filename
                     << ", line " << lineNumber
                     << ": First department must have parent NONE.\n";

                rejected++;
                continue;
            }

            if (addDepartment(departmentName, ""))
            {
                accepted++;
            }
            else
            {
                rejected++;
            }

            continue;
        }

        // NONE is not allowed as a later parent.
        if (normalize(parentName) == "NONE")
        {
            cout << "Warning: " << filename
                 << ", line " << lineNumber
                 << ": Unknown parent NONE.\n";

            rejected++;
            continue;
        }

        // The parent must already exist.
        if (searchDepartment(parentName) == nullptr)
        {
            cout << "Warning: " << filename
                 << ", line " << lineNumber
                 << ": Unknown parent: "
                 << parentName << "\n";

            rejected++;
            continue;
        }

        // Add the department.
        if (addDepartment(departmentName, parentName))
        {
            accepted++;
        }
        else
        {
            rejected++;
        }
    }

    inputFile.close();

    cout << filename << ": "
         << accepted << " accepted, "
         << rejected << " rejected\n";
}

// Displays the hierarchy with four spaces per depth.
void DepartmentTree::displayPreorderIndented(
    DepartmentNode* current,
    int depth) const
{
    if (current == nullptr)
    {
        return;
    }

    for (int i = 0; i < depth; i++)
    {
        cout << "    ";
    }

    cout << current->name << "\n";

    for (DepartmentNode* child : current->children)
    {
        displayPreorderIndented(child, depth + 1);
    }
}

// Displays the entire hierarchy.
void DepartmentTree::displayHierarchy() const
{
    if (root == nullptr)
    {
        cout << "No departments are loaded.\n";
        return;
    }

    displayPreorderIndented(root, 0);
}

// Preorder traversal: parent before children.
void DepartmentTree::preorder(
    DepartmentNode* current) const
{
    if (current == nullptr)
    {
        return;
    }

    cout << current->name << " ";

    for (DepartmentNode* child : current->children)
    {
        preorder(child);
    }
}

// Displays preorder traversal.
void DepartmentTree::preorderTraversal() const
{
    if (root == nullptr)
    {
        cout << "No departments are loaded.\n";
        return;
    }

    preorder(root);
    cout << "\n";
}

// Postorder traversal: children before parent.
void DepartmentTree::postorder(
    DepartmentNode* current) const
{
    if (current == nullptr)
    {
        return;
    }

    for (DepartmentNode* child : current->children)
    {
        postorder(child);
    }

    cout << current->name << " ";
}

// Displays postorder traversal.
void DepartmentTree::postorderTraversal() const
{
    if (root == nullptr)
    {
        cout << "No departments are loaded.\n";
        return;
    }

    postorder(root);
    cout << "\n";
}

// Displays departments using level-order traversal.
void DepartmentTree::levelOrderTraversal() const
{
    if (root == nullptr)
    {
        cout << "No departments are loaded.\n";
        return;
    }

    queue<DepartmentNode*> departmentQueue;

    departmentQueue.push(root);

    while (!departmentQueue.empty())
    {
        DepartmentNode* current =
            departmentQueue.front();

        departmentQueue.pop();

        cout << current->name << " ";

        for (DepartmentNode* child : current->children)
        {
            departmentQueue.push(child);
        }
    }

    cout << "\n";
}

// Counts all departments in the tree.
int DepartmentTree::countDepartments(
    DepartmentNode* current) const
{
    if (current == nullptr)
    {
        return 0;
    }

    int total = 1;

    for (DepartmentNode* child : current->children)
    {
        total += countDepartments(child);
    }

    return total;
}

// Counts leaf departments.
int DepartmentTree::countLeaves(
    DepartmentNode* current) const
{
    if (current == nullptr)
    {
        return 0;
    }

    // A node with no children is a leaf.
    if (current->children.empty())
    {
        return 1;
    }

    int total = 0;

    for (DepartmentNode* child : current->children)
    {
        total += countLeaves(child);
    }

    return total;
}

// Calculates tree height.
int DepartmentTree::calculateHeight(
    DepartmentNode* current) const
{
    if (current == nullptr)
    {
        return -1;
    }

    int maximumHeight = -1;

    for (DepartmentNode* child : current->children)
    {
        int childHeight = calculateHeight(child);

        if (childHeight > maximumHeight)
        {
            maximumHeight = childHeight;
        }
    }

    return maximumHeight + 1;
}

// Displays tree statistics.
void DepartmentTree::displayStatistics() const
{
    if (root == nullptr)
    {
        cout << "No departments are loaded.\n";
        cout << "Total departments: 0\n";
        cout << "Leaf departments: 0\n";
        cout << "Tree height: N/A\n";

        return;
    }

    cout << "Total departments: "
         << countDepartments(root) << "\n";

    cout << "Leaf departments: "
         << countLeaves(root) << "\n";

    cout << "Tree height: "
         << calculateHeight(root) << "\n";
}
