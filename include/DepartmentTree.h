
/*
 * File: DepartmentTree.h
 * Purpose: Defines the general department tree.
 * Author: Member C
 */

#ifndef DEPARTMENTTREE_H
#define DEPARTMENTTREE_H

#include <string>
#include <vector>

class DepartmentNode
{
public:
    std::string name;
    DepartmentNode* parent;
    std::vector<DepartmentNode*> children;

    // Constructor
    DepartmentNode(const std::string& departmentName,
                   DepartmentNode* parentNode = nullptr);
};

class DepartmentTree
{
private:
    DepartmentNode* root;

    // Helper functions
    DepartmentNode* findDepartment(
        DepartmentNode* current,
        const std::string& departmentName) const;

    void displayPreorderIndented(
        DepartmentNode* current,
        int depth) const;

    void preorder(
        DepartmentNode* current) const;

    void postorder(
        DepartmentNode* current) const;

    void deleteTree(
        DepartmentNode* current);

    int countDepartments(
        DepartmentNode* current) const;

    int countLeaves(
        DepartmentNode* current) const;

    int calculateHeight(
        DepartmentNode* current) const;

public:
    // Constructor and destructor
    DepartmentTree();
    ~DepartmentTree();

    // Prevent copying of dynamically allocated tree nodes
    DepartmentTree(const DepartmentTree&) = delete;
    DepartmentTree& operator=(const DepartmentTree&) = delete;

    // Basic tree operations
    bool isEmpty() const;

    bool addDepartment(
        const std::string& departmentName,
        const std::string& parentName);

    DepartmentNode* searchDepartment(
        const std::string& departmentName) const;

    // File loading
    void loadFromFile(const std::string& filename);

    // Displays
    void displayHierarchy() const;
    void preorderTraversal() const;
    void postorderTraversal() const;
    void levelOrderTraversal() const;

    // Statistics
    void displayStatistics() const;
};

#endif
