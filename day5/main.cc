#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stack>
#include <stdio.h>

namespace {

void prints(std::vector<std::stack<char>> vec_stack) {
  for (auto &stack : vec_stack) {
    while (!stack.empty()) {
      auto &w = stack.top();
      std::cout << w << " ";
      stack.pop();
    }
    std::cout << "\n";
  }
}

void move_crate(std::vector<std::stack<char>> &vec_stack, const int N, int from,
                int to) {
  to--;
  from--;
  for (int i = 0; i < N; ++i) {
    vec_stack[to].push(vec_stack[from].top());
    vec_stack[from].pop();
  }
}

void move_crate2(std::vector<std::stack<char>> &vec_stack, const int N,
                 int from, int to) {
  to--;
  from--;
  std::stack<char> tmp_stack;
  for (int i = 0; i < N; ++i) {
    tmp_stack.push(vec_stack[from].top());
    vec_stack[from].pop();
  }
  for (int i = 0; i < N; ++i) {
    vec_stack[to].push(tmp_stack.top());
    tmp_stack.pop();
  }
}

} // namespace

int main()
{
  std::fstream file("../inp_1.txt");
  std::string line;

  std::cout << "Building stack.\n";
  std::vector<std::stack<char>> vec_stack_rev;
  while (std::getline(file, line)) {
    if (std::regex_match(line, std::regex(".*\\d")))
      break;

    for (int i = 1, j = 0; i < (int)line.size(); i += 4, ++j) {
      if (i >= (int)vec_stack_rev.size())
        vec_stack_rev.push_back(std::stack<char>());

      if (line[i] != ' ') {
        vec_stack_rev[j].push(line[i]);
      }
    }
    }

    prints(vec_stack_rev);

    std::cout << "Reversing stack.\n";
    // Reverse stack
    std::vector<std::stack<char>> vec_stack(vec_stack_rev.size());
    for (int i = 0; i < (int)vec_stack_rev.size(); ++i) {
      while (!vec_stack_rev[i].empty()) {
        vec_stack[i].push(vec_stack_rev[i].top());
        vec_stack_rev[i].pop();
      }
    }

    prints(vec_stack);

    std::vector<std::stack<char>> vec_stack2(vec_stack);

    while (std::getline(file, line)) {
        if (line.size() == 0)
            continue;

        int N = 0, from = 0, to = 0;
        std::string space;
        std::stringstream ss(line);
        ss >> space >> N >> space >> from >> space >> to;

        move_crate(vec_stack, N, from, to);
        move_crate2(vec_stack2, N, from, to);
    }

    prints(vec_stack);

    file.close();

    printf("Part1: ");
    for (auto &stack : vec_stack) {
      if (!stack.empty()) {
        std::cout << stack.top();
      }
    }

    printf("\nPart2: ");
    for (auto &stack : vec_stack2) {
      if (!stack.empty()) {
        std::cout << stack.top();
      }
    }

    printf("\nDone.\n");
}
