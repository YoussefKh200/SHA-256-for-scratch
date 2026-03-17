#include <iostream>
#include <bitset>
#include <thread>
#include <chrono>
#include <cstdlib>

using namespace std;

// --- Utility functions ---
string bits(uint32_t x) {
    return bitset<32>(x).to_string();
}

uint32_t rotr(int n, uint32_t x) {
    return (x >> n) | (x << (32 - n));
}

uint32_t shr(int n, uint32_t x) {
    return x >> n;
}

void clear_screen() {
    system("clear"); // works on Linux
}

void sleep_ms(int ms) {
    this_thread::sleep_for(chrono::milliseconds(ms));
}

// --- Animation for σ0(x) ---
void animate_sigma0(uint32_t x) {
    clear_screen();
    cout << "x:       " << bits(x) << "\n"
         << "         " << string(32, '-') << "\n"
         << "ROTR 7:  " << bits(x) << "\n"
         << "ROTR 18: " << bits(x) << "\n"
         << " SHR 3:  " << bits(x) << "\n"
         << "         " << string(32, '-') << "\n"
         << "σ0(x):\n";
    sleep_ms(100);

    // ROTR(7, x)
    for (int i = 0; i <= 7; i++) {
        clear_screen();
        cout << "x:       " << bits(x) << "\n"
             << "         " << string(32, '-') << "\n"
             << "ROTR 7:  " << bits(rotr(i, x)) << "\n"
             << "ROTR 18: " << bits(x) << "\n"
             << " SHR 3:  " << bits(x) << "\n"
             << "         " << string(32, '-') << "\n"
             << "σ0(x):\n";
        sleep_ms(100);
    }

    // ROTR(18, x)
    for (int i = 0; i <= 18; i++) {
        clear_screen();
        cout << "x:       " << bits(x) << "\n"
             << "         " << string(32, '-') << "\n"
             << "ROTR 7:  " << bits(rotr(7, x)) << "\n"
             << "ROTR 18: " << bits(rotr(i, x)) << "\n"
             << " SHR 3:  " << bits(x) << "\n"
             << "         " << string(32, '-') << "\n"
             << "σ0(x):\n";
        sleep_ms(100);
    }

    // SHR(3, x)
    for (int i = 0; i <= 3; i++) {
        clear_screen();
        cout << "x:       " << bits(x) << "\n"
             << "         " << string(32, '-') << "\n"
             << "ROTR 7:  " << bits(rotr(7, x)) << "\n"
             << "ROTR 18: " << bits(rotr(18, x)) << "\n"
             << " SHR 3:  " << bits(shr(i, x)) << "\n"
             << "         " << string(32, '-') << "\n"
             << "σ0(x):\n";
        sleep_ms(100);
    }

    // Final XOR
    uint32_t result = rotr(7, x) ^ rotr(18, x) ^ shr(3, x);
    for (int i = 32; i >= 1; i--) {
        clear_screen();
        cout << "x:       " << bits(x) << "\n"
             << "         " << string(32, '-') << "\n"
             << "ROTR 7:  " << bits(rotr(7, x)) << "\n"
             << "ROTR 18: " << bits(rotr(18, x)) << " XOR\n"
             << " SHR 3:  " << bits(shr(3, x)) << " XOR\n"
             << "         " << string(32, '-') << "\n"
             << "σ0(x):   " << bits(result).substr(32 - i) << "\n";
        sleep_ms(100);
    }
    sleep_ms(1000);
}

// --- Animation for σ1(x) ---
void animate_sigma1(uint32_t x) {
    clear_screen();
    cout << "x:       " << bits(x) << "\n"
         << "         " << string(32, '-') << "\n"
         << "ROTR 17: " << bits(x) << "\n"
         << "ROTR 19: " << bits(x) << "\n"
         << " SHR 10: " << bits(x) << "\n"
         << "         " << string(32, '-') << "\n"
         << "σ1(x):\n";
    sleep_ms(100);

    // ROTR(17, x)
    for (int i = 0; i <= 17; i++) {
        clear_screen();
        cout << "x:       " << bits(x) << "\n"
             << "         " << string(32, '-') << "\n"
             << "ROTR 17: " << bits(rotr(i, x)) << "\n"
             << "ROTR 19: " << bits(x) << "\n"
             << " SHR 10: " << bits(x) << "\n"
             << "         " << string(32, '-') << "\n"
             << "σ1(x):\n";
        sleep_ms(100);
    }

    // ROTR(19, x)
    for (int i = 0; i <= 19; i++) {
        clear_screen();
        cout << "x:       " << bits(x) << "\n"
             << "         " << string(32, '-') << "\n"
             << "ROTR 17: " << bits(rotr(17, x)) << "\n"
             << "ROTR 19: " << bits(rotr(i, x)) << "\n"
             << " SHR 10: " << bits(x) << "\n"
             << "         " << string(32, '-') << "\n"
             << "σ1(x):\n";
        sleep_ms(100);
    }

    // SHR(10, x)
    for (int i = 0; i <= 10; i++) {
        clear_screen();
        cout << "x:       " << bits(x) << "\n"
             << "         " << string(32, '-') << "\n"
             << "ROTR 17: " << bits(rotr(17, x)) << "\n"
             << "ROTR 19: " << bits(rotr(19, x)) << "\n"
             << " SHR 10: " << bits(shr(i, x)) << "\n"
             << "         " << string(32, '-') << "\n"
             << "σ1(x):\n";
        sleep_ms(100);
    }

    // Final XOR
    uint32_t result = rotr(17, x) ^ rotr(19, x) ^ shr(10, x);
    for (int i = 32; i >= 1; i--) {
        clear_screen();
        cout << "x:       " << bits(x) << "\n"
             << "         " << string(32, '-') << "\n"
             << "ROTR 17: " << bits(rotr(17, x)) << "\n"
             << "ROTR 19: " << bits(rotr(19, x)) << " XOR\n"
             << " SHR 10: " << bits(shr(10, x)) << " XOR\n"
             << "         " << string(32, '-') << "\n"
             << "σ1(x):   " << bits(result).substr(32 - i) << "\n";
        sleep_ms(100);
    }
    sleep_ms(1000);
}

// --- Main driver ---
int main(int argc, char* argv[]) {
    uint32_t x = 0b00000000000000000011111111111111;
    if (argc > 1) {
        string arg = argv[1];
        if (arg.size() > 32) {
            cout << "We only operate on 32-bit words in SHA-256. Your x is "
                 << arg.size() << " bits.\n";
            return 1;
        }
        x = bitset<32>(arg).to_ulong();
    }

    animate_sigma0(x);
    animate_sigma1(x);

    return 0;
}
