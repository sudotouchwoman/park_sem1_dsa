#pragma once

#include <system_error>

#include "IGraph.hpp"

class MatrixGraph : public IGraph {
public:
    MatrixGraph() = default;
    explicit MatrixGraph(const size_t vertices) : vertices(vertices), adjacencyMatrix(vertices, std::vector<int>(vertices)) {}
    MatrixGraph(const IGraph & other);
    ~MatrixGraph() = default;
    void AddEdge(int from, int to) override;
    int VerticesCount() const override { return vertices; }
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
private:
    size_t vertices = 0lu;
    std::vector<std::vector<int>> adjacencyMatrix;
private:
    bool hasEdge(const int from, const int to) const;
    void checkVertex(const int vertex) const;
};
