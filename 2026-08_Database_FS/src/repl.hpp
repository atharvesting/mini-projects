#include <memory>       // unique_ptr
#include <string>       // string

typedef struct {
    std::string buffer;
    size_t buffer_length;
    int32_t input_length;
} InputBuffer;

std::unique_ptr<InputBuffer> new_input_buffer();

void print_prompt();

void read_input(std::unique_ptr<InputBuffer>& input_buffer);