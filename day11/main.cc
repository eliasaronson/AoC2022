#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct monkey {
    int id;
    std::vector<long long int> items;

    std::function<long long int(long long int, long long int)> op;
    int op_val;

    int div_val;
    int true_send_id;
    int false_send_id;

    int n_inspected = 0;

    friend std::ostream& operator<<(std::ostream& os, const monkey& m);
};

std::ostream& operator<<(std::ostream& os, const monkey& m)
{
    os << "Monkey: " << m.id << "\nitems: ";
    for (auto& i : m.items) {
        os << i << ", ";
    }
    os << "Op: " << m.op_val;
    os << "\nDiv: " << m.div_val;
    os << "\tIf true: " << m.true_send_id;
    os << "\tIf false: " << m.false_send_id << "\n";
    os << "Inspected: " << m.n_inspected << "\n\r";
    return os;
}

std::map<char, std::function<long long int(long long int, long long int)>>
    op_map { { '-', std::minus<>() }, { '+', std::plus<>() },
        { '*', std::multiplies<>() }, { '/', std::divides<>() },
        { '^', [](int a, int b) { return std::pow(a, b); } } };

template <class T> void printv(std::vector<T> vec)
{
    std::cout << "Vec size: " << vec.size() << "\n";
    for (auto v : vec) {
        std::cout << v << ", ";
    }
    std::cout << "\n\r";
}

std::vector<monkey> parse_to_monkey(std::string filename)
{
    auto fs = std::fstream(filename);

    std::vector<monkey> monkeys(8);

    std::string line;
    std::vector<std::string> sline;
    int i = 0;
    while (std::getline(fs, line)) {
        monkey m;
        boost::split(sline, line, boost::is_any_of(" "));
        m.id = stoi(sline[1]);

        std::getline(fs, line);
        boost::split(sline, line.substr(16), [](char c) { return c == ' '; });
        m.items.resize(sline.size());
        std::transform(sline.begin(), sline.end(), m.items.begin(),
            [&](std::string s) { return stoi(s); });

        std::getline(fs, line);
        boost::split(sline, line, boost::is_any_of(" "));
        if (sline[5] == "old") {
            m.op = op_map['^'];
            m.op_val = 2;
        } else {
            m.op = op_map[sline[4][0]];
            m.op_val = std::stoi(sline[5]);
        }

        std::getline(fs, line);
        boost::split(sline, line, boost::is_any_of(" "));
        m.div_val = std::stoi(sline[sline.size() - 1]);

        std::getline(fs, line);
        boost::split(sline, line, boost::is_any_of(" "));
        m.true_send_id = std::stoi(sline[sline.size() - 1]);

        std::getline(fs, line);
        boost::split(sline, line, boost::is_any_of(" "));
        m.false_send_id = std::stoi(sline[sline.size() - 1]);

        monkeys[i] = m;
        i++;

        std::getline(fs, line);
    }

    fs.close();

    return monkeys;
}

void printm(std::vector<monkey>& monkeys)
{
    std::cout << "=====================\n";
    for (auto& m : monkeys) {
        std::cout << m;
        std::cout << "\n";
    }
    std::cout << "=====================\n\r";
}

void sim(std::vector<monkey>& monkeys, int n_rounds, const bool do_div = true)
{
    for (int i = 0; i < n_rounds; ++i) {
        for (auto& m : monkeys) {
            for (long long int item : m.items) {
                m.n_inspected++;

                /*if (m.n_inspected
                        > std::numeric_limits<long long int>::max() - 10
                    || m.n_inspected
                        < std::numeric_limits<long long int>::min()) {
                    std::cout << "WARNING \n\r";
                }*/

                item = m.op(item, m.op_val);

                if (do_div)
                    item /= 3;

                if (item % m.div_val == 0) {
                    monkeys[m.true_send_id].items.push_back(item);
                } else {
                    monkeys[m.false_send_id].items.push_back(item);
                }
            }
            m.items.clear();
        }
    }
}

int monkey_business(std::vector<monkey>& monkeys)
{
    std::sort(monkeys.begin(), monkeys.end(),
        [](monkey m1, monkey m2) { return m1.n_inspected > m2.n_inspected; });

    return monkeys[0].n_inspected * monkeys[1].n_inspected;
}

void part1(const int n_rounds, const int do_div)
{
    auto start = std::chrono::high_resolution_clock::now();

    auto monkeys = parse_to_monkey("../inp_t1.txt");
    printm(monkeys);
    sim(monkeys, n_rounds, do_div == 1);
    printm(monkeys);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration
        = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Res: " << monkey_business(monkeys) << "\n\r";
    std::cout << duration.count() << " μs\n\n\r";
}

void part2() { }

int main(int argn, char* argv[])
{
    for (int i = 0; i < argn; ++i) {
        std::cout << argv[i] << " ";
    }

    std::cout << "Number of rounds: " << argv[1] << " - Part: " << argv[2]
              << "\n\r";
    part1(std::stoi(std::string(argv[1])), std::stoi(std::string(argv[2])));
    // part2();
}
