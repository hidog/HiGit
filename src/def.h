#ifndef		DEF_H
#define		DEF_H


#include <string>
#include <stdio.h>
#include <QColor>
#include <QString>


#define		PRINT_ENUM(e)	printf("%s\n",#e)

#define ERRLOG(...) \
	{ \
		printf("[ERR] [%s] [%d] ", __FILE__, __LINE__); \
		printf(__VA_ARGS__); \
		printf("\n"); \
	}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// DbProj
struct  DbProj
{
    std::string		name;
    std::string		path;
	std::string		username;
	std::string		password;
};
// FileInfo
struct	FileInfo
{
	bool		is_dir;
	qint64		size;
	QString		name;
	QString		path;
	QString		status;
	QColor		font_color;
};
typedef QList<FileInfo>		FileInfoList;



#endif
