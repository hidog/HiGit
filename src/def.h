#ifndef		DEF_H
#define		DEF_H


#include <string>
#include <stdio.h>
#include <QString>
#include <QColor>


#define		PRINT_ENUM(e)	printf("%s\n",#e)

#define ERRLOG(...) \
	{ \
		printf("[ERR] [%s] [%d] ", __FILE__, __LINE__); \
		printf(__VA_ARGS__); \
		printf("\n"); \
		assert(0); \
	}

#define GET_MACRO(type,name) type get_##name() const { return name; }
#define SET_MACRO(type,name) void set_##name( type n ) { name = n; }


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// DbProj
struct  DbProj
{
    std::string		name;
    std::string		path;
	std::string		username;
	std::string		password;
};

// FileStatus
struct FileStatus
{
    QString     name;
	QString		status;
	QColor		color;
};
typedef QVector<FileStatus>		FileStatusVec;
typedef QList<FileStatus>		FileStatusList;

#endif
