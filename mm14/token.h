#ifndef _TOKEN_H_
#define _TOKEN_H_

typedef enum { tokLabel, tokCmd, tokOp, tokDec, tokNL, tokError, tokNum, tokString } tokenType ;

typedef struct token_t *token;

/* A simple storag class with setters and getters */
token newToken( char *text, tokenType type );

void deleteToken( token tok );

tokenType getTokType( token tok );

/* The char* returned points to the token object's internal memory and it
 * might go away, so if you want to do anything with the string copy it, don't
 * store a pointer to it.
 */
char *getTokenString( token tok );

#endif
