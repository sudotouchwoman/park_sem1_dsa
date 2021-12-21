#pragma once

#include <system_error>

#include "IGraph.hpp"

class ListGraph : public IGraph {
public:
    ListGraph() = default;
    explicit ListGraph(const size_t vertices) : vertices(vertices), adjacencyLists(vertices) {}
    ListGraph(const IGraph & other);
    ~ListGraph() = default;
    void AddEdge(int from, int to) override;
    int VerticesCount() const override { return vertices; }
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
private:
    size_t vertices = 0lu;
    std::vector<std::vector<int>> adjacencyLists;
private:
    bool hasEdge(const int from, const int to) const;
    void checkVertex(const int vertex) const;
};
