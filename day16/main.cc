#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <boost/graph/graphviz.hpp>
#include <chrono>
#include <complex>
#include <fstream>
#include <iostream>
#include <numeric>
#include <set>
#include <thread>
#include <vector>

#define RANGE(VAR, END)                                                       \
    int VAR = 0;                                                              \
    VAR < END;                                                                \
    ++VAR
using namespace std;
typedef unsigned long long int bigint;
typedef std::complex<int> coord;

struct valve {
    int id;
    string name;
    int flow_rate;
    vector<string> named_edges;
    vector<int> edges;

    bool open = false;

    valve(int i, string n, int fr, vector<string> e)
        : id(i)
        , name(n)
        , flow_rate(fr)
        , named_edges(e) {};
};

std::vector<valve> parse(std::string filename, int& start)
{
    std::fstream fs(filename);
    std::vector<valve> res;

    map<string, int> name_to_id;

    string line;
    vector<string> lsplit;
    int id = 0;
    while (getline(fs, line)) {
        boost::split(lsplit, line, boost::is_any_of(" "));
        vector<string> edges(lsplit.begin() + 2, lsplit.end());
        res.push_back(valve(id, lsplit[0], stoi(lsplit[1]), edges));

        name_to_id[lsplit[0]] = id;
        id++;
    }

    start = name_to_id["AA"];

    for (auto& val : res) {
        for (auto e : val.named_edges) {
            val.edges.push_back(name_to_id[e]);
        }
    }

    return res;
}

typedef int t_weight;
typedef boost::property<boost::edge_weight_t, t_weight> EdgeWeightProperty;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
    boost::no_property, EdgeWeightProperty>
    graph_t;
typedef boost::exterior_vertex_property<graph_t, t_weight> DistanceProperty;
typedef DistanceProperty::matrix_map_type DistanceMatrixMap;
typedef boost::property_map<graph_t, boost::edge_weight_t>::type WeightMap;
typedef DistanceProperty::matrix_type DistanceMatrix;
DistanceMatrix floyed_warshall(vector<valve>& valves)
{

    graph_t graph(valves.size());

    for (auto& val : valves) {
        for (int e : val.edges) {
            boost::add_edge(val.id, e, 1, graph);
        }
    }

    WeightMap weight_pmap = boost::get(boost::edge_weight, graph);

    DistanceMatrix distances(num_vertices(graph));
    DistanceMatrixMap dm(distances, graph);

    // find all pairs shortest paths
    floyd_warshall_all_pairs_shortest_paths(
        graph, dm, boost::weight_map(weight_pmap));

    if (false) {
        std::ofstream ofs("network.dot");
        vector<string> names;
        for (auto val : valves) {
            names.push_back(val.name + ":" + to_string(val.id) + " "
                + to_string(val.flow_rate));
        }
        write_graphviz(ofs, graph, boost::make_label_writer(names.data()));
        ofs.close();
    }

    return distances;
}

inline int calc_flow(
    vector<valve>& valves, vector<valve>& ivalves, int flow, int dt)
{
    for (auto& val : ivalves) {
        if (valves[val.id].open) {
            flow += dt * val.flow_rate;
        }
    }

    return flow;
}

int find_path(int pos, vector<valve> valves, vector<valve>& ivalves,
    DistanceMatrix distances, int time, int flow, int n_closed)
{
    valves[pos].open = true;
    if (time <= 1) {
        return flow;
    }

    if (n_closed <= 0) {
        return calc_flow(valves, ivalves, flow, time);
    }

    // Worst case stay where you are and do nothing.
    vector<int> res(n_closed, calc_flow(valves, ivalves, flow, time));

    int idx = 0;
    for (auto v : ivalves) {
        int dist = distances[pos][v.id] + 1;
        if (dist < time && !valves[v.id].open) {
            res[idx] = find_path(v.id, valves, ivalves, distances, time - dist,
                calc_flow(valves, ivalves, flow, dist), n_closed - 1);
            idx++;
        }
    }

    return *std::max_element(res.begin(), res.end(),
        [](const int p1, const int p2) { return p1 < p2; });
}

int part1(vector<valve> valves, int start, DistanceMatrix& distances)
{
    int time_lim = 30;

    vector<valve> ivalves;

    for (auto val : valves) {
        if (val.flow_rate > 0) {
            ivalves.push_back(valve(val)); // Free error otherwise.
        }
    }

    return find_path(
        start, valves, ivalves, distances, time_lim++, 0, ivalves.size());
}

int find_path2(int pos, vector<valve> valves, vector<valve>& ivalves,
    DistanceMatrix distances, int time, int flow, int n_closed)
{
    cout << pos << ", time = " << time << ", flow = " << flow << "\n";
    valves[pos].open = true;
    if (time <= 1) {
        return flow;
    }

    if (n_closed <= 0) {
        return calc_flow(valves, ivalves, flow, time);
    }

    // Worst case stay where you are and do nothing.
    vector<int> res(n_closed, calc_flow(valves, ivalves, flow, time));

    int idx = 0;
    for (auto v : ivalves) {
        int dist = distances[pos][v.id] + 1;
        if (dist < time && !valves[v.id].open) {
            res[idx] = find_path(v.id, valves, ivalves, distances, time - dist,
                calc_flow(valves, ivalves, flow, dist), n_closed - 1);
            idx++;
        }
    }

    return *std::max_element(res.begin(), res.end(),
        [](const int p1, const int p2) { return p1 < p2; });
}

int part2(vector<valve> valves, int start, DistanceMatrix& distances)
{
    int time_lim = 24;

    vector<valve> ivalves;

    for (auto val : valves) {
        if (val.flow_rate > 0) {
            ivalves.push_back(valve(val)); // Free error otherwise.
        }
    }

    vector<vector<int>> possible_paths;
    for (RANGE(i, 10)) {
        possible_paths[i].push_back(find_path2(
            start, valves, ivalves, distances, time_lim++, 0, ivalves.size()));
    }

    return find_comb(possible_paths);
}

int main()
{
    int start;
    auto valves = parse("../inp_1.txt", start);
    auto distances = floyed_warshall(valves);

    auto t_start = std::chrono::high_resolution_clock::now();
    int res1 = part1(valves, start, distances);
    auto stop = std::chrono::high_resolution_clock::now();
    auto t1 = std::chrono::duration_cast<std::chrono::microseconds>(
        stop - t_start);

    t_start = std::chrono::high_resolution_clock::now();
    int res2 = part2(valves, start, distances);
    stop = std::chrono::high_resolution_clock::now();
    auto t2 = std::chrono::duration_cast<std::chrono::microseconds>(
        stop - t_start);

    cout << "Part 1: " << res1 << " - Time: " << t1.count() << " μs.\n";
    cout << "Part 2: " << res2 << " - Time: " << t2.count() << " μs.\n";
}
