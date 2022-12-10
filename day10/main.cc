#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int check_reg(int cycle, int reg, int start_check, int interval)
{
    if ((cycle - start_check) % interval == 0) {
        return cycle * reg;
    }
    return 0;
}

void part1()
{
    auto start = std::chrono::high_resolution_clock::now();
    auto fs = std::fstream("../inp_1.txt");

    int start_check = 20, interval = 40, check = 0;

    std::string instruct;
    int val, reg = 1, cycle = 0;

    while (1) {
        fs >> instruct;

        if (fs.eof())
            break;

        switch (instruct[0]) {
        case 'a':
            fs >> val;
            cycle++;
            check += check_reg(cycle, reg, start_check, interval);
            cycle++;
            check += check_reg(cycle, reg, start_check, interval);
            reg += val;
            break;
        case 'n':
            cycle++;
            check += check_reg(cycle, reg, start_check, interval);
            break;
        default:
            break;
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration
        = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Part 1: " << check << "\n\r";
    std::cout << duration.count() << " μs\n\n\r";

    fs.close();
}

void draw(int cycle, int reg, std::vector<char>& screen)
{
    const int x_pos = cycle % 40;
    const char pixel
        = (x_pos == std::clamp(x_pos, reg - 1, reg + 1)) ? '#' : ' ';
    screen[cycle] = pixel;
}

void render(int width, int height, std::vector<char>& screen)
{
    // Set color.
    std::cout << "\x1b[38;2;200;10;2m\n";
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::cout << screen[j + i * width];
        }
        std::cout << "\n";
    }
    std::cout << "\033[0m\n\r";
}

void part2()
{
    auto start = std::chrono::high_resolution_clock::now();
    auto fs = std::fstream("../inp_1.txt");

    int width = 40, height = 6;
    auto screen = std::vector<char>(width * height, ' ');

    std::string instruct;
    int val, reg = 1, cycle = 0;

    draw(cycle, reg, screen);
    while (1) {
        fs >> instruct;

        if (fs.eof())
            break;

        switch (instruct[0]) {
        case 'a':
            fs >> val;
            cycle++;
            draw(cycle, reg, screen);
            cycle++;
            reg += val;
            draw(cycle, reg, screen);
            break;
        case 'n':
            cycle++;
            draw(cycle, reg, screen);
            break;
        default:
            break;
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration
        = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << " μs\n\r";

    std::cout << "Part 2:\n\r";
    render(width, height, screen);

    fs.close();
}

int main()
{
    part1();
    part2();
}
