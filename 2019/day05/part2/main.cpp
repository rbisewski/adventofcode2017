#include <cmath>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

#include "main.hpp"

using namespace std;

void run_program(vector<int> v, vector<int> *output, int input) {

    int first_address = 0;
    int second_address = 0;
    int first = 0;
    int second = 0;
    int output_address = 0;

    vector<int> vect = v;

    int opcode = 0;
    int first_param_mode = 0;
    int second_param_mode = 0;

    // cycle thru the program
    for (int i = 0; i < int(vect.size());i++) {

        opcode = 0;
        first_param_mode = 0;
        second_param_mode = 0;

        switch (to_string(vect[i]).length()) {
            case 1:
                opcode = stoi(to_string(vect[i]).substr(0,1));
                break;
            case 2:
                opcode = stoi(to_string(vect[i]).substr(0,2));
                break;
            case 3:
                opcode = stoi(to_string(vect[i]).substr(1,2));
                first_param_mode = stoi(to_string(vect[i]).substr(0,1));
                break;
            case 4:
                opcode = stoi(to_string(vect[i]).substr(2,2));
                first_param_mode = stoi(to_string(vect[i]).substr(1,1));
                second_param_mode = stoi(to_string(vect[i]).substr(0,1));
                break;
            case 5:
                opcode = stoi(to_string(vect[i]).substr(3,2));
                first_param_mode = stoi(to_string(vect[i]).substr(2,1));
                second_param_mode = stoi(to_string(vect[i]).substr(1,1));
                break;
            default:
                break;
        }

        // ADD - handle the opcode 1 case
        if (opcode == 1) {

            output_address = vect[i+3];

            first = vect[i+1];
            if (first_param_mode == 0) {
                first = vect[vect[i+1]];
            }

            second = vect[i+2];
            if (second_param_mode == 0) {
                second = vect[vect[i+2]];
            }

            vect[output_address] = first + second;

            i += 4;
            i--;
            continue;
        }

        // MULTIPLY - handle the opcode 2 case
        if (opcode == 2) {

            output_address = vect[i+3];

            first_address = vect[i+1];
            if (first_param_mode == 0) {
                first = vect[first_address];
            } else if (first_param_mode == 1) {
                first = first_address;
            }

            second_address = vect[i+2];
            if (second_param_mode == 0) {
                second = vect[second_address];
            } else if (second_param_mode == 1) {
                second = second_address;
            }

            vect[output_address] = first * second;

            i += 4;
            i--;
            continue;
        }

        // INPUT - handle the opcode 3 case
        if (opcode == 3) {
            output_address = vect[i+1];
            vect[output_address] = input;
            i += 2;
            i--;
            continue;
        }

        // OUTPUT - handle the opcode 4 case
        if (opcode == 4) {

            first_address = vect[i+1];
            if (first_param_mode == 0) {
                first = vect[first_address];
            } else if (first_param_mode == 1) {
                first = first_address;
            }

            output->push_back(first);
            i += 2;
            i--;
            continue;
        }

        // JUMP-IF-TRUE - handle the opcode 5 case
        if (opcode == 5) {

            first = vect[i+1];
            if (first_param_mode == 0) {
                first = vect[vect[i+1]];
            }

            second = vect[i+2];
            if (second_param_mode == 0) {
                second = vect[vect[i+2]];
            }

            if (first != 0) {
                vect[i] = second;
                i = second;
                i--;
                continue;
            }

            i += 3;
            i--;
            continue;
        }

        // JUMP-IF-FALSE - handle the opcode 6 case
        if (opcode == 6) {

            first_address = vect[i+1];
            if (first_param_mode == 0) {
                first = vect[first_address];
            } else if (first_param_mode == 1) {
                first = first_address;
            }

            second_address = vect[i+2];
            if (second_param_mode == 0) {
                second = vect[second_address];
            } else if (second_param_mode == 1) {
                second = second_address;
            }

            if (first == 0) {
                vect[i] = second;
                i = second;
                i--;
                continue;
            }

            i += 3;
            i--;
            continue;
        }

        // LESS-THAN - handle the opcode 7 case
        if (opcode == 7) {

            output_address = vect[i+3];

            first_address = vect[i+1];
            if (first_param_mode == 0) {
                first = vect[first_address];
            } else if (first_param_mode == 1) {
                first = first_address;
            }

            second_address = vect[i+2];
            if (second_param_mode == 0) {
                second = vect[second_address];
            } else if (second_param_mode == 1) {
                second = second_address;
            }

            vect[output_address] = 0;
            if (first < second) {
                vect[output_address] = 1;
            }

            i += 4;
            i--;
            continue;
        }

        // EQUALS - handle the opcode 8 case
        if (opcode == 8) {

            output_address = vect[i+3];

            first_address = vect[i+1];
            if (first_param_mode == 0) {
                first = vect[first_address];
            } else if (first_param_mode == 1) {
                first = first_address;
            }

            second_address = vect[i+2];
            if (second_param_mode == 0) {
                second = vect[second_address];
            } else if (second_param_mode == 1) {
                second = second_address;
            }

            vect[output_address] = 0;
            if (first == second) {
                vect[output_address] = 1;
            }

            i += 4;
            i--;
            continue;
        }

        // EXIT - handle the opcode 99 case
        if (opcode == 99) {
            break;
        }
    }
}

int main() {

    string fileName = "input1.txt";
    string str;
    vector<int> vect;

    vector<int> output;

    int i = 0;

    ifstream in(fileName.c_str());

    if(!in) {
    	cerr << "Error: Unable to open the following file... " << fileName << endl;
    	return 1;
    }
    getline(in, str);
    in.close();

    if (str.size() <= 0) {
        cerr << "This input file is empty... " << fileName << endl;
    }

    stringstream ss(str);

    for (i = 0; ss >> i;) {
        vect.push_back(i);
        if (ss.peek() == ',') {
            ss.ignore();
        }
    }

    run_program(vect, &output, 5);

    for (size_t j = 0; j < output.size(); j++) {
        cout << output[j] << endl;
    }

    return 0;
}
