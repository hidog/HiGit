#ifndef GIT_STATUS_H
#define GIT_STATUS_H

#include <QString>
#include <QColor>
#include <QFileInfo>


#include "git_base.h"
#include "../def.h"

#ifndef Q_MOC_RUN
#	include <boost/function.hpp>
#endif


#define GIT_STATUS_TRACKED		"tracked"
#define GIT_STATUS_MODIFY		"modified"
#define GIT_STATUS_ADDED		"added"
#define GIT_STATUS_UNTRACKED	"untracked"
#define GIT_STATUS_DELETED		"deleted"

#define	HANDLE_FUNC_t	boost::function< void( QList<DataType>&, STATUS_TYPE, const QString, QByteArray )>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum	STATUS_TYPE
{
	STATUS_DEFAULT		=	0,
	STATUS_TRACKED		=	1,
	STATUS_MODIFIED		=	2,
	STATUS_ADDED		=	4,
	STATUS_UNTRACKED	=	8,
	STATUS_DELETED		=	16,
};

#define		STATUS_ALL	(STATUS_TRACKED|STATUS_MODIFIED|STATUS_ADDED|STATUS_UNTRACKED|STATUS_DELETED)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct FileStatus
{
    QString     name;
	QString		status;
	QColor		color;
};
typedef QVector<FileStatus>		StatusVec;
typedef QList<FileStatus>		StatusList;

/*******************************************************************
	GitStatus
********************************************************************/
class   GitStatus : public GitBase
{
public:

    GitStatus( QObject *parent = 0 );
    ~GitStatus();
    

	QString			get_file_status( QString path, QString filename );
	QColor			get_file_color( QString path, QString filename );

	//QStringList		get_delete_files( QString path );
	//void			get_delete_files_func( QList<QString> &list, STATUS_TYPE type, const QString path, QByteArray filename );

	QFileInfoList	get_untracked_files( QString path );
	void			get_untracked_files_func( QList<QFileInfo> &list, STATUS_TYPE type, const QString path, QByteArray filename );

	StatusList		get_modify_files( QString path );
	void			get_modify_files_func( QList<FileStatus> &list, STATUS_TYPE type, const QString path, QByteArray filename );

	static QColor	get_status_color( const QString& status );
	static QColor	get_status_color( const STATUS_TYPE status );


	//QStatusVec  	get_all_status( QString path );
    //void			get_all_status_parser( QStatusVec &vec, const QByteArray &str );
    
private:

	template<typename DataType>
	QList<DataType>		git_status_list( QString path, int filter, HANDLE_FUNC_t handle_func );

	STATUS_TYPE		parse_short_status( const QByteArray &status );

};





#endif
