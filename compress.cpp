#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <thread>
#include <chrono>

using namespace std;

string bits(uint32_t v) {
    string s;
    for (int i = 31; i >= 0; --i) s.push_back((v & (1u << i)) ? '1' : '0');
    return s;
}

uint32_t rotr(uint32_t x, int n) {
    return (x >> n) | (x << (32 - n));
}

uint32_t usigma0(uint32_t x) {
    return rotr(x,2) ^ rotr(x,13) ^ rotr(x,22);
}

uint32_t usigma1(uint32_t x) {
    return rotr(x,6) ^ rotr(x,11) ^ rotr(x,25);
}

uint32_t ch(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (~x & z);
}

uint32_t maj(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

uint32_t add(uint32_t a, uint32_t b) { return (a + b) % (1u<<32); }
uint32_t add(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e) { return (a + b + c + d + e) % (1u<<32); }

void delay(string level) {
    if (level=="normal") this_thread::sleep_for(chrono::milliseconds(400));
    else if (level=="slowest") this_thread::sleep_for(chrono::seconds(2));
    else if (level=="fastest") this_thread::sleep_for(chrono::milliseconds(100));
    else if (level=="end") this_thread::sleep_for(chrono::seconds(3));
}

int main() {
    vector<uint32_t> initial = {0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
                                0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
    vector<uint32_t> hash = initial;
    vector<uint32_t> w(64,0);
    w[0] = 0x61626380;
    vector<uint32_t> k(64,0);
    k[0] = 0x428a2f98;

    string indent = "  ";

    uint32_t t1 = add(w[0],k[0],usigma1(hash[4]),ch(hash[4],hash[5],hash[6]),hash[7]);
    uint32_t t2 = add(usigma0(hash[0]),maj(hash[0],hash[1],hash[2]));

    system("clear");
    cout << indent << "-----------\n";
    cout << indent << "compression: (H0 -> H1)\n";
    cout << indent << "-----------\n";
    cout << indent << "W0 = " << bits(w[0]) << " (message schedule)\n";
    cout << indent << "K0 = " << bits(k[0]) << " (constant)\n\n";
    cout << indent << "T1 = Σ1(e)+Ch(e,f,g)+h+K0+W0 = " << bits(t1) << "\n";
    cout << indent << "T2 = Σ0(a)+Maj(a,b,c)        = " << bits(t2) << "\n\n";
    cout << indent << "a = " << bits(add(t1,t2)) << " <- T1+T2\n";
    cout << indent << "b = " << bits(hash[0]) << "\n";
    cout << indent << "c = " << bits(hash[1]) << "\n";
    cout << indent << "d = " << bits(hash[2]) << "\n";
    cout << indent << "e = " << bits(add(hash[3],t1)) << " + T1\n";
    cout << indent << "f = " << bits(hash[4]) << "\n";
    cout << indent << "g = " << bits(hash[5]) << "\n";
    cout << indent << "h = " << bits(hash[6]) << "\n";
    delay("slowest");

    hash[7] = hash[6];
    hash[6] = hash[5];
    hash[5] = hash[4];
    hash[4] = add(hash[3],t1);
    hash[3] = hash[2];
    hash[2] = hash[1];
    hash[1] = hash[0];
    hash[0] = add(t1,t2);

    for (int i=1;i<64;i++) {
        t1 = add(w[i],k[i],usigma1(hash[4]),ch(hash[4],hash[5],hash[6]),hash[7]);
        t2 = add(usigma0(hash[0]),maj(hash[0],hash[1],hash[2]));
        system("clear");
        cout << indent << "-----------\n";
        cout << indent << "compression: (round " << i << ")\n";
        cout << indent << "-----------\n";
        cout << indent << "W" << i << " = " << bits(w[i]) << " (message schedule)\n";
        cout << indent << "K" << i << " = " << bits(k[i]) << " (constant)\n\n";
        cout << indent << "T1 = Σ1(e)+Ch(e,f,g)+h+K" << i << " + W" << i << " = " << bits(t1) << "\n";
        cout << indent << "T2 = Σ0(a)+Maj(a,b,c)        = " << bits(t2) << "\n\n";
        cout << indent << "a = " << bits(add(t1,t2)) << " <- T1+T2\n";
        cout << indent << "b = " << bits(hash[1]) << "\n";
        cout << indent << "c = " << bits(hash[2]) << "\n";
        cout << indent << "d = " << bits(hash[3]) << "\n";
        cout << indent << "e = " << bits(add(hash[4],t1)) << " + T1\n";
        cout << indent << "f = " << bits(hash[5]) << "\n";
        cout << indent << "g = " << bits(hash[6]) << "\n";
        cout << indent << "h = " << bits(hash[7]) << "\n";
        delay("fastest");

        hash[7] = hash[6];
        hash[6] = hash[5];
        hash[5] = hash[4];
        hash[4] = add(hash[3],t1);
        hash[3] = hash[2];
        hash[2] = hash[1];
        hash[1] = hash[0];
        hash[0] = add(t1,t2);
    }

    system("clear");
    cout << indent << "-----------\n";
    cout << indent << "compression: (H1)\n";
    cout << indent << "-----------\n";
    for (int i=0;i<8;i++) {
        cout << indent << char('a'+i) << " = " << bits(add(initial[i],hash[i])) << "\n";
    }
    delay("end");

    for (int i=0;i<8;i++) hash[i] = add(initial[i],hash[i]);

    cout << indent << "Final state:\n";
    for (int i=0;i<8;i++) {
        cout << indent << char('a'+i) << " = " << bits(hash[i]) << "\n";
    }

    return 0;
}
