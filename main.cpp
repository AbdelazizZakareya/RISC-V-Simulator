#include "compiler.h"

int main() {
    int starting_address;
    // User input
    cout << "Enter the starting address: ";
    cin >> starting_address;

    cout << "Please enter the name of the instructions file: ";
    string file_name; cin >> file_name;
    ifstream instructions_file(file_name);
    compiler c(--starting_address); // 1-based indexing, decrement
    c.take_input(instructions_file);
    c.compile_all();
}