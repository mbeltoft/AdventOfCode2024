#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <thread>
#include <mutex>

// Try https://github.com/mquig42/AdventOfCode2024/blob/main/src/day17.cpp

uint64_t Combo(uint64_t operand, uint64_t regA, uint64_t regB, uint64_t regC)
{
    uint64_t number = 0;
    switch (operand) {
        case 4:
            number = regA;
        break;
        case 5:
            number = regB;
        break;
        case 6:
            number = regC;
        break;
        case 7:
            std::cout << "Illegal operand" << std::endl;
        break;
        default:
            number = operand;
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

bool search(uint64_t &min, uint64_t &max)
{
    std::cout << "min: " << min << " max: " << max << std::endl;
    uint64_t mid = (max - min) / 2 + min; // Avoid overflow
    std::cout << "Trying register A = " << mid << std::endl;
    uint64_t regA = mid;
    uint64_t regB = 0;
    uint64_t regC = 0;
    std::vector<unsigned int> program{2,4,1,2,7,5,4,7,1,3,5,5,0,3,3,0};
    uint64_t program_num = 2412754713550330LL;
    std::vector<unsigned int> output = Run3bitComputer(regA, regB, regC, program);
    // Convert vector to number
    bool overflow = output.size() > 64;
    uint64_t number = 0;
    for (auto o : output) {
        number = number * 10 + o;
    }
    std::cout << "Output number: " << number << std::endl;
    if (number == program_num) {
        std::cout << "Match found for register A = " << mid << std::endl;
        return true;
    }
    else if (overflow || number > program_num) {
        max = mid;
        return false;
        //return search(min, mid);
    }
    else {
        min = mid;
        return false;
        //return search(mid, max);
    }
}

void Test1()
{
    uint64_t regA = 729;
    uint64_t regB = 0;
    uint64_t regC = 0;
    std::vector<unsigned int> program{0,1,5,4,3,0};
    std::vector<unsigned int> output = Run3bitComputer(regA, regB, regC, program);
    assert(OutputToString(output) == "4635635210");
    // 4,6,3,5,6,3,5,2,1,0
}

void test()
{
    // The answer should be 202356708354602 - but my computer does not agree
    // My output: 1634151101741554

    uint64_t regA = 202356708354602LL;
    uint64_t regB = 0;
    uint64_t regC = 0;
    std::vector<unsigned int> program{2,4,1,2,7,5,4,7,1,3,5,5,0,3,3,0};
    std::vector<unsigned int> output = Run3bitComputer(regA, regB, regC, program);
    std::cout << OutputToString(output) << std::endl;
#if 0
    bool first = true;
    for (auto o : output) {
        std::cout << (first ? "" : ",") << o;
        first = false;
    }
    std::cout << std::endl;
#endif
#if 0
    uint64_t min = 0;
    uint64_t max = INT64_MAX;
    while (!search(min, max)) {
        if ((max - min) < 2) {
            std::cout << "No match found" << std::endl;
            break;
        }
    }
#endif
}

void Test2()
{
    // Program outputs a copy of itself
    uint64_t regA = 117440;
    uint64_t regB = 0;
    uint64_t regC = 0;
    std::vector<unsigned int> program{0,3,5,4,3,0};
    std::vector<unsigned int> output = Run3bitComputer(regA, regB, regC, program);
    assert(OutputToString(output) == "035430");
    // 0,3,5,4,3,0
}

std::mutex console_mutex;
int64_t solution = 0;

void Part2Job(int thread_num, int64_t min, int64_t max)
{
    //std::cout << "Thread " << thread_num << " running" << std::endl;
    std::vector<unsigned int> program{2,4,1,2,7,5,4,7,1,3,5,5,0,3,3,0};
    int64_t chunk = max - min;
    bool displayed = false;
    for (int64_t i = min; i < max; i++) {
        int64_t diff = max - i;
        int percentage = 100 * (chunk - diff) / chunk;
        if (percentage % 2 == 0 && percentage > 1) {
            if (!displayed) {
                std::lock_guard<std::mutex> guard(console_mutex);
                std::cout << "Thread " << thread_num << " " << percentage << "%" << std::endl;
                displayed = true;
            }
        } else {
            displayed = false;
        }

        uint64_t regA = i;
        uint64_t regB = 0;
        uint64_t regC = 0;
        std::vector<unsigned int> output = Run3bitComputer(regA, regB, regC, program);
        if (output.size() != program.size()) {
            continue;
        }
        bool match = true;
        for (size_t i = 0; i < output.size(); i++) {
            if (output[i] != program[i]) {
                match = false;
                break;
            }
        }
        if (match) {
            std::lock_guard<std::mutex> guard(console_mutex);
            std::cout << "Match found for register A = " << i << std::endl;
            solution = i;
            break;
        }
    }
}

void Part2()
{
    std::vector<std::thread> threads;

    // This is brutal brute force carried out by 32 threads
    // It will probably take days to finish
    int num_threads = 32;
    // Split INT64_MAX into 32 chunks
    int64_t chunk = (INT64_MAX / 1000000000000LL) / num_threads; // TODO Was INT64_MAX
    std::cout << "Chunk: " << chunk << std::endl;

    time_t start = time(nullptr);

    // Pass a chunk to each of the threads
    for (int thread_num = 0; thread_num < num_threads; thread_num++) {
        int64_t min = thread_num * chunk;
        int64_t max = (thread_num + 1) * chunk;
        threads.push_back(std::thread(Part2Job, thread_num, min, max));
    }

    for (auto &thread : threads) {
        thread.join();
    }

    time_t end = time(nullptr);
    std::cout << "Time spent: " << (end - start) << " sec" << std::endl;

    std::cout << "Solution: " << solution << std::endl;
}

void main()
{
    //Tests();
    Part1();
    //Test1();
    //Test2();
    //Part2();
    //test();
}