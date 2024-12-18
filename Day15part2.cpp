#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int box_id = 0;

struct Box
{
public:
    Box() = default;
    Box(int y, int x1, int id) : y(y), x1(x1), x2(x1 + 1), id(id) {};
    int x1 = 0;
    int x2 = 0;
    int y = 0;
    int id = -1;
};

const int Y_SIZE = 50;
const int X_SIZE = 100;
char map[Y_SIZE][X_SIZE];

void PutTileAt(int y, int x, char tile, std::vector<Box> &boxes, int &ry, int &rx)
{
    switch (tile) {
        case '#':
        case '.':
            map[y][x] = tile;
            map[y][x + 1] = tile;
            break;
        case 'O':
            map[y][x] = '[';
            map[y][x + 1] = ']';
            boxes.push_back(Box(y, x, box_id++));
            break;
        case '@':
            ry = y;
            rx = x;
            map[y][x] = '.';
            map[y][x + 1] = '.';
            break;
        default:
            std::cerr << "Unknown tile: " << tile << std::endl;
            break;
    }
}

void LoadMap(const std::string &filename, std::vector<Box> &boxes, std::vector<char> &commands, int &ry, int &rx)
{
    std::string line;
    int y = 0;
    bool read_commands = false;

    std::ifstream file(filename);
    while (getline(file, line)) {
        if (line.size() == 0) {
            read_commands = true;
        }
        if (read_commands) {
            for(auto cmd : line) {
                commands.push_back(cmd);
            }
        } else {
            for (size_t i = 0; i < line.length(); i++) {
                PutTileAt(y, i * 2, line[i], boxes, ry, rx);
            }
            y++;
        }
    }
}

bool IsFreeAt(int y, int x)
{
    return map[y][x] == '.';
}

bool IsWallAt(int y, int x)
{
    return map[y][x] == '#';
}

bool BoxAt(int y, int x, int &id, std::vector<Box> &boxes)
{
    id = -1;
    for (auto box : boxes) {
        if (y == box.y && (x == box.x1 || x == box.x2)) {
            id = box.id;
            return true;
        }
    }
    return false;
}

bool CanBoxAtMove(int y, int x, char direction, std::vector<Box> &boxes)
{
    bool can_move = false;
    for (auto box : boxes) {
        //map[box.y][box.x1] = box.id + '0';
        if (y == box.y && (x == box.x1 || x == box.x2)) {
            switch (direction) {
                case '^':
                    if (!IsWallAt(y - 1, box.x1) && !IsWallAt(y - 1, box.x2)) {
                        if (IsFreeAt(y - 1, box.x1) && IsFreeAt(y - 1, box.x2)) {
                            can_move = true;
                        } else {
                            int id1 = -1;
                            int id2 = -1;
                            BoxAt(y - 1, box.x1, id1, boxes);
                            BoxAt(y - 1, box.x2, id2, boxes);
                            if (id1 > -1 && id2 > -1 && id1 == id2) {
                                can_move = CanBoxAtMove(y - 1, box.x1, direction, boxes);
                            } else if (id1 > -1 && id2 > 0 && id1 != id2) {
                                can_move = CanBoxAtMove(y - 1, box.x1, direction, boxes) && CanBoxAtMove(y - 1, box.x2, direction, boxes);
                            } else if (id1 > -1) {
                                can_move = CanBoxAtMove(y - 1, box.x1, direction, boxes);
                            } else if (id2 > -1) {
                                can_move = CanBoxAtMove(y - 1, box.x2, direction, boxes);
                            }
                        }
                    }
                    break;
                case '>':
                    if (!IsWallAt(y, box.x2 + 1)) {
                        if (IsFreeAt(y, box.x2 + 1)) {
                            can_move = true;
                        } else {
                            int id = -1;
                            BoxAt(y, box.x2 + 1, id, boxes);
                            if (id > -1) {
                                can_move = CanBoxAtMove(y, box.x2 + 1, direction, boxes);
                            }
                        }
                    }
                    break;
                case 'v':
                    if (!IsWallAt(y + 1, box.x1) && !IsWallAt(y + 1, box.x2)) {
                        if (IsFreeAt(y + 1, box.x1) && IsFreeAt(y + 1, box.x2)) {
                            can_move = true;
                        } else {
                            int id1 = -1;
                            int id2 = -1;
                            BoxAt(y + 1, box.x1, id1, boxes);
                            BoxAt(y + 1, box.x2, id2, boxes);
                            if (id1 > -1 && id2 > -1 && id1 == id2) {
                                can_move = CanBoxAtMove(y + 1, box.x1, direction, boxes);
                            } else if (id1 > -1 && id2 > -1 && id1 != id2) {
                                can_move = CanBoxAtMove(y + 1, box.x1, direction, boxes) && CanBoxAtMove(y + 1, box.x2, direction, boxes);
                            } else if (id1 > -1) {
                                can_move = CanBoxAtMove(y + 1, box.x1, direction, boxes);
                            } else if (id2 > -1) {
                                can_move = CanBoxAtMove(y + 1, box.x2, direction, boxes);
                            }
                        }
                    }
                    break;
                case '<':
                    if (!IsWallAt(y, box.x1 - 1)) {
                        if (IsFreeAt(y, box.x1 - 1)) {
                            can_move = true;
                        } else {
                            int id = -1;
                            BoxAt(y, box.x1 - 1, id, boxes);
                            if (id > -1) {
                                can_move = CanBoxAtMove(y, box.x1 - 1, direction, boxes);
                            }
                        }
                    }
                    break;
            }
            //if (!can_move) {
            //    std::cout << "Box " << box.id << " can't move " << direction << std::endl;
            //}
        }
    }
    return can_move;
}

