#include <iostream>
#include <algorithm>

#include "ListGraph.hpp"
#include "MatrixGraph.hpp"
#include "SetGraph.hpp"
#include "ArcGraph.hpp"

void run_graphs(std::istream & in, std::ostream & out) {
    size_t n_vertices, n_commands;
    in >> n_vertices >> n_commands;

    // create some graph from user input
    // then print its adjacency lists, pretty-formatted
    MatrixGraph MG(n_vertices);
    out << "Created Graph on " << n_vertices << " vertices. Waiting for " << n_commands << " commands.\n";

    for (size_t i = 0; i < n_commands; ++i) {
        int from, to;
        in >> from >> to;
        if (from < 0 or to < 0) break;

        MG.AddEdge(from, to);

        for (size_t i = 0; i < n_vertices; ++i) {

            out << "[ID " << i << "] [NEXT:";
            const auto next = MG.GetNextVertices(i);

            std::for_each(
                next.cbegin(),
                next.cend(),
                [&out](const int x) { out << ' ' << x; });

            out << "][PREV:";
            const auto prev = MG.GetPrevVertices(i);
            
            std::for_each(
                prev.cbegin(),
                prev.cend(),
                [&out](const int x) { out << ' ' << x; });
            
            out << ']' << '\n';
        }
    }

    // the first check is the list graph
    ListGraph LG(MG);
    out << "Created ListGraph from MatrixGraph\nIt contains:\n";

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

    // the second check is set graph
    // it utilizes std::map to have a O(log(n)) lookup time
    // this is better for adjacency lookup
    // and is a favourite of mine
    SetGraph SG(MG);
    out << "Created SetGraph from MatrixGraph\nIt contains:\n";

    for (size_t i = 0; i < n_vertices; ++i) {

            out << "[ID " << i << "] [NEXT:";
            const auto next = SG.GetNextVertices(i);
            std::for_each(
                next.cbegin(),
                next.cend(),
                [&out](const int x) { out << ' ' << x;});

            out << "][PREV:";
            const auto prev = SG.GetPrevVertices(i);
            std::for_each(
                prev.cbegin(),
                prev.cend(),
                [&out](const int x) { out << ' ' << x;});
            
            out << ']' << '\n';
        }

    // finally, check the arc graph
    // this one fits sparse graphs best
    // as it basically only contains edges
    // I guess, if one added set of nodes for quicker lookup
    // this would be the best one for containing really sparse graphs
    // the drawback here is the edge search time
    // then, well, why not use map to contain the edges, where key would be
    // the edge pair and value will be, well, some edge properties
    ArcGraph AG(MG);
    out << "Created ArcGraph from MatrixGraph\nIt contains:\n";

    for (size_t i = 0; i < n_vertices; ++i) {

            out << "[ID " << i << "] [NEXT:";
            const auto next = AG.GetNextVertices(i);
            std::for_each(
                next.cbegin(),
                next.cend(),
                [&out](const int x) { out << ' ' << x;});

            out << "][PREV:";
            const auto prev = AG.GetPrevVertices(i);
            std::for_each(
                prev.cbegin(),
                prev.cend(),
                [&out](const int x) { out << ' ' << x;});
            
            out << ']' << '\n';
        }
}

int main(int argc, char* argv[]) {
    run_graphs(std::cin, std::cout);
    return EXIT_SUCCESS;
}
