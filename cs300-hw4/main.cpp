// HUSEYIN EREN YILDIZ - 31047 - HOMEWORK4

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>

using namespace std;

// Graph class: Handles checking if the graph is bipartite and finding maximum matching
class Graph {
private:
    int numVertices; // Number of vertices
    vector<vector<int>> adjacent; // Adjacency list to store graph connections
    vector<int> vertexColors;  // -1: uncolored, 0 and 1: two different colors for bipartite graphs
    vector<int> vertexMatches;  // Array to track matched vertices for maximum matching

    // Helper function to color the graph and check if it is bipartite
    bool clr_Graph(int start);
    // Helper function for finding a match for a given vertex
    bool find_Match_Helper(int v, int index, vector<bool>& visited);

public:
    // Constructor: Initializes the graph with the given number of vertices
    Graph(int vertices) : numVertices(vertices) {
        adjacent.resize(vertices);  // Initialize adjacency list
        vertexColors.resize(vertices, -1); // All vertices start as uncolored
        vertexMatches.resize(vertices, -1); // No vertices are vertexMatches initially
    }

    // Adds an undirected edge between two vertices
    void addEdge(int u, int v) {
        adjacent[u].push_back(v);
        adjacent[v].push_back(u);
    }

    // Checks if the graph is bipartite
    bool isGraphBipartiate();
    // Calculates the maximum matching in the graph
    int calculateMaxMatching();
    // Main function for finding matches
    bool get_matching(int v, vector<bool>& visited);
};

// Uses BFS to color the graph and checks if it is bipartite
bool Graph::clr_Graph(int start) {
    // If the vertex is already colored, no need to process it
    if (vertexColors[start] != -1) {
        return true;
    }

    // Start coloring the vertex with color 0
    vertexColors[start] = 0;

    // Queue for BFS traversal
    queue<int> bfsQueue;
    bfsQueue.push(start);

    // BFS loop to explore all connected vertices
    while (!bfsQueue.empty()) {
        int current = bfsQueue.front();
        bfsQueue.pop();

        // Traverse all neighbors of the current vertex
        for (int i = 0; i < adjacent[current].size(); i++) {
            int neighbor = adjacent[current][i];
            // If the neighbor is uncolored, assign the opposite color
            if (vertexColors[neighbor] == -1) {
                vertexColors[neighbor] = 1 - vertexColors[current];
                bfsQueue.push(neighbor);
            }
                // If the neighbor has the same color as the current vertex, the graph is not bipartite
            else if (vertexColors[neighbor] == vertexColors[current]) {
                return false;
            }
        }
    }
    return true;
}

// Recursive helper function for finding matches
bool Graph::find_Match_Helper(int currentVertex, int neighborIndex, vector<bool>& visited) {
    // If all neighbors have been checked, return false
    if (neighborIndex == adjacent[currentVertex].size()) {
        return false;
    }

    int neighborVertex  = adjacent[currentVertex][neighborIndex];

    // Check if the neighbor can be vertexMatches
    if (vertexColors[neighborVertex ] != vertexColors[currentVertex] && !visited[neighborVertex ]) {
        visited[neighborVertex ] = true;

        // If the neighbor is unmatched or can find a new match, update the matching
        if (vertexMatches[neighborVertex ] == -1 || get_matching(vertexMatches[neighborVertex ], visited)) {
            vertexMatches[neighborVertex ] = currentVertex;
            return true;
        }
    }

    // Try the next neighbor
    return find_Match_Helper(currentVertex, neighborIndex + 1, visited);
}

// Wrapper function to start finding matches for a vertex
bool Graph::get_matching(int v, vector<bool>& visitedVertices) {
    return find_Match_Helper(v, 0, visitedVertices);
}

// Checks if the graph is bipartite by attempting to color each connected component
bool Graph::isGraphBipartiate() {
    for (int j = 0; j < numVertices; j++) {
        // If a vertex is uncolored, try to color it
        if (vertexColors[j] == -1) {
            if (!clr_Graph(j)) {
                return false; // If coloring fails, the graph is not bipartite
            }
        }
    }
    return true;
}

// Finds the maximum matching in the bipartite graph
int Graph::calculateMaxMatching() {
    vector<int> zeroColorVertices; // Store vertices with color 0

    // Collect all vertices with color 0
    for (int i = 1; i < numVertices; i++) {
        if (vertexColors[i] == 0) {
            zeroColorVertices.push_back(i);
        }
    }

    int totalMatches = 0; // Counter for the number of matches found

    // Try to find matches for each vertex in zeroColorVertices
    for (int i = 0; i < zeroColorVertices.size(); i++) {
        int currentVertex = zeroColorVertices[i];
        vector<bool> visitedNodes(numVertices, false); // Reset visited array for each vertex

        // If a match is found, increment the match counter
        if (get_matching(currentVertex, visitedNodes)) {
            totalMatches++;
        }
    }

    return totalMatches;
}

int main() {
    string graphInput, graphFileName;
    int totalVertices, totalEdges, vertex1, vertex2;

    // Main input loop: processes multiple graphs until "exit" is entered
    while (cout << "Enter the graph name (e.g., 'graph1' or 'exit' to terminate): " &&
           cin >> graphInput && graphInput != "exit") {

        graphFileName = graphInput + ".txt";
        ifstream file(graphFileName); // Open the file with the graph data

        // Skip if the file cannot be opened
        if (!file.is_open()) continue;

        file >> totalVertices >> totalEdges;
        Graph graph(totalVertices + 1);  // +1 for 1-based indexing of vertices

        // Read edges and build the graph
        while (totalEdges-- && file >> vertex1 >> vertex2) {
            graph.addEdge(vertex1, vertex2);
        }

        // Check if the graph is bipartite
        if (graph.isGraphBipartiate()) {
            cout << "The graph is bipartite.\n"
                 << "Maximum matching size: " << graph.calculateMaxMatching() << endl;
        } else {
            cout << "The graph is not bipartite." << endl;
        }

        file.close(); // Close the file after processing
    }
    return 0;
}
