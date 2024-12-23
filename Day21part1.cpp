#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#if 0
inline int64_t Mix(int64_t tmp, int64_t secret)
{
    return tmp ^ secret;
}

inline int64_t Prune(int64_t secret)
{
    return secret % 16777216;
}

inline int64_t NewSecretNumber(int64_t old_secret)
{
    int64_t tmp = old_secret * 64;
    int64_t new_secret = Prune(Mix(tmp, old_secret));
    tmp = new_secret >> 5; // Divide by 32
    new_secret = Prune(Mix(tmp, new_secret));
    tmp = new_secret << 11; // Multiply by 2048
    new_secret = Prune(Mix(tmp, new_secret));
    return new_secret;
}
#else
inline int64_t NewSecretNumber(int64_t old_secret)
{
    int64_t tmp = old_secret * 64;
    int64_t new_secret = (tmp ^ old_secret) % 16777216;
    tmp = new_secret >> 5; // Divide by 32
    new_secret = (tmp ^ new_secret) % 16777216;
    tmp = new_secret << 11; // Multiply by 2048
    new_secret = (tmp ^ new_secret) % 16777216;
    return new_secret;
}
#endif

void test1()
{
    // Test 10 iterations with 123 as origin
    int secret = 123;
    for (int i = 0; i < 10; i++) {
        secret = NewSecretNumber(secret);
        std::cout << secret << std::endl;
    }
}

void test2()
{
    // 2000 iterations over 4 buyers secret numbers
    const int num_buyers = 4;
    int32_t buyer_secrets[num_buyers] = {1, 10, 100, 2024};
    int64_t sum = 0;
    for (int b = 0; b < num_buyers; b++) {
        for (int i = 0; i < 2000; i++) {
            buyer_secrets[b] = NewSecretNumber(buyer_secrets[b]);
        }
        sum += buyer_secrets[b];
        std::cout << "Buyer " << b << ": " << buyer_secrets[b] << std::endl;
    }
    std::cout << "Sum: " << sum << std::endl;
}

void main()
{
    std::vector<int64_t> buyer_secrets;
    std::fstream file("input21.txt");
    std::string line;

    while (std::getline(file, line)) {
        buyer_secrets.push_back(strtoull(line.c_str(), nullptr, 10));
    }

    // 2000 iterations over buyers secret numbers
    int64_t sum = 0;
    for (int b = 0; b < buyer_secrets.size(); b++) {
        //std::cout << "Buyer " << b << ": " << buyer_secrets[b] << std::endl;
        for (int i = 0; i < 2000; i++) {
            buyer_secrets[b] = NewSecretNumber(buyer_secrets[b]);
        }
        sum += buyer_secrets[b];
    }
    std::cout << "Sum: " << sum << std::endl;
}