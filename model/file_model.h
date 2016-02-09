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

	int     rowCount( const QModelIndex &parent = QModelIndex() ) const ;
	int     columnCount( const QModelIndex &parent = QModelIndex() ) const ;

    void    set_root_path( QString path );
    void    init_file_list();

	void	refresh_view();

	QString			get_extension( QString filename ) const;
	QVariant		data(const QModelIndex &index, int role = Qt::DisplayRole) const ;
	QVariant		headerData(int section, Qt::Orientation orientation, int role) const ;
    
	QFileInfoList	get_file_list();

public slots:
	void	enter_dir_slot( const QModelIndex &index );


signals:
	void	refresh_signal();


private:
    
    QStringList     head_list;
    QString         root_path;
    
    QDir            dir;
    QFileInfoList   file_list;

	QStringList		delete_file_list;

	QModelIndex		last_index;
    
};


#endif