#include "MatrixGraph.hpp"

#include <algorithm>

MatrixGraph::MatrixGraph(const IGraph & other) :
    vertices(other.VerticesCount()),
    adjacencyMatrix(vertices, std::vector<int>(vertices))
    {
        for (size_t i = 0; i < vertices; ++i) {
            for (const int j: other.GetNextVertices(i))
                ++adjacencyMatrix[i][j];
        }
    }

void MatrixGraph::AddEdge(const int from, const int to) {
    checkVertex(from);
    checkVertex(to);
    ++adjacencyMatrix[from][to];
}

std::vector<int> MatrixGraph::GetNextVertices(const int vertex) const {
    checkVertex(vertex);
    std::vector<int> next;
    size_t i = 0;

    // STL for life
    // I wonder whether these algos
    // make the code easier to read, bc personally
    // they do not in terms of C++, but do in terms
    // of programming in general, because this one is basically a map
    std::for_each(
        adjacencyMatrix[vertex].cbegin(),
        adjacencyMatrix[vertex].cend(),
        [&i, &next](const int multiplicity) {
            for (int x = 0; x < multiplicity; ++x)
                next.emplace_back(i);
            ++i;
            }
    );

    return next;
}

std::vector<int> MatrixGraph::GetPrevVertices(const int vertex) const {
    checkVertex(vertex);
    std::vector<int> preceding;

    // I wanted to use one of the STL algos here
    // but it is quite tricky to define
    // the iterator
    // over the matrix row-wise
    // these are basically independent objects
    for (size_t i = 0; i < vertices; ++i) {
        const int multiplicity = adjacencyMatrix[i][vertex];
        for (int x = 0; x < multiplicity; ++x)
            preceding.emplace_back(i);
    }

    return preceding;
}

bool MatrixGraph::hasEdge(const int from, const int to) const {
    checkVertex(from);
    checkVertex(to);
    return adjacencyMatrix[from][to] != 0;
}

void MatrixGraph::checkVertex(const int vertex) const {
    if (vertex < static_cast<int>(vertices) and vertex >= 0) return;
    throw std::runtime_error("Invalid vertex ID!");
}
