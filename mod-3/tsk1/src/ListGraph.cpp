#include "ListGraph.hpp"

#include <algorithm>

ListGraph::ListGraph(const IGraph & other) :
    vertices(other.VerticesCount()),
    adjacencyLists(vertices)
    {
        for (size_t i = 0; i < vertices; ++i) {
            for (const int v: other.GetNextVertices(i))
                adjacencyLists[i].push_back(v);
        }
    }

void ListGraph::AddEdge(const int from, const int to) {
    checkVertex(from);
    checkVertex(to);

    adjacencyLists[from].push_back(to);
}

std::vector<int> ListGraph::GetNextVertices(const int vertex) const {
    checkVertex(vertex);
    return adjacencyLists[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(const int vertex) const {
    std::vector<int> preceding;

    for (size_t v = 0; v < adjacencyLists.size(); ++v) {
        for (const int adjacent : adjacencyLists[v])
            if (adjacent == vertex)
                preceding.push_back(v);
    }

    return preceding;
}

bool ListGraph::hasEdge(const int from, const int to) const {
    checkVertex(from);
    checkVertex(to);

    return adjacencyLists[from].cend() != std::find_if(
        adjacencyLists[from].cbegin(),
        adjacencyLists[from].cend(),
        [to](const int a) { return to == a; }
    );
}

void ListGraph::checkVertex(const int vertex) const {
    if (vertex < static_cast<int>(vertices) and vertex >= 0) return;
    throw std::runtime_error("Invalid vertex ID!");
}
