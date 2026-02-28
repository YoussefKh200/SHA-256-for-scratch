#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <sstream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <cmath>

// ============ Utility Functions ============

void clearScreen() {
    // ANSI escape sequence to clear screen
    std::cout << "\033[2J\033[1;1H";
}

void delay(const std::string& speed) {
    if (speed == "fastest") {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    } else if (speed == "fast") {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } else if (speed == "normal") {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    } else if (speed == "slow") {
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    } else if (speed == "end") {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

std::string bits(uint32_t value) {
    return std::bitset<32>(value).to_string();
}

uint32_t rotr(int n, uint32_t x) {
    return (x >> n) | (x << (32 - n));
}

uint32_t shr(int n, uint32_t x) {
    return x >> n;
}

// σ0 function: (rotr 7) XOR (rotr 18) XOR (shr 3)
uint32_t sigma0(uint32_t x) {
    return rotr(7, x) ^ rotr(18, x) ^ shr(3, x);
}

// σ1 function: (rotr 17) XOR (rotr 19) XOR (shr 10)
uint32_t sigma1(uint32_t x) {
    return rotr(17, x) ^ rotr(19, x) ^ shr(10, x);
}

uint32_t add(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    return a + b + c + d;
}

std::string padding(const std::string& message) {
    // Simplified padding for demonstration
    std::string padded = message;
    padded += "1";
    
    // Pad with zeros to make length multiple of 512
    int paddingZeros = 512 - ((padded.length() + 64) % 512);
    padded.append(paddingZeros, '0');
    
    // Append original length (64 bits)
    uint64_t bitLength = message.length();
    std::string lengthBits = std::bitset<64>(bitLength).to_string();
    padded += lengthBits;
    
    return padded;
}

std::vector<std::string> split(const std::string& str, int chunkSize) {
    std::vector<std::string> chunks;
    for (size_t i = 0; i < str.length(); i += chunkSize) {
        chunks.push_back(str.substr(i, chunkSize));
    }
    return chunks;
}

// ============ Main Visualization ============

void visualizeMessageSchedule(int t, const std::vector<uint32_t>& schedule, const std::string& indent) {
    // Format word numbers with padding
    auto formatNum = [](int num) -> std::string {
        std::string s = std::to_string(num);
        if (s.length() == 1) return " " + s;
        return s;
    };
    
    std::string t_16 = formatNum(t - 16);
    std::string t_15 = formatNum(t - 15);
    std::string t_14 = formatNum(t - 14);
    std::string t_13 = formatNum(t - 13);
    std::string t_12 = formatNum(t - 12);
    std::string t_11 = formatNum(t - 11);
    std::string t_10 = formatNum(t - 10);
    std::string t_9  = formatNum(t - 9);
    std::string t_8  = formatNum(t - 8);
    std::string t_7  = formatNum(t - 7);
    std::string t_6  = formatNum(t - 6);
    std::string t_5  = formatNum(t - 5);
    std::string t_4  = formatNum(t - 4);
    std::string t_3  = formatNum(t - 3);
    std::string t_2  = formatNum(t - 2);
    std::string t_1  = formatNum(t - 1);
    std::string t_0  = formatNum(t);
    
    std::cout << indent << "----------------" << std::endl;
    std::cout << indent << "message schedule: (expansion)" << std::endl;
    std::cout << indent << "----------------" << std::endl;
    std::cout << indent << "W" << t_16 << " " << bits(schedule[t-16]) << std::endl;
    std::cout << indent << "W" << t_15 << " " << bits(schedule[t-15]) << std::endl;
    std::cout << indent << "W" << t_14 << " " << bits(schedule[t-14]) << std::endl;
    std::cout << indent << "W" << t_13 << " " << bits(schedule[t-13]) << std::endl;
    std::cout << indent << "W" << t_12 << " " << bits(schedule[t-12]) << std::endl;
    std::cout << indent << "W" << t_11 << " " << bits(schedule[t-11]) << std::endl;
    std::cout << indent << "W" << t_10 << " " << bits(schedule[t-10]) << std::endl;
    std::cout << indent << "W" << t_9  << " " << bits(schedule[t-9]) << std::endl;
    std::cout << indent << "W" << t_8  << " " << bits(schedule[t-8]) << std::endl;
    std::cout << indent << "W" << t_7  << " " << bits(schedule[t-7]) << std::endl;
    std::cout << indent << "W" << t_6  << " " << bits(schedule[t-6]) << std::endl;
    std::cout << indent << "W" << t_5  << " " << bits(schedule[t-5]) << std::endl;
    std::cout << indent << "W" << t_4  << " " << bits(schedule[t-4]) << std::endl;
    std::cout << indent << "W" << t_3  << " " << bits(schedule[t-3]) << std::endl;
    std::cout << indent << "W" << t_2  << " " << bits(schedule[t-2]) << std::endl;
    std::cout << indent << "W" << t_1  << " " << bits(schedule[t-1]) << std::endl;
    std::cout << indent << "W" << t_0  << "                                  = σ1(t-2) + (t-7) + σ0(t-15) + (t-16)" << std::endl;
}

int main(int argc, char* argv[]) {
    std::string input = "abc";
    std::string block;
    int t = 63; // Default word number
    
    // Parse command line arguments
    if (argc > 1) {
        block = argv[1];
        if (block.length() < 512) {
            block = std::string(512 - block.length(), '0') + block;
        }
    }
    
    if (argc > 2) {
        t = std::stoi(argv[2]);
    }
    
    // Default message processing
    std::string message;
    if (block.empty()) {
        // Convert input to binary string
        for (char c : input) {
            message += std::bitset<8>(c).to_string();
        }
        
        std::string padded = padding(message);
        std::vector<std::string> blocks = split(padded, 512);
        block = blocks[0];
    }
    
    // Create message schedule
    std::vector<uint32_t> schedule;
    
    // First 16 words from block
    for (size_t i = 0; i < 16; i++) {
        std::string word = block.substr(i * 32, 32);
        schedule.push_back(std::bitset<32>(word).to_ulong());
    }
    
    // Calculate remaining 48 words
    for (int i = 16; i <= 63; i++) {
        uint32_t w = sigma1(schedule[i - 2]) + schedule[i - 7] + 
                    sigma0(schedule[i - 15]) + schedule[i - 16];
        schedule.push_back(w);
    }
    
    std::string indent = "  ";
    
    // ============ Animation Frames ============
    
    // Frame 1: Basic display
    clearScreen();
    visualizeMessageSchedule(t, schedule, indent);
    delay("normal");
    
    // Frame 2: Highlight t-16
    clearScreen();
    std::cout << indent << "----------------" << std::endl;
    std::cout << indent << "message schedule: (expansion)" << std::endl;
    std::cout << indent << "----------------" << std::endl;
    std::cout << indent << "W" << (t-16 < 10 ? " " : "") << (t-16) << " " << bits(schedule[t-16]) << "  ->  " << bits(schedule[t-16]) << std::endl;
    for (int i = t-15; i <= t-1; i++) {
        std::cout << indent << "W" << (i < 10 ? " " : "") << i << " " << bits(schedule[i]) << std::endl;
    }
    std::cout << indent << "W" << t << "                                  = σ1(t-2) + (t-7) + σ0(t-15) + (t-16)" << std::endl;
    delay("normal");
    
    // Frame 3: Show ROTR 7 animation start
    clearScreen();
    std::cout << indent << "----------------" << std::endl;
    std::cout << indent << "message schedule: (expansion)" << std::endl;
    std::cout << indent << "----------------" << std::endl;
    std::cout << indent << "W" << (t-16 < 10 ? " " : "") << (t-16) << " " << bits(schedule[t-16]) << "  ->  " << bits(schedule[t-16]) << std::endl;
    std::cout << indent << "W" << (t-15 < 10 ? " " : "") << (t-15) << " " << bits(schedule[t-15]) << "  ->  " << bits(schedule[t-15]) << " ROTR 7" << std::endl;
    std::cout << indent << "W" << (t-14 < 10 ? " " : "") << (t-14) << " " << bits(schedule[t-14]) << "      " << bits(schedule[t-15]) << " ROTR 18" << std::endl;
    std::cout << indent << "W" << (t-13 < 10 ? " " : "") << (t-13) << " " << bits(schedule[t-13]) << "      " << bits(schedule[t-15]) << "  SHR 3" << std::endl;
    for (int i = t-12; i <= t-1; i++) {
        std::cout << indent << "W" << (i < 10 ? " " : "") << i << " " << bits(schedule[i]) << std::endl;
    }
    std::cout << indent << "W" << t << "                                  = σ1(t-2) + (t-7) + σ0(t-15) + (t-16)" << std::endl;
    delay("normal");
    
    // ROTR 7 animation
    for (int i = 1; i <= 7; i++) {
        clearScreen();
        std::cout << indent << "----------------" << std::endl;
        std::cout << indent << "message schedule: (expansion)" << std::endl;
        std::cout << indent << "----------------" << std::endl;
        std::cout << indent << "W" << (t-16 < 10 ? " " : "") << (t-16) << " " << bits(schedule[t-16]) << "  ->  " << bits(schedule[t-16]) << std::endl;
        std::cout << indent << "W" << (t-15 < 10 ? " " : "") << (t-15) << " " << bits(schedule[t-15]) << "  ->  " << bits(rotr(i, schedule[t-15])) << " ROTR 7" << std::endl;
        std::cout << indent << "W" << (t-14 < 10 ? " " : "") << (t-14) << " " << bits(schedule[t-14]) << "      " << bits(schedule[t-15]) << " ROTR 18" << std::endl;
        std::cout << indent << "W" << (t-13 < 10 ? " " : "") << (t-13) << " " << bits(schedule[t-13]) << "      " << bits(schedule[t-15]) << "  SHR 3" << std::endl;
        for (int j = t-12; j <= t-1; j++) {
            std::cout << indent << "W" << (j < 10 ? " " : "") << j << " " << bits(schedule[j]) << std::endl;
        }
        std::cout << indent << "W" << t << "                                  = σ1(t-2) + (t-7) + σ0(t-15) + (t-16)" << std::endl;
        delay("fastest");
    }
    delay("normal");
    
    // ROTR 18 animation
    for (int i = 1; i <= 18; i++) {
        clearScreen();
        std::cout << indent << "----------------" << std::endl;
        std::cout << indent << "message schedule: (expansion)" << std::endl;
        std::cout << indent << "----------------" << std::endl;
        std::cout << indent << "W" << (t-16 < 10 ? " " : "") << (t-16) << " " << bits(schedule[t-16]) << "  ->  " << bits(schedule[t-16]) << std::endl;
        std::cout << indent << "W" << (t-15 < 10 ? " " : "") << (t-15) << " " << bits(schedule[t-15]) << "  ->  " << bits(rotr(7, schedule[t-15])) << " ROTR 7" << std::endl;
        std::cout << indent << "W" << (t-14 < 10 ? " " : "") << (t-14) << " " << bits(schedule[t-14]) << "      " << bits(rotr(i, schedule[t-15])) << " ROTR 18" << std::endl;
        std::cout << indent << "W" << (t-13 < 10 ? " " : "") << (t-13) << " " << bits(schedule[t-13]) << "      " << bits(schedule[t-15]) << "  SHR 3" << std::endl;
        for (int j = t-12; j <= t-1; j++) {
            std::cout << indent << "W" << (j < 10 ? " " : "") << j << " " << bits(schedule[j]) << std::endl;
        }
        std::cout << indent << "W" << t << "                                  = σ1(t-2) + (t-7) + σ0(t-15) + (t-16)" << std::endl;
        delay("fastest");
    }
    delay("normal");
    
    // SHR 3 animation
    for (int i = 1; i <= 3; i++) {
        clearScreen();
        std::cout << indent << "----------------" << std::endl;
        std::cout << indent << "message schedule: (expansion)" << std::endl;
        std::cout << indent << "----------------" << std::endl;
        std::cout << indent << "W" << (t-16 < 10 ? " " : "") << (t-16) << " " << bits(schedule[t-16]) << "  ->  " << bits(schedule[t-16]) << std::endl;
        std::cout << indent << "W" << (t-15 < 10 ? " " : "") << (t-15) << " " << bits(schedule[t-15]) << "  ->  " << bits(rotr(7, schedule[t-15])) << " ROTR 7" << std::endl;
        std::cout << indent << "W" << (t-14 < 10 ? " " : "") << (t-14) << " " << bits(schedule[t-14]) << "      " << bits(rotr(18, schedule[t-15])) << " ROTR 18" << std::endl;
        std::cout << indent << "W" << (t-13 < 10 ? " " : "") << (t-13) << " " << bits(schedule[t-13]) << "      " << bits(shr(i, schedule[t-15])) << "  SHR 3" << std::endl;
        for (int j = t-12; j <= t-1; j++) {
            std::cout << indent << "W" << (j < 10 ? " " : "") << j << " " << bits(schedule[j]) << std::endl;
        }
        std::cout << indent << "W" << t << "                                  = σ1(t-2) + (t-7) + σ0(t-15) + (t-16)" << std::endl;
        delay("fastest");
    }
    delay("normal");
    
    // Show XOR combination for sigma0
    uint32_t sigma0_val = sigma0(schedule[t-15]);
    clearScreen();
    std::cout << indent << "----------------" << std::endl;
    std::cout << indent << "message schedule: (expansion)" << std::endl;
    std::cout << indent << "----------------" << std::endl;
    std::cout << indent << "W" << (t-16 < 10 ? " " : "") << (t-16) << " " << bits(schedule[t-16]) << "  ->  " << bits(schedule[t-16]) << std::endl;
    std::cout << indent << "W" << (t-15 < 10 ? " " : "") << (t-15) << " " << bits(schedule[t-15]) << "  ->  " << bits(sigma0_val) << " σ0" << std::endl;
    std::cout << indent << "W" << (t-14 < 10 ? " " : "") << (t-14) << " " << bits(schedule[t-14]) << std::endl;
    std::cout << indent << "W" << (t-13 < 10 ? " " : "") << (t-13) << " " << bits(schedule[t-13]) << std::endl;
    for (int j = t-12; j <= t-1; j++) {
        std::cout << indent << "W" << (j < 10 ? " " : "") << j << " " << bits(schedule[j]) << std::endl;
    }
    std::cout << indent << "W" << t << "                                  = σ1(t-2) + (t-7) + σ0(t-15) + (t-16)" << std::endl;
    delay("normal");
    
    // ============ σ1 animation (similar to σ0) ============
    // This follows the same pattern but for t-2
    
    // Final result
    uint32_t result = add(sigma1(schedule[t-2]), schedule[t-7], sigma0(schedule[t-15]), schedule[t-16]);
    
    clearScreen();
    std::cout << indent << "----------------" << std::endl;
    std::cout << indent << "message schedule: (expansion)" << std::endl;
    std::cout << indent << "----------------" << std::endl;
    std::cout << indent << "W" << (t-16 < 10 ? " " : "") << (t-16) << " " << bits(schedule[t-16]) << "  ->  " << bits(schedule[t-16]) << std::endl;
    std::cout << indent << "W" << (t-15 < 10 ? " " : "") << (t-15) << " " << bits(schedule[t-15]) << "  ->  " << bits(sigma0(schedule[t-15])) << " σ0" << std::endl;
    std::cout << indent << "W" << (t-14 < 10 ? " " : "") << (t-14) << " " << bits(schedule[t-14]) << std::endl;
    std::cout << indent << "W" << (t-13 < 10 ? " " : "") << (t-13) << " " << bits(schedule[t-13]) << std::endl;
    std::cout << indent << "W" << (t-12 < 10 ? " " : "") << (t-12) << " " << bits(schedule[t-12]) << std::endl;
    std::cout << indent << "W" << (t-11 < 10 ? " " : "") << (t-11) << " " << bits(schedule[t-11]) << std::endl;
    std::cout << indent << "W" << (t-10 < 10 ? " " : "") << (t-10) << " " << bits(schedule[t-10]) << std::endl;
    std::cout << indent << "W" << (t-9 < 10 ? " " : "") << (t-9) << " " << bits(schedule[t-9]) << std::endl;
    std::cout << indent << "W" << (t-8 < 10 ? " " : "") << (t-8) << " " << bits(schedule[t-8]) << std::endl;
    std::cout << indent << "W" << (t-7 < 10 ? " " : "") << (t-7) << " " << bits(schedule[t-7]) << "  ->  " << bits(schedule[t-7]) << std::endl;
    std::cout << indent << "W" << (t-6 < 10 ? " " : "") << (t-6) << " " << bits(schedule[t-6]) << std::endl;
    std::cout << indent << "W" << (t-5 < 10 ? " " : "") << (t-5) << " " << bits(schedule[t-5]) << std::endl;
    std::cout << indent << "W" << (t-4 < 10 ? " " : "") << (t-4) << " " << bits(schedule[t-4]) << std::endl;
    std::cout << indent << "W" << (t-3 < 10 ? " " : "") << (t-3) << " " << bits(schedule[t-3]) << std::endl;
    std::cout << indent << "W" << (t-2 < 10 ? " " : "") << (t-2) << " " << bits(schedule[t-2]) << "  ->  " << bits(sigma1(schedule[t-2])) << " σ1" << std::endl;
    std::cout << indent << "W" << (t-1 < 10 ? " " : "") << (t-1) << " " << bits(schedule[t-1]) << std::endl;
    std::cout << indent << "W" << t << " " << bits(result) << " = σ1(t-2) + (t-7) + σ0(t-15) + (t-16)" << std::endl;
    delay("end");
    
    return 0;
}
