#ifndef SHA256_INITIAL_H
#define SHA256_INITIAL_H

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
extern std::string g_state;

// ============ Utility Functions ============
void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

void delay(const std::string& speed) {
    // This would be the full delay implementation from sha256lib
    // For now, a simplified version
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

// ============ Initial Hash Values ============

// Calculate the initial hash values (H0-H7)
inline std::vector<uint32_t> calculateInitialHashValues() {
    std::vector<uint32_t> initial;
    int primes[] = {2, 3, 5, 7, 11, 13, 17, 19};
    
    for (int prime : primes) {
        double sqrtVal = std::sqrt(prime);
        double fractional = sqrtVal - std::floor(sqrtVal);
        uint32_t value = static_cast<uint32_t>(fractional * std::pow(2, 32));
        initial.push_back(value);
    }
    
    return initial;
}

// The actual IV constants used in SHA-256
const std::vector<uint32_t> IV = calculateInitialHashValues();

// For reference, the standard SHA-256 IV values are:
// H0 = 0x6a09e667
// H1 = 0xbb67ae85
// H2 = 0x3c6ef372
// H3 = 0xa54ff53a
// H4 = 0x510e527f
// H5 = 0x9b05688c
// H6 = 0x1f83d9ab
// H7 = 0x5be0cd19

// ============ Visualization ============

void showInitialHashValues() {
    int primes[] = {2, 3, 5, 7, 11, 13, 17, 19};
    std::string registers[] = {"a", "b", "c", "d", "e", "f", "g", "h"};
    std::string indent = "  ";
    
    // Calculate the actual values once
    std::vector<uint32_t> values = calculateInitialHashValues();
    
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
        std::cout << indent << registers[i] << " = " << values[i] << std::endl;
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
        std::cout << indent << registers[i] << " = " << bits(values[i]) << std::endl;
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
        std::cout << indent << registers[i] << " = " << bits(values[i]) << std::endl;
    }
    delay("end");
}

#endif // SHA256_INITIAL_H
