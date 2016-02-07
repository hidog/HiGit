#ifndef		DEF_H
#define		DEF_H


#include <string>
#include <stdio.h>



#define		PRINT_ENUM(e)	printf("%s\n",#e)

#define ERRLOG(...) \
	{ \
		printf("[ERR] [%s] [%d] ", __FILE__, __LINE__); \
		printf(__VA_ARGS__); \
		printf("\n"); \
	}


// ~~~~~~~~~~~~~~~~~~~~~ struct ~~~~~~~~~~~~~~~~~~~~~
struct  DbProj
{
    std::string		name;
    std::string		path;
	std::string		username;
	std::string		password;
};


#endif
