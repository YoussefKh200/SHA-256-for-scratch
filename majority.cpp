#include <iostream>
#include <string>
#include <bitset>
#include <thread>
#include <chrono>
#include <cstdint>
#include <map>

// ============ Utility Functions ============

void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

std::string bits(uint32_t x) {
    return std::bitset<32>(x).to_string();
}

// Majority function: (x & y) ^ (x & z) ^ (y & z)
uint32_t maj(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

// Calculate majority of three bits
char maj_bit(char x, char y, char z) {
    int count = (x - '0') + (y - '0') + (z - '0');
    return (count >= 2) ? '1' : '0';
}

// ============ Main Visualization ============

int main(int argc, char* argv[]) {
    uint32_t x, y, z;
    
    // Default values (first three constants from SHA-256)
    x = 0b00000000111111110000000011111111; // 0x428a2f98
    y = 0b00000000000000001111111111111111; // 0x71374491
    z = 0b11111111111111110000000000000000; // 0xb5c0fbcf
    
    // Parse command line arguments (binary strings)
    if (argc == 4) {
        x = std::bitset<32>(argv[1]).to_ulong();
        y = std::bitset<32>(argv[2]).to_ulong();
        z = std::bitset<32>(argv[3]).to_ulong();
    }
    
    // Calculate majority result
    uint32_t result = maj(x, y, z);
    std::string x_bits = bits(x);
    std::string y_bits = bits(y);
    std::string z_bits = bits(z);
    std::string result_bits = bits(result);
    
    std::cout << "Majority Function: Maj(x, y, z) = (x & y) ^ (x & z) ^ (y & z)" << std::endl;
    std::cout << "The result bit is 1 if at least two of the three input bits are 1." << std::endl;
    std::cout << "Press Enter to start animation...";
    std::cin.get();
    
    // Animation: show bit-by-bit from MSB to LSB
    for (int i = 32; i >= 1; i--) {
        clearScreen();
        
        // Header
        std::cout << "Majority Function - Bit " << (33 - i) << " (from left)" << std::endl;
        std::cout << "Maj(x, y, z) = (x & y) ^ (x & z) ^ (y & z)" << std::endl;
        std::cout << std::endl;
        
        // Arrow pointing to current bit
        std::cout << std::string(i + 3, ' ') << "▼" << std::endl;
        
        // Display x, y, z with current bit highlighted
        std::cout << "x: " << x_bits << " " << x_bits[i-1] << std::endl;
        std::cout << "y: " << y_bits << " " << y_bits[i-1] << std::endl;
        std::cout << "z: " << z_bits << " " << z_bits[i-1] << std::endl;
        
        // Separator
        std::cout << "   " << std::string(32, '-') << std::endl;
        
        // Show result bits (from current position to end)
        std::string result_subset = result_bits.substr(i-1);
        std::cout << std::string(35 - result_subset.length(), ' ') << result_subset << std::endl;
        
        // Show calculation for current bit
        std::cout << std::endl;
        std::cout << "Current bit calculation:" << std::endl;
        std::cout << "  x" << (33 - i) << " = " << x_bits[i-1] << std::endl;
        std::cout << "  y" << (33 - i) << " = " << y_bits[i-1] << std::endl;
        std::cout << "  z" << (33 - i) << " = " << z_bits[i-1] << std::endl;
        std::cout << "  Majority: " << maj_bit(x_bits[i-1], y_bits[i-1], z_bits[i-1]) 
                  << " (since " << (x_bits[i-1] == '1' ? "1" : "0") << " + " 
                  << (y_bits[i-1] == '1' ? "1" : "0") << " + " 
                  << (z_bits[i-1] == '1' ? "1" : "0") << " = " 
                  << ((x_bits[i-1]-'0') + (y_bits[i-1]-'0') + (z_bits[i-1]-'0')) 
                  << " >= 2)" << std::endl;
        
        // Delay
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    
    // Final result
    clearScreen();
    std::cout << "========================" << std::endl;
    std::cout << "Final Majority Result" << std::endl;
    std::cout << "========================" << std::endl;
    std::cout << "x: " << x_bits << std::endl;
    std::cout << "y: " << y_bits << std::endl;
    std::cout << "z: " << z_bits << std::endl;
    std::cout << "   " << std::string(32, '-') << std::endl;
    std::cout << "   " << result_bits << std::endl;
    std::cout << std::endl;
    std::cout << "Hex: 0x" << std::hex << result << std::dec << std::endl;
    std::cout << "========================" << std::endl;
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    return 0;
}
