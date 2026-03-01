#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <cmath>
#include <iomanip>
#include <thread>
#include <chrono>
#include <fstream>
#include <csignal>
#include <cstdint>

// ============ Global Variables ============
std::string g_delay = "normal";
std::string g_state = "";
std::string g_input;
std::string g_type;
std::vector<uint8_t> g_bytes;
std::string g_message;
std::vector<std::string> g_blocks;
std::string g_block;
int g_block_number = 0;
std::vector<uint32_t> g_hash;

// ============ Signal Handler ============
void signalHandler(int signum) {
    std::cout << "\nInterrupted. Exiting cleanly." << std::endl;
    exit(signum);
}

// ============ Utility Functions ============
void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

void delay(const std::string& speed) {
    if (g_delay == "enter") {
        std::cin.get();
    } else if (g_delay == "nodelay") {
        std::this_thread::sleep_for(std::chrono::milliseconds(0));
    } else {
        double multiplier = (g_delay == "fast") ? 0.5 : 1.0;
        
        int sleepTime = 0;
        if (speed == "fastest") sleepTime = 100 * multiplier;
        else if (speed == "fast") sleepTime = 200 * multiplier;
        else if (speed == "normal") sleepTime = 400 * multiplier;
        else if (speed == "slow") sleepTime = 600 * multiplier;
        else if (speed == "slowest") sleepTime = 800 * multiplier;
        else if (speed == "end") sleepTime = 1000 * multiplier;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }
}

