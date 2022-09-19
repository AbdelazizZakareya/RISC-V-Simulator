#include "compiler.h"

compiler::compiler(int initial_count) {
    program_counter = initial_count;
    registers.resize(32);
    initialize_commands();
}

void compiler::initialize_commands() {
    ifstream commands_file("commands_file.txt");
    for (int i = 0; i < 40; i++) {
        string command, type;
        commands_file >> command >> type;
        instr_type[command] = type;
    }
}

instruction compiler::decompose_instruction(string instruction_text, int instruction_idx) {
    instruction ans;
    // 1. Check if there exists a label (Use the map "labels" if there)
    int idx_colon = -1;
    for (int i = 0; i < (int)instruction_text.length(); i++) 
        if (instruction_text[i] == ':') {
            idx_colon = i;
            break;
        }
    if (idx_colon != -1) {
        stringstream for_label(instruction_text.substr(0, idx_colon));
        string label; for_label >> label;
        labels[label] = instruction_idx;
        int len = idx_colon + 1;
        reverse(instruction_text.begin(), instruction_text.end());
        for (int i = 0; i < len; i++) instruction_text.pop_back();
        reverse(instruction_text.begin(), instruction_text.end());
    }
    auto remove_back_spaces = [&]() { 
        while (!instruction_text.empty() && instruction_text.back() == ' ') instruction_text.pop_back(); };
    remove_back_spaces();
    if ((int)instruction_text.length() <= 2) {
        ans.command = "-1";
        return ans;
    }
    // 2. Use both map instr_type and to_type function to get the function type
    stringstream sin(instruction_text);
    string operation; sin >> operation;
    ans.command = operation;
    ans.type = to_type(instr_type[operation]);
    if (ans.type == exiting) return ans;
    // 3. Take any parameters that are given 
    // (and use convert register or label if there is a label)
    int num_parameters = 1;
    for (auto& c : instruction_text) if (c == ',') num_parameters++;
    ans.operands.resize(num_parameters);
    vector<string> parameters(num_parameters);
    // remove the operation 
    reverse(instruction_text.begin(), instruction_text.end());
    remove_back_spaces();
    while (instruction_text.back() != ' ') instruction_text.pop_back();
    remove_back_spaces();
    reverse(instruction_text.begin(), instruction_text.end());
    // remove comments
    int idx_comment = -1;
    for (int i = 0; i < (int)instruction_text.length(); i++) if (instruction_text[i] == '#') {
        idx_comment = i;
        break;
    }
    if (idx_comment != -1) {
        int len = instruction_text.substr(idx_comment, (int)instruction_text.length() - idx_comment).length();
        for (int i = 0; i < len; i++) instruction_text.pop_back();
    }
    // take parameters as strings
    int ptr = 0;
    for (auto& c : instruction_text) if (ptr < num_parameters) {
        if (c != ',' && c != ';' && c != ' ') parameters[ptr] += string(1, c);
        else if (c == ',' || c == ';') ptr++;
    }
    ptr = 0;
    for (auto& parameter : parameters) {
        // if register
        if (parameter[0] == 'x') {
            ans.operands[ptr++] = convert_register(parameter);
            continue;
        }
        // Handle the memory
        int idx_bracket = -1;
        for (int i = 0; i < (int)parameter.length(); i++) if (parameter[i] == '(') {
            idx_bracket = i;
            break;
        }
        if (idx_bracket != -1) {
            ans.operands.emplace_back(stoi(parameter.substr(0, idx_bracket)));
            int idx_bracket_2 = -1;
            for (int i = 0; i < (int)parameter.length(); i++) if (parameter[i] == ')') {
                idx_bracket_2 = i;
                break;
            }
            ans.operands[1] = convert_register(parameter.substr(idx_bracket + 1, idx_bracket_2 - idx_bracket - 1));
            break;
        }
        // if just a number
        if ((parameter[0] >= '0' && parameter[0] <= '9') || parameter[0] == '-') { 
            ans.operands[ptr++] = stoi(parameter);
            continue;
        }
        // if label
        ans.operands[ptr++] = labels[parameter];
    }
    return ans;
}

void compiler::take_input(ifstream& instruc_file) {
    while (instruc_file.good()) {
        string instruction_text;
        getline(instruc_file, instruction_text);
        if (instruction_text.size() <= 2) continue;
        auto instruc = decompose_instruction(instruction_text, (int)instructions.size());
        if (instruc.command != "-1") instructions.push_back(instruc);
    }
}

void compiler::compile_all() {
    auto runtime_err = [&]() { cout << "RUNTIME ERROR: CAN'T MODIFY x0 REGISTER"; exit(0); };
    while (program_counter < (int)instructions.size()) {
        print_state();
        auto& instr = instructions[program_counter];
        switch (instr.type) {
            case r_type: 
                if (instr.operands[0] == 0) runtime_err();
                compile_r_type(instr.command, registers[instr.operands[0]], 
                    registers[instr.operands[1]], registers[instr.operands[2]]);
                break;
            case i_type:
                if (instr.operands[0] == 0) runtime_err();
                compile_i_type(instr.command, registers[instr.operands[0]], 
                    registers[instr.operands[1]], instr.operands[2]);
                break;
            case s_type:
                compile_s_type(instr.command, registers[instr.operands[0]], 
                    registers[instr.operands[1]], instr.operands[2]);
                break;
            case b_type:
                compile_b_type(instr.command, registers[instr.operands[0]], 
                    registers[instr.operands[1]], instr.operands[2]);
                break;
            case u_type:
                if (instr.operands[0] == 0) runtime_err();
                compile_u_type(instr.command, registers[instr.operands[0]], instr.operands[1]);
                break;
            case j_type:
                if (instr.operands[0] == 0) runtime_err();
                compile_j_type(instr.command, registers[instr.operands[0]], instr.operands[1]);
                break;
            case exiting:
                cout << "Program compiled successfully!\n";
                exit(0);
                break;
        };
    }
}