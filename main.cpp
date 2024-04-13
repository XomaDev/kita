#include <iostream>
#include <sstream>
#include <fstream>
#include "tokenizer.h"

using namespace std;

string read_string(string &file_path) {
    ifstream file(file_path);
    if (!file.is_open()) {
        throw runtime_error("Unable to open file path '" + file_path + "'");
    }
    stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

int main() {
    string static_names = "/home/kumaraswamy/Documents/kita/static_names.txt";
    string source_file = "/home/kumaraswamy/Documents/kita/playground/hi.kita";

    auto tokenizer = new class tokenizer(static_names, read_string(source_file));
    tokenizer->load_names();
    tokenizer->tokenize();

    return 0;
}
