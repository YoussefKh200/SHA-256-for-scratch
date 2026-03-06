#include <iostream>
#include <string>
#include <bitset>
#include <thread>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <cmath>

// ============ Global Variables ============
std::string g_delay = "normal";
bool g_showBinary = true;
bool g_showHex = false;

// ============ Utility Functions ============

void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

void delay(int ms) {
    if (g_delay != "nodelay") {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
}

void waitForEnter() {
    if (g_delay == "enter") {
        std::cin.get();
    }
}

std::string bits(uint32_t x) {
    return std::bitset<32>(x).to_string();
}

std::string hex(uint32_t x) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(8) << x;
    return ss.str();
}

// ============ Rotate Right Function ============

uint32_t rotr(int n, uint32_t x) {
    uint32_t mask = 0xFFFFFFFF;
    uint32_t right = (x >> n) & mask;
    uint32_t left = (x << (32 - n)) & mask;
    return right | left;
}

// ============ Visualization with Step-by-Step Explanation ============

void showRotrWithExplanation(uint32_t x, int max_n) {
    std::string x_bits = bits(x);
    
    for (int i = 1; i <= max_n; i++) {
        clearScreen();
        
        std::string s = std::to_string(i);
        if (s.length() == 1) s = " " + s;
        
        uint32_t result = rotr(i, x);
        std::string result_bits = bits(result);
        
        std::cout << "========================================" << std::endl;
        std::cout << "Rotate Right (ROTR) - Step " << i << " of " << max_n << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << std::endl;
        
        // Show operation formula
        std::cout << "ROTR(" << i << ", x) = (x >> " << i << ") | (x << " << (32 - i) << ")" << std::endl;
        std::cout << std::endl;
        
        // Show original
        std::cout << "      x: " << x_bits << " (0x" << hex(x) << ")" << std::endl;
        
        // Show right shift
        uint32_t right = (x >> i) & 0xFFFFFFFF;
        std::string right_bits = bits(right);
        std::cout << "x >> " << i << ": " << std::string(i, ' ') << right_bits.substr(0, 32 - i) 
                  << "  [right shift by " << i << "]" << std::endl;
        
        // Show left shift
        uint32_t left = (x << (32 - i)) & 0xFFFFFFFF;
        std::string left_bits = bits(left);
        std::cout << "x << " << (32 - i) << ": " << left_bits.substr(32 - i, i) 
                  << std::string(32 - i, ' ') << "  [left shift by " << (32 - i) << "]" << std::endl;
        
        // Show separator
        std::cout << std::string(50, '-') << std::endl;
        
        // Show result
        std::cout << "ROTR " << s << ": " << result_bits << " (0x" << hex(result) << ")" << std::endl;
        std::cout << std::endl;
        
        // Visual representation of rotation
        std::cout << "Visual rotation:" << std::endl;
        std::cout << "  Original: [";
        for (int j = 0; j < 32; j++) {
            if (j > 0 && j % 8 == 0) std::cout << " ";
            std::cout << x_bits[j];
        }
        std::cout << "]" << std::endl;
        
        std::cout << "  Rotated:  [";
        for (int j = 0; j < 32; j++) {
            if (j > 0 && j % 8 == 0) std::cout << " ";
            if (j < i) {
                // Bits that wrapped from the end
                std::cout << "\033[32m" << result_bits[j] << "\033[0m";
            } else {
                std::cout << result_bits[j];
            }
        }
        std::cout << "]" << std::endl;
        
        // Show which bits moved
        std::cout << "            ";
        for (int j = 0; j < 32; j++) {
            if (j > 0 && j % 8 == 0) std::cout << " ";
            if (j < i) {
                std::cout << "\033[32m↑\033[0m";
            } else if (j >= 32 - i) {
                std::cout << "\033[33m↑\033[0m";
            } else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
        
        std::cout << "  Legend: \033[32m← " << i << " LSBs wrapped to MSB\033[0m" << std::endl;
        std::cout << "          \033[33m→ remaining bits shifted right\033[0m" << std::endl;
        
        waitForEnter();
        delay(100);
    }
}

// ============ Simple Animation ============

void showRotrSimple(uint32_t x, int n) {
    std::string s_pad = std::to_string(n);
    if (s_pad.length() == 1) s_pad = " " + s_pad;
    
    for (int i = 1; i <= n; i++) {
        clearScreen();
        
        std::string s = std::to_string(i);
        if (s.length() == 1) s = " " + s;
        
        std::cout << "      x: " << bits(x) << std::endl;
        std::cout << "ROTR " << s << ": " << bits(rotr(i, x)) << std::endl;
        
        if (g_delay == "enter") {
            std::cin.get();
        } else {
            delay(100);
        }
    }
    delay(500);
}

// ============ Interactive Mode ============

void interactiveMode() {
    std::string input;
    uint32_t x;
    int n;
    
    std::cout << "Enter a 32-bit binary number (or hex with 0x prefix): ";
    std::getline(std::cin, input);
    
    if (input.substr(0, 2) == "0x") {
        // Hex input
        x = std::stoul(input, nullptr, 16);
    } else {
        // Binary input - remove any spaces
        input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());
        if (input.substr(0, 2) == "0b") {
            input = input.substr(2);
        }
        if (input.length() > 32) {
            std::cout << "Error: Maximum 32 bits" << std::endl;
            return;
        }
        // Pad with leading zeros
        if (input.length() < 32) {
            input = std::string(32 - input.length(), '0') + input;
        }
        x = std::bitset<32>(input).to_ulong();
    }
    
    std::cout << "Enter number of rotations to show (1-32): ";
    std::cin >> n;
    if (n < 1) n = 1;
    if (n > 32) n = 32;
    
    std::cout << "\nShowing rotation for x = " << bits(x) << " (0x" << hex(x) << ")" << std::endl;
    std::cout << "Press Enter to start...";
    std::cin.ignore();
    std::cin.get();
    
    showRotrWithExplanation(x, n);
}

