#include "ArcGraph.hpp"

#include <algorithm>

ArcGraph::ArcGraph(const IGraph & other) :
    vertices(other.VerticesCount())
    {
        for (size_t i = 0; i < vertices; ++i) {
            for (const auto & v: other.GetNextVertices(i))
                duples.emplace_back(i, v);
        }
    }

void ArcGraph::AddEdge(const int from, const int to) {
    checkVertex(from);
    checkVertex(to);
    duples.emplace_back(from, to);
}

std::vector<int> ArcGraph::GetNextVertices(const int vertex) const {
    checkVertex(vertex);
    std::vector<int> next;

    std::for_each(
        duples.cbegin(),
        duples.cend(),
        [&next, vertex](const std::pair<int, int> & duple) {
            if (duple.first != vertex) return;
            next.emplace_back(duple.second);
        }
    );

    return next;
}

std::vector<int> ArcGraph::GetPrevVertices(const int vertex) const {
    checkVertex(vertex);
    std::vector<int> preceding;

    std::for_each(
        duples.cbegin(),
        duples.cend(),
        [&preceding, vertex](const std::pair<int, int> & duple) {
            if (duple.second != vertex) return;
            preceding.emplace_back(duple.first);
        }
    );

    return preceding;
}

bool ArcGraph::hasEdge(const int from, const int to) const {
    checkVertex(from);
    checkVertex(to);

    return duples.cend() != std::find_if(
        duples.cbegin(),
        duples.cend(),
        [from, to](const std::pair<int, int> & duple) {
            const bool first_match = from == duple.first;
            const bool second_match = from == duple.second;
            return first_match and second_match;
        }
    );
}

void ArcGraph::checkVertex(const int vertex) const {
    if (vertex < static_cast<int>(vertices) and vertex >= 0) return;
    throw std::runtime_error("Invalid vertex ID!");
}
