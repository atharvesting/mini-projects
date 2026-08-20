#include <memory>       // unique_ptr
#include <iostream>     // cout
#include "repl.hpp"


typedef unsigned long size_t;

char prompt[] = "db";

std::unique_ptr<InputBuffer> new_input_buffer() {
    auto input_buffer = std::make_unique<InputBuffer>();
    input_buffer->buffer;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;
    return input_buffer;
}

void print_prompt() {
    std::cout << prompt << " > ";
}

void read_input(std::unique_ptr<InputBuffer>& input_buffer) {
    if (!std::getline(std::cin, input_buffer->buffer)) {
        std::cout << "Error reading input\n";
        exit(EXIT_FAILURE);
    }

    input_buffer->input_length = input_buffer->buffer.length();
    input_buffer->buffer[input_buffer->input_length] = 0;
}
