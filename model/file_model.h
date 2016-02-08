#ifndef FILE_MODEL_H
#define FILE_MODEL_H



#include <QAbstractTableModel>



/*******************************************************************
	FileWidget
********************************************************************/
class FileModel : public QAbstractTableModel
{
    Q_OBJECT

public:

	FileModel( QObject *parent = 0 );
	~FileModel();

	int rowCount( const QModelIndex &parent = QModelIndex() ) const ;
	int columnCount( const QModelIndex &parent = QModelIndex() ) const ;

	QVariant	data(const QModelIndex &index, int role = Qt::DisplayRole) const ;
	QVariant	headerData(int section, Qt::Orientation orientation, int role) const ;

private:

};


#endif