#ifndef SHA256LIB_H
#define SHA256LIB_H

#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <cstdint>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <thread>
#include <chrono>

// ============ Global Variables ============
extern std::string g_delay;

// ============ Utility Functions ============
void clearScreen();
std::string bits(uint32_t x, int n = 32);
std::string hex(uint32_t i);
std::string bitstring(const std::string& str);
void delay(const std::string& speed);
std::string input_type(const std::string& input);
std::vector<uint8_t> bytes(const std::string& input, const std::string& type);

// ============ Operations ============
uint32_t add(const std::vector<uint32_t>& values);
uint32_t add(uint32_t a, uint32_t b);
uint32_t add(uint32_t a, uint32_t b, uint32_t c);
uint32_t add(uint32_t a, uint32_t b, uint32_t c, uint32_t d);
uint32_t add(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e);
uint32_t rotr(int n, uint32_t x);
uint32_t shr(int n, uint32_t x);

// ============ SHA-256 Functions ============
uint32_t sigma0(uint32_t x);
uint32_t sigma1(uint32_t x);
uint32_t usigma0(uint32_t x);
uint32_t usigma1(uint32_t x);
uint32_t ch(uint32_t x, uint32_t y, uint32_t z);
uint32_t maj(uint32_t x, uint32_t y, uint32_t z);

// ============ Preprocessing ============
std::string padding(const std::string& message);
std::vector<std::string> split(const std::string& message, int size = 512);

// ============ Message Schedule ============
std::vector<uint32_t> calculate_schedule(const std::string& block);

// ============ Constants ============
extern const std::vector<uint32_t> K;
extern const std::vector<uint32_t> IV;

// ============ Compression ============
std::vector<uint32_t> compression(const std::vector<uint32_t>& initial, 
                                  const std::vector<uint32_t>& schedule,
                                  const std::vector<uint32_t>& constants);

// ============ SHA-256 ============
std::string sha256(const std::string& str);

#endif // SHA256LIB_H
