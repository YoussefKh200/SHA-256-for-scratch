#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <sstream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <cmath>
#include <cstdint>

// ============ Global Variables ============
std::string g_delay = "normal";
std::string g_state = "";
std::string g_input = "abc";
std::string g_message;
std::string g_padded;
std::vector<std::string> g_blocks;
std::vector<uint32_t> g_hash;
std::string g_digest;

// ============ Utility Functions ============

void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

void delay(const std::string& speed) {
    int sleepTime = 0;
    
    if (speed == "fastest") sleepTime = 100;
    else if (speed == "fast") sleepTime = 200;
    else if (speed == "normal") sleepTime = 400;
    else if (speed == "slow") sleepTime = 600;
    else if (speed == "slowest") sleepTime = 800;
    else if (speed == "end") sleepTime = 1000;
    else sleepTime = 400; // default
    
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
}

std::string bits(uint32_t x, int n = 32) {
    std::string binary = std::bitset<32>(x).to_string();
    return binary.substr(32 - n, n);
}

std::string hex(uint32_t i) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(8) << i;
    return ss.str();
}

std::string bitstring(const std::string& str) {
    std::string result;
    for (unsigned char c : str) {
        result += std::bitset<8>(c).to_string();
    }
    return result;
}

// ============ SHA-256 Operations ============

uint32_t rotr(int n, uint32_t x) {
    return (x >> n) | (x << (32 - n));
}

uint32_t shr(int n, uint32_t x) {
    return x >> n;
}

uint32_t sigma0(uint32_t x) {
    return rotr(7, x) ^ rotr(18, x) ^ shr(3, x);
}

uint32_t sigma1(uint32_t x) {
    return rotr(17, x) ^ rotr(19, x) ^ shr(10, x);
}

uint32_t usigma0(uint32_t x) {
    return rotr(2, x) ^ rotr(13, x) ^ rotr(22, x);
}

uint32_t usigma1(uint32_t x) {
    return rotr(6, x) ^ rotr(11, x) ^ rotr(25, x);
}

uint32_t ch(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ ((~x) & z);
}

