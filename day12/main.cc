#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

#include <boost/config.hpp>
#include <fstream>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_traits.hpp>

void printm(std::vector<std::vector<char>>& hmap)
{
    for (auto& v : hmap) {
        for (auto& c : v) {
            std::cout << c;
        }
        std::cout << "\n";
    }
    std::cout << "\n\r";
}

std::vector<std::vector<char>> parse(std::string filename)
{
    std::fstream fs(filename);
    std::vector<std::vector<char>> hmap;

    std::string line;
    fs >> line;
    while (!fs.eof()) {
        hmap.push_back(std::vector<char>(line.begin(), line.end()));
        fs >> line;
    }

    std::cout << hmap.size() << "x" << hmap[0].size() << "\n";

    return hmap;
}

std::pair<int, int> find_start(std::vector<std::vector<char>>& hmap, char goal)
{
    for (size_t i = 0; i < hmap.size(); ++i) {
        for (size_t j = 0; j < hmap[0].size(); ++j) {
            if (hmap[i][j] == goal) {
                return std::pair<int, int>(i, j);
            }
        }
    }
    std::cout << "Warning! Start not found.\n\r";
    return std::pair<int, int>(-1, -1);
}

template <class T> void printv(std::vector<T> vec)
{
    std::cout << "Vec size: " << vec.size() << "\n";
    for (auto v : vec) {
        std::cout << v << ", ";
    }
    std::cout << "\n\r";
}

using namespace boost;
typedef std::pair<int, int> Edge;
typedef adjacency_list<listS, vecS, directedS, no_property,
    property<edge_weight_t, int>>
    graph_t;
typedef graph_traits<graph_t>::vertex_descriptor vertex_descriptor;
typedef graph_traits<graph_t>::edge_descriptor edge_descriptor;

void to_dot_file(std::vector<std::vector<char>>& hmap, graph_t g,
    std::vector<vertex_descriptor> p,
    property_map<graph_t, edge_weight_t>::type weightmap)
{
    std::ofstream dot_file("dijkstra-eg.dot");

    dot_file << "digraph D {\n"
             << "  rankdir=LR\n"
             << "  size=\"4,3\"\n"
             << "  ratio=\"fill\"\n"
             << "  edge[style=\"bold\"]\n"
             << "  node[shape=\"circle\"]\n";

    std::vector<char> name;
    for (auto& m : hmap) {
        for (auto c : m) {
            name.push_back(c);
        }
    }

    graph_traits<graph_t>::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
        graph_traits<graph_t>::edge_descriptor e = *ei;
        graph_traits<graph_t>::vertex_descriptor u = source(e, g),
                                                 v = target(e, g);
        dot_file << name[u] << " -> " << name[v] << "[label=\""
                 << get(weightmap, e) << "\"";
        if (p[v] == u)
            dot_file << ", color=\"black\"";
        else
            dot_file << ", color=\"grey\"";
        dot_file << "]";
    }
    dot_file << "}";
}

inline int d1(std::vector<std::vector<char>>& hmap, Edge e)
{
    return e.first * hmap[0].size() + e.second;
}

int calc_dist(
    Edge start, Edge end, graph_t g, std::vector<std::vector<char>>& hmap)
{
    std::vector<vertex_descriptor> p(num_vertices(g));
    std::vector<int> d(num_vertices(g));
    vertex_descriptor s = vertex(d1(hmap, start), g);
    dijkstra_shortest_paths(g, s, predecessor_map(&p[0]).distance_map(&d[0]));

    return d[d1(hmap, end)];
}

int part2(Edge end, graph_t g, std::vector<std::vector<char>>& hmap)
{
    std::vector<int> res;
    for (std::size_t i = 0; i < hmap.size(); ++i) {
        for (std::size_t j = 0; j < hmap[0].size(); ++j) {
            if (hmap[i][j] == 'a') {
                Edge start(i, j);
                res.push_back(calc_dist(start, end, g, hmap));
            }
        }
    }
    std::sort(res.begin(), res.end());
    return res[0];
}

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    auto hmap = parse("../inp_1.txt");

    std::pair<int, int> start_first = find_start(hmap, 'S');
    std::pair<int, int> end = find_start(hmap, 'E');
    // std::cout << "Start: (" << start_first.first << ", " <<
    // start_first.second
    // << ")\n";
    // std::cout << "End: (" << end.first << ", " << end.second << ")\n\r";

    hmap[start_first.first][start_first.second] = 'a';
    hmap[end.first][end.second] = 'z';

    // printm(hmap);

    const int num_nodes = hmap.size() * hmap[0].size();

    graph_t g(num_nodes);
    property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);

    for (std::size_t i = 0; i < hmap.size(); ++i) {
        for (std::size_t j = 0; j < hmap[0].size(); ++j) {
            Edge start(i, j);
            Edge left_pos = Edge(start.first, start.second - 1);
            if (left_pos.second >= 0) {
                if (hmap[left_pos.first][left_pos.second]
                        - hmap[start.first][start.second]
                    <= 1) {
                    edge_descriptor e;
                    bool inserted;
                    boost::tie(e, inserted)
                        = add_edge(d1(hmap, start), d1(hmap, left_pos), g);
                    weightmap[e] = 1;
                }
            }

            Edge right_pos = Edge(start.first, start.second + 1);
            if (right_pos.second < (int)hmap[0].size()) {
                if (hmap[right_pos.first][right_pos.second]
                        - hmap[start.first][start.second]
                    <= 1) {
                    edge_descriptor e;
                    bool inserted;
                    boost::tie(e, inserted)
                        = add_edge(d1(hmap, start), d1(hmap, right_pos), g);
                    weightmap[e] = 1;
                }
            }

            Edge up_pos = Edge(start.first - 1, start.second);
            if (up_pos.first >= 0) {
                if (hmap[up_pos.first][up_pos.second]
                        - hmap[start.first][start.second]
                    <= 1) {
                    edge_descriptor e;
                    bool inserted;
                    boost::tie(e, inserted)
                        = add_edge(d1(hmap, start), d1(hmap, up_pos), g);
                    weightmap[e] = 1;
                }
            }

            Edge down_pos = Edge(start.first + 1, start.second);
            if (down_pos.first < (int)hmap.size()) {
                if (hmap[down_pos.first][down_pos.second]
                        - hmap[start.first][start.second]
                    <= 1) {
                    edge_descriptor e;
                    bool inserted;
                    boost::tie(e, inserted)
                        = add_edge(d1(hmap, start), d1(hmap, down_pos), g);
                    weightmap[e] = 1;
                }
            }
        }
    }

    int d1 = calc_dist(start_first, end, g, hmap);
    int d2 = part2(end, g, hmap);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration
        = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Part 1: " << d1 << "\n";
    std::cout << "Part 2: " << d2 << "\n";
    std::cout << duration.count() << " μs\n\r";
}
