
#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H

#include <string>
#include <vector>

class CampusGraph {
private:
    std::vector<std::string> buildings;
    std::vector<std::vector<int>> adjacencyList;

public:
    CampusGraph();

    void addBuilding(const std::string& name);
    void addConnection(const std::string& buildingA,
                       const std::string& buildingB);

    int findBuilding(const std::string& name) const;

    void displayGraph() const;

    void BFS(const std::string& start) const;
    void DFS(const std::string& start) const;
};

#endif
