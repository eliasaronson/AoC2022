#include <bits/stdc++.h>
#include <fstream>
#include <stdio.h>
#include <vector>

int main()
{
    std::fstream file("../inp_1.txt");
    std::string line;

    std::vector<int> elfs;
    int sum = 0;
    while (std::getline(file, line)) {
        if (line.size() == 0) {
            elfs.push_back(sum);
            sum = 0;
            continue;
        }
        sum += stoi(line);
    }
    std::sort(elfs.begin(), elfs.end());
    printf("Max: %i\n", elfs.back());

    int top_three = 0;
    for (int i = 0; i < 4; ++i) {
        top_three += elfs[elfs.size() - i];
        printf("l: %i\n", elfs[elfs.size() - i]);
    }

    printf("Max: %i\n", top_three);

    file.close();
    printf("Done.\n");
}
