/*
 * abook.c
 *
 * Description: Address Book Example
 *
 */
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <ctype.h>

#define DAT_FILENAME "abook.dat"
#define STR_LENGTH 256

struct address_book
{
    char fname[STR_LENGTH];
    char mname[STR_LENGTH];
    char lname[STR_LENGTH];
    char email[STR_LENGTH];
    struct address_book *next;
};

struct address_book *addr_book = NULL;

int add_record(char *fname, char *mname, char *lname, char *email);
int del_record(struct address_book *node);
struct address_book * search_record(char *fname, char *mname, char *lname, char *email);
void data_load(void);
void data_print(void);
void data_save(void);
void free_book(void);
void term_mode (int mode, struct termios *term);
char * trim(char *str);

int main(int argc, char *argv[])
{
    struct termios term;
    struct address_book *s = NULL;
    char str1[STR_LENGTH];
    char str2[STR_LENGTH];
    char str3[STR_LENGTH];
    char str4[STR_LENGTH];
    char cmd;

    printf("------------------- Address Book Example ----------------------\n");
    printf("--- L (List) * A (Add) * S (Search) * D (Delete) * Q (Exit) ---\n");
    printf("---------------------------------------------------------------\n");

    data_load();

    tcgetattr(fileno(stdin), &term);
    term_mode(0, &term);

    do {
	cmd = getchar();

	switch (cmd) {
	case 108:	// 'L'
	    printf("----------------------- Address Book --------------------------\n");
	    data_print();
	    break;
	case 97:	// 'A'
	    printf("------------------------ Add Record ---------------------------\n");
	    term_mode(1, &term);
	    printf("\rLast Name: ");
	    if (!fgets(str3, STR_LENGTH-1, stdin)) *str3 = '\0';
	    printf("\rFirst Name: ");
	    if (!fgets(str1, STR_LENGTH-1, stdin)) *str1 = '\0';
	    printf("\rMiddle Name: ");
	    if (!fgets(str2, STR_LENGTH-1, stdin)) *str2 = '\0';
	    printf("\rE-mail: ");
	    if (!fgets(str4, STR_LENGTH-1, stdin)) *str4 = '\0';
	    if (add_record(trim(str1), trim(str2), trim(str3), trim(str4)))
		fprintf(stderr, "Error: Not enough memory\n");
	    else
		printf("\rRecord added.\n");
	    term_mode(0, &term);
	    break;
	case 115:	// 'S'
	    printf("----------------------- Search Record -------------------------\n");
	    term_mode(1, &term);
	    printf("\rSearch Last Name: ");
	    if (!fgets(str3, STR_LENGTH-1, stdin)) *str3 = '\0';
	    printf("\rSearch First Name: ");
	    if (!fgets(str1, STR_LENGTH-1, stdin)) *str1 = '\0';
	    printf("\rSearch Middle Name: ");
	    if (!fgets(str2, STR_LENGTH-1, stdin)) *str2 = '\0';
	    printf("\rSearch E-mail: ");
	    if (!fgets(str4, STR_LENGTH-1, stdin)) *str4 = '\0';
	    s = search_record(trim(str1), trim(str2), trim(str3), trim(str4));
	    if (s) {
		printf("---------------------- Search Results -------------------------\n");
		printf("%s\t%s\t%s\t%s\n", s->fname, s->mname, s->lname, s->email);
	    } else {
		printf("\rNothing found.\n");
	    }
	    term_mode(0, &term);
	    break;
	case 100:	// 'D'
	    if (s && del_record(s)) {
		s = NULL;
		printf("\rRecord removed.\n");
	    } else {
		printf("\rPlease use 'Search' to select the record to remove.\n");
	    }
	    break;
	}
    } while (cmd != 113); // 113 = 'Q'

    term_mode(1, &term);

    (void) data_save();

    return 0;
}

///////////////////////////////////////////////////////////////////

char * trim(char *str)
{
    char *end;

    while (isspace((unsigned char)*str)) str++;
    if(*str == 0) return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end+1) = 0;
    return str;
}

