#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include "../src/def.h"
#include <string>
#include <boost/container/list.hpp>


#define HIGIT_DB_BUF_SIZE	1000



using namespace std;



// ~~~~~~~~~~~~~~~~~~~~~ class declare ~~~~~~~~~~~~~~~~~~~~~
struct	sqlite3;





// ~~~~~~~~~~~~~~~~~~~~~ typedef ~~~~~~~~~~~~~~~~~~~~~
typedef boost::container::list<DbProj>  bcListDbProj;




/*******************************************************************
	DbManager
********************************************************************/
class	DbManager
{
public:
	DbManager();
	~DbManager();

	bool	is_exist_proj( DbProj proj );
	bool	add_proj( DbProj proj );
    
    void    delete_proj( DbProj proj );

    bcListDbProj    get_all_proj();
    
    
private:
	void	load_main_db();

	sqlite3		*db;
};



#endif