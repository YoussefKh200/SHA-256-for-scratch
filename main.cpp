#include <iostream>
#include <string>
#include <bitset>
#include <cstdlib>
#include <thread>
#include <chrono>

// --- Utility functions ---
std::string bits(uint32_t x) {
    return std::bitset<32>(x).to_string();
}

uint32_t rotr(int n, uint32_t x) {
    return (x >> n) | (x << (32 - n));
}

uint32_t shr(int n, uint32_t x) {
    return x >> n;
}

void clear_screen() {
    system("clear"); // Linux terminal clear
}

void sleep_ms(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// --- σ0(x) ---
uint32_t sigma0(uint32_t x) {
    return rotr(7, x) ^ rotr(18, x) ^ shr(3, x);
}

// --- σ1(x) ---
uint32_t sigma1(uint32_t x) {
    return rotr(17, x) ^ rotr(19, x) ^ shr(10, x);
}

// --- Main driver ---
int main(int argc, char* argv[]) {
    uint32_t x = 0b00000000000000000011111111111111; // default

    if (argc > 1) {
        std::string arg = argv[1];
        if (arg.size() > 32) {
            std::cout << "We only operate on 32-bit words in SHA-256. Your x is "
                      << arg.size() << " bits.\n";
            return 1;
        }
        x = std::bitset<32>(arg).to_ulong();
    }

    clear_screen();
    std::cout << "Input x: " << bits(x) << "\n\n";

    uint32_t s0 = sigma0(x);
    uint32_t s1 = sigma1(x);

    std::cout << "σ0(x) = ROTR(7,x) ^ ROTR(18,x) ^ SHR(3,x)\n";
    std::cout << "σ0(x): " << bits(s0) << "\n\n";

    std::cout << "σ1(x) = ROTR(17,x) ^ ROTR(19,x) ^ SHR(10,x)\n";
    std::cout << "σ1(x): " << bits(s1) << "\n";

    return 0;
}
