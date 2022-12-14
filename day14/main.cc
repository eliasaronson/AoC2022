#include <boost/algorithm/string.hpp>
#include <fstream>
#include <map>
#include <iostream>
#include <vector>

enum Block { sand, rock };
typedef std::pair<int, int> coord;
typedef std::vector<std::map<int, Block>> World;

coord to_coord(std::string &s) {
    std::vector<std::string> lsplit;
    boost::split(lsplit, s, [](char c) { return (c == ','); });
    return coord(std::stoi(lsplit[0]), std::stoi(lsplit[1]));
}

void draw(World& world, int x_min, int x_max, int y_min, int y_max) {
    std::vector<std::vector<char>> canv(y_max, std::vector<char>(x_max - x_min, '.'));

    int ix = 0;
    for (auto& map : world) {
        for (const auto& [key, value] : map) {
            std::cout << key << "\n";
            switch (value) {
                case sand:
                    canv[key][ix] = '*';
                    break;
                case rock:
                    canv[key][ix] = '#';
                    break;
                default:
                    std::cout << "Warning\n";
                    break;
            }
        }
        ix++;
    }

    std::cout << canv.size() << ", " << canv[0].size() << "\n";
    for (std::vector<char>& vec : canv) {
        for (char& c : vec) {
            std::cout << c;
        }
        std::cout << "\n";
    }
    std::cout << "\n\r";
}

int main() {
    std::string file = "../inp_t1.txt";
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

    std::cout << "xmin: " << x_min << ", ymin: " << y_min << ", xmax: " << x_max << ", ymax: " << y_max << "\n";

    int offset = x_min - 1;
    coord source = coord(500 - offset, 0);

    World world(x_max - offset);

    for (auto idx : rock_idxs) {
        std::cout << idx.first << ", " << idx.second << " - ";
        std::cout << idx.first - offset << ", " << idx.second << "\n";
        world[idx.first - offset][idx.second] = rock;
    }

    draw(world, offset, x_max, y_min, y_max);
}
