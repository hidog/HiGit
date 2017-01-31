#ifndef FILE_MODEL_H
#define FILE_MODEL_H



#include <QAbstractTableModel>
#include <QString>
#include <QDir>
#include <QColor>
#include "../src/def.h"
#include "../src/git_cmd/git_status.h"


#define	REFRESH_TIMEOUT		3000


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class declare ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace boost{
	class	thread;
} // end namespace boost

class	QTimer;



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

	void			get_file_list();

	int				get_header_count();

	int			    rowCount( const QModelIndex &parent = QModelIndex() ) const ;
	int				columnCount( const QModelIndex &parent = QModelIndex() ) const ;
	QVariant		data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;
	QVariant		headerData( int section, Qt::Orientation orientation, int role ) const override;
    
	QVariant		text_data( const QModelIndex &index, int role ) const;
	QVariant		icon_data( const QModelIndex &index, int role ) const;
	QVariant		get_font_color( const QModelIndex &index, int role ) const;

	void			update_file_status();

	QFileInfoList	get_add_selected_list( const QModelIndexList index_list ) const;

public slots:
	void	enter_dir_slot( const QModelIndex &index );
	void	path_change_slot( const QString &new_path );
	void	refresh_slot();

signals:
	void	refresh_signal();
	void	path_change_signal(QString);


private:
    
	bool			file_loop;
	bool			file_loop_fisish;

    QStringList     head_list;
    QString         root_path;
    
    QDir            dir;
    QFileInfoList   file_list;
	FileStatusVec		status_vec;

	QModelIndex		last_index;
    
	boost::thread	*thr;

	QTimer			*timer;

};


#endif