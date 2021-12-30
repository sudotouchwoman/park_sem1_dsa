#include <bits/stdc++.h>

// Interface declaration
struct IGraph {
    virtual ~IGraph() {}
    // add edge from from to to
    virtual void AddEdge(int from, int to, size_t weight) = 0;
    // return the total number of vertices
    virtual size_t VerticesCount() const  = 0;
    // obtain the vertices adjacent to the given one
    virtual std::vector<std::pair<int, size_t>> GetNextVertices(int vertex) const = 0;
    virtual std::vector<std::pair<int, size_t>> GetPrevVertices(int vertex) const = 0;
    virtual int ShortestPath(const int from, const int to, const int K) const = 0;
};

// I considered ListGraph to be the most useful in this case,
// as the edge lookup takes O(1) time
class ListGraph : public IGraph {
public:
    ListGraph() = default;
    explicit ListGraph(const size_t vertices) : vertices(vertices), adjacencyLists(vertices) {}
    ListGraph(const IGraph & other);
    ~ListGraph() = default;
    void AddEdge(int from, int to, size_t weight) override;
    size_t VerticesCount() const override { return vertices; }
    virtual std::vector<std::pair<int, size_t>> GetNextVertices(int vertex) const override;
    virtual std::vector<std::pair<int, size_t>> GetPrevVertices(int vertex) const override;
    virtual int ShortestPath(const int from, const int to, const int K) const override;
private:
    size_t vertices = 0lu;
    std::vector<std::vector<std::pair<int, size_t>>> adjacencyLists;
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
            for (const auto & v: other.GetNextVertices(i))
                adjacencyLists[i].emplace_back(v);
        }
    }

void ListGraph::AddEdge(const int from, const int to, size_t weight) {
    checkVertex(from);
    checkVertex(to);
    adjacencyLists[from].emplace_back(to, weight);
}

std::vector<std::pair<int, size_t>> ListGraph::GetNextVertices(const int vertex) const {
    checkVertex(vertex);
    return adjacencyLists[vertex];
}

std::vector<std::pair<int, size_t>> ListGraph::GetPrevVertices(const int vertex) const {
    checkVertex(vertex);
    std::vector<std::pair<int, size_t>> preceding;

    for (size_t v = 0; v < adjacencyLists.size(); ++v) {
        for (const auto & adjacent : adjacencyLists[v])
            if (adjacent.first == vertex)
                preceding.emplace_back(v, adjacent.second);
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
        [to](const std::pair<int, size_t> & a) { return to == a.first; }
    );
}

void ListGraph::checkVertex(const int vertex) const {
    if (vertex < static_cast<int>(vertices) and vertex >= 0) return;
    throw std::runtime_error("Invalid vertex ID!");
}

int ListGraph::ShortestPath(const int from, const int to, const int K) const {
    checkVertex(from);
    checkVertex(to);

    // here I assume that the path lengths will be
    // unsigned (well these are kinda road distances)
    // and, according to the Dijkstra's algorithm,
    // assume that each vertex is unreachable from the initial one
    // this said, the distance from 'from' to each vertex
    // (except itself, of course)
    // is INF (here I used LONG_MAX macro)
    std::vector<int> paths(vertices, INT_MAX);
    std::vector<size_t> path_lengths(vertices, 0lu);
    paths[from] = 0;

    // this set is the optimization
    // useful for sparse graphs (when 
    // |E| is ~O(|V|), i.e. linear with number of vertices)
    std::set<std::pair<int, size_t>> aux;
    // set is a red-black tree
    // and is ordered, so one can
    // use it as a queue
    // actually, a heap would be great DS to use here
    aux.emplace(from, 0);

    while (not aux.empty()) {
        const int curr_v = aux.begin()->first;
        aux.erase(aux.begin());

        for (const auto & next: GetNextVertices(curr_v)) {

            // next is a pair of shape [vertex, distance from curr_v to that vertex]
            // if the new distance is not shorter, pass
            // otherwise we should update the aux queue
            if (paths[next.first] <= paths[curr_v] + next.second) continue;
            
            // update the distance to the vertex,
            // i.e., remove the current corresponding pair
            // and emplace the pair with updated distance
            aux.erase({next.first, paths[next.first]});
            paths[next.first] = paths[curr_v] + next.second;

            // remember the length of the current path
            path_lengths[next.first] = path_lengths[curr_v] + 1;

            aux.emplace(next.first, paths[next.first]);
        }
    }

    if (path_lengths[to] > K) return -1;
    return paths[to];
}

void run_shortest_path(std::istream & in, std::ostream & out) {
    size_t vertices = 0, edges = 0;
    int rick = 0, conference = 0, K = 0;

    in >> vertices >> edges >> K;
    in >> rick >> conference;

    ListGraph g(vertices);

    for (size_t i = 0; i < edges; ++i) {
        size_t weight = 0;
        int from = 0, to = 0;
        in >> from >> to >> weight;
        // remember that the graph
        // is directed!
        g.AddEdge(from - 1, to - 1, weight);
    }

    out << g.ShortestPath(rick - 1, conference - 1, K);
}

int main(int argc, char * argv[]) {
    run_shortest_path(std::cin, std::cout);
    return EXIT_SUCCESS;
}