uint32_t maj(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

uint32_t add(uint32_t a, uint32_t b) {
    return (a + b) & 0xFFFFFFFF;
}

uint32_t add(uint32_t a, uint32_t b, uint32_t c) {
    return (a + b + c) & 0xFFFFFFFF;
}

uint32_t add(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    uint64_t total = static_cast<uint64_t>(a) + b + c + d;
    return static_cast<uint32_t>(total & 0xFFFFFFFF);
}

uint32_t add(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e) {
    uint64_t total = static_cast<uint64_t>(a) + b + c + d + e;
    return static_cast<uint32_t>(total & 0xFFFFFFFF);
}

// ============ SHA-256 Constants ============

std::vector<uint32_t> calculateK() {
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
}

std::vector<uint32_t> calculateIV() {
    std::vector<uint32_t> initial;
    int primes[] = {2, 3, 5, 7, 11, 13, 17, 19};
    
    for (int prime : primes) {
        double squareRoot = std::sqrt(prime);
        double fractional = squareRoot - std::floor(squareRoot);
        uint32_t value = static_cast<uint32_t>(fractional * std::pow(2, 32));
        initial.push_back(value);
    }
    return initial;
}

const std::vector<uint32_t> K = calculateK();
const std::vector<uint32_t> IV = calculateIV();

// ============ SHA-256 Core Functions ============

std::string padding(const std::string& message) {
    size_t l = message.size();
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

std::vector<uint32_t> calculate_schedule(const std::string& block) {
    std::vector<uint32_t> schedule;

    for (size_t i = 0; i < 16; i++) {
        std::string word = block.substr(i * 32, 32);
        schedule.push_back(std::bitset<32>(word).to_ulong());
    }

    for (int i = 16; i <= 63; i++) {
        schedule.push_back(add(sigma1(schedule[i - 2]), 
                               schedule[i - 7], 
                               sigma0(schedule[i - 15]), 
                               schedule[i - 16]));
    }
    return schedule;
}

std::vector<uint32_t> compression(const std::vector<uint32_t>& initial, 
                                  const std::vector<uint32_t>& schedule) {
    uint32_t h = initial[7];
    uint32_t g = initial[6];
    uint32_t f = initial[5];
    uint32_t e = initial[4];
    uint32_t d = initial[3];
    uint32_t c = initial[2];
    uint32_t b = initial[1];
    uint32_t a = initial[0];

    for (int i = 0; i < 64; i++) {
        uint32_t t1 = add(schedule[i], K[i], usigma1(e), ch(e, f, g), h);
        uint32_t t2 = add(usigma0(a), maj(a, b, c));

        h = g;
        g = f;
        f = e;
        e = add(d, t1);
        d = c;
        c = b;
        b = a;
        a = add(t1, t2);
    }

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

// ============ SHA-256 Main Function ============

void computeSHA256(const std::string& input) {
    g_message = bitstring(input);
    g_padded = padding(g_message);
    g_blocks = split(g_padded, 512);
    
    // Set initial hash state
    g_hash = IV;

    // Process each block
    for (const auto& block : g_blocks) {
        // Prepare message schedule
        std::vector<uint32_t> schedule = calculate_schedule(block);

        // Remember starting hash values
        std::vector<uint32_t> initial = g_hash;

        // Apply compression function
        g_hash = compression(initial, schedule);
    }
}

// ============ Visualization ============

void showFinalHash() {
    std::string registers[] = {"a", "b", "c", "d", "e", "f", "g", "h"};
    
    // Frame 1: Show only binary
    clearScreen();
    if (!g_state.empty()) {
        std::cout << g_state << "\n" << std::endl;
    }
    std::cout << "----------------" << std::endl;
    std::cout << "final hash value: (H" << g_blocks.size() << ")" << std::endl;
    std::cout << "----------------" << std::endl;
    for (int i = 0; i < 8; i++) {
        std::cout << registers[i] << " = " << bits(g_hash[i]) << std::endl;
    }
    delay("slowest");
    
    // Frame 2: Add hexadecimal
    clearScreen();
    if (!g_state.empty()) {
        std::cout << g_state << "\n" << std::endl;
    }
    std::cout << "----------------" << std::endl;
    std::cout << "final hash value: (H" << g_blocks.size() << ")" << std::endl;
    std::cout << "----------------" << std::endl;
    for (int i = 0; i < 8; i++) {
        std::cout << registers[i] << " = " << bits(g_hash[i]) << " = " << hex(g_hash[i]) << std::endl;
    }
    delay("slowest");
    
    // Frame 3: Concatenate hex values one by one
    g_digest = "";
    for (int i = 0; i < 8; i++) {
        g_digest += hex(g_hash[i]);
        
        clearScreen();
        if (!g_state.empty()) {
            std::cout << g_state << "\n" << std::endl;
        }
        std::cout << "----------------" << std::endl;
        std::cout << "final hash value: (H" << g_blocks.size() << ")" << std::endl;
        std::cout << "----------------" << std::endl;
        for (int j = 0; j < 8; j++) {
            std::cout << registers[j] << " = " << bits(g_hash[j]) << " = " << hex(g_hash[j]) << std::endl;
        }
        std::cout << std::endl;
        std::cout << g_digest << std::endl;
        delay("fastest");
    }
    
    // Final pause
    delay("end");
    delay("end");
}

// ============ Main ============

int main(int argc, char* argv[]) {
    // Parse command line arguments
    if (argc >= 2) {
        g_input = argv[1];
    }
    
    if (argc >= 3) {
        g_delay = argv[2];
    }
    
    if (argc >= 4) {
        g_state = argv[3];
    }
    
    // Compute SHA-256 hash
    computeSHA256(g_input);
    
    // Show visualization
    showFinalHash();
    
    return 0;
}