// ============ Main ============

int main(int argc, char* argv[]) {
    uint32_t x = 0b11111111000000001111111100000000; // default
    int n = 32; // default
    
    // Parse command line arguments
    if (argc >= 2) {
        std::string x_str = argv[1];
        
        // Check for hex input
        if (x_str.substr(0, 2) == "0x") {
            x = std::stoul(x_str, nullptr, 16);
        } else {
            // Binary input
            if (x_str.substr(0, 2) == "0b") {
                x_str = x_str.substr(2);
            }
            
            // Check bit length
            if (x_str.length() > 32) {
                std::cout << "We only operate on 32-bit words in SHA-256. ";
                std::cout << "Your x is " << x_str.length() << " bits." << std::endl;
                return 1;
            }
            
            // Pad with leading zeros if needed
            if (x_str.length() < 32) {
                x_str = std::string(32 - x_str.length(), '0') + x_str;
            }
            
            x = std::bitset<32>(x_str).to_ulong();
        }
    }
    
    if (argc >= 3) {
        n = std::stoi(argv[2]);
        if (n > 32) n = 32;
        if (n < 1) n = 1;
    }
    
    if (argc >= 4) {
        g_delay = argv[3];
    }
    
    // Check for interactive mode
    if (argc >= 2 && std::string(argv[1]) == "--interactive") {
        interactiveMode();
        return 0;
    }
    
    // Show explanation for first few steps, then speed up
    std::cout << "ROTR Demonstration for x = " << bits(x) << " (0x" << hex(x) << ")" << std::endl;
    std::cout << "Press Enter to start...";
    if (g_delay == "enter") {
        std::cin.get();
    }
    
    // Show detailed explanation for first 5 steps
    int detailed = std::min(5, n);
    if (detailed > 0) {
        showRotrWithExplanation(x, detailed);
    }
    
    // Show remaining steps with simple animation
    if (n > detailed) {
        for (int i = detailed + 1; i <= n; i++) {
            clearScreen();
            
            std::string s = std::to_string(i);
            if (s.length() == 1) s = " " + s;
            
            std::cout << "      x: " << bits(x) << std::endl;
            std::cout << "ROTR " << s << ": " << bits(rotr(i, x)) << std::endl;
            
            if (g_delay == "enter") {
                std::cin.get();
            } else {
                delay(50); // Faster for remaining steps
            }
        }
    }
    
    // Final result
    delay(500);
    clearScreen();
    std::cout << "========================================" << std::endl;
    std::cout << "ROTR - Final Summary" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "x: " << bits(x) << " (0x" << hex(x) << ")" << std::endl;
    std::cout << std::endl;
    
    for (int i = 1; i <= std::min(8, n); i += 2) {
        std::cout << "ROTR(" << std::setw(2) << i << "): " << bits(rotr(i, x)) << "  ";
        std::cout << "ROTR(" << std::setw(2) << (i+1) << "): " << bits(rotr(i+1, x)) << std::endl;
    }
    std::cout << std::endl;
    
    return 0;
}
