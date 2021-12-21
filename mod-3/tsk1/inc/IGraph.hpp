#pragma once
#include <vector>

struct IGraph {
    virtual ~IGraph() {}
    // add edge from from to to
    virtual void AddEdge(int from, int to) = 0;
    // return the total number of vertices
    virtual int VerticesCount() const  = 0;
    // obtain the vertices adjacent to the given one
    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};
