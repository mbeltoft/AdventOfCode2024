#include <iostream>
#include <cstdint>

void main()
{
    std::cout << "char " << sizeof(char) << std::endl;
    std::cout << "short " << sizeof(short) << std::endl;
    std::cout << "int " << sizeof(int) << std::endl;
    std::cout << "long " << sizeof(long) << std::endl;
    std::cout << "long long " << sizeof(long long) << std::endl;
    std::cout << "int8_t " << sizeof(int8_t) << std::endl;
    std::cout << "int16_t " << sizeof(int16_t) << std::endl;
    std::cout << "int32_t " << sizeof(int32_t) << std::endl;
    std::cout << "int64_t " << sizeof(int64_t) << std::endl;
}