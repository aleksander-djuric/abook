/*
 * tools.h
 *
 * Description: Address Book functions
 *
 */

#ifndef _TOOLS_H
#define _TOOLS_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <ctype.h>

#define STR_LENGTH 256

struct address_book
{
    char fname[STR_LENGTH];
    char mname[STR_LENGTH];
    char lname[STR_LENGTH];
    char email[STR_LENGTH];
    struct address_book *next;
};

int add_record(char *fname, char *mname, char *lname, char *email);
int del_record(struct address_book *node);
struct address_book * search_record(char *fname, char *mname, char *lname, char *email);
void data_load(char *datfile);
void data_save(char *datfile);
void data_print(void);
void free_book(void);
void term_mode (int mode, struct termios *term);
char * trim(char *str);

#endif // _TOOLS_H
