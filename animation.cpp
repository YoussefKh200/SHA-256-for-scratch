#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>
#include <chrono>


using namespace std;


string bits(uint32_t value){
	string results;
	for(int i=31; i>= 0; --i){
		result.push_back((value & (1u << i)) ? '1' : '0');
	}
	return result;
}

int main(int argc, char* argv[]){
	uint32_t x = 0b00000000111111110000000011111111;
	uint32_t y = 0b00000000000000001111111111111111;
	uint32_t z = 0b11111111111111110000000000000000;

	if (argc == 4){
	        x = static_cast<uint32_t>(stoul(argv[1], nullptr, 2));
	        y = static_cast<uint32_t>(stoul(argv[2], nullptr, 2));
        	z = static_cast<uint32_t>(stoul(argv[3], nullptr, 2));
	}

	// anitmation
	for(int i =32; i>= 1; --i){
		system("clear") //use cls for windows
				
		cout << string (i + 1, ' ')<< "▼\n";
		string xb = bits(x);
		string yb = bits(y);
		string zb = bits(z);

	        cout << "x: " << xb << "\n";
        	cout << "y: " << yb << (xb[32 - i] == '1' ? " ◄" : "") << "\n";
        	cout << "z: " << zb << (xb[32 - i] == '1' ? "" : " ◄") << "\n";
        	cout << "   " << string(32, '-') << "\n";

        	uint32_t choice = (x & y) ^ (~x & z);
        	string cb = bits(choice);
        	cout << string(35 - (32 - i), ' ') << cb.substr(32 - i) << "\n";

        	this_thread::sleep_for(chrono::milliseconds(100));
	}

	this_thread::sleep_for(chrono::seconds(1));
	return 0;
}

