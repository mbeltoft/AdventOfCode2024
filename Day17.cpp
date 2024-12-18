#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <thread>
#include <mutex>

uint64_t register_A = 0;
uint64_t register_B = 0;
uint64_t register_C = 0;

uint64_t Combo(uint64_t operand)
{
    uint64_t number = 0;
    switch (operand) {
        case 4:
            number = register_A;
        break;
        case 5:
            number = register_B;
        break;
        case 6:
            number = register_C;
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

std::vector<int> Run3bitComputer(uint64_t regA, uint64_t regB, uint64_t regC, std::vector<int> program)
{
    // 3 bit computer with 3 registers and 7 opcodes

    register_A = regA;
    register_B = regB;
    register_C = regC;
    size_t instruction_pointer = 0;
    std::vector<int> output;

    while (instruction_pointer < program.size()) {
        int opcode = program[instruction_pointer];
        int operand = program[instruction_pointer + 1];
        uint64_t result = 0;
        bool jump = false;

        switch (opcode) {
            case 0: // adv - division with register A and combo operand
                result = register_A / (1 << Combo(operand));
                register_A = result;
                //std::cout << "0 A:" << result << std::endl;
                break;
            case 1: // bxl - bitwise XOR of register B and operand
                result = register_B ^ operand;
                register_B = result;
                //std::cout << "1 B:" << result << std::endl;
                break;
            case 2: // bst - combo operand
                result = Combo(operand) % 8;
                register_B = result;
                //std::cout << "2 B:" << result << std::endl;
                break;
            case 3: // jnz - jump not zero in register A
                if (register_A != 0) {
                    instruction_pointer = operand;
                    jump = true;
                }
                break;
            case 4: // bxc - bitwise XOR of register B and register C
                result = register_B ^ register_C;
                register_B = result;
                //std::cout << "4 B:" << result << std::endl;
                break;
            case 5: // out - calculate value of combo operand and outputs it
                output.push_back(Combo(operand) % 8);
                break;
            case 6: // bdv - same as adv but store in register B
                result = register_A / (1 << Combo(operand));
                register_B = result;
                //std::cout << "6 B:" << result << std::endl;
                break;
            case 7: // cdv - same as adv but store in register C
                //std::cout << "7 A:" << register_A << " C:" << register_C << " Combo:" << Combo(operand) << " Operand:" << operand << std::endl;
                //std::cout << "Divisor: " << (1 << Combo(operand)) << std::endl;
                result = register_A / (1 << Combo(operand));
                register_C = result;
                //std::cout << "7 C:" << result << std::endl;
                break;
        }
        if (!jump) {
            instruction_pointer += 2;
        }
    }
    return output;
}

std::string OutputToString(std::vector<int> output)
{
    std::string result;
    for (auto o : output) {
        result += std::to_string(o);
    }
    return result;
}

void Tests()
{
    std::vector<int> program{2,6};
    std::vector<int> output = Run3bitComputer(0, 0, 9, program);
    assert(register_B == 1);

    program = {5,0,5,1,5,4};
    output = Run3bitComputer(10, 0, 0, program);
    assert(OutputToString(output) == "012");

    program = {0,1,5,4,3,0};
    output = Run3bitComputer(2024, 0, 0, program);
    assert(OutputToString(output) == "42567777310");
    assert(register_A == 0);

    program = {1,7};
    output = Run3bitComputer(0, 29, 0, program);
    assert(register_B == 26);

    program = {4,0};
    output = Run3bitComputer(0, 2024, 43690, program);
    assert(register_B == 44354);
}

void Part1()
{
    std::vector<int> program{2,4,1,2,7,5,4,7,1,3,5,5,0,3,3,0};
    std::vector<int> output = Run3bitComputer(35200350L, 0, 0, program);
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
    std::vector<int> program{2,4,1,2,7,5,4,7,1,3,5,5,0,3,3,0};
    uint64_t program_num = 2412754713550330;
    std::vector<int> output = Run3bitComputer(mid, 0, 0, program);
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

void test()
{
    // The answer should be 202356708354602 - but my computer does not agree

    std::vector<int> program{2,4,1,2,7,5,4,7,1,3,5,5,0,3,3,0};
    std::vector<int> output = Run3bitComputer(202356708354602L, 0, 0, program);
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
    std::vector<int> program{0,3,5,4,3,0};
    std::vector<int> output = Run3bitComputer(117440, 0, 0, program);
    assert(OutputToString(output) == "035430");
    // 0,3,5,4,3,0
}

std::mutex console_mutex;
int64_t solution = 0;

void Part2Job(int thread_num, int64_t min, int64_t max)
{
    //std::cout << "Thread " << thread_num << " running" << std::endl;
    std::vector<int> program{2,4,1,2,7,5,4,7,1,3,5,5,0,3,3,0};
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

        //if ((i % 1000000L) == 0) {
        //    std::cout << i << std::endl;
        //}
        //std::cout << "Trying register A = " << i << std::endl;
        std::vector<int> output = Run3bitComputer(i, 0, 0, program);
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
    //
}

void Part2()
{
    std::vector<std::thread> threads;

    int num_threads = 32;
    for (int thread_num = 0; thread_num < num_threads; thread_num++) {
        // Split INT64_MAX into 256 chunks
        int64_t chunk = INT64_MAX / num_threads; // TODO Was INT64_MAX
        int64_t min = thread_num * chunk;
        int64_t max = (thread_num + 1) * chunk;
        threads.push_back(std::thread(Part2Job, thread_num, min, max));
    }

    for (auto &thread : threads) {
        thread.join();
    }

    std::cout << "Solution: " << solution << std::endl;
}

void main()
{
    //Tests();
    //Part1();
    //Test2();
    Part2();
}