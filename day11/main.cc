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
    std::vector<int> items;

    std::function<int(int, int)> op;
    int op_val;
    char oper;

    int div_val;
    int true_send_id;
    int false_send_id;

    int n_inspected = 0;

    friend std::ostream& operator<<(std::ostream& os, const monkey& m);
};

struct item {
    std::vector<int> mul;
    int sum;

    item(std::vector<int> multi, int summ)
        : mul(multi)
        , sum(summ) {};
};

const std::vector<int> moduli = { 2, 3, 5, 7, 11, 13, 17, 19, 23 };
inline void num_to_rns(std::vector<int>& rns, int num)
{
    for (size_t i = 0; i < moduli.size(); ++i) {
        rns[i] = num % moduli[i];
    }
}

inline void rns_add(std::vector<int>& num1, std::vector<int>& num2)
{
    for (size_t i = 0; i < moduli.size(); ++i) {
        num1[i] = (num1[i] + num2[i]) % moduli[i];
    }
}

inline void rns_sub(std::vector<int>& num1, std::vector<int>& num2)
{
    for (size_t i = 0; i < moduli.size(); ++i) {
        num1[i] = (num1[i] - num2[i]) % moduli[i];
    }
}

inline void rns_pow(std::vector<int>& num1, std::vector<int>&)
{
    for (size_t i = 0; i < moduli.size(); ++i) {
        num1[i] = (num1[i] * num1[i]) % moduli[i];
    }
}

inline void rns_mul(std::vector<int>& num1, std::vector<int>& num2)
{
    for (size_t i = 0; i < moduli.size(); ++i) {
        num1[i] = (num1[i] * num2[i]) % moduli[i];
    }
}

struct monkey2 {
    int id;
    std::vector<std::vector<int>> items;

    std::function<void(std::vector<int>&, std::vector<int>&)> op;
    std::vector<int> op_val;
    char oper;

    int div_val;
    int true_send_id;
    int false_send_id;

    int n_inspected = 0;

    monkey2() = default;
    monkey2(monkey& m, const std::vector<int> moduli)
    {
        id = m.id;
        items.resize(m.items.size());
        for (size_t i = 0; i < items.size(); ++i) {
            items[i].resize(moduli.size());
            num_to_rns(items[i], m.items[i]);
        }

        op_val.resize(moduli.size());
        num_to_rns(op_val, m.op_val);
        oper = m.oper;

        auto it = std::find(moduli.begin(), moduli.end(), m.div_val);
        div_val = it - moduli.begin();
        true_send_id = m.true_send_id;
        false_send_id = m.false_send_id;
    }
    friend std::ostream& operator<<(std::ostream& os, const monkey2& m);
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

std::ostream& operator<<(std::ostream& os, const monkey2& m)
{
    os << "Monkey: " << m.id << "\nitems: ";
    // os << "Op: " << m.op_val;
    os << "\nDiv: " << m.div_val;
    os << "\tIf true: " << m.true_send_id;
    os << "\tIf false: " << m.false_send_id << "\n";
    os << "Inspected: " << m.n_inspected << "\n\r";
    return os;
}

std::map<char, std::function<int(int, int)>> op_map { { '-', std::minus<>() },
    { '+', std::plus<>() }, { '*', std::multiplies<>() },
    { '/', std::divides<>() },
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
            m.oper = '^';
            m.op_val = 2;
        } else {
            m.op = op_map[sline[4][0]];
            m.op_val = std::stoi(sline[5]);
            m.oper = sline[4][0];
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

template <typename T> void printm(std::vector<T>& monkeys)
{
    std::cout << "=====================\n";
    for (auto& m : monkeys) {
        std::cout << m;
        std::cout << "\n";
    }
    std::cout << "=====================\n\r";
}

void sim(std::vector<monkey>& monkeys, int n_rounds)
{
    for (int i = 0; i < n_rounds; ++i) {
        for (auto& m : monkeys) {
            for (int item : m.items) {
                m.n_inspected++;

                item = m.op(item, m.op_val);

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

void sim2(std::vector<monkey2>& monkeys, int n_rounds)
{
    for (int i = 0; i < n_rounds; ++i) {
        for (auto& m : monkeys) {
            for (std::vector<int> item : m.items) {
                m.n_inspected++;

                switch (m.oper) {
                case '*':
                    rns_mul(item, m.op_val);
                    break;
                case '+':
                    rns_add(item, m.op_val);
                    break;
                case '^':
                    rns_pow(item, m.op_val);
                    break;
                }

                if (item[m.div_val] == 0) {
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

long long int monkey_business(std::vector<monkey2>& monkeys)
{
    std::sort(monkeys.begin(), monkeys.end(), [](monkey2 m1, monkey2 m2) {
        return m1.n_inspected > m2.n_inspected;
    });

    return (long long int)monkeys[0].n_inspected
        * (long long int)monkeys[1].n_inspected;
}

void part1(const int n_rounds, const int do_div)
{
    auto start = std::chrono::high_resolution_clock::now();

    auto monkeys = parse_to_monkey("../inp_1.txt");

    long long int res = 0;
    if (do_div == 1) {
        sim(monkeys, n_rounds);
        res = monkey_business(monkeys);
    } else {
        std::vector<monkey2> monkeys2(monkeys.size());
        std::transform(monkeys.begin(), monkeys.end(), monkeys2.begin(),
            [](monkey& m) { return monkey2(m, moduli); });
        sim2(monkeys2, n_rounds);
        res = monkey_business(monkeys2);
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration
        = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Res: " << res << "\n\r";
    std::cout << duration.count() << " μs\n\n\r";
}

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
