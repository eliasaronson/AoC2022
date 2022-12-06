#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <sstream>
#include <stdio.h>

namespace {

void prints(std::vector<std::queue<char>> vec_queue)
{
    for (auto& queue : vec_queue) {
        while (!queue.empty()) {
            auto& w = queue.front();
            std::cout << w << " ";
            queue.pop();
        }
        std::cout << "\n";
    }
}

void move_crate(
    std::vector<std::queue<char>>& vec_queue, const int N, int from, int to)
{
    printf("N: %i, from: %i, to: %i\n", N, from, to);
    to--;
    from--;
    for (int i = 0; i < N; ++i) {
        vec_queue[to].push(vec_queue[from].front());
        vec_queue[from].pop();
        printf("mov\n");
        prints(vec_queue);
    }
}

} // namespace

int main()
{
    std::fstream file("../inp_t1.txt");
    std::string line;

    std::vector<std::queue<char>> vec_queue;
    while (std::getline(file, line)) {
        if (std::regex_match(line, std::regex(".*\\d")))
            break;

        for (int i = 1, j = 0; i < (int)line.size(); i += 4, ++j) {
            if (i >= (int)vec_queue.size())
                vec_queue.push_back(std::queue<char>());

            if (line[i] != ' ') {
                vec_queue[j].push(line[i]);
            }
        }
    }

    prints(vec_queue);

    while (std::getline(file, line)) {
        printf("Sz: %i\n", (int)line.size());
        if (line.size() == 0)
            continue;

        std::cout << line << "\n";
        int N = 0, from = 0, to = 0;
        std::string space;
        std::stringstream ss(line);
        ss >> space >> N >> space >> from >> space >> to;

        move_crate(vec_queue, N, from, to);
    }

    prints(vec_queue);

    file.close();
    printf("Done.\n");
}
