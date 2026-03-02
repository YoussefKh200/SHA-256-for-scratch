#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include <thread>
#include <chrono>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <fstream>
#include <regex>

// ============ Global Variables ============
std::string g_delay = "normal";

// ============ Utility Functions ============

void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

std::string bits(uint32_t x, int n = 32) {
    if (x >= 0) {
        std::string binary = std::bitset<32>(x).to_string();
        return binary.substr(32 - n, n);
    } else {
        // Handle negative numbers by getting individual bits
        std::string result;
        for (int i = n - 1; i >= 0; i--) {
            result += ((x >> i) & 1) ? '1' : '0';
        }
        return result;
    }
}

std::string hex(uint32_t i) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(8) << i;
    return ss.str();
}

std::string bitstring(const std::string& str) {
    std::string result;
    for (unsigned char c : str) {
        result += bits(c, 8);
    }
    return result;
}

void delay(const std::string& speed) {
    if (g_delay == "enter") {
        std::cin.get();
    } else if (g_delay == "nodelay") {
        std::this_thread::sleep_for(std::chrono::milliseconds(0));
    } else {
        double multiplier = 1.0;
        if (g_delay == "fast") {
            multiplier = 0.5;
        }

        int sleepTime = 0;
        if (speed == "fastest") sleepTime = 100 * multiplier;
        else if (speed == "fast") sleepTime = 200 * multiplier;
        else if (speed == "normal") sleepTime = 400 * multiplier;
        else if (speed == "slow") sleepTime = 600 * multiplier;
        else if (speed == "slowest") sleepTime = 800 * multiplier;
        else if (speed == "end") sleepTime = 1000 * multiplier;
        else sleepTime = std::stoi(speed);

        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }
}

std::string input_type(const std::string& input) {
    // Check if input is referencing a file
    std::ifstream file(input);
    if (file.good()) {
        return "file";
    } else {
        // Check for hex or binary prefix
        if (input.length() >= 2) {
            std::string prefix = input.substr(0, 2);
            if (prefix == "0b") {
                // Check it's a valid binary string
                std::string binary = input.substr(2);
                if (binary.find_first_not_of("01") != std::string::npos) {
                    std::cout << "Invalid binary string: " << input << std::endl;
                    exit(1);
                }
                return "binary";
            } else if (prefix == "0x") {
                // Check it's a valid hex string
                std::string hexStr = input.substr(2);
                std::regex hexPattern("^[0-9A-Fa-f]+$");
                if (!std::regex_match(hexStr, hexPattern)) {
                    std::cout << "Invalid hex string: " << input << std::endl;
                    exit(1);
                }
                return "hex";
            }
        }
        return "string";
    }
}

std::vector<uint8_t> bytes(const std::string& input, const std::string& type) {
    std::vector<uint8_t> result;

    if (type == "binary") {
        std::string bin = input.substr(2); // trim 0b prefix
        if (bin.length() % 8 == 0) {
            for (size_t i = 0; i < bin.length(); i += 8) {
                std::string byteStr = bin.substr(i, 8);
                result.push_back(std::bitset<8>(byteStr).to_ulong());
            }
        }
    } else if (type == "hex") {
        std::string hexStr = input.substr(2); // trim 0x prefix
        for (size_t i = 0; i < hexStr.length(); i += 2) {
            std::string byteStr = hexStr.substr(i, 2);
            result.push_back(std::stoi(byteStr, nullptr, 16));
        }
    } else {
        for (char c : input) {
            result.push_back(static_cast<uint8_t>(c));
        }
    }

    return result;
}

// ============ Operations ============

uint32_t add(const std::vector<uint32_t>& values) {
    uint64_t total = 0;
    for (uint32_t val : values) {
        total += val;
    }
    return static_cast<uint32_t>(total & 0xFFFFFFFF);
}

uint32_t add(uint32_t a, uint32_t b) {
    return (a + b) & 0xFFFFFFFF;
}

uint32_t add(uint32_t a, uint32_t b, uint32_t c) {
    return (a + b + c) & 0xFFFFFFFF;
}

uint32_t add(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    return (a + b + c + d) & 0xFFFFFFFF;
}

uint32_t add(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e) {
    uint64_t total = static_cast<uint64_t>(a) + b + c + d + e;
    return static_cast<uint32_t>(total & 0xFFFFFFFF);
}

// Rotate right (circular right shift)
uint32_t rotr(int n, uint32_t x) {
    uint32_t right = (x >> n);
    uint32_t left = (x << (32 - n));
    uint32_t result = right | left;
    return result & 0xFFFFFFFF;
}

// Shift right
uint32_t shr(int n, uint32_t x) {
    return x >> n;
}

// ============ Functions ============

// σ0
uint32_t sigma0(uint32_t x) {
    return rotr(7, x) ^ rotr(18, x) ^ shr(3, x);
}

// σ1
uint32_t sigma1(uint32_t x) {
    return rotr(17, x) ^ rotr(19, x) ^ shr(10, x);
}

// Σ0 (uppercase sigma)
uint32_t usigma0(uint32_t x) {
    return rotr(2, x) ^ rotr(13, x) ^ rotr(22, x);
}

// Σ1
uint32_t usigma1(uint32_t x) {
    return rotr(6, x) ^ rotr(11, x) ^ rotr(25, x);
}

// Choice - Use first bit to choose the (1)second or (0)third bit
uint32_t ch(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ ((~x) & z);
}

