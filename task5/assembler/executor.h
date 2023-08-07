#ifndef EXECUTOR_H
#define EXECUTOR_H

/**
 * This method is the "orchestrator" for the whole project
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "statement_handler.h"

/**
 * @brief Main project entry point
*/
void run(char *[], size_t);
/**
 * @brief Checks for errors in the code, logs the error and stops the project if found
*/
int clear_if_error(Assembler *);
/**
 * @brief Validate the whole am file
*/
void syntax_validation(Assembler *);
/**
 * @brief Doing the first data collection
*/
void data_collection(Assembler *);
/**
 * @brief Doing the second data collection
*/
void build_data_for_files(Assembler *);
/**
 * @brief checks if the statemnet is a entry directive and marks all found symbols as entry
*/
void mark_entry(Assembler *, Statement *);
/**
 * @brief Find symbols usages in instructions, if the symbol is external - adds it to the table
*/
void mark_external(Assembler *, Statement *);
/**
 * @private
*/
void mark_external_if_needed(Assembler *, Operand *, int);
/**
 * @private
*/
void first_data_collection(Assembler *);
/**
 * @private
*/
void second_data_collection(Assembler *);
#endif /* EXECUTOR_H */
