/**
 * @author Shay Gali
 * @link shay.gali@msmail.ariel.ac.il
 */

#include <iostream>
#include <vector>

#include "algorithms/Algorithms.hpp"
#include "graph/Graph.hpp"

using namespace std;
using namespace shayg;

int main() {
    cout << "~~~~~~~~~~~ Algorithms demo ~~~~~~~~~~~" << endl;
    Graph g;

    // 3x3 matrix that represents a connected graph.
    vector<vector<int>> graph = {
        {NO_EDGE, 1, NO_EDGE},
        {1, NO_EDGE, 1},
        {NO_EDGE, 1, NO_EDGE}};
    g.loadGraph(graph);  // Load the graph to the object.

    g.printGraph();                                     // Should print: "Graph with 3 vertices and 2 edges."
    cout << Algorithms::isConnected(g) << endl;         // Should print: "1" (true).
    cout << Algorithms::shortestPath(g, 0, 2) << endl;  // Should print: 0->1->2.
    cout << Algorithms::isContainsCycle(g) << endl;     // Should print: "-1"
    cout << Algorithms::isBipartite(g) << endl;         // Should print: "The graph is bipartite: A={0, 2}, B={1}."

    // 5x5 matrix that represents a non-connected graph with a cycle.
    vector<vector<int>> graph2 = {
        {NO_EDGE, 1, 1, NO_EDGE, NO_EDGE},
        {1, NO_EDGE, 1, NO_EDGE, NO_EDGE},
        {1, 1, NO_EDGE, 1, NO_EDGE},
        {NO_EDGE, NO_EDGE, 1, NO_EDGE, NO_EDGE},
        {NO_EDGE, NO_EDGE, NO_EDGE, NO_EDGE, NO_EDGE}};

    g.loadGraph(graph2);  // Load the graph to the object.

    g.printGraph();                                     // Should print: "Graph with 5 vertices and 4 edges."
    cout << Algorithms::isConnected(g) << endl;         // Should print: "0" (false).
    cout << Algorithms::shortestPath(g, 0, 4) << endl;  // Should print: "-1" (there is no path between 0 and 4).
    cout << Algorithms::isContainsCycle(g) << endl;     // Should print: "The cycle is: 0->1->2->0".
    cout << Algorithms::isBipartite(g) << endl;         // Should print: "The graph is not bipartite"

    // 5x5 matrix that reprsents a connected weighted graph.
    vector<vector<int>> graph3 = {
        {NO_EDGE, 1, 2, NO_EDGE, NO_EDGE},
        {1, NO_EDGE, 3, NO_EDGE, NO_EDGE},
        {2, 3, NO_EDGE, 4, NO_EDGE},
        {NO_EDGE, NO_EDGE, 4, NO_EDGE, 5},
        {NO_EDGE, NO_EDGE, NO_EDGE, 5, NO_EDGE}};
    g.loadGraph(graph3);  // Load the graph to the object.

    g.printGraph();                                     // Should print: "Graph with 5 vertices and 5 edges."
    cout << Algorithms::isConnected(g) << endl;         // Should print: "1" (true).
    cout << Algorithms::shortestPath(g, 0, 4) << endl;  // Should print: 0->2->3->4.
    cout << Algorithms::isContainsCycle(g) << endl;     // Should print: 0->1->2->0
    cout << Algorithms::isBipartite(g) << endl;         // Should print: "The graph is not bipartite"

    // 5x4 matrix that reprsents invalid graph.
    vector<vector<int>> graph4 = {
        {NO_EDGE, 1, 2, NO_EDGE},
        {1, NO_EDGE, 3, NO_EDGE},
        {2, 3, NO_EDGE, 4},
        {NO_EDGE, NO_EDGE, 4, NO_EDGE},
        {NO_EDGE, NO_EDGE, NO_EDGE, 5}};
    try {
        g.loadGraph(graph4);  // Load the graph to the object.
    } catch (const std::invalid_argument &e) {
        cout << e.what() << endl;  // Should print: "Invalid graph: The graph is not a square matrix."
    }

    cout << "~~~~~~~~~~~ Graph demo ~~~~~~~~~~~" << endl;
    Graph g1;
    // 3x3 matrix that represents a connected graph.
    graph = {
        // clang-format off
        {NO_EDGE, 1,       NO_EDGE},
        {1,       NO_EDGE, 1      },
        {NO_EDGE, 1,       NO_EDGE}
        // clang-format on
    };
    g1.loadGraph(graph);  // Load the graph to the object.

    cout << g1 << endl;  // Should print the matrix of the graph: [X, 1, X], [1, X, 1], [X, 1, X]

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    // 3x3 matrix that represents a weighted connected graph.
    vector<vector<int>> weightedGraph = {
        // clang-format off
        {NO_EDGE, 1,       1      },
        {1,       NO_EDGE, 2      },
        {1,       2,       NO_EDGE}
        // clang-format on
    };
    Graph g2;
    g2.loadGraph(weightedGraph);  // Load the graph to the object.

    Graph g3 = g1 + g2;  // Add the two graphs together.
    cout << g3 << endl;  // Should print the matrix of the graph: [X, 2, 1], [2, X, 3], [1, 3, X]
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

    g1 *= -2;            // Multiply the graph by -2.
    cout << g1 << endl;  // Should print the matrix of the graph: [X, -2, X], [-2, X, -2], [X, -2, X]
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

    g1 /= -2;
    Graph g4 = g1 * g2;  // Multiply the two graphs together.
    cout << g4 << endl;  // Should print the multiplication of the matrices of g1 and g2: [X, X, 2], [1, X, 1], [1, X, X]
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

    // 5x5 matrix that represents a connected graph.
    graph2 = {
        // clang-format off
        {NO_EDGE, 1,       NO_EDGE, NO_EDGE, 1      },
        {1,       NO_EDGE, 1,       NO_EDGE, NO_EDGE},
        {NO_EDGE, 1,       NO_EDGE, 1,       NO_EDGE},
        {NO_EDGE, NO_EDGE, 1,       NO_EDGE, 1      },
        {1,       NO_EDGE, NO_EDGE, 1,       NO_EDGE}
        // clang-format on
    };
    Graph g5;
    g5.loadGraph(graph2);  // Load the graph to the object.
    try {
        Graph g6 = g5 * g1;  // Multiply the two graphs together.
    } catch (const std::invalid_argument &e) {
        cout << e.what() << endl;  // Should print "The number of columns in the first matrix must be equal to the number of rows in the second matrix."
    }
}
