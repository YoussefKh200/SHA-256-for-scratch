#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <thread>
#include <chrono>


using namespace std;

void delay(const string& level){
	if (level == "normal"){
		this_thread::sleep_for(chrono::milliseconds(400));
	}else if (level == "slow"){
		this_thread::sleep_for(chrono::seconds(1));
	}

}

vector split(const string& input, size_t blockSize){
	vector block;
	for (size_t i=0; i< input.size(); i+= blockSize){
		blocks.push_back(input.substr(i, blockSize));
	}
	return blocks;
}

string padding(const string& message){
	size_t padLen =(512 -( message.size() % 512)) %512;
	return message + (padLen, '0');
}

int main (int argc, char* argv[]) {
	string input ="abc";
	string message;

	for (char c: input){
		for (int i=7; i>= 0; --i) {
			message.push_back(((c >> i) & 1) ? '1' : '0');
		}
	}

	if (argc > 1){
		message = argv[1];
	}

	string padded = padding(message);
	vector blocks = split (padded , 512);

	system ("clear"); // use cls on windows
	string state = "";
	if (!state.empty())
		cout << state << "\n";

	cout << "--------------\n";
    	cout << "message blocks:\n";
    	cout << "--------------\n";
	delay("normal");

	system("clear");
	if (!state.empty()) std::cout << state << "\n";
    	cout << "--------------\n";
    	cout << "message blocks:\n";
    	cout << "--------------\n";

	for (size_t i=0 ; i< blocks.size(); i++){
		cout << i << ":" << block[i] << "\n";
		delay("normal");
	}
	delay ("slow");

	string contents;
	for (size_t i=0 ; i< blocks.size(); i++){
		content += to_string(i) +":" + block[i];
		if (i < blocks.size() -1) contents += "\n";
	}

	state += "\n--------------\nmessage blocks:\n--------------\n" + contents;

	cout << state << "\n";

	return 0
}
