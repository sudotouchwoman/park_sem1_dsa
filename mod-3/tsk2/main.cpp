#include <bits/stdc++.h>

// Interface declaration
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

// I considered ListGraph to be the most useful in this case,
// as the edge lookup takes O(1) time
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

// methods definition (copied from the tsk1 libs)
// copy constructor
ListGraph::ListGraph(const IGraph & other) :
    vertices(other.VerticesCount()),
    adjacencyLists(vertices)
    {
        for (size_t i = 0; i < vertices; ++i) {
            for (const int v: other.GetNextVertices(i))
                adjacencyLists[i].emplace_back(v);
        }
    }

void ListGraph::AddEdge(const int from, const int to) {
    checkVertex(from);
    checkVertex(to);
    adjacencyLists[from].emplace_back(to);
}

std::vector<int> ListGraph::GetNextVertices(const int vertex) const {
    checkVertex(vertex);
    return adjacencyLists[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(const int vertex) const {
    checkVertex(vertex);
    std::vector<int> preceding;

    for (size_t v = 0; v < adjacencyLists.size(); ++v) {
        for (const int adjacent : adjacencyLists[v])
            if (adjacent == vertex)
                preceding.emplace_back(v);
    }

    return preceding;
}

// check is there is an edge between two given nodes
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

// I had to modify the BFS a bit to let it take two arguments, the source and
// destination to watch
// the great thing here is that as soon as we find
// any path with BFS algorithm, we can be sure this is
// the shortest one and thus stop searching
size_t count_shortest_paths(const IGraph & g, const int from, const int to) {
    size_t path_counts = 0;
    const int vertices = g.VerticesCount();
    // the maximal possible path (actually impossibly long)
    // is the number of edges in a complete graph
    // for the given number of vertices
    size_t min_path = vertices * vertices;

    // we will just perform bfs until the path shorter than min_path is found
    std::queue<std::pair<int, size_t>> to_visit;
    to_visit.emplace(from, 0);

    while(not to_visit.empty()) {
        const auto current_pair = to_visit.front();
        to_visit.pop();
        // the trick here is that we stop adding vertices
        // into the queue once any path
        // shorter than the min_path is found
        if (current_pair.second > min_path) continue;

        // ordinary bfs but no need for outer loop as we are sure that such
        // path will be found
        for (const int next_v : g.GetNextVertices(current_pair.first)) {
            to_visit.emplace(next_v, current_pair.second + 1);
            if (next_v != to) continue;
            min_path = current_pair.second;
            ++path_counts;
        }
    }
    return path_counts;
}

void run_shortest_paths(std::istream & in, std::ostream & out) {
    int vertices = 0, edges = 0;
    int from = 0, to = 0;
    in >> vertices >> edges;

    ListGraph g(vertices);

    for (size_t i = 0; i < edges; ++i) {
        in >> from >> to;
        g.AddEdge(from, to);
    }

    in >> from >> to;
    out << count_shortest_paths(g, from, to);
}

void test_shortest_paths() {
    // make sure the example from the contest works out correctly
    std::stringstream in, out;

    in << "4 5 " << "0 1 0 2 1 2 1 3 2 3" << " 0 3";
    run_shortest_paths(in, out);
    assert(out.str() == "2");

    std::cerr << "OK\n";
}

int main(int argc, char* argv[]) {
    // test_shortest_paths();
    run_shortest_paths(std::cin, std::cout);
    return EXIT_SUCCESS;
}