#ifndef FILE_MODEL_H
#define FILE_MODEL_H



#include <QAbstractTableModel>
#include <QString>
#include <QDir>


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

	QString			get_extension( QString filename ) const;
	QFileInfoList	get_file_list();

	int				get_header_count();

	int			    rowCount( const QModelIndex &parent = QModelIndex() ) const ;
	int				columnCount( const QModelIndex &parent = QModelIndex() ) const ;
	QVariant		data( const QModelIndex &index, int role = Qt::DisplayRole ) const ;
	QVariant		headerData( int section, Qt::Orientation orientation, int role ) const ;
    
	QVariant		text_data( const QModelIndex &index, int role ) const;
	QVariant		icon_data( const QModelIndex &index, int role ) const;
	QVariant		get_font_color( const QModelIndex &index, int role ) const;

	QString			get_filesize_str( qint64 size ) const;


public slots:
	void	enter_dir_slot( const QModelIndex &index );


signals:
	void	refresh_signal();


private:
    
    QStringList     head_list;
    QString         root_path;
    
    QDir            dir;
    QFileInfoList   file_list;

	QModelIndex		last_index;
    
};


#endif