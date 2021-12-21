#pragma once

#include <system_error>

#include "IGraph.hpp"

class ArcGraph : public IGraph {
public:
    ArcGraph() = default;
    explicit ArcGraph(const size_t vertices) : vertices(vertices) {}
    ArcGraph(const IGraph & other);
    ~ArcGraph() = default;
    void AddEdge(int from, int to) override;
    int VerticesCount() const override { return vertices; }
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
private:
    size_t vertices = 0lu;
    std::vector<std::pair<int, int>> duples;
private:
    bool hasEdge(const int from, const int to) const;
    void checkVertex(const int vertex) const;
};
