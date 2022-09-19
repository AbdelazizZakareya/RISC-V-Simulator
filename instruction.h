#pragma once
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <map>
#include <fstream>
#include <sstream>
using namespace std;

enum instruct_type { r_type, i_type, s_type, b_type, u_type, j_type, exiting };

struct instruction {
    instruct_type type;
    string command;
    vector<int> operands; // if immediate then value, if register then index
};