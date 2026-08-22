#pragma once
#include <memory>       // unique_ptr
#include <string>       // string

typedef struct {
    std::string buffer;
    size_t buffer_length;
    int32_t input_length;
} InputBuffer;

using BufferPtr = std::unique_ptr<InputBuffer>;

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef struct {
    StatementType type;
} Statement;

BufferPtr new_input_buffer();
void print_prompt();
void read_input(BufferPtr& input_buffer);

MetaCommandResult do_meta_command(BufferPtr& input_buffer);
PrepareResult prepare_statement(BufferPtr& input_buffer, Statement& statement);
void execute_statement(Statement& statement);