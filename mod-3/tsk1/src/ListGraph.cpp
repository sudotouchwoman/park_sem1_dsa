#include "ListGraph.hpp"

ListGraph::ListGraph(const IGraph & other) :
    vertices(other.VerticesCount()),
    adjacencyLists(vertices) {
        for (size_t i = 0; i < vertices; ++i) {
            for (const int v: other.GetNextVertices(i))
                adjacencyLists[i].push_back(v);
        }
    }

void ListGraph::AddEdge(const int from, const int to) {
    if (from > static_cast<int>(vertices)) throw std::runtime_error("Invalid vertex id");
    if (to > static_cast<int>(vertices)) throw std::runtime_error("Invalid vertex id");

    adjacencyLists[from].push_back(to);
}

std::vector<int> ListGraph::GetNextVertices(const int vertex) const {
    return adjacencyLists.at(vertex);
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