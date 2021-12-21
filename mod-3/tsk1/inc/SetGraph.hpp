#pragma once

#include <system_error>
#include <map>

#include "IGraph.hpp"

class SetGraph : public IGraph {
public:
    SetGraph() = default;
    explicit SetGraph(const size_t vertices) : vertices(vertices), adjacencySets(vertices) {}
    SetGraph(const IGraph & other);
    ~SetGraph() = default;
    void AddEdge(int from, int to) override;
    int VerticesCount() const override { return vertices; }
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
private:
    size_t vertices = 0lu;
    std::vector<std::map<int, size_t>> adjacencySets;
private:
    bool hasEdge(const int from, const int to) const;
    void checkVertex(const int vertex) const;
};
