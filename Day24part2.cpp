#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

struct IO {
public:
    IO(const std::string &name, bool value) : name(name), value(value) {};

    bool operator < (const IO& rhs) const
    {
        if (name < rhs.name)
            return true;
        return false;
    };

    std::string name;
    bool value = false;
};

std::vector<IO> inputs;
std::vector<IO> outputs;

struct Gate {
public:
    Gate(const std::string &function, const std::string &input1name, const std::string &input2name, const std::string &outputname) :
        function(function), input1name(input1name), input2name(input2name), outputname(outputname) {};

    bool operator < (const Gate& rhs) const
    {
        if (input1name < rhs.input1name)
            return true;
        else if (input1name > rhs.input1name)
            return false;
        else
            return input2name < rhs.input2name;
    };

    std::string function;
    std::string input1name;
    std::string input2name;
    std::string outputname;
    bool value = false;
};

std::vector<Gate> gates;


// https://rosettacode.org/wiki/Combinations#C.2B.2B
std::vector<std::string> comb(int N, int K)
{
    std::vector<std::string> output;
    std::string bitmask(K, 1); // K leading 1's
    bitmask.resize(N, 0); // N-K trailing 0's
 
    // print integers and permute bitmask
    do {
        std::string line;
        for (int i = 0; i < N; ++i) // [0..N-1] integers
        {
            //if (bitmask[i]) std::cout << " " << i;
            if (bitmask[i]) line += std::to_string(i) + " ";
        }
        //std::cout << std::endl;
        output.push_back(line);
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
    return output;
}
 
void main()
{
    // This does not produce a result...
    std::cout << comb(222, 8).size() << std::endl;

    // https://www.calculatorsoup.com/calculators/discretemathematics/combinations.php
    // C(222,8) = 1.287952833 E+14
    // This will never work brute force...
}

void xmain()
{
    std::srand(std::time(nullptr)); // use current time as seed for random generator

    std::fstream file("input24.txt");
    std::string line;

    // x00: 1
    // or
    // ntg XOR fgs -> mjb
    while (std::getline(file, line)) {
        char input[20];
        int value;
        if (sscanf(line.c_str(), "%3s: %d", &input, &value) == 2) {
            inputs.push_back(IO(input, value == 1));
        }

        char input1[20];
        char function[20];
        char input2[20];
        char output_name[20];
        if (sscanf(line.c_str(), "%3s %s %3s -> %3s", &input1, &function, &input2, &output_name) == 4) {
            bool found = false;
            for (const auto &input : inputs) {
                if (input.name == input1) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                inputs.push_back(IO(input1, false));
            }
            found = false;
            for (const auto &input : inputs) {
                if (input.name == input2) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                inputs.push_back(IO(input2, false));
            }
            found = false;
            for (const auto &output : outputs) {
                if (output.name == output_name) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                outputs.push_back(IO(output_name, false));
            }
            gates.push_back(Gate(function, input1, input2, output_name));
        }
    }
    //std::cout << "Inputs: " << inputs.size() << std::endl;
    std::cout << "Gates: " << gates.size() << std::endl;
    //std::cout << "Outputs: " << outputs.size() << std::endl;
exit(0);
    // Sort inputs
    //std::sort(inputs.begin(), inputs.end(), [](const IO &io1, const IO &io2){ return io1.name.compare(io2.name); }); // TODO Doesn't work
    std::sort(inputs.begin(), inputs.end());
    // Sort gates
    //std::sort(gates.begin(), gates.end(), [](const Gate &g1, const Gate &g2){ return g1.input1name.compare(g2.input1name); }); // TODO Doesn't work
    std::sort(gates.begin(), gates.end());
    // Sort outputs
    //std::sort(outputs.begin(), outputs.end(), [](const IO &io1, const IO &io2){ return io1.name.compare(io2.name); }); // TODO Doesn't work
    std::sort(outputs.begin(), outputs.end());

    int64_t x_value = 0;
    int64_t y_value = 0;
    int64_t z_value = 0;
    int x_cnt = 0;
    int y_cnt = 0;
    for (const auto &input : inputs) {
        if (input.name[0] == 'x') {
            //std::cout << input.name << ": " << input.value << std::endl;
            x_value += (int64_t)input.value << x_cnt++;
        }
        if (input.name[0] == 'y') {
            //std::cout << input.name << ": " << input.value << std::endl;
            y_value += (int64_t)input.value << y_cnt++;
        }
    }
    std::cout << "X value: " << x_value << std::endl;
    std::cout << "Y value: " << y_value << std::endl;
    int64_t correct_z_value = x_value + y_value;
    std::cout << "Calc Z value: " << correct_z_value << std::endl;

    std::vector<int> picked_indexes;

    int64_t count = 0;
    do {
        picked_indexes.clear();

        // Prepare a vector of all indexes into the gates vector
        std::vector<int> gate_indexes;
        for (size_t i = 0; i < gates.size(); i++) {
            gate_indexes.push_back(i);
        }

        // Pick 8 random indexes into the gates vector
        for (int i = 0; i < 8; i++) {
            size_t idx = std::rand() % gate_indexes.size();
            picked_indexes.push_back(gate_indexes.at(idx)); // Add index
            gate_indexes.erase(gate_indexes.begin() + idx); // Remove index
        }

#if 0
        for (auto picked : picked_indexes) {
            std::cout << "Picked: " << picked << std::endl;
        }
        for (auto gate_idx : gate_indexes) {
            std::cout << "Gate index: " << gate_idx << std::endl;
        }
        break;
#endif
        // Copy gates so we can swap outputs
        std::vector<Gate> gates_copy(gates);

        // Swap outputs in pairs
        // 0 and 1 are swapped, 2 and 3 are swapped, 4 and 5 are swapped, 6 and 7 are swapped
        for (int i = 0; i < 8; i += 2) {
            auto tmp = gates_copy[picked_indexes[i]].outputname;
            gates_copy[picked_indexes[i]].outputname = gates_copy[picked_indexes[i + 1]].outputname;
            gates_copy[picked_indexes[i + 1]].outputname = tmp;
        }

        // Repeat so gates settle down and outputs are correct for next inputs
        // We are not processing in correct gate order...
        // It appears that we have gates 6 deep, so we need 6 repetitions
        for (int rep = 0; rep < 6; rep++) {
            for (const auto &gate : gates_copy) {
                bool input1 = false;
                bool input2 = false;
                bool found1 = false;
                bool found2 = false;
                for (const auto &output : outputs) {
                    //std::cout << output.name << " " << gate.input1name << std::endl;
                    if (output.name == gate.input1name) {
                        input1 = output.value;
                        found1 = true;
                    }
                    if (output.name == gate.input2name) {
                        input2 = output.value;
                        found2 = true;
                    }
                }
                for (const auto &input : inputs) {
                    //std::cout << input.name << " " << gate.input1name << std::endl;
                    if (input.name == gate.input1name && !found1) {
                        input1 = input.value;
                    }
                    if (input.name == gate.input2name && !found2) {
                        input2 = input.value;
                    }
                }
                //std::cout << gate.function << " " << input1 << " " << input2 << std::endl;
                bool output_value = false;
                if (gate.function == "AND") {
                    output_value = input1 && input2;
                }
                else if (gate.function == "OR") {
                    output_value = input1 || input2;
                }
                else if (gate.function == "XOR") {
                    output_value = input1 ^ input2;
                }
                for (auto &output : outputs) {
                    if (output.name == gate.outputname) {
                        output.value = output_value;
                    }
                }
            }
        }

        z_value = 0;
        int cnt = 0;
        for (const auto &output : outputs) {
            if (output.name[0] == 'z') {
                //std::cout << output.name << ": " << output.value << std::endl;
                z_value += (int64_t)output.value << cnt++;
            }
        }
        //std::cout << "Value: " << z_value << std::endl;

        if (count++ % 1000 == 0) {
            std::cout << "Count: " << count << std::endl;
        }
    } while (z_value != correct_z_value);
    
    for (int i = 0; i < 8; i ++) {
        std::cout << gates[picked_indexes[i]].outputname << std::endl;
    }
}