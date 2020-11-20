#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

typedef std::vector<std::string> layout_t;

void displayLayout(const layout_t &layout)
{
    for (const std::string &line : layout) {
        std::cout << line << std::endl;
    }
}

layout_t getContent(const std::string &filename)
{
    std::string line;
    std::ifstream file(filename);
    layout_t layout;

    while (std::getline(file, line))
        layout.push_back(line);

    file.close();
    return layout;
}

std::vector<char> checkSurroundings(const layout_t &layout, size_t i, size_t j)
{
    size_t maxi = layout.size() - 1;
    size_t maxj = layout[0].size() - 1;
    std::vector<char> result;

    result.push_back((j <= 0) ? '.' : layout[i][j - 1]);
    result.push_back((i <= 0) ? '.' : layout[i - 1][j]);
    result.push_back((j + 1 > maxj) ? '.' : layout[i][j + 1]);
    result.push_back((i + 1 > maxi) ? '.' : layout[i + 1][j]);

    return result;
}

layout_t compute(const layout_t &layout)
{
    layout_t newLayout = layout;

    for (size_t i = 0; i < layout.size(); i++) {
        for (size_t j = 0; j < layout[i].size(); j++) {
            std::vector<char> surroundings = checkSurroundings(layout, i, j);
            size_t bugs = std::count(surroundings.begin(), surroundings.end(), '#');

            if (layout[i][j] == '#' && bugs != 1) {
                newLayout[i][j] = '.';
            } else if (layout[i][j] == '.' && (bugs == 1 || bugs == 2)) {
                newLayout[i][j] = '#';
            }
        }
    }

    return newLayout;
}

bool isInHistory(const std::vector<layout_t> &history, const layout_t &layout)
{
    for (const layout_t &hLayout : history) {

        bool inHistory = true;
        for (size_t yIndex = 0; yIndex < hLayout.size(); yIndex++)
            for (size_t xIndex = 0; xIndex < hLayout[yIndex].size(); xIndex++)
                inHistory = (inHistory) ? (hLayout[yIndex][xIndex] == layout[yIndex][xIndex]) : inHistory;

        if (inHistory)
            return true;
    }

    return false;
}

int main(int ac, char **av)
{
    if (ac != 2)
        return 84;

    try {
        std::vector<layout_t> history;
        layout_t layout = getContent(std::string(av[1]));

        history.push_back(layout);
        for (size_t i = 0; true; i++) {
            layout = compute(layout);
            if (isInHistory(history, layout))
                break;
            history.push_back(layout);
        }

        size_t result = 0;
        size_t powerTwo = 1;
        for (size_t y = 0; y < layout.size(); y++) {
            for (size_t x = 0; x < layout[y].size(); x++) {
                if (layout[y][x] == '#')
                    result = result + powerTwo;
                powerTwo = powerTwo * 2;
            }
        }

        std::cout << result << std::endl;

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }

    return 0;
}