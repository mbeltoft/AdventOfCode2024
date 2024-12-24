#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

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

void main()
{
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
    std::cout << "Inputs: " << inputs.size() << std::endl;
    std::cout << "Gates: " << gates.size() << std::endl;
    std::cout << "Outputs: " << outputs.size() << std::endl;

    // Sort inputs
    //std::sort(inputs.begin(), inputs.end(), [](const IO &io1, const IO &io2){ return io1.name.compare(io2.name); }); // TODO Doesn't work
    std::sort(inputs.begin(), inputs.end());
    // Sort gates
    //std::sort(gates.begin(), gates.end(), [](const Gate &g1, const Gate &g2){ return g1.input1name.compare(g2.input1name); }); // TODO Doesn't work
    std::sort(gates.begin(), gates.end());
    // Sort outputs
    //std::sort(outputs.begin(), outputs.end(), [](const IO &io1, const IO &io2){ return io1.name.compare(io2.name); }); // TODO Doesn't work
    std::sort(outputs.begin(), outputs.end());

    // Repeat so gates settle down and outputs are correct for next inputs
    // We are not processing in correct gate order...
    // It appears that we have gates 6 deep, so we need 6 repetitions
    for (int rep = 0; rep < 6; rep++) {
        for (const auto &gate : gates) {
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

    int64_t value = 0;
    int cnt = 0;
    for (const auto &output : outputs) {
        if (output.name[0] == 'z') {
            std::cout << output.name << ": " << output.value << std::endl;
            value += (int64_t)output.value << cnt++;
        }
    }
    std::cout << "Value: " << value << std::endl;
    // 50411513338638
}