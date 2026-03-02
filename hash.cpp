#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <csignal>
#include <cstdint>
#include <thread>
#include <chrono>
#include <cmath>

// ============ Global Variables ============
std::string g_input;
std::string g_delay = "fast";
std::string g_type;
std::vector<uint8_t> g_bytes;
std::string g_message;
std::string g_digest;  // Will hold the result of first SHA-256

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
        double multiplier = 1.0;
        if (g_delay == "fast") multiplier = 0.5;
        
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
    std::string binary = std::bitset<32>(x).to_string();
    return binary.substr(32 - n, n);
}

std::string hex(uint32_t i) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(8) << i;
    return ss.str();
}

std::string input_type(const std::string& input) {
    if (input.length() >= 2) {
        std::string prefix = input.substr(0, 2);
        if (prefix == "0x") {
            // Validate hex string
            std::string hexStr = input.substr(2);
            for (char c : hexStr) {
                if (!isxdigit(c)) {
                    std::cout << "Invalid hex string: " << input << std::endl;
                    exit(1);
                }
            }
            return "hex";
        } else if (prefix == "0b") {
            return "binary";
        }
    }
    return "string";
}

std::vector<uint8_t> bytes(const std::string& input, const std::string& type) {
    std::vector<uint8_t> result;

    if (type == "hex") {
        std::string hexStr = input.substr(2); // trim 0x prefix
        for (size_t i = 0; i < hexStr.length(); i += 2) {
            std::string byteStr = hexStr.substr(i, 2);
            result.push_back(static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16)));
        }
    } else if (type == "binary") {
        std::string bin = input.substr(2);
        if (bin.length() % 8 == 0) {
            for (size_t i = 0; i < bin.length(); i += 8) {
                std::string byteStr = bin.substr(i, 8);
                result.push_back(std::bitset<8>(byteStr).to_ulong());
            }
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

// Single SHA-256 function
std::string sha256(const std::string& message) {
    // Pad message
    std::string padded = padding(message);
    
    // Split into blocks
    std::vector<std::string> blocks = split(padded, 512);
    
    // Initialize hash
    std::vector<uint32_t> hash = IV;
    
    // Process each block
    for (const auto& block : blocks) {
        // Calculate message schedule
        std::vector<uint32_t> schedule = calculate_schedule(block);
        
        // Remember starting hash values
        std::vector<uint32_t> initial = hash;
        
        // Apply compression function
        hash = compression(initial, schedule);
    }
    
    // Convert to hex string
    std::string result;
    for (uint32_t w : hash) {
        result += hex(w);
    }
    return result;
}

// ============ Visualization Functions ============

void showFirstHash() {
    clearScreen();
    std::cout << "========================================" << std::endl;
    std::cout << "First SHA-256 Hash (Round 1)" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Input: " << g_input << std::endl;
    std::cout << "Bytes: ";
    for (uint8_t byte : g_bytes) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
    }
    std::cout << std::dec << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    delay(g_delay);
}

void showSecondHash() {
    clearScreen();
    std::cout << "========================================" << std::endl;
    std::cout << "Second SHA-256 Hash (Round 2)" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Input: " << g_input << std::endl;
    std::cout << "(First hash result treated as new input)" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    delay(g_delay);
}

void showFinalHash() {
    clearScreen();
    std::cout << "========================================" << std::endl;
    std::cout << "Hash256 (Double SHA-256) Result" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "First hash:  " << g_digest << std::endl;
    std::cout << "Second hash: " << g_input.substr(2) << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Final:       " << g_input.substr(2) << std::endl;
    std::cout << "========================================" << std::endl;
    delay("end");
}

// ============ Main Hash256 Function ============

int main(int argc, char* argv[]) {
    // Set up signal handler
    signal(SIGINT, signalHandler);

    // Parse command line arguments
    if (argc >= 2) {
        g_input = argv[1];
    } else {
        // Default Bitcoin block header (first block)
        g_input = "0x0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a29ab5f49ffff001d1dac2b7c";
    }
    
    if (argc >= 3) {
        g_delay = argv[2];
    }

    // Ensure input has 0x prefix for hex detection
    if (g_input.substr(0, 2) != "0x") {
        g_input = "0x" + g_input;
    }

    // Validate input size (even number of hex characters)
    if ((g_input.length() - 2) % 2 != 0) {
        std::cout << "Invalid input. Expecting even number of hex characters (i.e., bytes)." << std::endl;
        return 1;
    }

    // Note about hitting enter to step
    if (g_delay == "enter") {
        std::cout << "Hit enter to step through." << std::endl;
        std::cin.get();
    }

    // ============ First SHA-256 ============
    
    // Process input
    g_type = input_type(g_input);
    g_bytes = bytes(g_input, g_type);
    
    // Convert to binary message
    g_message = "";
    for (uint8_t byte : g_bytes) {
        g_message += std::bitset<8>(byte).to_string();
    }
    
    showFirstHash();
    
    // Calculate first hash
    g_digest = sha256(g_message);
    
    // ============ Second SHA-256 ============
    
    // Use output of first hash as input to second hash
    g_input = "0x" + g_digest;  // prepend 0x to show it's hex bytes
    
    showSecondHash();
    
    // Process the new input
    g_type = input_type(g_input);
    g_bytes = bytes(g_input, g_type);
    
    // Convert to binary message
    g_message = "";
    for (uint8_t byte : g_bytes) {
        g_message += std::bitset<8>(byte).to_string();
    }
    
    // Calculate second hash (overwrites g_digest)
    g_digest = sha256(g_message);
    g_input = "0x" + g_digest;  // Update for display
    
    // Show final result
    showFinalHash();
    
    return 0;
}
