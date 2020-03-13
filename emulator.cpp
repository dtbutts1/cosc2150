// Dawson Butts
// emulator.cpp 

#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char** argv) {

	
	int i;
	char* in, * out;

	in = NULL;
	out = NULL;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--in") == 0) {
			if ((i + 1) < argc) {
				in = argv[i+1];
				i++;
			}
			else {
				cerr << "Error: --in missing file name\n";
				exit(1);
			}
		}
		else if (strcmp(argv[i], "--out") == 0) {
			if ((i + 1) < argc) {
				out = argv[i+1];
				i++;
			}
			else {
				cerr << "Error: --out missing file name\n";
				exit(1);
			}
		}
		else {
			cerr << "Error: invalid parameter >" << argv[i] << "<\n";
			exit(1);
		}
	}

	if (in == NULL) {
		cerr << "Error: missing --in <fn> parameter\n";
		exit(1);
	}
	if (out == NULL) {
		cerr << "Error: missing --out <fn> parameter\n";
		exit(1);
	}


	// Read in Input file into `memory` 

	string instruction;
	int number;
	vector<int> mem;


	ifstream inputFile;
	inputFile.open(in);
	
	while (inputFile >> instruction) {
		number = stoi(instruction, nullptr, 16);
		mem.push_back(number);
	}
	inputFile.close();

	ofstream outputFile;
	outputFile.open(out);

	// Loop until 'Halt' instruction 

	unsigned int PC = 0;
	unsigned int IR = 0;
	unsigned int AC = 0;
	unsigned int MAR = 0;
	unsigned int MDR = 0;
	unsigned int op;
	unsigned int hand;

	bool stop = false;

	while (PC < 5000 && !stop) {
		IR = mem[PC];
		PC++;
		op = (IR & 0xF000);
		hand = (IR & 0x0FFF);
		
		switch (op) {
		case 0x1000: //Load
			MAR = hand;
			MDR = mem[MAR];
			AC = MDR;
			break;

		case 0x2000: //Store
			MAR = hand;
			MDR = AC;
			mem[MAR] = MDR;
			break;

		case 0x3000: //Add
			MAR = hand;
			MDR = mem[MAR];
			AC = AC + MDR;
			break;

		case 0x4000: //Subtract
			MAR = hand;
			MDR = mem[MAR];
			AC = AC - MDR;
			break;

		case 0x5000: //Input
			cout << "Enter Input: ";
			cin >> hand;
			AC = hand;
			break;

		case 0x6000: //Output
			outputFile << AC << endl;
			//cout << AC << endl;
			break;

		case 0x7000: //Halt
			stop = true;
			break;

		case 0x8000: //Skips

			//SkipEq0
			if (hand  == 0x0400) {
				if (AC == 0) {
					PC++;
				}
			}
			//skipGt0
			else if (hand == 0x0800) {
				if (AC > 0) {
					PC++;
				}
			}
			//skipLt0
			else {
				if (AC < 0) {
					PC++;
				}
			}
			break;

		case 0x9000: //Jump 
			PC = hand;
			break;

		case 0xA000: //Clear
			AC = 0;
			break;

		case 0xB000: //AddI
			MAR = hand;
			MDR = mem[MAR];
			MAR = MDR;
			MDR = mem[MAR];
			AC = AC + MDR;
			break;

		case 0xC000: //JumpI
			MAR = hand;
			MDR = mem[MAR];
			PC = MDR;
			break;

		case 0xE000: //StoreI
			MAR = hand;
			MDR = mem[MAR];
			MAR = MDR;
			MDR = AC;
			mem[MAR] = MDR;
			break;

		case 0xD000: //LoadI
			MAR = hand;
			MDR = mem[MAR];
			MAR = MDR;
			MDR = mem[MAR];
			AC = MDR;
			break;


		case 0x0000: //JnS
			MDR = PC;
			MAR = hand;
			mem[MAR] = MDR;
			MDR = hand;
			AC = 1;
			AC = AC + MDR;
			PC = AC;
			break;

		}
	}

	outputFile.close();
	

	return(0);
}



