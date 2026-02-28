#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include <thread>
#include <chrono>
#include <cmath>
#include <sstream>

void clearScreen(){
	std::cout << "\033[2J\033[1;1H";
}
void sleep(double seconds){
	std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(seconds*1000)));
}

std::string toBinaryString(uint32_t value){
	return std::bitset<32>(value).to_string();
}

uint32_t rotr(int n, uint32_t x) {
    uint32_t mask = 0xFFFFFFFF;
    uint32_t right = (x >> n) & mask;
    uint32_t left = (x << (32 - n)) & mask;
    return right | left;
}



void showXORAnimation() {
    uint32_t off = 0b00000000000000000000000000000000;
    uint32_t on = 0b11111111111111111111111111111111;
    uint32_t split = 0b10101010101010101010101010101010;
    double delay = 0.4;

    // Up To All 1
    clearScreen();
    std::cout << toBinaryString(off) << std::endl;
    std::cout << toBinaryString(off) << " XOR" << std::endl;
    std::cout << toBinaryString(off) << " XOR" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << toBinaryString(off ^ off ^ off) << std::endl;
    sleep(delay);

    clearScreen();
    std::cout << toBinaryString(on) << std::endl;
    std::cout << toBinaryString(off) << " XOR" << std::endl;
    std::cout << toBinaryString(off) << " XOR" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << toBinaryString(on ^ off ^ off) << std::endl;
    sleep(delay);

    clearScreen();
    std::cout << toBinaryString(on) << std::endl;
    std::cout << toBinaryString(on) << " XOR" << std::endl;
    std::cout << toBinaryString(off) << " XOR" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << toBinaryString(on ^ on ^ off) << std::endl;
    sleep(delay);

    clearScreen();
    std::cout << toBinaryString(on) << std::endl;
    std::cout << toBinaryString(on) << " XOR" << std::endl;
    std::cout << toBinaryString(on) << " XOR" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << toBinaryString(on ^ on ^ on) << std::endl;
    sleep(delay);

    // Back To All 0
    clearScreen();
    std::cout << toBinaryString(off) << std::endl;
    std::cout << toBinaryString(on) << " XOR" << std::endl;
    std::cout << toBinaryString(on) << " XOR" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << toBinaryString(off ^ on ^ on) << std::endl;
    sleep(delay);

    clearScreen();
    std::cout << toBinaryString(off) << std::endl;
    std::cout << toBinaryString(off) << " XOR" << std::endl;
    std::cout << toBinaryString(on) << " XOR" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << toBinaryString(off ^ off ^ on) << std::endl;
    sleep(delay);

    clearScreen();
    std::cout << toBinaryString(off) << std::endl;
    std::cout << toBinaryString(off) << " XOR" << std::endl;
    std::cout << toBinaryString(off) << " XOR" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << toBinaryString(off ^ off ^ off) << std::endl;
    sleep(delay);

    // Only One 1
    clearScreen();
    std::cout << toBinaryString(on) << std::endl;
    std::cout << toBinaryString(off) << " XOR" << std::endl;
    std::cout << toBinaryString(off) << " XOR" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << toBinaryString(on ^ off ^ off) << std::endl;
    sleep(delay);

    clearScreen();
    std::cout << toBinaryString(off) << std::endl;
    std::cout << toBinaryString(on) << " XOR" << std::endl;
    std::cout << toBinaryString(off) << " XOR" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << toBinaryString(off ^ on ^ off) << std::endl;
    sleep(delay);

    clearScreen();
    std::cout << toBinaryString(off) << std::endl;
    std::cout << toBinaryString(off) << " XOR" << std::endl;
    std::cout << toBinaryString(on) << " XOR" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << toBinaryString(off ^ off ^ on) << std::endl;
    sleep(delay);

    // Majority 1
    clearScreen();
    std::cout << toBinaryString(off) << std::endl;
    std::cout << toBinaryString(on) << " XOR" << std::endl;
    std::cout << toBinaryString(on) << " XOR" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << toBinaryString(off ^ on ^ on) << std::endl;
    sleep(delay);

    clearScreen();
    std::cout << toBinaryString(on) << std::endl;
    std::cout << toBinaryString(off) << " XOR" << std::endl;
    std::cout << toBinaryString(on) << " XOR" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << toBinaryString(on ^ off ^ on) << std::endl;
    sleep(delay);

    clearScreen();
    std::cout << toBinaryString(on) << std::endl;
    std::cout << toBinaryString(on) << " XOR" << std::endl;
    std::cout << toBinaryString(off) << " XOR" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << toBinaryString(on ^ on ^ off) << std::endl;
    sleep(delay);

    // All 1
    clearScreen();
    std::cout << toBinaryString(on) << std::endl;
    std::cout << toBinaryString(on) << " XOR" << std::endl;
    std::cout << toBinaryString(on) << " XOR" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << toBinaryString(on ^ on ^ on) << std::endl;
    sleep(delay);
    sleep(delay);

    // Split
    clearScreen();
    std::cout << toBinaryString(on) << std::endl;
    std::cout << toBinaryString(on) << " XOR" << std::endl;
    std::cout << toBinaryString(split) << " XOR" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << toBinaryString(on ^ on ^ split) << std::endl;
    sleep(delay);

    // Split - Rotate
    for (int i = 1; i <= 9; i++) {
        clearScreen();
        std::cout << toBinaryString(on) << std::endl;
        std::cout << toBinaryString(on) << " XOR" << std::endl;
        std::cout << toBinaryString(rotr(i, split)) << " XOR" << std::endl;
        std::cout << "--------------------------------" << std::endl;
        std::cout << toBinaryString(on ^ on ^ rotr(i, split)) << std::endl;
        sleep(delay / 2);
    }
    sleep(delay);

    for (int i = 1; i <= 9; i++) {
        clearScreen();
        std::cout << toBinaryString(on) << std::endl;
        std::cout << toBinaryString(off) << " XOR" << std::endl;
        std::cout << toBinaryString(rotr(i, split)) << " XOR" << std::endl;
        std::cout << "--------------------------------" << std::endl;
        std::cout << toBinaryString(on ^ off ^ rotr(i, split)) << std::endl;
        sleep(delay / 2);
    }
    sleep(delay);

    for (int i = 1; i <= 9; i++) {
        clearScreen();
        std::cout << toBinaryString(off) << std::endl;
        std::cout << toBinaryString(off) << " XOR" << std::endl;
        std::cout << toBinaryString(rotr(i, split)) << " XOR" << std::endl;
        std::cout << "--------------------------------" << std::endl;
        std::cout << toBinaryString(off ^ off ^ rotr(i, split)) << std::endl;
        sleep(delay / 2);
    }
    sleep(delay);
}

