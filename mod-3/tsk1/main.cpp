#include <iostream>
#include <algorithm>

#include "ListGraph.hpp"

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
