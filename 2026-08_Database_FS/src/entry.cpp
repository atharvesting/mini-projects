#include <iostream>     // cout
#include <memory>       // unique_ptr
#include "repl.hpp"

int main() 
{
    std::unique_ptr<InputBuffer> input_buffer = new_input_buffer();
    while (true) 
    {
        print_prompt();
        read_input(input_buffer);

        if (input_buffer->buffer == "/zzz") 
        {
            std::cout << "Database connection terminated!" << std::endl;
            return 0;
        } 
        else 
        {
            std::cout << "Unrecognized command " << input_buffer->buffer << "\n";
        }
    }

    return 0;
}