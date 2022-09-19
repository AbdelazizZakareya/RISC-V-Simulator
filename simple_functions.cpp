#include "compiler.h"

int compiler::convert_register(string reg) {
    reg.erase(reg.begin());
    return stoi(reg);
}

string compiler::to_binary(int num) {
    string ans;
    while (num) {
        ans += to_string(num & 1);
        num >>= 1;
    }
    reverse(ans.begin(), ans.end());
    return (ans.empty() ? "0" : ans);
}

string compiler::to_hexa(int num) {
    string ans;
    while (num) {
        int temp = (num % 16);
        if (temp >= 10) ans += string(1, 'A' + temp - 10);
        else ans += to_string(temp);
        num >>= 4;
    }
    reverse(ans.begin(), ans.end());
    return (ans.empty() ? "0" : ans);
}

instruct_type compiler::to_type(string type) {
    if (type == "r_type")       return r_type;
    else if (type == "i_type") return i_type;
    else if (type == "s_type") return s_type;
    else if (type == "b_type") return b_type;
    else if (type == "u_type") return u_type;
    else if (type == "j_type") return j_type;
    else if (type == "exiting") return exiting;
}

void compiler::print_state() {
    auto print_value = [&](int num) {
        cout << "-------- Value --------\n";
        cout << "In decimal: " << num << "\n";
        cout << "In binary: " << to_binary(num) << "\n";
        cout << "In hexadecimal: " << to_hexa(num) << "\n";
        cout << "-----------------------\n";
    };
    cout << "\n\nProgram counter is at: -\n";
    print_value((program_counter + 1) * 4);
    cout << "\n\n-------------------- Registers ---------------------\n";
    for (int i = 0; i < 32; i++) {
        cout << "Value of " << 'x' << i << " is: - \n";
        print_value(registers[i]);
    }
    cout << "----------------------------------------------------\n\n";
    if (memory.empty()) return void(cout << "The memory is empty\n");
    cout << "\n-------------------- Memory ---------------------\n";
    cout << "In the memory: \n";
    for (auto& block : memory) {
        cout << "The address is: -\n";
        print_value(block.first);
        cout << "The value is: -";
        print_value(block.second);
    }
}