#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <bitset>
#include <thread>
#include <chrono>

// ============ Global Variables ============
extern std::string g_delay;
extern std::string g_state; // This would be defined elsewhere if needed

// ============ Utility Functions ============

void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

void delay(const std::string& speed) {
    // This would use the same delay logic from sha256lib
    // Simplified version here
    int sleepTime = 400; // default normal
    
    if (speed == "fastest") sleepTime = 100;
    else if (speed == "fast") sleepTime = 200;
    else if (speed == "normal") sleepTime = 400;
    else if (speed == "slow") sleepTime = 600;
    else if (speed == "slowest") sleepTime = 800;
    else if (speed == "end") sleepTime = 1000;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
}

std::string bits(uint32_t x) {
    return std::bitset<32>(x).to_string();
}

// ============ Main Visualization ============

void showInitialHashValues() {
    // First 8 prime numbers
    int primes[] = {2, 3, 5, 7, 11, 13, 17, 19};
    std::string registers[] = {"a", "b", "c", "d", "e", "f", "g", "h"};
    std::string indent = "  ";
    
    // Frame 1: Basic compression title
    clearScreen();
    if (!g_state.empty()) {
        std::cout << g_state << "\n" << std::endl;
    }
    std::cout << indent << "-----------" << std::endl;
    std::cout << indent << "compression:" << std::endl;
    std::cout << indent << "-----------" << std::endl;
    for (int i = 0; i < 8; i++) {
        std::cout << indent << registers[i] << " =" << std::endl;
    }
    delay("slow");
    
    // Frame 2: Add H0
    clearScreen();
    if (!g_state.empty()) {
        std::cout << g_state << "\n" << std::endl;
    }
    std::cout << indent << "-----------" << std::endl;
    std::cout << indent << "compression: (H0)" << std::endl;
    std::cout << indent << "-----------" << std::endl;
    for (int i = 0; i < 8; i++) {
        std::cout << indent << registers[i] << " =" << std::endl;
    }
    delay("slow");
    
    // Frame 3: Add description
    clearScreen();
    if (!g_state.empty()) {
        std::cout << g_state << "\n" << std::endl;
    }
    std::cout << indent << "-----------" << std::endl;
    std::cout << indent << "compression: (H0) (initial hash value)" << std::endl;
    std::cout << indent << "-----------" << std::endl;
    for (int i = 0; i < 8; i++) {
        std::cout << indent << registers[i] << " =" << std::endl;
    }
    delay("slowest");
    
    // Frame 4: Show square roots
    clearScreen();
    if (!g_state.empty()) {
        std::cout << g_state << "\n" << std::endl;
    }
    std::cout << indent << "-----------" << std::endl;
    std::cout << indent << "compression: (H0) (initial hash value)" << std::endl;
    std::cout << indent << "-----------" << std::endl;
    for (int i = 0; i < 8; i++) {
        std::cout << indent << registers[i] << " = √" << primes[i] << std::endl;
    }
    delay("slowest");
    
    // Frame 5: Show square root values
    clearScreen();
    if (!g_state.empty()) {
        std::cout << g_state << "\n" << std::endl;
    }
    std::cout << indent << "-----------" << std::endl;
    std::cout << indent << "compression: (H0) (initial hash value)" << std::endl;
    std::cout << indent << "-----------" << std::endl;
    for (int i = 0; i < 8; i++) {
        double sqrtVal = std::sqrt(primes[i]);
        std::cout << indent << registers[i] << " = " << std::fixed << std::setprecision(10) << sqrtVal << std::endl;
    }
    delay("slow");
    
    // Frame 6: Show fractional parts
    clearScreen();
    if (!g_state.empty()) {
        std::cout << g_state << "\n" << std::endl;
    }
    std::cout << indent << "-----------" << std::endl;
    std::cout << indent << "compression: (H0) (initial hash value)" << std::endl;
    std::cout << indent << "-----------" << std::endl;
    for (int i = 0; i < 8; i++) {
        double sqrtVal = std::sqrt(primes[i]);
        double fractional = sqrtVal - std::floor(sqrtVal);
        std::cout << indent << registers[i] << " = " << std::fixed << std::setprecision(10) << fractional << std::endl;
    }
    delay("slow");
    
    // Frame 7: Show multiplication by 2^32
    clearScreen();
    if (!g_state.empty()) {
        std::cout << g_state << "\n" << std::endl;
    }
    std::cout << indent << "-----------" << std::endl;
    std::cout << indent << "compression: (H0) (initial hash value)" << std::endl;
    std::cout << indent << "-----------" << std::endl;
    for (int i = 0; i < 8; i++) {
        double sqrtVal = std::sqrt(primes[i]);
        double fractional = sqrtVal - std::floor(sqrtVal);
        std::cout << indent << registers[i] << " = " << std::fixed << std::setprecision(10) << fractional << " * 2^32" << std::endl;
    }
    delay("slow");
    
    // Frame 8: Show floored values
    clearScreen();
    if (!g_state.empty()) {
        std::cout << g_state << "\n" << std::endl;
    }
    std::cout << indent << "-----------" << std::endl;
    std::cout << indent << "compression: (H0) (initial hash value)" << std::endl;
    std::cout << indent << "-----------" << std::endl;
    for (int i = 0; i < 8; i++) {
        double sqrtVal = std::sqrt(primes[i]);
        double fractional = sqrtVal - std::floor(sqrtVal);
        uint32_t value = static_cast<uint32_t>(fractional * std::pow(2, 32));
        std::cout << indent << registers[i] << " = " << value << std::endl;
    }
    delay("slow");
    
    // Frame 9: Show binary representation
    clearScreen();
    if (!g_state.empty()) {
        std::cout << g_state << "\n" << std::endl;
    }
    std::cout << indent << "-----------" << std::endl;
    std::cout << indent << "compression: (H0) (initial hash value)" << std::endl;
    std::cout << indent << "-----------" << std::endl;
    for (int i = 0; i < 8; i++) {
        double sqrtVal = std::sqrt(primes[i]);
        double fractional = sqrtVal - std::floor(sqrtVal);
        uint32_t value = static_cast<uint32_t>(fractional * std::pow(2, 32));
        std::cout << indent << registers[i] << " = " << bits(value) << std::endl;
    }
    delay("slow");
    
    // Frame 10: Final frame without description
    clearScreen();
    if (!g_state.empty()) {
        std::cout << g_state << "\n" << std::endl;
    }
    std::cout << indent << "-----------" << std::endl;
    std::cout << indent << "compression: (H0)" << std::endl;
    std::cout << indent << "-----------" << std::endl;
    for (int i = 0; i < 8; i++) {
        double sqrtVal = std::sqrt(primes[i]);
        double fractional = sqrtVal - std::floor(sqrtVal);
        uint32_t value = static_cast<uint32_t>(fractional * std::pow(2, 32));
        std::cout << indent << registers[i] << " = " << bits(value) << std::endl;
    }
    delay("end");
}

// ============ Main ============

int main(int argc, char* argv[]) {
    // Parse command line arguments for delay if needed
    if (argc > 1) {
        g_delay = argv[1];
    } else {
        g_delay = "normal";
    }
    
    // Optional state string
    if (argc > 2) {
        g_state = argv[2];
    }
    
    showInitialHashValues();
    
    return 0;
}
