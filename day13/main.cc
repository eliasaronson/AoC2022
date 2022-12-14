#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>
#include <vector>

template <class T> void printv(std::vector<T> vec)
{
    std::cout << "Vec size: " << vec.size() << "\n";
    for (auto v : vec) {
        std::cout << v << ", ";
    }
    std::cout << "\n\r";
}

int main()
{
    std::ifstream f("../inp_t1.txt");

    std::vector<int> correct;
    int pair_num = 0;
    std::string line1, line2, del;
    while (std::getline(f, line1)) {
        std::getline(f, line2);
        std::getline(f, del);

        std::cout << "Line 1: " << line1 << "\n\r";
        std::cout << "Line 2: " << line2 << "\n\r";

        int n1 = 0, n2 = 0;

        pair_num++;
    }
}
