#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

struct Point
{
public:
    Point(int r, int c) : r(r), c(c) {};
    int r = 0;
    int c = 0;
    int corners = 0;
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
    int corners = 0;
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
            if (plant_type == '.') {
                continue;
            }

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

            bool new_region = false;
            if (match_left) {
                //std::cout << " match left" << std::endl;
                // Find region to the left
                bool found = false;
                for (auto &r : farm) {
                    if (r.plant_type == plant_type && r.IsPointInRegion(Point(row, col - 1))) {
                        r.area += area;
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
                    // Remove left region
                    farm.erase(r_left);
                }
            }

            if (new_region) {
                //std::cout << " new region" << std::endl;
                Region r = Region(plant_type);
                r.area += area;
                r.points.push_back(Point(row, col));
                farm.push_back(r);
            }
        }
        //std::cout << std::endl;
    }

    // Plot each reagion on a fresh map
    int total_score = 0;
    for (auto &region : farm) {
        //std::cout << std::endl;
        char map[ROWS][COLS];
        memset(map, '.', sizeof(map));
        for (auto p : region.points) {
            map[p.r][p.c] = region.plant_type;
        }

        // RRRR......
        // RRRR......
        // ..RRR.....
        // ..R.......
        // ..........

        // I counted the number of corners for each region, which is equal to the number of edges
        // (each edge has 2 corners, and each corner has 2 edges).
        // Corners can be determined locally (just need to check 3 adjacent squares) so that's easier to do.

        //std::cout << "Start" << std::endl;

        int corners = 0;
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                if (map[r][c] != '.') {
                    bool can_go_E = false;
                    bool can_go_SE = false;
                    bool can_go_S = false;
                    bool can_go_SW = false;
                    bool can_go_W = false;
                    bool can_go_NW = false;
                    bool can_go_N = false;
                    bool can_go_NE = false;
                    if (c < COLS - 1 && map[r][c + 1] != '.') {
                        can_go_E = true;
                    }
                    if (r < ROWS - 1 && c < COLS - 1 && map[r + 1][c + 1] != '.') {
                        can_go_SE = true;
                    }
                    if (r < ROWS - 1 && map[r + 1][c] != '.') {
                        can_go_S = true;
                    }
                    if (r < ROWS - 1 && c > 0 && map[r + 1][c - 1] != '.') {
                        can_go_SW = true;
                    }
                    if (c > 0 && map[r][c - 1] != '.') {
                        can_go_W = true;
                    }
                    if (r > 0 && c > 0 && map[r - 1][c - 1] != '.') {
                        can_go_NW = true;
                    }
                    if (r > 0 && map[r - 1][c] != '.') {
                        can_go_N = true;
                    }
                    if (r > 0 && c < COLS - 1 && map[r - 1][c + 1] != '.') {
                        can_go_NE = true;
                    }

                    std::stringstream ss;
                    ss << r << "," << c;

                    // Corners pointing out
                    bool corner_NE = !can_go_N && !can_go_NE && !can_go_E;
                    ss << " NE:" << corner_NE;
                    bool corner_SE = !can_go_S && !can_go_SE && !can_go_E;
                    ss << " SE:" << corner_SE;
                    bool corner_SW = !can_go_S && !can_go_SW && !can_go_W;
                    ss << " SW:" << corner_SW;
                    bool corner_NW = !can_go_N && !can_go_NW && !can_go_W;
                    ss << " NW:" << corner_NW;

                    // Corners pointing in
                    bool corner_i_NE = can_go_N && !can_go_NE && can_go_E;
                    ss << " iNE:" << corner_i_NE;
                    bool corner_i_SE = can_go_S && !can_go_SE && can_go_E;
                    ss << " iSE:" << corner_i_SE;
                    bool corner_i_SW = can_go_S && !can_go_SW && can_go_W;
                    ss << " iSW:" << corner_i_SW;
                    bool corner_i_NW = can_go_N && !can_go_NW && can_go_W;
                    ss << " iNW:" << corner_i_NW;

                    // Double corners
                    bool corner_up_fw = !can_go_E && can_go_NE && !can_go_N;
                    ss << " upfw:" << corner_up_fw;
                    bool corner_up_rw = !can_go_W && can_go_NW && !can_go_N;
                    ss << " uprw:" << corner_up_rw;
                    bool corner_dw_fw = !can_go_E && can_go_SE && !can_go_S;
                    ss << " dwfw:" << corner_dw_fw;
                    bool corner_dw_rw = !can_go_W && can_go_SW && !can_go_S;
                    ss << " dwrw:" << corner_dw_rw;

                    //std::cout << ss.str() << std::endl;

                    int count = 0;
                    count += corner_NE;
                    count += corner_SE;
                    count += corner_SW;
                    count += corner_NW;
                    count += corner_i_NE;
                    count += corner_i_SE;
                    count += corner_i_SW;
                    count += corner_i_NW;
                    count += corner_up_fw;
                    count += corner_up_rw;
                    count += corner_dw_fw;
                    count += corner_dw_rw;
                    for (auto &point : region.points) {
                        if (point.r == r && point.c == c) {
                            point.corners = count;
                        }
                    }
                    corners += count;
                }
            }
        }
        //std::cout << "Corners: " << corners << std::endl;
        region.corners = corners;

        std::cout << "Plant:" << region.plant_type << " Area: " << region.area << " Corners:" << region.corners << std::endl;
        total_score += region.area * region.corners;

#if 0
        // Visualize number of corners - doesn't work
        memset(map, '.', sizeof(map));
        for (auto point : region.points) {
            map[point.r][point.c] = '0' + point.corners;
        }
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                std::cout << map[c][r];
            }
            std::cout << std::endl;
        }
#endif
    }

    std::cout << "Total score: " << total_score << std::endl;
    // 814302
}