void term_mode (int mode, struct termios *term)
{
    if (mode) {
	term->c_lflag |= ICANON;
	term->c_lflag |= ECHO;
	tcsetattr(fileno(stdin), TCSANOW, term);
    } else {
	term->c_lflag &= ~ICANON;
	term->c_lflag &= ~ECHO;
	tcsetattr(fileno(stdin), TCSANOW, term);
    }
}

int add_record (char *fname, char *mname, char *lname, char *email)
{
    struct address_book *node;

    node = (struct address_book *) malloc (sizeof (struct address_book));
    if (!node) return 1;

    strncpy(node->fname, fname, STR_LENGTH-1);
    strncpy(node->mname, mname, STR_LENGTH-1);
    strncpy(node->lname, lname, STR_LENGTH-1);
    strncpy(node->email, email, STR_LENGTH-1);

    node->next = addr_book;
    addr_book = node;

    return 0;
}

int del_record (struct address_book *node)
{
    struct address_book *p = addr_book;
    struct address_book *prev = NULL;
    int ret = 0;

    while (addr_book)
    {
	if (addr_book == node) {
	    ret = 1;
	    if (prev) {
		prev->next = addr_book->next;
		free(addr_book);
		addr_book = prev->next;
	    } else {
    		p = addr_book->next;
    		free(addr_book);
    		addr_book = p;
	    }
	} else {
	    prev = addr_book;
	    addr_book = addr_book->next;
	}
    }
    
    addr_book = p;

    return ret;
}

struct address_book * search_record (char *fname, char *mname, char *lname, char *email)
{
    int found = 0;
    struct address_book *node = NULL;
    struct address_book *p = addr_book;

    while (addr_book)
    {
	node = addr_book;

	if ((*fname != '\0' && strcasestr(node->fname, fname)) ||
	    (*mname != '\0' && strcasestr(node->mname, mname)) ||
	    (*lname != '\0' && strcasestr(node->lname, lname)) ||
	    (*email != '\0' && strcasestr(node->email, email))) {
		found = 1;
		break;
	}

	addr_book = addr_book->next;
    }
    
    addr_book = p;
    
    return (found)? node : NULL;
}

void data_load (void)
{
    FILE *fd;
    char buffer[STR_LENGTH * 4 + 4];
    char fname[STR_LENGTH];
    char mname[STR_LENGTH];
    char lname[STR_LENGTH];
    char email[STR_LENGTH];

    if ((fd = fopen (DAT_FILENAME, "r")) == NULL) {
	fprintf(stderr, "Error: Data file is bad or missing: %s\n", DAT_FILENAME);
	return;
    }

    while (fgets (buffer, sizeof(buffer) - 1, fd)) {
	if (sscanf(buffer, "%[^;];%[^;];%[^;];%s", fname, mname, lname, email) == 4) {
	    if (add_record(fname, mname, lname, email)) {
		fprintf(stderr, "Error: Not enough memory\n");
		(void) fclose (fd);
		return;
	    }
	} else fprintf(stderr, "Warning: Incorrect data line: %s\n", buffer);
    }

    (void) fclose (fd);
}

void data_print (void)
{
    struct address_book *node = NULL;
    struct address_book *p = addr_book;

    while (addr_book)
    {
	node = addr_book;
	addr_book = addr_book->next;
	printf("%s\t%s\t%s\t%s\n", node->fname, node->mname, node->lname, node->email);
    }
    
    addr_book = p;
}

void data_save (void)
{
    FILE *fd;
    struct address_book *node = NULL;
    struct address_book *p = addr_book;
    
    if ((fd = fopen (DAT_FILENAME, "w")) == NULL) {
	fprintf(stderr, "Error: Data file is bad or missing: %s\n", DAT_FILENAME);
	return;
    }
    
    while (addr_book)
    {
	node = addr_book;
	addr_book = addr_book->next;
	fprintf(fd, "%s;%s;%s;%s\n", node->fname, node->mname, node->lname, node->email);
    }
    
    addr_book = p;
    (void) fclose(fd);
}

void free_book (void)
{
    struct address_book *node = NULL;

    while (addr_book)
    {
	node = addr_book;
	addr_book = addr_book->next;
	free(node);
    }
}

/*
end
*/
