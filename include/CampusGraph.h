
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
    
    void connectivityReport() const;
    void shortestPath(const std::string& start, const std::string& destination) const;
    void addBuilding(const std::string& name);
    void addConnection(const std::string& buildingA,
                       const std::string& buildingB);
    bool loadBuildings(const std::string& filename);
    bool loadConnections(const std::string& filename);
    int findBuilding(const std::string& name) const;

    void displayGraph() const;
    int getBuildingCount() const;
    int getConnectionCount() const;
    void displayDegrees() const;
    void BFS(const std::string& start) const;
    void DFS(const std::string& start) const;
};

#endif
