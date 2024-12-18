#include <string>
#include <vector>
#include <fstream>
#include <iostream>

struct Point
{
public:
    Point(int r, int c) : r(r), c(c) {};
    int r = 0;
    int c = 0;
};

struct Region
{
public:
    Region(char plant_type) : plant_type(plant_type) {};
    bool IsPointInRegion(Point x) {
        for (auto p : points) {
            if (p.r == x.r && p.c == x.c) {
                //std::cout << "[" << plant_type << " " << p.r << "," << p.c << " " << x.r << "," << x.c << " hit]" << std::endl;
                return true;
            }
        }
        //std::cout << "[" << plant_type << " " << x.r << "," << x.c << " miss]" << std::endl;
        return false;
    }
    char plant_type = 0;
    std::vector<Point> points;
    int area = 0;
    int perimeter = 0;
    int sides = 0;
};

std::vector<Region> farm;

void main()
{
    int row = 0;
    std::fstream file("input12.txt");
    std::string line;
    const int COLS = 140;
    const int ROWS = 140;
    char map[ROWS][COLS];

    // Load map
    while (std::getline(file, line)) {
        for (size_t c = 0; c < line.size(); c++) {
            map[row][c] = line[c];
        }
        row++;
    }

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            char plant_type = map[row][col];
            //std::cout << row << "," << col << " " << plant_type;

            bool match_up = false;
            bool match_right = false;
            bool match_down = false;
            bool match_left = false;

            // Look up
            if (row > 0 && map[row-1][col] == plant_type) {
                match_up = true;
            }
            // Look right
            if (col < COLS - 1 && map[row][col+1] == plant_type) {
                match_right = true;
            }
            // Look down
            if (row < ROWS - 1 && map[row+1][col] == plant_type) {
                match_down = true;
            }
            // Look left
            if (col > 0 && map[row][col-1] == plant_type) {
                match_left = true;
            }

            int area = 1;
            //std::cout << " area: " << area;
            int perimeter = 4 - ((int)match_up + (int)match_right + (int)match_down + (int)match_left);
            //std::cout << " perimeter: " << perimeter;

            bool new_region = false;
            if (match_left) {
                //std::cout << " match left" << std::endl;
                // Find region to the left
                bool found = false;
                for (auto &r : farm) {
                    if (r.plant_type == plant_type && r.IsPointInRegion(Point(row, col - 1))) {
                        r.area += area;
                        r.perimeter += perimeter;
                        r.points.push_back(Point(row, col));
                        found = true;
                        break;
                    }
                }
                new_region = !found;
            } else if (match_up) {
                //std::cout << " match up" << std::endl;
                // Find region up
                bool found = false;
                for (auto &r : farm) {
                    if (r.plant_type == plant_type && r.IsPointInRegion(Point(row - 1, col))) {
                        r.area += area;
                        r.perimeter += perimeter;
                        r.points.push_back(Point(row, col));
                        found = true;
                        break;
                    }
                }
                new_region = !found;
            } else {
                new_region = true;
            }

            if (match_left && match_up) {
                // Merge regions left and up regions and erase left region

                // Find region to the left
                auto r_left = farm.end();
                for (auto r = farm.begin(); r != farm.end(); r++) {
                    if (r->plant_type == plant_type && r->IsPointInRegion(Point(row, col - 1))) {
                        r_left = r;
                        break;
                    }
                }
                // Find region up
                auto r_up = farm.end();
                for (auto r = farm.begin(); r != farm.end(); r++) {
                    if (r->plant_type == plant_type && r->IsPointInRegion(Point(row - 1, col))) {
                        r_up = r;
                    }
                }
                // Same region?
                if (r_left != r_up) {
                    // Copy content from left region to up region
                    for (auto p : r_left->points) {
                        r_up->points.push_back(p);
                    }
                    r_up->area += r_left->area;
                    r_up->perimeter += r_left->perimeter;
                    // Remove left region
                    farm.erase(r_left);
                }
            }

            if (new_region) {
                //std::cout << " new region" << std::endl;
                Region r = Region(plant_type);
                r.area += area;
                r.perimeter += perimeter;
                r.points.push_back(Point(row, col));
                farm.push_back(r);
            }
        }
        //std::cout << std::endl;
    }

    int total_score = 0;
    for (auto r : farm) {
        //std::cout << "Region with plant type " << r.plant_type << " has area " << r.area << " and perimeter " << r.perimeter << std::endl;
        total_score += r.area * r.perimeter;
    }

    std::cout << "Total score: " << total_score << std::endl;
    // 1344578
}
