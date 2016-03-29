#ifndef TOOLS_H
#define TOOLS_H

#include <QString>


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool		qstring_contain_full_width( const QString &str );
QString		get_extension( QString filename );
QString		get_filesize_str( qint64 size );



#endif