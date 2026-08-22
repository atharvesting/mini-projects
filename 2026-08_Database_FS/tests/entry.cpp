#include <iostream>     // cout
#include <memory>       // unique_ptr
#include "src/repl.hpp"

int main() 
{
    std::unique_ptr<InputBuffer> input_buffer = new_input_buffer();
    while (true) 
    {
        print_prompt();
        read_input(input_buffer);

        if (input_buffer->buffer[0] == '-') 
        {
            switch (do_meta_command(input_buffer))
            {
            case (META_COMMAND_SUCCESS):
                continue;

            case (META_COMMAND_UNRECOGNIZED_COMMAND):
                std::cout << "Unrecognized command " << input_buffer->buffer << "\n";
                continue;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buffer, statement))
        {
        case (PREPARE_SUCCESS):
            break;
        
        case (PREPARE_UNRECOGNIZED_STATEMENT):
            std::cout << "Unrecognized keyword\n";
            continue;
        }

        execute_statement(statement);
        std::cout << "Executed!\n";
    }

    return 0;
}