// ============ Command Line XOR Calculator ============

void calculateXOR(const std::vector<std::string>& binaries) {
    // Find the longest binary string for display
    size_t maxLength = 0;
    for (const auto& bin : binaries) {
        maxLength = std::max(maxLength, bin.length());
    }

    clearScreen();

    // Display each binary string with right justification
    for (const auto& bin : binaries) {
        std::cout << std::string(maxLength - bin.length(), ' ') << bin << std::endl;
    }

    // Display separator
    std::cout << std::string(maxLength, '-') << std::endl;

    // Calculate XOR of all binary strings
    uint32_t result = 0;
    for (const auto& bin : binaries) {
        // Convert binary string to integer
        uint32_t value = std::bitset<32>(bin).to_ulong();
        result ^= value;
    }

    // Display result with leading zeros
    std::string resultStr = toBinaryString(result);
    std::cout << std::string(maxLength - resultStr.length(), '0') << resultStr << std::endl;
}

// ============ Main ============

int main(int argc, char* argv[]) {
    // Check if binary string arguments are passed
    if (argc >= 2) {
        std::vector<std::string> binaries;

        // Collect all binary string arguments
        for (int i = 1; i < argc; i++) {
            binaries.push_back(argv[i]);
        }

        calculateXOR(binaries);
    } else {
        // No arguments - show animation
        showXORAnimation();
    }

    return 0;
}
