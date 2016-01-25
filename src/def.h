#ifndef		DEF_H
#define		DEF_H


#define		PRINT_ENUM(e)	printf("%s\n",#e)

#define ERRLOG(...) \
	{ \
		printf("[ERR] [%s] [%d] ", __FILE__, __LINE__); \
		printf(__VA_ARGS__); \
		printf("\n"); \
	}


#endif