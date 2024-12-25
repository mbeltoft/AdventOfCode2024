#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <thread>
#include <mutex>

uint64_t Combo(uint64_t operand, uint64_t regA, uint64_t regB, uint64_t regC)
{
    uint64_t number = 0;
    switch (operand) {
        case 0:
            number = 0;
            break;
        case 1:
            number = 1;
            break;
        case 2:
            number = 2;
            break;
        case 3:
            number = 3;
            break;
        case 4:
            number = regA;
            break;
        case 5:
            number = regB;
            break;
        case 6:
            number = regC;
            break;
        default:
            std::cout << "Illegal operand" << std::endl;
            break;
    }
    return number;
}

std::vector<unsigned int> Run3bitComputer(uint64_t &regA, uint64_t &regB, uint64_t &regC, std::vector<unsigned int> program)
{
    // 3 bit computer with 3 registers and 7 opcodes

    size_t instruction_pointer = 0;
    std::vector<unsigned int> output;

    while (instruction_pointer < program.size()) {
        int opcode = program[instruction_pointer];
        int operand = program[instruction_pointer + 1];
        bool jump = false;

        switch (opcode) {
            case 0: // adv - division with register A and combo operand
                regA = regA / (1 << Combo(operand, regA, regB, regC));
                break;
            case 1: // bxl - bitwise XOR of register B and operand
                regB = regB ^ operand;
                break;
            case 2: // bst - combo operand
                regB = Combo(operand, regA, regB, regC) % 8;
                break;
            case 3: // jnz - jump not zero in register A
                if (regA != 0) {
                    instruction_pointer = operand;
                    jump = true;
                }
                break;
            case 4: // bxc - bitwise XOR of register B and register C
                regB = regB ^ regC;
                break;
            case 5: // out - calculate value of combo operand and outputs it
                output.push_back(Combo(operand, regA, regB, regC) % 8);
                break;
            case 6: // bdv - same as adv but store in register B
                regB = regA / (1 << Combo(operand, regA, regB, regC));
                break;
            case 7: // cdv - same as adv but store in register C
                regC = regA / (1 << Combo(operand, regA, regB, regC));
                break;
        }
        if (!jump) {
            instruction_pointer += 2;
        }
    }
    return output;
}

std::string OutputToString(std::vector<unsigned int> output)
{
    std::string result;
    for (auto o : output) {
        result += std::to_string(o);
    }
    return result;
}

void Tests()
{
    uint64_t regA = 0;
    uint64_t regB = 0;
    uint64_t regC = 0;

    regA = 0;
    regB = 0;
    regC = 9;
    std::vector<unsigned int> program{2,6};
    std::vector<unsigned int> output = Run3bitComputer(regA, regB, regC, program);
    assert(regB == 1);

    regA = 10;
    regB = 0;
    regC = 0;
    program = {5,0,5,1,5,4};
    output = Run3bitComputer(regA, regB, regC, program);
    assert(OutputToString(output) == "012");

    regA = 2024;
    regB = 0;
    regC = 0;
    program = {0,1,5,4,3,0};
    output = Run3bitComputer(regA, regB, regC, program);
    assert(OutputToString(output) == "42567777310");
    assert(regA == 0);

    regA = 0;
    regB = 29;
    regC = 0;
    program = {1,7};
    output = Run3bitComputer(regA, regB, regC, program);
    assert(regB == 26);

    regA = 0;
    regB = 2024;
    regC = 43690;
    program = {4,0};
    output = Run3bitComputer(regA, regB, regC, program);
    assert(regB == 44354);
}

void Test1()
{
    uint64_t regA = 729;
    uint64_t regB = 0;
    uint64_t regC = 0;
    std::vector<unsigned int> program{0,1,5,4,3,0};
    std::vector<unsigned int> output = Run3bitComputer(regA, regB, regC, program);
    //std::cout << OutputToString(output) << std::endl;
    assert(OutputToString(output) == "4635635210");
    // 4,6,3,5,6,3,5,2,1,0
}

void Part1()
{
    uint64_t regA = 35200350L;
    uint64_t regB = 0;
    uint64_t regC = 0;
    std::vector<unsigned int> program{2,4,1,2,7,5,4,7,1,3,5,5,0,3,3,0};
    std::vector<unsigned int> output = Run3bitComputer(regA, regB, regC, program);
    bool first = true;
    for (auto o : output) {
        std::cout << (first ? "" : ",") << o;
        first = false;
    }
    std::cout << std::endl;
    // 2,7,4,7,2,1,7,5,1
}

void main()
{
    Tests();
    Test1();
    Part1();
}