bool BoxAtMove(int y, int x, char direction, std::vector<Box> &boxes)
{
    bool moved = false;
    for (auto &box : boxes) {
        //map[box.y][box.x1] = box.id + '0';
        if (y == box.y && (x == box.x1 || x == box.x2)) {
            switch (direction) {
                case '^':
                    if (!IsWallAt(y - 1, box.x1) && !IsWallAt(y - 1, box.x2)) {
                        if (IsFreeAt(y - 1, box.x1) && IsFreeAt(y - 1, box.x2)) {
                            moved = true;
                        } else {
                            int id1 = -1;
                            int id2 = -1;
                            BoxAt(y - 1, box.x1, id1, boxes);
                            BoxAt(y - 1, box.x2, id2, boxes);
                            if (id1 > -1 && id2 > -1 && id1 == id2) {
                                moved = BoxAtMove(y - 1, box.x1, direction, boxes);
                            } else if (id1 > -1 && id2 > -1 && id1 != id2) {
                                moved = BoxAtMove(y - 1, box.x1, direction, boxes) && BoxAtMove(y - 1, box.x2, direction, boxes);
                            } else if (id1 > -1) {
                                moved = BoxAtMove(y - 1, box.x1, direction, boxes);
                            } else if (id2 > -1) {
                                moved = BoxAtMove(y - 1, box.x2, direction, boxes);
                            }
                        }
                    }
                    if (moved) {
                        // Erase old box position
                        map[box.y][box.x1] = '.';
                        map[box.y][box.x2] = '.';
                        // Move box
                        box.y--;
                        // Draw new box position
                        map[box.y][box.x1] = '[';
                        map[box.y][box.x2] = ']';
                    }
                    break;
                case '>':
                    if (!IsWallAt(y, box.x2 + 1)) {
                        if (IsFreeAt(y, box.x2 + 1)) {
                            moved = true;
                        } else {
                            int id = -1;
                            BoxAt(y, box.x2 + 1, id, boxes);
                            if (id > -1) {
                                moved = BoxAtMove(y, box.x2 + 1, direction, boxes);
                            }
                        }
                    }
                    if (moved) {
                        // Erase old box position
                        map[box.y][box.x1] = '.';
                        map[box.y][box.x2] = '.';
                        // Move box
                        box.x1++;
                        box.x2++;
                        // Draw new box position
                        map[box.y][box.x1] = '[';
                        map[box.y][box.x2] = ']';
                    }
                    break;
                case 'v':
                    if (!IsWallAt(y + 1, box.x1) && !IsWallAt(y + 1, box.x2)) {
                        if (IsFreeAt(y + 1, box.x1) && IsFreeAt(y + 1, box.x2)) {
                            moved = true;
                        } else {
                            int id1 = -1;
                            int id2 = -1;
                            BoxAt(y + 1, box.x1, id1, boxes);
                            BoxAt(y + 1, box.x2, id2, boxes);
                            if (id1 > -1 && id2 > -1 && id1 == id2) {
                                moved = BoxAtMove(y + 1, box.x1, direction, boxes);
                            } else if (id1 > -1 && id2 > -1 && id1 != id2) {
                                moved = BoxAtMove(y + 1, box.x1, direction, boxes) && BoxAtMove(y + 1, box.x2, direction, boxes);
                            } else if (id1 > -1) {
                                moved = BoxAtMove(y + 1, box.x1, direction, boxes);
                            } else if (id2 > -1) {
                                moved = BoxAtMove(y + 1, box.x2, direction, boxes);
                            }
                        }
                    }
                    if (moved) {
                        // Erase old box position
                        map[box.y][box.x1] = '.';
                        map[box.y][box.x2] = '.';
                        // Move box
                        box.y++;
                        // Draw new box position
                        map[box.y][box.x1] = '[';
                        map[box.y][box.x2] = ']';
                    }
                    break;
                case '<':
                    if (!IsWallAt(y, box.x1 - 1)) {
                        if (IsFreeAt(y, box.x1 - 1)) {
                            moved = true;
                        } else {
                            int id = -1;
                            BoxAt(y, box.x1 - 1, id, boxes);
                            if (id > -1) {
                                moved = BoxAtMove(y, box.x1 - 1, direction, boxes);
                            }
                        }
                    }
                    if (moved) {
                        // Erase old box position
                        map[box.y][box.x1] = '.';
                        map[box.y][box.x2] = '.';
                        // Move box
                        box.x1--;
                        box.x2--;
                        // Draw new box position
                        map[box.y][box.x1] = '[';
                        map[box.y][box.x2] = ']';
                    }
                    break;
            }
            if (moved) {
                //std::cout << "Box " << box.id << " moved to " << box.y << "," << box.x1 << "-" << box.x2 << std::endl;
            }
        }
    }
    return moved;
}

