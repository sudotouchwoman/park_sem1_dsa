#include "SetGraph.hpp"

#include <algorithm>

SetGraph::SetGraph(const IGraph & other) :
    vertices(other.VerticesCount()),
    adjacencySets(vertices)
    {
        for (size_t i = 0; i < vertices; ++i) {
            for (const int j: other.GetNextVertices(i))
                ++adjacencySets[i][j];
        }
    }

void SetGraph::AddEdge(const int from, const int to) {
    checkVertex(from);
    checkVertex(to);
    ++adjacencySets[from][to];
}

std::vector<int> SetGraph::GetNextVertices(const int vertex) const {
    checkVertex(vertex);
    std::vector<int> next;

    for (const auto & adjacent : adjacencySets[vertex]) {
        for (size_t i = 0; i < adjacent.second; ++i)
            next.push_back(adjacent.first);
    }

    return next;
}

std::vector<int> SetGraph::GetPrevVertices(const int vertex) const {
    checkVertex(vertex);
    std::vector<int> preceding;

    // I expected STL to let me write expressive code
    // but this is like
    // such a mess
    for (size_t v = 0; v < vertices; ++v) {
        if (adjacencySets[v].count(vertex) == 0) continue;
        // being multigraph, this one should be able to remember
        // the multiplicity of each pair of adjacent vertices
        // this said, we should push the preceding vertex
        // (possibly) several times
        // for heaven's sake what did I bring on this cursed land
        for (size_t i = 0; i < adjacencySets[v].at(vertex); ++i)
            preceding.push_back(v);
    }

    return preceding;
}

bool SetGraph::hasEdge(const int from, const int to) const {
    checkVertex(from);
    checkVertex(to);

    return adjacencySets[from].find(to) != adjacencySets[from].cend();
}

void SetGraph::checkVertex(const int vertex) const {
    if (vertex < static_cast<int>(vertices) and vertex >= 0) return;
    throw std::runtime_error("Invalid vertex ID!");
}