// Majority - Result is the majority of the three bits
uint32_t maj(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

// ============ Preprocessing ============

std::string padding(const std::string& message) {
    size_t l = message.size();  // size of message (in bits)
    int k = (448 - static_cast<int>(l) - 1) % 512;
    if (k < 0) k += 512;
    
    std::string l64 = bits(l, 64);
    return message + "1" + std::string(k, '0') + l64;
}

std::vector<std::string> split(const std::string& message, int size = 512) {
    std::vector<std::string> blocks;
    for (size_t i = 0; i < message.length(); i += size) {
        blocks.push_back(message.substr(i, size));
    }
    return blocks;
}

// ============ Message Schedule ============

std::vector<uint32_t> calculate_schedule(const std::string& block) {
    std::vector<uint32_t> schedule;

    // First 16 words from block
    for (size_t i = 0; i < 16; i++) {
        std::string word = block.substr(i * 32, 32);
        schedule.push_back(std::bitset<32>(word).to_ulong());
    }

    // Calculate remaining 48 words
    for (int i = 16; i <= 63; i++) {
        schedule.push_back(add(sigma1(schedule[i - 2]), 
                               schedule[i - 7], 
                               sigma0(schedule[i - 15]), 
                               schedule[i - 16]));
    }

    return schedule;
}

// ============ Constants ============

// Constants = Cube roots of the first 64 prime numbers (first 32 bits of the fractional part)
const std::vector<uint32_t> K = []() {
    std::vector<uint32_t> constants;
    int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 
                    59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 
                    127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 
                    191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 
                    257, 263, 269, 271, 277, 281, 283, 293, 307, 311};
    
    for (int prime : primes) {
        double cubeRoot = std::pow(prime, 1.0 / 3.0);
        double fractional = cubeRoot - std::floor(cubeRoot);
        uint32_t value = static_cast<uint32_t>(fractional * std::pow(2, 32));
        constants.push_back(value);
    }
    return constants;
}();

// ============ Compression ============

// Initial Hash Values = Square roots of the first 8 prime numbers (first 32 bits of the fractional part)
const std::vector<uint32_t> IV = []() {
    std::vector<uint32_t> initial;
    int primes[] = {2, 3, 5, 7, 11, 13, 17, 19};
    
    for (int prime : primes) {
        double squareRoot = std::sqrt(prime);
        double fractional = squareRoot - std::floor(squareRoot);
        uint32_t value = static_cast<uint32_t>(fractional * std::pow(2, 32));
        initial.push_back(value);
    }
    return initial;
}();

std::vector<uint32_t> compression(const std::vector<uint32_t>& initial, 
                                  const std::vector<uint32_t>& schedule,
                                  const std::vector<uint32_t>& constants) {
    // state registers
    uint32_t h = initial[7];
    uint32_t g = initial[6];
    uint32_t f = initial[5];
    uint32_t e = initial[4];
    uint32_t d = initial[3];
    uint32_t c = initial[2];
    uint32_t b = initial[1];
    uint32_t a = initial[0];

    // compression function
    for (int i = 0; i < 64; i++) {
        // calculate temporary words
        uint32_t t1 = add(schedule[i], constants[i], usigma1(e), ch(e, f, g), h);
        uint32_t t2 = add(usigma0(a), maj(a, b, c));

        // rotate state registers
        h = g;
        g = f;
        f = e;
        e = add(d, t1);
        d = c;
        c = b;
        b = a;
        a = add(t1, t2);
    }

    // Final hash values
    std::vector<uint32_t> hash(8);
    hash[7] = add(initial[7], h);
    hash[6] = add(initial[6], g);
    hash[5] = add(initial[5], f);
    hash[4] = add(initial[4], e);
    hash[3] = add(initial[3], d);
    hash[2] = add(initial[2], c);
    hash[1] = add(initial[1], b);
    hash[0] = add(initial[0], a);

    return hash;
}

// ============ SHA-256 ============

std::string sha256(const std::string& str) {
    // 0. Convert String to Binary
    std::string message = bitstring(str);

    // 1. Preprocessing
    std::string padded = padding(message);
    std::vector<std::string> blocks = split(padded, 512);

    // 2. Hash Computation
    std::vector<uint32_t> hash = IV;

    for (const auto& block : blocks) {
        // Prepare 64 word message schedule
        std::vector<uint32_t> schedule = calculate_schedule(block);

        // Remember starting hash values
        std::vector<uint32_t> initial = hash;

        // Apply compression function
        hash = compression(initial, schedule, K);
    }

    // 3. Result
    std::string result;
    for (uint32_t w : hash) {
        result += hex(w);
    }
    return result;
}

// ============ Main ============

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::string input = argv[1];
        std::string type = input_type(input);
        
        if (type == "file") {
            std::ifstream file(input);
            std::string content((std::istreambuf_iterator<char>(file)),
                                std::istreambuf_iterator<char>());
            std::cout << sha256(content) << std::endl;
        } else {
            std::string str;
            if (type == "binary" || type == "hex") {
                auto byteVec = bytes(input, type);
                str = std::string(byteVec.begin(), byteVec.end());
            } else {
                str = input;
            }
            std::cout << sha256(str) << std::endl;
        }
    } else {
        // Test with "abc" which should produce:
        // ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad
        std::cout << "SHA-256 of \"abc\": " << sha256("abc") << std::endl;
    }
    
    return 0;
}
