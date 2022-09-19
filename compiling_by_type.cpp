#include "compiler.h"
#define uint unsigned int

void compiler::compile_r_type(string command, int& rd, int rs1, int rs2) {
    if      (command == "ADD")   rd = (rs1 + rs2);              
    else if (command == "SUB" )  rd = (rs1 - rs2);              
    else if (command == "SLL" )  rd = (rs1 << rs2);             
    else if (command == "SLT" )  rd = (rs1 < rs2);              
    else if (command == "SLTU")  rd = ((uint)rs1 < (uint)rs2);  
    else if (command == "XOR" )  rd = (rs1 ^ rs2);              
    else if (command == "SRL" )  rd = (((uint)rs1) >> rs2);     
    else if (command == "SRA" )  rd = (rs1 >> rs2);             
    else if (command == "OR"  )  rd = (rs1 | rs2);              
    else if (command == "AND" )  rd = (rs1 & rs2);              
    program_counter++; 
}

void compiler::compile_i_type(string command, int& rd, int rs1, int imm) {
    if      (command == "LB")    rd = (int8_t)   memory[imm + rs1];                 
    else if (command == "LH"   ) rd = (int16_t)  memory[imm + rs1];                 
    else if (command == "LW"   ) rd =            memory[imm + rs1];                 
    else if (command == "LBU"  ) rd = (uint8_t)  memory[imm + rs1];                 
    else if (command == "LHU"  ) rd = (uint16_t) memory[imm + rs1];                 
    else if (command == "SLTI" ) rd = (rs1 < imm);                                  
    else if (command == "SLTIU") rd = ((uint)rs1 < (uint)imm);                      
    else if (command == "XORI" ) rd = (rs1 ^ imm);                                  
    else if (command == "ORI"  ) rd = (rs1 | imm);                                  
    else if (command == "ANDI" ) rd = (rs1 & imm);                                 
    else if (command == "ADDI" ) rd = (rs1 + imm);                                 
    else if (command == "JALR" ) rd = program_counter, program_counter = rs1 + imm; 
    else if (command == "SRLI" ) rd = ((uint)rs1 >> imm);                          
    else if (command == "SRAI" ) rd = (rs1 >> imm);                                
    else if (command == "SLLI" ) rd = ((uint)rs1 << imm);                          
    program_counter++;
}

void compiler::compile_s_type(string command, int& rd, int rs1, int imm) {
    if      (command == "SB") memory[rs1 + imm] = (int8_t)  rd;                     
    else if (command == "SH") memory[rs1 + imm] = (int16_t) rd;                     
    else if (command == "SW") memory[rs1 + imm] =           rd;                     
    program_counter++;
}

void compiler::compile_b_type(string command, int rs1, int rs2, int imm) {
    if      (command == "BEQ")  (rs1 == rs2 ? program_counter = imm : program_counter++);                
    else if (command == "BNE" ) (rs1 != rs2 ? program_counter = imm : program_counter++);                
    else if (command == "BLT" ) (rs1 < rs2  ? program_counter = imm : program_counter++);                
    else if (command == "BGE" ) (rs1 >= rs2 ? program_counter = imm : program_counter++);                
    else if (command == "BLTU") ((uint) rs1 < (uint)rs2  ? program_counter = imm : program_counter++);   
    else if (command == "BGEU") ((uint) rs1 >= (uint)rs2 ? program_counter = imm : program_counter++);   
}

void compiler::compile_u_type(string command, int& rd, int imm) {
    if      (command == "LUI")   rd = (imm << 12);                     
    else if (command == "AUIPC") rd = program_counter + (imm << 12);   
    program_counter++;
}

void compiler::compile_j_type(string command, int& rd, int imm) {
    if (command == "JAL") rd = program_counter + 1; program_counter = imm; 
}