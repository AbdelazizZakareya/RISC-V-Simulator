#pragma once
#include "instruction.h"

class compiler {
    int program_counter;
    vector<instruction> instructions;
    map<int, int> memory;
    map<string, int> labels;
    vector<int> registers;
    map<string, string> instr_type; // instr_type["ADD"] = "r_type"
public:
    compiler(int initial_count); // initializes parameters, program count to initial, calls "initalize commands"
    void initialize_commands(); // initialize commands
    void take_input(ifstream& instruc_file); // takes the input and calls decompose_instruction for each line of string
    instruction decompose_instruction(string instruction_text, int instruction_idx);  
    void compile_all(); // compiles each instruction and calls print() 
    void compile_r_type(string command, int& rd, int rs1, int rs2); 
    void compile_i_type(string command, int& rd, int rs1, int imm); 
    void compile_s_type(string command, int& rd, int rs1, int imm); 
    void compile_b_type(string command, int rs1, int rs2, int imm);
    void compile_u_type(string command, int& rd, int imm);
    void compile_j_type(string command, int& rd, int imm);
    int convert_register(string reg); // takes x0, x1, ..., x31, and converts it to register number
    void print_state(); // prints current state
    string to_binary(int num);
    string to_hexa(int num);
    instruct_type to_type(string type);
};