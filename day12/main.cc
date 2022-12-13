#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

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
    while (!fs.eof()) {
        fs >> line;
        hmap.push_back(std::vector<char>(line.begin(), line.end()));
    }

    return hmap;
}

std::pair<int, int> find_start(std::vector<std::vector<char>>& hmap, char goal)
{
    for (size_t i = 0; i < hmap.size(); ++i) {
        for (size_t j = 0; j < hmap.size(); ++j) {
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

typedef std::pair<int, int> coord;

inline void clear(int n_chars)
{
    for (int i = 0; i < n_chars; ++i) {
        std::cout << "\b";
    }
}

void draw(std::vector<std::vector<char>> hmap, std::vector<coord> prev,
    coord now, std::vector<char> dirm)
{
    printv(dirm);
    clear(hmap.size() * (hmap[0].size() + 1));

    int ii = 0;
    for (auto& c : prev) {
        hmap[c.first][c.second] = dirm[ii];
        ii++;
    }
    std::cout << "(" << now.first << ", " << now.second << ")\n";
    hmap[now.first][now.second] = '#';

    printm(hmap);
}

int search(int n_steps, std::vector<std::vector<char>>& hmap, coord start,
    coord end, std::vector<coord> prev = std::vector<coord>(),
    std::vector<char> dirm = std::vector<char>(1, '0'))
{
    if (start == end) {
        if (n_steps < 32) {
            std::cout << "N_steps: " << n_steps << "\n\r";
            std::cout << "Goal!\n";
            draw(hmap, prev, start, dirm);
        }
        return n_steps;
    }

    if (n_steps > 45) {
        return -1;
    }

    prev.push_back(start);
    // draw(hmap, prev, start);

    std::vector<int> dirs(4, -1); // [up, down, left, right]

    coord up_pos = coord(start.first - 1, start.second);
    if (up_pos.first >= 0 && !std::count(prev.begin(), prev.end(), up_pos)) {
        if (hmap[up_pos.first][up_pos.second] - hmap[start.first][start.second]
            <= 1) {
            dirm.push_back('^');
            dirs[0] = search(n_steps + 1, hmap, up_pos, end, prev, dirm);
        }
    }

    coord down_pos = coord(start.first + 1, start.second);
    if (down_pos.first < (int)hmap.size()
        && !std::count(prev.begin(), prev.end(), down_pos)) {

        /*std::cout << "diff: "
                  << hmap[down_pos.first][down_pos.second]
                - hmap[start.first][start.second]
                  << ", char new: " << hmap[down_pos.first][down_pos.second]
                  << ", char start: " << hmap[start.first][start.second]
                  << "\n\r";*/

        if (hmap[down_pos.first][down_pos.second]
                - hmap[start.first][start.second]
            <= 1) {
            dirm.push_back('v');
            dirs[1] = search(n_steps + 1, hmap, down_pos, end, prev, dirm);
        }
    }

    coord left_pos = coord(start.first, start.second - 1);
    if (left_pos.second >= 0
        && !std::count(prev.begin(), prev.end(), left_pos)) {
        if (hmap[left_pos.first][left_pos.second]
                - hmap[start.first][start.second]
            <= 1) {
            dirm.push_back('<');
            dirs[2] = search(n_steps + 1, hmap, left_pos, end, prev, dirm);
        }
    }

    coord right_pos = coord(start.first, start.second + 1);
    if (right_pos.second < (int)hmap[0].size()
        && !std::count(prev.begin(), prev.end(), right_pos)) {
        if (hmap[right_pos.first][right_pos.second]
                - hmap[start.first][start.second]
            <= 1) {
            dirm.push_back('>');
            dirs[3] = search(n_steps + 1, hmap, right_pos, end, prev, dirm);
        }
    }

    // printv(dirs);
    std::vector<int> dirs_filterd;
    std::copy_if(dirs.begin(), dirs.end(), std::back_inserter(dirs_filterd),
        [](int i) { return i >= 0; });

    if (dirs_filterd.size() == 0) {
        return -1;
    }

    std::sort(dirs_filterd.begin(), dirs_filterd.end());

    return dirs_filterd[0];
}

void search_path(std::vector<std::vector<char>>& hmap)
{
    std::pair<int, int> start = find_start(hmap, 'S');
    std::pair<int, int> end = find_start(hmap, 'E');
    std::cout << "Start: (" << start.first << ", " << start.second << ")\n";
    std::cout << "End: (" << end.first << ", " << end.second << ")\n\r";

    hmap[start.first][start.second] = '~';
    hmap[end.first][end.second] = '!';

    int n_step = search(0, hmap, start, end);

    std::cout << "Number of steps: " << n_step << "\n\r";
}

int main()
{
    auto hmap = parse("../inp_t1.txt");

    printm(hmap);

    search_path(hmap);
}
