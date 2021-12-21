#include "IGraph.hpp"

void BFS(const IGraph & g, const std::function<void(const int &)> & visitor) {
    std::vector<bool> visited(g.VerticesCount(), false);

    auto BFS_internal = [&visited, &g, &visitor](const int vertex) {
        std::queue<int> to_visit;
        to_visit.push(vertex);
        visited[vertex] = true;

        while(not to_visit.empty()) {
            const int current_v = to_visit.front();
            to_visit.pop();

            visitor(current_v);

            for (const int next_v : g.GetNextVertices(current_v)) {
                if (visited[next_v]) continue;
                visited[next_v] = true;
                to_visit.push(next_v);
            }
        }
    };

    for (size_t i = 0; i < visited.size(); ++i) {
        if (visited[i]) continue;
        BFS_internal(i);
    }
}