std::string bits(uint32_t x, int n = 32) {
    if (x >= 0) {
        std::string binary = std::bitset<32>(x).to_string();
        return binary.substr(32 - n, n);
    } else {
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

// ============ Input Processing ============
std::string input_type(const std::string& input) {
    std::ifstream file(input);
    if (file.good()) {
        return "file";
    } else {
        if (input.length() >= 2) {
            std::string prefix = input.substr(0, 2);
            if (prefix == "0b") {
                std::string binary = input.substr(2);
                if (binary.find_first_not_of("01") != std::string::npos) {
                    std::cout << "Invalid binary string: " << input << std::endl;
                    exit(1);
                }
                return "binary";
            } else if (prefix == "0x") {
                std::string hexStr = input.substr(2);
                for (char c : hexStr) {
                    if (!isxdigit(c)) {
                        std::cout << "Invalid hex string: " << input << std::endl;
                        exit(1);
                    }
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
        std::string bin = input.substr(2);
        if (bin.length() % 8 == 0) {
            for (size_t i = 0; i < bin.length(); i += 8) {
                std::string byteStr = bin.substr(i, 8);
                result.push_back(std::bitset<8>(byteStr).to_ulong());
            }
        }
    } else if (type == "hex") {
        std::string hexStr = input.substr(2);
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

// ============ Constants ============
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

// ============ Visualization Functions ============
void showMessage() {
    clearScreen();
    std::cout << "========================" << std::endl;
    std::cout << "Original Message: " << g_input << std::endl;
    std::cout << "Type: " << g_type << std::endl;
    std::cout << "Bytes: ";
    for (uint8_t byte : g_bytes) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte << " ";
    }
    std::cout << std::dec << std::endl;
    std::cout << "Binary: " << g_message.substr(0, 64) << "..." << std::endl;
    std::cout << "========================" << std::endl;
    delay("normal");
}

void showPadding() {
    clearScreen();
    std::cout << "========================" << std::endl;
    std::cout << "Padding applied" << std::endl;
    std::cout << "========================" << std::endl;
    std::cout << "Message length: " << g_message.length() << " bits" << std::endl;
    std::cout << "Padding adds '1' bit + zeros + 64-bit length" << std::endl;
    delay("normal");
}

void showBlocks() {
    clearScreen();
    std::cout << "========================" << std::endl;
    std::cout << "Message Blocks (" << g_blocks.size() << " blocks of 512 bits)" << std::endl;
    std::cout << "========================" << std::endl;
    for (size_t i = 0; i < g_blocks.size(); i++) {
        std::cout << "Block " << i << ": " << g_blocks[i].substr(0, 64) << "..." << std::endl;
    }
    delay("normal");
}

void showInitialHashValues() {
    int primes[] = {2, 3, 5, 7, 11, 13, 17, 19};
    std::string registers[] = {"a", "b", "c", "d", "e", "f", "g", "h"};
    std::string indent = "  ";
    
    clearScreen();
    std::cout << indent << "-----------" << std::endl;
    std::cout << indent << "Initial Hash Values (H0)" << std::endl;
    std::cout << indent << "-----------" << std::endl;
    
    for (int i = 0; i < 8; i++) {
        std::cout << indent << registers[i] << " = " << bits(IV[i]) << " (0x" << hex(IV[i]) << ")" << std::endl;
        std::cout << indent << "    √" << primes[i] << " fractional part * 2^32" << std::endl;
    }
    delay("end");
}

void showSchedule() {
    clearScreen();
    std::cout << "========================" << std::endl;
    std::cout << "Message Schedule for Block " << g_block_number << std::endl;
    std::cout << "========================" << std::endl;
    
    auto schedule = calculate_schedule(g_block);
    for (int i = 0; i < 16; i++) {
        std::cout << "W" << std::setw(2) << i << ": " << bits(schedule[i]) << std::endl;
    }
    std::cout << "..." << std::endl;
    delay("normal");
}

void showCompression() {
    clearScreen();
    std::cout << "========================" << std::endl;
    std::cout << "Compression Function - Block " << g_block_number << std::endl;
    std::cout << "========================" << std::endl;
    std::cout << "Current hash values:" << std::endl;
    for (int i = 0; i < 8; i++) {
        std::cout << "h" << i << ": " << bits(g_hash[i]) << " (0x" << hex(g_hash[i]) << ")" << std::endl;
    }
    delay("normal");
}

void showFinalHash() {
    clearScreen();
    std::cout << "========================" << std::endl;
    std::cout << "Final SHA-256 Hash" << std::endl;
    std::cout << "========================" << std::endl;
    
    std::cout << "\nFull Hash: ";
    for (uint32_t val : g_hash) {
        std::cout << hex(val);
    }
    std::cout << std::dec << std::endl;
    std::cout << "========================" << std::endl;
}

// ============ Main SHA-256 Driver ============
int main(int argc, char* argv[]) {
    signal(SIGINT, signalHandler);

    // Parse arguments
    if (argc >= 2) {
        g_input = argv[1];
    } else {
        g_input = "abc";
    }
    
    if (argc >= 3) {
        g_delay = argv[2];
    }

    // Process input
    g_type = input_type(g_input);
    
    if (g_type == "file") {
        std::ifstream file(g_input);
        if (file.is_open()) {
            std::string content((std::istreambuf_iterator<char>(file)),
                                std::istreambuf_iterator<char>());
            g_input = content;
            file.close();
        }
    }

    g_bytes = bytes(g_input, g_type);

    if (g_type == "string" || g_type == "file" || g_type == "hex") {
        g_message = "";
        for (uint8_t byte : g_bytes) {
            g_message += std::bitset<8>(byte).to_string();
        }
    } else if (g_type == "binary") {
        g_message = g_input.substr(2);
    }

    if (g_delay == "enter") {
        std::cout << "Hit enter to step through." << std::endl;
        std::cin.get();
    }

    // Visualization steps
    showMessage();
    
    std::string padded = padding(g_message);
    g_blocks = split(padded, 512);
    showPadding();
    showBlocks();
    
    g_hash = IV;
    showInitialHashValues();
    
    for (size_t i = 0; i < g_blocks.size(); i++) {
        g_block = g_blocks[i];
        g_block_number = i;
        
        showSchedule();
        showCompression();
        
        auto schedule = calculate_schedule(g_block);
        g_hash = compression(g_hash, schedule);
    }
    
    showFinalHash();
    
    return 0;
}
