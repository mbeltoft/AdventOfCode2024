#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const int X_SIZE = 50;
const int Y_SIZE = 50;
char map[Y_SIZE][X_SIZE];

bool move(int &rx, int &ry, char direction, bool &fatal)
{
    bool res = false;
    fatal = false;
    std::cout << "Move: " << direction << " from " << ry << "," << rx << std::endl;
    if (map[ry][X_SIZE - 1] != '#') {
        std::cerr << "Error detected (before)" << std::endl;
        fatal = true;
    }
    switch (direction) {
        case '^':
        {
            int y = ry - 1;
            bool boxes = false;
            while (y > 0 && map[y][rx] != '#') {
                if (map[y][rx] == '.') {
                    break;
                }
                boxes |= (map[y][rx] == 'O');
                y--;
            }
            if (map[y][rx] == '#') {
                //std::cout << "Can't go up" << std::endl;
                return false;
            }
            // y is now at a free spot
            //std::cout << "Free y:" << y << " ry:" << ry << std::endl;
            if (boxes) {
                // Move boxes up to down
                for (int ny = y; ny < ry; ny++) {
                    map[ny][rx] = map[ny + 1][rx];
                }
            }
            ry -= 1;
            res = true;
            break;
        }
        case '>':
        {
            int x = rx + 1;
            bool boxes = false;
            while (x < X_SIZE && map[ry][x] != '#') {
                if (map[ry][x] == '.') {
                    break;
                }
                boxes |= (map[ry][x] == 'O');
                x++;
            }
            if (map[ry][x] == '#') {
                //std::cout << "Can't go right" << std::endl;
                return false;
            }
            // x is now at a free spot
            //std::cout << "Free x:" << x << " rx:" << rx << std::endl;
            if (boxes) {
                // Move boxes right to left
                for (int nx = x; nx > rx; nx--) {
                    map[ry][nx] = map[ry][nx - 1];
                }
            }
            rx += 1;
            res = true;
            break;
        }
        case 'v':
        {
            int y = ry + 1;
            bool boxes = false;
            while (y < Y_SIZE && map[y][rx] != '#') {
                if (map[y][rx] == '.') {
                    break;
                }
                boxes |= (map[y][rx] == 'O');
                y++;
            }
            if (map[y][rx] == '#') {
                //std::cout << "Can't go down" << std::endl;
                return false;
            }
            // y is now at a free spotc
            //std::cout << "Free y:" << y << " ry:" << ry << std::endl;
            if (boxes) {
                // Move boxes down to up
                for (int ny = y; ny > ry; ny--) {
                    map[ny][rx] = map[ny - 1][rx];
                }
            }
            ry += 1;
            res = true;
            break;
        }
        case '<':
        {
            int x = rx - 1;
            bool boxes = false;
            while (x > 0 && map[ry][x] != '#') {
                if (map[ry][x] == '.') {
                    break;
                }
                boxes |= (map[ry][x] == 'O');
                x--;
            }
            if (map[ry][x] == '#') {
                //std::cout << "Can't go left" << std::endl;
                return false;
            }
            // x is now at a free spot
            //std::cout << "Free x:" << x << " rx:" << rx << std::endl;
            if (boxes) {
                // Move boxes left to right
                for (int nx = x; nx < rx; nx++) {
                    map[ry][nx] = map[ry][nx + 1];
                }
            }
            rx -= 1;
            res = true;
            break;
        }
        default:
            std::cerr << "Unsupported direction: '" << "'" << std::endl;
            exit(0);
            break;
    }
    if (map[ry][X_SIZE - 1] != '#') {
        std::cerr << "Error detected (after)" << std::endl;
        fatal = true;
    }

    return res;
}

void main()
{
    std::fstream file("input15.txt");
    std::string line;

    int rx = 0;
    int ry = 0;

    int y = 0;
    bool read_movements = false;
    std::string movements;

    while (std::getline(file, line)) {
        if (read_movements) {
            movements += line;
        }
        if (line.size() == 0) {
            read_movements = true;
        }
        if (!read_movements && line.size() > 0 && line[0] == '#') {
            for (int x = 0; x < line.size(); x++) {
                map[y][x] = line[x];
                if (line[x] == '@') {
                    // The robot is here
                    ry = y;
                    rx = x;
                    map[y][x] = '.';
                }
            }
            y++;
        }
    }
#if 0
    std::cout << "Initial" << std::endl;
    for (int y = 0; y < Y_SIZE; y++) {
        for (int x = 0; x < X_SIZE; x++) {
            std::cout << (x == rx && y == ry ? '@' : map[y][x]);
        }
        std::cout << std::endl;
    }
    std::cout << "Robot: " << ry << "," << rx << std::endl;
    std::cout << movements << std::endl;
#endif
    for (auto direction : movements) {
        bool fatal = false;
        move(rx, ry, direction, fatal);
        if (fatal) {
            for (int y = 0; y < Y_SIZE; y++) {
                for (int x = 0; x < X_SIZE; x++) {
                    std::cout << (x == rx && y == ry ? '@' : map[y][x]);
                }
                std::cout << std::endl;
            }
            exit(0);
        }
    }

    long total_score = 0;
    for (int y = 0; y < Y_SIZE; y++) {
        for (int x = 0; x < X_SIZE; x++) {
            std::cout << (x == rx && y == ry ? '@' : map[y][x]);
            if (map[y][x] == 'O') {
                total_score += 100 * y + x;
            }
        }
        std::cout << std::endl;
    }

    std::cout << "Total score: " << total_score << std::endl;
    // 1451928
}