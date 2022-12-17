#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <thread>
#include <vector>
#include <numeric>

#define RANGE(VAR, END) \
    int VAR = 0; \
    VAR < END; \
    ++VAR
using namespace std;
typedef unsigned long long int bigint;
typedef std::pair<int, int> coord;

void draw(std::vector<coord>& rock, set<coord>& world, int hmax) {
    std::vector<std::vector<char>> canv(hmax, vector<char>(7, '.'));

    for (auto r : rock) {
        canv[r.second][r.first] = '@';
    }

    for (auto r : world) {
        canv[r.second][r.first] = '#';
    }

    for (auto it = canv.rbegin(); it != canv.rend(); ++it) {
        printf("%s\n", (*it).data());
    }
    printf("-------\n\n");
}

std::vector<char> parse(std::string filename) {
    std::fstream fs(filename);

    std::string line;
    getline(fs, line);

    return std::vector<char>(line.begin(), line.end());
}

inline bool xmov(vector<coord>& rock, int dx, int width, set<coord>& world) {
    for (auto& r : rock) {
        if ((r.first + dx) >= width || (r.first + dx) < 0 || world.contains(coord(r.first + dx, r.second))) {
            return false;
        }
    }

    std::for_each(rock.begin(), rock.end(), [dx](coord& pos) { pos.first += dx; });
    return true;
}

inline void ymov(vector<coord>& rock, int dy) {
    std::for_each(rock.begin(), rock.end(), [dy](coord& pos) { pos.second += dy; });
}

inline bool check_y_collision(vector<coord>& rock, set<coord>& world) {
    for (auto& r : rock) {
        if (world.contains(r)) {
            return true;
        }
    }

    return false;
}

inline int find_ymax(int ymax, vector<coord> rock) {
    rock.push_back(coord(0, ymax));
    return std::max_element(
               rock.begin(),
               rock.end(),
               [](const coord c1, const coord c2) { return c1.second < c2.second; })
        ->second;
}

inline void filter_covered(set<coord>& world) {
    vector<int> cover(7, 0);

    for (auto w : world) {
        if (w.second > cover[w.first]) {
            cover[w.first] = w.second;
        }
    }

    if (std::find(cover.begin(), cover.end(), 0) == cover.end()) {
        std::erase_if(world, [cover](const coord& x) { return x.second < cover[x.first]; });
    }
}

int rock_sim(vector<coord> rock, set<coord>& world, vector<char>& winds, int width, int ymax, int& wind_idx) {
    const int xstart = 2;
    const int ystart = 4;

    ymov(rock, ymax + ystart);
    xmov(rock, xstart, width, world);

    vector<coord> rock_prev;
    while (1) {
        // draw(rock, world, ymax + 10);
        switch (winds[wind_idx]) {
            case '<':
                xmov(rock, -1, width, world);
                break;
            case '>':
                xmov(rock, 1, width, world);
                break;
        }
        wind_idx = (wind_idx + 1) % winds.size();

        rock_prev = rock;
        ymov(rock, -1);

        if (check_y_collision(rock, world)) {
            for (auto r : rock_prev) {
                world.insert(r);
            }

            return find_ymax(ymax, rock_prev);
        }
    }
}

vector<int>
sim(vector<vector<coord>>& rocks,
    set<coord>& world,
    vector<char>& winds,
    int& ymax,
    int width = 7,
    int num_sims = 2022) {
    int wind_idx = 0;
    vector<int> delta_h(num_sims, 0);

    for (int i = 0; i < num_sims; ++i) {
        int new_ymax = rock_sim(rocks[i % rocks.size()], world, winds, width, ymax, wind_idx);
        delta_h[i] = new_ymax - ymax;
        ymax = new_ymax;
    }

    std::cout << "Part 1: " << ymax << "\n\r";
    return delta_h;
}

void part2(vector<int>& delta_h, bigint ymax, bigint N_prev_rounds) {
    bigint N_rounds = 1000000000000;

    int offset = 500;
    int N_pattern = -1;

    // for (int i = (delta_h.size() - offset) / 2; i > 2; --i) {
    for (size_t i = 10; i < (delta_h.size() - offset) / 2; ++i) {
        bool pattern_found = true;
        for (size_t j = 0; j < delta_h.size() - offset - i; ++j) {
            if (delta_h[offset + j + i] != delta_h[offset + j % i]) {
                pattern_found = false;
                break;
            }
        }

        if (pattern_found) {
            N_pattern = i;
            break;
        }
    }

    if (N_pattern < 0) {
        cout << "Warning! No pattern_found.\n";
        return;
    }

    bigint sum = std::accumulate(delta_h.begin() + offset, delta_h.begin() + offset + N_pattern, bigint(0));

    N_rounds -= N_prev_rounds;
    bigint N_patterns = N_rounds / N_pattern;

    ymax += N_patterns * sum;

    bigint N_extra = N_rounds - N_pattern * N_patterns;

    for (bigint i = 0; i < N_extra; ++i) {
        ymax += delta_h[offset + i];
    }

    cout << "Pattern length: " << N_pattern << ". N_patterns: " << N_patterns << ". N_extra: " << N_extra << "\n";

    cout << "Part 2: " << ymax << "\n";
}

void part1(vector<char>& winds) {
    vector<coord> flat = {{0, 0}, {1, 0}, {2, 0}, {3, 0}};
    vector<coord> cross = {{1, 0}, {0, 1}, {1, 1}, {2, 1}, {1, 2}};
    vector<coord> L = {{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}};
    vector<coord> line = {{0, 0}, {0, 1}, {0, 2}, {0, 3}};
    vector<coord> cube = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};

    vector<vector<coord>> rocks = {flat, cross, L, line, cube};

    set<coord> world = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}};

    int ymax = 0;
    int num_sims = 1695 * 2 + 805;

    auto start = std::chrono::high_resolution_clock::now();
    auto delta_h = sim(rocks, world, winds, ymax, 7, num_sims);
    auto stop = std::chrono::high_resolution_clock::now();
    auto t1 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    start = std::chrono::high_resolution_clock::now();
    part2(delta_h, ymax, num_sims);
    stop = std::chrono::high_resolution_clock::now();
    auto t2 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Times:\n\tPrecalc - " << t1.count() << " μs.\n\tFinal - " << t2.count() << " μs.\n\tTotal - "
              << t1.count() + t2.count() << " μs\n";
}

int main() {
    auto winds = parse("../inp_1.txt");
    part1(winds);
}
