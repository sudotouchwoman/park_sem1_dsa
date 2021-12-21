#include <iostream>
#include <cassert>
#include <list>
#include <vector>
#include <unordered_set>
#include <algorithm>

class IGraph {
public:
    virtual ~IGraph() {}
        // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

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
};

ListGraph::ListGraph(const IGraph & other) :
    vertices(other.VerticesCount()),
    adjacencyLists(vertices) {
        for (size_t i = 0; i < vertices; ++i) {
            for (const int v: other.GetNextVertices(i))
                adjacencyLists[i].push_back(v);
        }
    }

void ListGraph::AddEdge(const int from, const int to) {
    if (from > vertices) throw std::runtime_error("Invalid vertex id");
    if (to > vertices) throw std::runtime_error("Invalid vertex id");

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

void run_graphs(std::istream & in, std::ostream & out) {
    size_t n_vertices;
    int from, to;
    in >> n_vertices;

    ListGraph LG(n_vertices);
    out << "Created Graph on " << n_vertices << " vertices\n";
    while (in >> from >> to) {
        if (from < 0 or to < 0) break;

        LG.AddEdge(from, to);
        for (size_t i = 0; i < n_vertices; ++i) {

            out << "[ID " << i << "] [NEXT:";
            const auto next = LG.GetNextVertices(i);
            std::for_each(
                next.cbegin(),
                next.cend(),
                [&out](const int x) { out << ' ' << x;});

            out << "][PREV:";
            const auto prev = LG.GetPrevVertices(i);
            std::for_each(
                prev.cbegin(),
                prev.cend(),
                [&out](const int x) { out << ' ' << x;});
            
            out << ']' << '\n';
        }
    }
}

int main(int argc, char* argv[]) {
    run_graphs(std::cin, std::cout);
    return EXIT_SUCCESS;
}