bool CanMove(int y, int x, char direction, std::vector<Box> &boxes)
{
    bool can_move = false;
    switch (direction) {
        case '^':
            if (!IsWallAt(y - 1, x)) {
                if (IsFreeAt(y - 1, x)) {
                    can_move = true;
                } else {
                    int id = -1;
                    BoxAt(y - 1, x, id, boxes);
                    if (id > -1) {
                        can_move = CanBoxAtMove(y - 1, x, direction, boxes);
                    }
                }
            }
            break;
        case '>':
            if (!IsWallAt(y, x + 1)) {
                if (IsFreeAt(y, x + 1)) {
                    can_move = true;
                } else {
                    int id = -1;
                    BoxAt(y, x + 1, id, boxes);
                    if (id > -1) {
                        can_move = CanBoxAtMove(y, x + 1, direction, boxes);
                    }
                }
            }
            break;
        case 'v':
            if (!IsWallAt(y + 1, x)) {
                if (IsFreeAt(y + 1, x)) {
                    can_move = true;
                } else {
                    int id = -1;
                    BoxAt(y + 1, x, id, boxes);
                    if (id > -1) {
                        can_move = CanBoxAtMove(y + 1, x, direction, boxes);
                    }
                }
            }
            break;
        case '<':
            if (!IsWallAt(y, x - 1)) {
                if (IsFreeAt(y, x - 1)) {
                    can_move = true;
                } else {
                    int id = -1;
                    BoxAt(y, x - 1, id, boxes);
                    if (id > -1) {
                        can_move = CanBoxAtMove(y, x - 1, direction, boxes);
                    }
                }
            }
            break;
    }
    return can_move;
}

