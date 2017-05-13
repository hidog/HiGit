#ifndef LOG_MODEL_H
#define LOG_MODEL_H


#include <QAbstractTableModel>
#include <QString>
#include "../src/git_cmd/git_log.h"
#include <QVector>


/*******************************************************************
	LogModel
********************************************************************/
class LogModel : public QAbstractTableModel
{
    Q_OBJECT

public:

	LogModel( QObject *parent = 0 );
	~LogModel();

    void			set_root_path( QString path );
    void			init_log_view();

	QString			get_title( int row );
	QString			get_commit( int row );

	QModelIndex		get_default_index();

	int				rowCount( const QModelIndex &parent = QModelIndex() ) const ;
	int				columnCount( const QModelIndex &parent = QModelIndex() ) const ;
	QVariant		data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;
	QVariant		headerData( int section, Qt::Orientation orientation, int role ) const override;

	QVariant		text_data( const QModelIndex &index, int role ) const;
	QVariant		graph_data( const QModelIndex &index, int role ) const;

	void			draw_git_log_graph( GitLineList &line_list, QList<int> &node_list, int node_size );

public slots:
	

signals:
	void	refresh_signal();


private:
	QString				root_path;    
	QStringList			head_list;
	LogDataList			log_list;
	GitLineList			line_list;

	QVector<QImage>		graph_list;	
    
};


#endif