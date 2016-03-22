#ifndef FILE_MODEL_H
#define FILE_MODEL_H



#include <QAbstractTableModel>
#include <QString>
#include <QDir>
#include <QColor>
#include "../src/def.h"
#include "../src/git_cmd/git_status.h"



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class declare ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace boost{
	class	thread;
} // end namespace boost


/*******************************************************************
	FileWidget
********************************************************************/
class FileModel : public QAbstractTableModel
{
    Q_OBJECT

public:

	FileModel( QObject *parent = 0 );
	~FileModel();

    void    set_root_path( QString path );
    void    init_file_list();

	void	refresh_view();
	void	refresh_singal( int row );


	QString			get_extension( QString filename ) const;
	void			get_file_list();

	int				get_header_count();

	int			    rowCount( const QModelIndex &parent = QModelIndex() ) const ;
	int				columnCount( const QModelIndex &parent = QModelIndex() ) const ;
	QVariant		data( const QModelIndex &index, int role = Qt::DisplayRole ) const ;
	QVariant		headerData( int section, Qt::Orientation orientation, int role ) const ;
    
	QVariant		text_data( const QModelIndex &index, int role ) const;
	QVariant		icon_data( const QModelIndex &index, int role ) const;
	QVariant		get_font_color( const QModelIndex &index, int role ) const;

	QString			get_filesize_str( qint64 size ) const;
	void			update_file_status();



public slots:
	void	enter_dir_slot( const QModelIndex &index );
	void	path_change_slot( const QString &new_path );

signals:
	void	refresh_signal();
	void	path_change_signal(QString);


private:
    
	bool			file_loop;

    QStringList     head_list;
    QString         root_path;
    
    QDir            dir;
    QFileInfoList   file_list;
	QStatusVec		status_vec;

	QModelIndex		last_index;
    
	boost::thread	*thr;

};


#endif