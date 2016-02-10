#ifndef LOG_MODEL_H
#define LOG_MODEL_H


#include <QAbstractTableModel>
#include <QString>



/*******************************************************************
	LogModel
********************************************************************/
class LogModel : public QAbstractTableModel
{
    Q_OBJECT

public:

	LogModel( QObject *parent = 0 );
	~LogModel();

    void    set_root_path( QString path );
    void    init_log_view();


	int				rowCount( const QModelIndex &parent = QModelIndex() ) const ;
	int				columnCount( const QModelIndex &parent = QModelIndex() ) const ;
	QVariant		data( const QModelIndex &index, int role = Qt::DisplayRole ) const ;
	QVariant		headerData( int section, Qt::Orientation orientation, int role ) const ;


public slots:
	

signals:


private:
	QString		root_path;
    

    
};


#endif