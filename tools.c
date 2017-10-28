/*
 * tools.c
 *
 * Description: Address Book functions
 *
 */

#include "tools.h"

struct address_book *addr_book = NULL;

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

void data_load (char *datfile)
{
    FILE *fd;
    char buffer[STR_LENGTH * 4 + 4];
    char fname[STR_LENGTH];
    char mname[STR_LENGTH];
    char lname[STR_LENGTH];
    char email[STR_LENGTH];

    if ((fd = fopen (datfile, "r")) == NULL) {
	fprintf(stderr, "Error: Data file is bad or missing: %s\n", datfile);
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

void data_save (char *datfile)
{
    FILE *fd;
    struct address_book *node = NULL;
    struct address_book *p = addr_book;
    
    if ((fd = fopen (datfile, "w")) == NULL) {
	fprintf(stderr, "Error: Data file is bad or missing: %s\n", datfile);
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

