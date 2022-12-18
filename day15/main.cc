#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <thread>
#include <vector>
#include <numeric>
#include <complex>

#define RANGE(VAR, END) \
    int VAR = 0; \
    VAR < END; \
    ++VAR
using namespace std;
typedef unsigned long long int bigint;
typedef std::complex<int> coord;

std::vector<pair<coord, coord>> parse(std::string filename) {
    std::fstream fs(filename);
    std::vector<pair<coord, coord>> res;

    int xs, ys, xb, yb;
    while (!fs.eof()) {
        fs >> xs >> ys >> xb >> yb;
        res.push_back(pair<coord, coord>(coord(xs, ys), coord(xb, yb)));
    }

    return res;
}

int manhattan(coord& sens, coord& bec) {
    coord dist = sens - bec;
    return abs(dist.imag()) + abs(dist.real());
}

vector<pair<int, int>> get_ranges(std::vector<pair<coord, coord>>& sensors, int row_of_interest) {
    vector<pair<int, int>> ranges(sensors.size());

    int idx = 0;
    for (auto& sens : sensors) {
        auto dist = manhattan(sens.first, sens.second);
        int d = dist - abs(row_of_interest - sens.first.imag());
        if (d > 0) {
            ranges[idx] = pair<int, int>(sens.first.real() - d, sens.first.real() + d);
        }
        idx++;
    }

    return ranges;
}

int part1(std::vector<pair<coord, coord>>& sensors, int row_of_interest) {
    auto ranges = get_ranges(sensors, row_of_interest);

    set<int> pos;
    for (auto rang : ranges) {
        for (int i = std::min(rang.first, rang.second); i < std::max(rang.first, rang.second); ++i) {
            pos.insert(i);
        }
    }

    return pos.size();
}

void vis(std::vector<pair<coord, coord>>& sensors, int row_from, int row_to) {
    vector<vector<char>> canv(row_to - row_from + 2, vector<char>(50, '.'));

    vector<char> bottom(canv[0].size(), ' ');
    for (RANGE(i, (int)canv[0].size() - 1)) {
        bottom[i] = char(48 + (i % 10));
    }
    canv[canv.size() - 1] = bottom;

    for (RANGE(i, (int)canv.size() - 1)) {
        canv[i][canv[0].size() - 1] = char(48 + (i % 10));
    }

    for (int i = row_from; i < row_to; ++i) {
        auto ranges = get_ranges(sensors, i);

        for (auto r : ranges) {
            for (int j = r.first; j <= r.second && j < (int)canv[0].size(); ++j) {
                if (j >= 0) {
                    canv.at(i).at(j) = '#';
                }
            }
        }
    }

    for (auto sens : sensors) {
        if (sens.first.imag() < (int)canv.size() && sens.first.real() < (int)canv[0].size() && sens.first.imag() >= 0
            && sens.first.real() >= 0) {
            canv.at(sens.first.imag()).at(sens.first.real()) = 'S';
        } else {
            cout << "Sensor: " << sens.first.real() << ", " << sens.first.imag() << "\n";
            cout << "Becon: " << sens.second.real() << ", " << sens.second.imag() << "\n";
        }
        if (sens.second.imag() < (int)canv.size() && sens.second.real() < (int)canv[0].size() && sens.second.imag() >= 0
            && sens.second.real() >= 0) {
            canv.at(sens.second.imag()).at(sens.second.real()) = 'B';
        } else {
            cout << "Sensor: " << sens.first.real() << ", " << sens.first.imag() << "\n";
            cout << "Becon: " << sens.second.real() << ", " << sens.second.imag() << "\n";
        }
    }

    canv.insert(canv.begin(), bottom);
    for (auto row : canv) {
        for (auto c : row) {
            cout << c;
        }
        cout << "\n";
    }
    cout << "\n";
}

inline int find_no_cover(vector<pair<int, int>>& ranges, int lower, int upper) {
    const size_t n_ranges = ranges.size();
    for (size_t idx_l = 0; idx_l < n_ranges; ++idx_l) {
        if (ranges[idx_l].second < lower) {
            continue;
        }
        if (ranges[idx_l].second >= upper) {
            break;
        }

        for (size_t idx_u = idx_l + 1; idx_u < n_ranges; ++idx_u) {
            if (ranges[idx_u].second > ranges[idx_l].second) {
                if (ranges[idx_u].first > ranges[idx_l].second + 1) {
                    return ranges[idx_l].second + 1;
                }
                idx_l = idx_u - 1;
                break;
            }
        }
    }

    return -1;
}

bigint part2(std::vector<pair<coord, coord>>& sensors, int row_from, int row_to) {
    for (int i = row_from; i < row_to; ++i) {
        auto ranges = get_ranges(sensors, i);

        sort(ranges.begin(), ranges.end(), [](const pair<int, int>& r1, const pair<int, int>& r2) {
            return r1.first < r2.first;
        });

        int res = find_no_cover(ranges, row_from, row_to);
        if (res != -1) {
            return bigint(res) * bigint(4000000) + bigint(i);
        }
    }

    return -1;
}

int main() {
    auto sensors = parse("../inp_1.txt");

    auto start = std::chrono::high_resolution_clock::now();
    int res1 = part1(sensors, 20);
    auto stop = std::chrono::high_resolution_clock::now();
    auto t1 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    cout << "Part 1: " << res1 << ". Time: " << t1.count() << " μs\n";

    start = std::chrono::high_resolution_clock::now();
    bigint res2 = part2(sensors, 0, 4000000);
    stop = std::chrono::high_resolution_clock::now();
    auto t2 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    cout << "Part 2: " << res2 << " . Time: " << t2.count() << " μs\n";
}
