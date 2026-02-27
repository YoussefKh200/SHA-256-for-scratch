#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <numeric>
#include <cstdlib>
#include <thread>
#include <chrono>


using namespace std;


int main(int argc, char* argv[]) {

	vector<uint32_t> numbers = {
	        0b01000000000000001111111111111111,
        	0b01000000000000000000000000000000,
        	0b01000000000000000000000000000000,
        	0b01000000000000000000000000000000,
        	0b01000000000000000000000000000000,
	};

	if (argc >= 3) {
		numbers.clear();
		for(int i=1 ;i<argc; i++){
			numbers.push_back(static_cast<uint32_t>(stoi(argv[i], nullptr, 2)));
		}
	}

	uint64_t total = accumulate(numbers.begin(), numbers.end, uint64_t(0));
	size_t width = bitset<64>(total).to_string().find('1');
	width = 64 - width;

	for (size_t j=0; j<number.size(); j++){
		string bin= bitset<32>(numbers[j]).to_string();
		cout << string(width - bin.size(), '  ') << bin;
		if(j > 0 && j<= i){
			cout << "+";
		}
		cout << "\n";
	}

	cout << string (width - 32, '  ') <<

	if (i == 0) {
            cout << "\n";
        } else if (i < numbers.size()) {
            uint64_t sum = accumulate(numbers.begin(), numbers.begin() + i + 1, uint64_t(0));
            cout << string(width - 32, ' ') << bitset<32>(sum).to_string() << "\n";
            if (i == numbers.size() - 1)
                this_thread::sleep_for(chrono::milliseconds(400));
        } else {
            uint64_t sum = accumulate(numbers.begin(), numbers.end(), uint64_t(0));
            uint32_t modSum = sum % (1ULL << 32);
            cout << string(width - 32, ' ') << bitset<32>(modSum).to_string() << " mod 2**32\n";
        }

        this_thread::sleep_for(chrono::milliseconds(400));
    }

    this_thread::sleep_for(chrono::seconds(1));
    return 0;
}

