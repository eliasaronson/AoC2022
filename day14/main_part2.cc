#include <boost/algorithm/string.hpp>
#include <fstream>
#include <istream>
#include <map>
#include <iostream>
#include <vector>
#include <chrono>

enum Block { sand, rock };
typedef std::pair<int, int> coord;
typedef std::vector<std::map<int, Block>> World;

coord to_coord(std::string& s) {
    std::vector<std::string> lsplit;
    boost::split(lsplit, s, [](char c) { return (c == ','); });
    return coord(std::stoi(lsplit[0]), std::stoi(lsplit[1]));
}

void draw(World& world, int x_start, int x_max, int y_max) {
    std::vector<std::vector<char>> canv(x_max + 2, std::vector<char>(y_max + 3, '.'));

    for (size_t i = 0; i < canv[0].size(); ++i) {
        canv[canv.size() - 1][i] = char(48 + i);
        canv[canv.size() - 2][i] = ' ';
    }

    for (size_t i = 0; i < canv.size(); ++i) {
        canv[i][canv[0].size() - 1] = char(48 + i);
        canv[i][canv[0].size() - 2] = ' ';
    }

    int ix = -1;
    for (int i = x_start; i < x_start + x_max; ++i) {
        std::cout << i << "\n";
        auto map = world[i];
        ix++;
        for (const auto& [key, value] : map) {
            switch (value) {
                case sand:
                    canv.at(ix).at(key) = '*';
                    break;
                case rock:
                    canv.at(ix).at(key) = '#';
                    break;
                default:
                    std::cout << "Warning\n";
                    break;
            }
        }
    }

    std::cout << canv.size() << ", " << canv[0].size() << "\n";
    for (size_t i = 0; i < canv[0].size(); ++i) {
        for (size_t j = 0; j < canv.size(); ++j) {
            std::cout << canv[j][i];
        }
        std::cout << "\n";
    }
    std::cout << "\n\r";
}

bool check_bounds(coord& start, int x_max, int y_max) {
    if (start.first < 0 || start.first > x_max || start.second < 0 || start.second > y_max) {
        return false;
    }
    return true;
}

bool sim_grain(World& world, coord start, int x_max, int y_max) {
    coord pos = start;
    while (1) {
        bool found_bottom = false;
        for (auto const& [key, val] : world[pos.first]) {
            if (key > pos.second) {
                found_bottom = true;
                pos.second = key - 1;
                break;
            }
        }

        if (!found_bottom) {
            return true;
        }

        if (world[pos.first - 1].count(pos.second + 1) == 0) {
            pos.first--;
            pos.second++;
        } else if (world[pos.first + 1].count(pos.second + 1) == 0) {
            pos.first++;
            pos.second++;
        } else {
            world[pos.first][pos.second] = sand;
            if (pos == start) {
                return true;
            } else {
                return false;
            }
        }

        if (!check_bounds(pos, x_max, y_max)) {
            std::cout << "Warning! Out of bounds\n";
            return true;
        }
    }
}

int sim(World& world, coord& start, int x_max, int y_max) {
    int n_grains = 0;
    while (1) {
        if (sim_grain(world, start, x_max, y_max)) {
            break;
        }
        n_grains++;
        // draw(world, x_max, y_max);
    }

    return n_grains;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    std::string file = "../inp_1.txt";
    std::fstream fs(file);
    std::vector<coord> rock_idxs;

    std::string line;
    std::vector<std::string> lsplit;
    while (std::getline(fs, line)) {
        boost::split(lsplit, line, [](char c) { return (c == '>'); });

        coord from = to_coord(lsplit[0]);
        for (size_t i = 1; i < lsplit.size(); ++i) {
            coord to = to_coord(lsplit[i]);

            if (from.first == to.first && from.second != to.second) {
                for (int i = std::min(from.second, to.second); i <= std::max(from.second, to.second); ++i) {
                    rock_idxs.push_back(coord(from.first, i));
                }
            }
            if (from.first != to.first && from.second == to.second) {
                for (int i = std::min(from.first, to.first); i <= std::max(from.first, to.first); ++i) {
                    rock_idxs.push_back(coord(i, from.second));
                }
            }

            from = to;
        }
    }

    int x_min = std::min_element(rock_idxs.begin(), rock_idxs.end(), [](const coord c1, const coord c2) {
                    return c1.first < c2.first;
                })->first;
    int y_min = std::min_element(rock_idxs.begin(), rock_idxs.end(), [](const coord c1, const coord c2) {
                    return c1.second < c2.second;
                })->second;
    int x_max = std::max_element(rock_idxs.begin(), rock_idxs.end(), [](const coord c1, const coord c2) {
                    return c1.first < c2.first;
                })->first;
    int y_max = std::max_element(rock_idxs.begin(), rock_idxs.end(), [](const coord c1, const coord c2) {
                    return c1.second < c2.second;
                })->second;

    for (int i = 0; i < x_max * 2; ++i) {
        rock_idxs.push_back(coord(i, y_max + 2));
    }

    // std::cout << "xmin: " << x_min << ", ymin: " << y_min << ", xmax: " << x_max << ", ymax: " << y_max << "\n";

    int offset = 0;
    coord source = coord(500 - offset, 0);

    int x_lim = 2 * x_max;
    World world(x_lim);

    for (auto idx : rock_idxs) {
        world[idx.first - offset][idx.second] = rock;
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto parsing = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    start = std::chrono::high_resolution_clock::now();
    int part2 = sim(world, source, x_lim, y_max + 2);
    stop = std::chrono::high_resolution_clock::now();

    auto t1 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    // draw(world, x_min - 10, x_max - x_min + 20, y_max + 2);
    std::cout << "Parsing: " << parsing.count() << " μs\n";
    std::cout << "Part 2: " << part2 << " - " << t1.count() << " μs\n\r";
}
