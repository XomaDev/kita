#include <iostream>
#include <sstream>
#include <fstream>
#include <id3/globals.h>
#include <chrono>
#include "compiler/tokenizer.h"
#include "compiler/parser.h"
#include "runtime/runtime.h"

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

unique_ptr<uchar[]> read_bytes(const string& path, long& file_size) {
    ifstream file(path, ios::binary | ios::ate);
    if (!file) {
        throw runtime_error("Could not open file " + path);
    }
    file_size = file.tellg();
    file.seekg(0, ios::beg);

    // auto *buffer = new uchar[file_size];
    auto buffer = make_unique<uchar[]>(file_size);

    if (!file.read(reinterpret_cast<char *>(buffer.get()), file_size)) {
        throw runtime_error("Error reading file " + path);
    }
    return buffer;
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

    auto parsed = std::move(parser->expressions);
    for (auto &element : parsed) {
        cout << element->to_string() << endl;
    }
    auto dump = new class dump(compiled_file, std::move(parsed));

    delete parser;
    delete dump;

    long file_size;
    auto bytes = read_bytes(compiled_file, file_size);

    auto runtime = new class runtime(std::move(bytes), file_size);
    auto then = std::chrono::duration_cast<chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    runtime->run();
    auto now = std::chrono::duration_cast<chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    cout << "Time taken: " << to_string(now - then) + " ns" << endl;
    delete runtime;
    return 0;
}
