#include <iostream>
#include <string>
#include <vector>
#include "StringSplit.h"

const int rows = 50; // 12 for test map, 50 for real input
const int cols = 50; // 12 for test map, 50 for real input
char map[rows][cols];

struct Point {
public:
    Point(int r_, int c_)
        : r(r_), c(c_) {};
    int r = -1;
    int c = -1;
};

struct Signal {
public:
    Signal(char feature_, int r1_, int c1_)
        : feature(feature_), r1(r1_), c1(c1_) {};
    Signal(char feature_, int r1_, int c1_, int r2_, int c2_)
        : feature(feature_), r1(r1_), c1(c1_), r2(r2_), c2(c2_)
        {
            int diffr = r2 - r1;
            int diffc = c2 - c1;
            int anti1r = r1 - diffr;
            int anti1c = c1 - diffc;
            int anti2r = r2 + diffr;
            int anti2c = c2 + diffc;
            auto anti1 = Point(anti1r, anti1c);
            auto anti2 = Point(anti2r, anti2c);
            // Check limits
            if (anti1r >= 0 && anti1r < rows && anti1c >= 0 && anti1c < cols) {
                antinodes.push_back(Point(anti1r, anti1c));
            }
            if (anti2r >= 0 && anti2r < rows && anti2c >= 0 && anti2c < cols) {
                antinodes.push_back(Point(anti2r, anti2c));
            }
        };
    char feature = '.';
    int r1 = -1;
    int c1 = -1;
    int r2 = -1;
    int c2 = -1;
    std::vector<Point> antinodes;
};

// https://tomeko.net/online_tools/cpp_text_escape.php?lang=en
std::vector<std::string> test_map_input{
"............",
"........0...",
".....0......",
".......0....",
"....0.......",
"......A.....",
"............",
"............",
"........A...",
".........A..",
"............",
"............"};

std::vector<std::string> map_input{
"...........6.b....................................",
"........6................8........................",
"..Y.......................................o.......",
"....V...j............B.............c..............",
"............8.........X.......L...................",
".....j..v6.......3.L..................c...........",
"..Mj.....p3.......b........Z....................J.",
"..........M...X...................................",
"V..............v......p.........Z.........c.......",
"..............3...................................",
".......V......U3.............c....................",
"..........b..v.M.U8...............................",
"..........j........8.....................J........",
"..........Y......q........LH..Z...D...........y...",
"..2Y........PX......6..................BQ.........",
"...0.Y...............XP...........w...............",
".........U.......2...............oH.y.............",
"0..............9........U.........................",
"...........P..............W.......z...Oy..........",
"...................t...p.W..o.............Q.......",
".....S.................t.....Q....B...............",
"S.k..................V..W...p.......H...O......m..",
"....S.h................W.......................O..",
"..h..P.2.............Z.............J..............",
".........k.......5v.......q...t.s.................",
".....Q.....h..........................J...B.......",
"........0.........l...............................",
".S................................................",
".............................M....................",
"2..................e.....o.....y..................",
"................k.................................",
"......4......k....t...s.q.........................",
".4.......................q........................",
".......................z....E.....................",
".............0.....d..............................",
"7..........D........z.............................",
".......D..5......7..9.............................",
"......5..................E........................",
"D..............K......d..9E..........w.....1..C...",
".......K..x.........d....s...........l............",
"........7......................u...C..............",
"..K........x..............9..C...u................",
"4..............s.........................l...T..w.",
".......5.....7..................m......T......1...",
"...........................E...z.m................",
"......................................u...C.......",
".............................em...................",
"..............................................T...",
"....................x.......................e.....",
".............................1e....w....l........."
};

int main(int argc, char* argv[])
{
    // Load input
    for (int row = 0; row < rows; row++) {
        //auto map_row = test_map_input[row];
        auto map_row = map_input[row];
        for (int col = 0; col < cols; col++) {
            //if (map_row[col] == 'j')
                map[row][col] = map_row[col];
            //else
            //    map[row][col] = '.';
            std::cout << map[row][col];
        }
        std::cout << std::endl;
    }

    // Create map features, which are map points other than '.'
    std::vector<Signal> features;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (map[row][col] != '.') {
                features.push_back(Signal(map[row][col], row, col));
            }
        }
    }
    std::cout << "Map features: " << features.size() << std::endl;

    // Match map features and create signals
    std::vector<Signal> signals;
    for (int f = 0; f < features.size() - 1; f++) {
        auto feature = features[f];
        //std::cout << feature.feature << " " << feature.c1 << " " << feature.r1 << std::endl;
        // Search through rest of map features to find matches
        for (int f2 = f + 1; f2 < features.size(); f2++) {
            auto feature2 = features[f2];
            if (feature.feature == feature2.feature) {
                signals.push_back(Signal(feature.feature,
                        feature.r1, feature.c1,
                        feature2.r1, feature2.c1));
                //std::cout << feature.feature << " " << feature.r1 << "," << feature.c1 << " + " << feature2.r1 << "," << feature2.c1 << std::endl;
            }
        }
    }
    std::cout << "Signals: " << signals.size() << std::endl;

    // Viasualize map with antinodes on top
    char visual[rows][cols];
    memset(visual, '.', sizeof(visual));
    // Plot features
    for (auto signal : signals) {
        visual[signal.r1][signal.c1] = signal.feature;
        visual[signal.r2][signal.c2] = signal.feature;
    }
    // Plot antinodes
    for (auto signal : signals) {
        for (auto antinode : signal.antinodes) {
            visual[antinode.r][antinode.c] = '#';
        }
    }
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            std::cout << visual[row][col];
        }
        std::cout << std::endl;
    }

    // Antinode heat map
    char antinodes[rows][cols];
    memset(antinodes, 0, sizeof(antinodes));
    for (auto signal : signals) {
        for (auto antinode : signal.antinodes) {
            antinodes[antinode.r][antinode.c]++;
        }
    }

    // Count unique locations
    int unique = 0;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (antinodes[row][col] > 0) {
                unique++;
            }
        }
    }
    std::cout << "Unique antinode locations: " << unique << std::endl;
    // 305

    return 0;
}