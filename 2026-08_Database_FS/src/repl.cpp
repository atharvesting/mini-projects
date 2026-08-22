#include <memory>       // unique_ptr, make_unique
#include <iostream>     // cout
#include <string>
#include "repl.hpp"


typedef unsigned long size_t;

char prompt[] = "db";

using BufferPtr = std::unique_ptr<InputBuffer>;

BufferPtr new_input_buffer() {
    auto input_buffer = std::make_unique<InputBuffer>();
    input_buffer->buffer;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;
    return input_buffer;
}

void print_prompt() {
    std::cout << prompt << " > ";
}

void read_input(BufferPtr& input_buffer) {
    if (!std::getline(std::cin, input_buffer->buffer)) 
    {
        std::cout << "Error reading input\n";
        exit(EXIT_FAILURE);
    }

    input_buffer->input_length = input_buffer->buffer.length();
    input_buffer->buffer[input_buffer->input_length] = 0;
}

MetaCommandResult do_meta_command(BufferPtr& input_buffer) {
    if (input_buffer->buffer == "-zzz") exit(EXIT_SUCCESS);
    else return META_COMMAND_UNRECOGNIZED_COMMAND;
}

PrepareResult prepare_statement(BufferPtr& input_buffer, Statement& statement) {
    if (input_buffer->buffer.substr(0, 6) == "insert") 
    {
        statement.type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    else if (input_buffer->buffer == "select")
    {
        statement.type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement& statement) {
    switch (statement.type) {
        case (STATEMENT_INSERT):
            std::cout << "Insert operation...\n";
            break;
        case (STATEMENT_SELECT):
            std::cout << "Select operation...\n";
            break;
    }
}