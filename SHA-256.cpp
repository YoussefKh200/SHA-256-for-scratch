#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <csignal>
#include <cstdlib>

// ============ Global Variables ============
std::string g_input;
std::string g_delay = "normal";
std::string g_type;
std::vector<uint8_t> g_bytes;
std::string g_message;
std::vector<std::string> g_blocks;
std::string g_block;
int g_block_number = 0;
std::vector<uint32_t> g_hash;

// Forward declarations of functions from sha256lib
std::string input_type(const std::string& input);
std::vector<uint8_t> bytes(const std::string& input, const std::string& type);
void delay(const std::string& speed);
void clearScreen();

// Signal handler for Ctrl-C
void signalHandler(int signum) {
    std::cout << "\nInterrupted. Exiting cleanly." << std::endl;
    exit(signum);
}

// ============ Helper to load Ruby scripts ============
// In C++, we'll implement the functionality directly rather than "loading" files

// Message display
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
    std::cout << "Binary: " << g_message << std::endl;
    std::cout << "========================" << std::endl;
    delay(g_delay);
}

// Padding display
void showPadding() {
    // This would be implemented in padding.rb
    // For now, just a placeholder
    clearScreen();
    std::cout << "========================" << std::endl;
    std::cout << "Padding applied" << std::endl;
    std::cout << "========================" << std::endl;
    delay(g_delay);
}

// Blocks display
void showBlocks() {
    clearScreen();
    std::cout << "========================" << std::endl;
    std::cout << "Message Blocks (" << g_blocks.size() << " blocks of 512 bits)" << std::endl;
    std::cout << "========================" << std::endl;
    for (size_t i = 0; i < g_blocks.size(); i++) {
        std::cout << "Block " << i << ": " << g_blocks[i].substr(0, 64) << "..." << std::endl;
    }
    delay(g_delay);
}

// Schedule display
void showSchedule() {
    clearScreen();
    std::cout << "========================" << std::endl;
    std::cout << "Message Schedule for Block " << g_block_number << std::endl;
    std::cout << "========================" << std::endl;
    // This would be expanded in schedule.rb
    delay(g_delay);
}

// Initial hash values display
void showInitialHash() {
    clearScreen();
    std::cout << "========================" << std::endl;
    std::cout << "Initial Hash Values (IV)" << std::endl;
    std::cout << "========================" << std::endl;
    std::cout << "h0: " << std::hex << g_hash[0] << std::endl;
    std::cout << "h1: " << std::hex << g_hash[1] << std::endl;
    std::cout << "h2: " << std::hex << g_hash[2] << std::endl;
    std::cout << "h3: " << std::hex << g_hash[3] << std::endl;
    std::cout << "h4: " << std::hex << g_hash[4] << std::endl;
    std::cout << "h5: " << std::hex << g_hash[5] << std::endl;
    std::cout << "h6: " << std::hex << g_hash[6] << std::endl;
    std::cout << "h7: " << std::hex << g_hash[7] << std::endl;
    std::cout << std::dec;
    delay(g_delay);
}

// Compression display
void showCompression() {
    clearScreen();
    std::cout << "========================" << std::endl;
    std::cout << "Compression Function - Block " << g_block_number << std::endl;
    std::cout << "========================" << std::endl;
    delay(g_delay);
}

// Final hash display
void showFinalHash() {
    clearScreen();
    std::cout << "========================" << std::endl;
    std::cout << "Final SHA-256 Hash" << std::endl;
    std::cout << "========================" << std::endl;
    for (size_t i = 0; i < g_hash.size(); i++) {
        std::cout << "h" << i << ": " << std::hex << std::setw(8) << std::setfill('0') << g_hash[i] << std::endl;
    }
    
    // Concatenate for full hash
    std::cout << "\nFull Hash: ";
    for (uint32_t val : g_hash) {
        std::cout << std::hex << std::setw(8) << std::setfill('0') << val;
    }
    std::cout << std::dec << std::endl;
    std::cout << "========================" << std::endl;
}

// ============ Main SHA-256 Driver ============

int main(int argc, char* argv[]) {
    // Set up signal handler for Ctrl-C
    signal(SIGINT, signalHandler);

    // Parse command line arguments
    if (argc >= 2) {
        g_input = argv[1];
    } else {
        g_input = "abc";
    }
    
    if (argc >= 3) {
        g_delay = argv[2];
    }

    // Detect input type
    g_type = input_type(g_input);

    // Read file if file is given
    if (g_type == "file") {
        std::ifstream file(g_input);
        if (file.is_open()) {
            std::string content((std::istreambuf_iterator<char>(file)),
                                std::istreambuf_iterator<char>());
            g_input = content;
            file.close();
        } else {
            std::cerr << "Error: Could not open file " << g_input << std::endl;
            return 1;
        }
    }

    // Convert input to bytes
    g_bytes = bytes(g_input, g_type);

    // Set message (binary representation of data)
    if (g_type == "string" || g_type == "file" || g_type == "hex") {
        g_message = "";
        for (uint8_t byte : g_bytes) {
            g_message += std::bitset<8>(byte).to_string();
        }
    } else if (g_type == "binary") {
        g_message = g_input.substr(2); // trim the leading 0b
    }

    // Note about hitting enter to step
    if (g_delay == "enter") {
        std::cout << "Hit enter to step through." << std::endl;
        std::cin.get();
    }

    // 1. Message
    showMessage();

    // 2. Padding
    showPadding();
    // In a full implementation, we would call padding() here
    // g_message = padding(g_message);

    // 3. Message Blocks
    showBlocks();
    // In a full implementation, we would split the padded message
    // g_blocks = split(g_message, 512);

    // For demonstration, create a dummy block
    if (g_blocks.empty()) {
        g_blocks.push_back(g_message + std::string(512 - g_message.length(), '0'));
    }

    // 4-5. Process each block
    for (size_t i = 0; i < g_blocks.size(); i++) {
        g_block = g_blocks[i];
        g_block_number = i;

        // 4. Message Schedule
        showSchedule();

        // 5. Initial Hash Values (only for first block)
        if (i == 0) {
            // Set initial hash values (IV)
            g_hash = IV; // This would come from sha256lib
            showInitialHash();
        }

        // 5. Compression
        showCompression();

        // In a full implementation, we would:
        // - Calculate schedule: std::vector<uint32_t> schedule = calculate_schedule(g_block);
        // - Run compression: g_hash = compression(g_hash, schedule, K);
    }

    // 6. Final Hash
    showFinalHash();

    return 0;
}
