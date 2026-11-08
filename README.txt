Smart campus navigation and facility analytics system

Project overview

Smart campus navigation and facility analytics system is a C++ software
that helps organize the campus data and provides insight into the relationship
between the different buildings and facilities. Our group has divided the
project into three main sections; facility management, campus navigation and
department organization. Each section uses a different data structure to 
manage information in an efficient manner. The purpose of this project is to
implement the data structures and algorithms covered in class in a real-world
application in the campus environment and to learn how to collaborate on a
project using GitHub.

Project features

1. Facility management – hash table

The facility management section manages the facility data of the campus in
a custom hash table. Separate chaining is used to resolve collisions in this
case. Facilities can be looked up by ID. It also checks for duplicate and 
invalid IDs of facilities during the process.

2. Campus navigation – graph and BFS

The campus navigation section models the buildings and their connections as 
an undirected and unweighted graph. The graph uses an adjacency list to 
represent buildings and their neighboring locations.

The features include:

-loading buildings and connections from text files.
-drawing buildings and connections.
-breadth first search and depth first search.
-finding shortest path between buildings using BFS.
-number of buildings, connections and reachability.
-finding isolated structures and connected components.
-finding duplicate connections, unknown buildings and capitalization issues.

3. Department organization – tree

The component department uses a tree to organize the department information.
This section of the project was developed to demonstrate the use of a tree
for hierarchical data management. The department tree has been implemented 
in this project and the functionality must be verified by the team before 
submission.

Technologies used:

-C++

-git and GitHub

-text files to store the input data

-PuTTY and unt cell server

-gnu C++ compiler (g++)

-vs code

-online C++ compilers

Project organization

Folders have been created in the project to separate the implementation,
data and test files.

Project_2-smart-campus-navigation-and-facility-analytics-system/

|

|-- include/

| |-- CampusGraph.h

| |-- DepartmentTree.h

| |-- Facility.h

| |-- FacilityHashTable.h

|

|-- src/

| |-- CampusGraph.cpp

| |-- DepartmentTree.cpp

| |-- Facility.cpp

| |-- FacilityHashTable.cpp

| |-- main.cpp

| |-- menu.cpp

| |-- FileLoader.cpp

|

|-- data/

| |-- buildings.txt

| |-- connections.txt

|

|-- tests/

| |-- department_test.cpp

| |-- facilitytest.cpp


|-- README.txt

(additional input files, test files and documentation may be present in
the repository.)

How to compile and run

Smart campus navigation and facility analytics system is written in C++ 
programming language and developed under the gnu C++ compiler (g++).

To compile the project, you should enter the unt cell server console and
navigate to the main directory of the project that contains src, include 
and data directories. The system consists of several modules; facilities 
manager, campus navigation module, and departments arrangement. These
modules are supposed to interact through main program. System uses provided 
text files to read information about the campus. Functionality of the 
system allows user to manage facility records, analyze relationships
between buildings, calculate shortest paths and department analysis.

Also there are separate testing programs for testing program functionality.
For example, to compile and run campus graph tests use:

g++ -std=c++11 -iinclude tests/CampusGraphTest.cpp src/CampusGraph.cpp -o campusGraphTest

Run the test program using:

./campusGraphTest

Step 6 of the compile process must be carried out utilizing the full 
program build command that has been validated by the group.

Campus graph example

In the campus graph given above, we have seven buildings connected by six
one-way connections.

For BFS starting from the library, BFS algorithm recognizes six accessible 
buildings. The observatory building is not accessible because there are no
connections from the observatory building to other buildings.

For example, the shortest path between the library and the engineering
building will be:

Library
Student union
Science building
Engineering building
Total connections: 3

The connectivity report provides additional information about the campus
graph, including:

Campus connectivity report:

Total buildings: 7
Total connections: 6

Highest degree buildings:

Student union (3)

Isolated buildings:

Observatory
Connected components: 2

These findings illustrate how the graph can be utilized in order
to comprehend campus links and find reachable places.

Testing and validation

Testing was one of the key steps in the project's development. Tests
were implemented for each module to verify that the data structures
and algorithms of each module provide the required output.

Facilities hash table tests were conducted on:

-collision resolution
-duplicate facilities IDs
-ID normalization
-invalid records

Campus graph tests were conducted on:

-loading the correct number of buildings and connections;
-BFS execution from connected and unconnected buildings;
-building search with varying capitalization and surrounding spaces;
-avoiding duplicate buildings and connections;
-dealing with unknown buildings and invalid connections;
-searching the shortest path between connected buildings;
-unification of unrelated buildings and same starting/ending buildings;
-measuring connectivity properties in graphs.

The test on the campus graph produced the anticipated results, including
7 buildings, 6 connections, and 2 connected components. Other test
information is described in the test documentation for the project.

Team contributions

Ruth Omoyi – facility management

Overseeing the development of facility records and hash table. Her 
tasks include validation of facility ID, polynomial hash function,
separate chaining, handling of collisions, and testing of facility methods.

Janiya Maduagwu – campus navigation

Responsible for designing and implementing functionality related to
the graph of the campus and its navigation. This covers the adjacency 
list representation of graphs, traversal algorithms BFS and DFS, 
shortest paths finding, graph measures, connectedness check and testing.

Michaela Bosmans – department organization

Assigned the task of designing the tree structure for the departments 
section in order to aid in the organization of department information 
withinthe campus. Assigned the duty of designing the tree, writing the
insertionalgorithm for adding department information to the tree, 
searching algorithm for locating specific department, and traversal of 
the tree in order to display department information.

Conclusion

Smart campus navigation and facility analytics system demonstrates the
use of several data structures for different purposes within one project.
Hash table is employed for managing records of facilities, the graph for
navigation within the campus, while the tree is used for handling information
related to departments. This project helped us as a team gain experience
in division of programming tasks, testing each component independently, 
and usage of GitHub for managing changes in the repository.
