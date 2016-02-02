#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <string>



#define HIGIT_DB_BUF_SIZE	1000


using namespace std;



// ~~~~~~~~~~~~~~~~~~~~~ class declare ~~~~~~~~~~~~~~~~~~~~~
struct	sqlite3;


/*******************************************************************
	DbManager
********************************************************************/
class	DbManager
{
public:
	DbManager();
	~DbManager();

	bool	is_exist_path( string path );
	bool	add_proj( string path, string name, string username = "", string password = "" );

private:
	void	load_main_db();

	sqlite3		*db;
};



#endif