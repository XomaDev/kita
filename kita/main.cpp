#include <iostream>
#include <sstream>
#include <fstream>
#include "compiler/tokenizer.h"
#include "compiler/parser.h"

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
    string static_names = "/home/kumaraswamy/Documents/project_kita/kiita/config/static_names.txt";
    string source_file = "/home/kumaraswamy/Documents/project_kita/kiita/imagine/hi.kita";
    string compiled_file = "/home/kumaraswamy/Documents/project_kita/kiita/imagine/hi.ki";

    string source = read_string(source_file);
    cout << source << endl << endl;
    auto tokenizer = new class tokenizer(static_names, source);
    tokenizer->load_names();

    auto tokens = tokenizer->tokenize();
    delete tokenizer;

    cout << endl << endl;

    auto parser = new class parser(std::move(tokens));
    parser->parse();

    auto dump = new class dump(compiled_file, std::move(parser->expressions));

    delete parser;
    delete dump;
    return 0;
}
