#include <algorithm>
#include <chrono>
#include <complex>
#include <fstream>
#include <iostream>
#include <numeric>
#include <set>
#include <thread>
#include <vector>

#define RANGE(VAR, END) \
    int VAR = 0; \
    VAR < END; \
    ++VAR
using namespace std;
typedef long long int bigint;
typedef pair<int, bigint> coord;

std::vector<int> parse(std::string filename) {
    std::fstream fs(filename);
    std::vector<int> res;

    int val;
    while (!fs.eof()) {
        fs >> val;
        if (fs.eof()) {
            break;
        }
        res.push_back(val);
    }

    return res;
}

bigint part1(vector<int> coords, int n_runs = 1, bigint mult = 1) {
    vector<bigint> coords_mul(coords.size());
    for (size_t i = 0; i < coords.size(); ++i) {
        coords_mul[i] = bigint(coords[i]) * mult;
    }
    vector<coord> pos(coords.size());
    const int len = coords_mul.size() - 1;

    for (size_t i = 0; i < coords_mul.size(); ++i) {
        pos[i] = coord(i, coords_mul[i]);
    }

    for (int n_run = 0; n_run < n_runs; ++n_run) {
        for (size_t i = 0; i < coords_mul.size(); ++i) {
            auto focus = find_if(pos.begin(), pos.end(), [i](const coord c) { return c.first == (int)i; });

            if (focus == pos.end()) {
                cout << "Warning!\n";
            }
            coord tmp = *focus;

            const bigint old_pos = abs(distance(pos.begin(), focus));
            const bigint mov = tmp.second;
            bigint insert_pos = ((abs(mov) / len + 1) * len + old_pos + mov) % len;
            // insert_pos = (insert_pos == 0) ? len : insert_pos;

            pos.erase(focus);
            pos.insert(pos.begin() + insert_pos, tmp);
        }
    }

    auto focus = find_if(pos.begin(), pos.end(), [](const coord c) { return c.second == 0; });
    const int old_pos = abs(distance(pos.begin(), focus));
    bigint res = 0;

    for (size_t i = 1; i < 4; ++i) {
        int res_pos = (old_pos + i * 1000) % (len + 1);
        res += bigint(pos[res_pos].second);
    }

    return res;
}

int main() {
    auto coordinates = parse("../inp_1.txt");

    auto start = std::chrono::high_resolution_clock::now();
    int res1 = part1(coordinates);
    auto stop = std::chrono::high_resolution_clock::now();
    auto t1 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    start = std::chrono::high_resolution_clock::now();
    bigint res2 = part1(coordinates, 10, 811589153);
    stop = std::chrono::high_resolution_clock::now();
    auto t2 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    cout << "Part 1: " << res1 << " - Time: " << t1.count() << "μs \n";
    cout << "Part 2: " << res2 << " - Time: " << t2.count() << "μs \n";
}