bool Move(int y, int x, char direction, std::vector<Box> &boxes, int &ry, int &rx)
{
    bool moved = false;
    switch (direction) {
        case '^':
            if (!IsWallAt(y - 1, x)) {
                if (IsFreeAt(y - 1, x)) {
                    ry--;
                    moved = true;
                } else {
                    int id = -1;
                    BoxAt(y - 1, x, id, boxes);
                    if (id > -1) {
                        moved = BoxAtMove(y - 1, x, direction, boxes);
                        if (moved) {
                            ry--;
                        }
                    }
                }
            }
        break;
        case '>':
            if (!IsWallAt(y, x + 1)) {
                if (IsFreeAt(y, x + 1)) {
                    rx++;
                    moved = true;
                } else {
                    int id = -1;
                    BoxAt(y, x + 1, id, boxes);
                    if (id > -1) {
                        moved = BoxAtMove(y, x + 1, direction, boxes);
                        if (moved) {
                            rx++;
                        }
                    }
                }
            }
        break;
        case 'v':
            if (!IsWallAt(y + 1, x)) {
                if (IsFreeAt(y + 1, x)) {
                    ry++;
                    moved = true;
                } else {
                    int id = -1;
                    BoxAt(y + 1, x, id, boxes);
                    if (id > -1) {
                        moved = BoxAtMove(y + 1, x, direction, boxes);
                        if (moved) {
                            ry++;
                        }
                    }
                }
            }
        break;
        case '<':
            if (!IsWallAt(y, x - 1)) {
                if (IsFreeAt(y, x - 1)) {
                    rx--;
                    moved = true;
                } else {
                    int id = -1;
                    BoxAt(y, x - 1, id, boxes);
                    if (id > -1) {
                        moved = BoxAtMove(y, x - 1, direction, boxes);
                        if (moved) {
                            rx--;
                        }
                    }
                }
            }
        break;
    }
    return moved;
}

void main()
{
    std::vector<Box> boxes;
    std::vector<char> commands;
    int ry = 0;
    int rx = 0;

    LoadMap("input15.txt", boxes, commands, ry, rx);
#if 0
    // List boxes
    for (auto box : boxes) {
        std::cout << "Box " << box.id << " at " << box.y << "," << box.x1 << "-" << box.x2 << std::endl;
    }

    // Initial visualization
    for (int y = 0; y < Y_SIZE; y++) {
        for (int x = 0; x < X_SIZE; x++) {
            std::cout << (x == rx && y == ry ? '@' : map[y][x]);
        }
        std::cout << std::endl;
    }
    std::cout << "Commands: " << commands << std::endl;
#endif

    // Run simulation
    for (auto cmd : commands) {
        //std::cout << "Move: " << cmd << std::endl;
        if (CanMove(ry, rx, cmd, boxes)) {
            if (!Move(ry, rx, cmd, boxes, ry, rx)) {
                std::cout << "Error moving" << std::endl;
            }
        }
    }

    // Visualization
    for (int y = 0; y < Y_SIZE; y++) {
        for (int x = 0; x < X_SIZE; x++) {
            std::cout << (x == rx && y == ry ? '@' : map[y][x]);
        }
        std::cout << std::endl;
    }

    long total_score = 0;
    for (auto box : boxes) {
        //std::cout << "Box " << box.id << " at " << box.y << "," << box.x1 << "-" << box.x2 << std::endl;
        total_score += 100 * box.y + box.x1;
    }
    std::cout << "Total score: " << total_score << std::endl;
}
