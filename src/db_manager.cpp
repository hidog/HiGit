#include "db_manager.h"

#include "sqlite3.h"
#include "def.h"

#include <iostream>
#include <cassert>


/*******************************************************************
	DbManager
********************************************************************/
DbManager::DbManager()
{
	db	=	NULL;
	load_main_db();
}


	

/*******************************************************************
	DbManager
********************************************************************/
DbManager::~DbManager()
{}





/*********************************************************************
	load_main_db
**********************************************************************/
void	DbManager::load_main_db()
{
	char	str[HIGIT_DB_BUF_SIZE];
	char	*err_msg;
	int		rc;

	// open main db
	sprintf( str, "HiGit.db" );
	rc	=	sqlite3_open( str, &db );
	if( rc != SQLITE_OK )
		ERRLOG("open db fail.")

	// create main_table
	sprintf( str, "CREATE TABLE IF NOT EXISTS GIT_PROJ_TABLE(PATH TEXT UNIQUE NOT NULL,NAME TEXT NOT NULL, USERNAME TEXT, PASSWORD TEXT)" );
	rc	=	sqlite3_exec( db, str, 0, 0, &err_msg );
	if( rc != SQLITE_OK )
		ERRLOG("create table fail. msg = %s", err_msg )
}




/*********************************************************************
	add_proj
**********************************************************************/
bool	DbManager::add_proj( string path, string name, string username, string password )
{
	int		i,	rc;
	char	*err_msg;	
	char	str[HIGIT_DB_BUF_SIZE];

	// 
	sprintf( str, "INSERT OR IGNORE INTO GIT_PROJ_TABLE(PATH,NAME,USERNAME,PASSWORD) VALUES('%s','%s','%s','%s')" 
			,path.c_str(), name.c_str(), username.c_str(), password.c_str() );

	rc	=	sqlite3_exec( db, str, 0, 0, &err_msg );
	if( rc != SQLITE_OK )
	{
		ERRLOG("add proj fail. msg = %s", err_msg)
		return	false;
	}
	else
		return	true;
}





/*********************************************************************
	get_all_proj
**********************************************************************/
bcListDbProj    DbManager::get_all_proj()
{
    bcListDbProj    proj_list;
    DbProj          data;
    
    proj_list.clear();
    
    int     rc;
    char    *err_msg    =   NULL;
    char    str[HIGIT_DB_BUF_SIZE];
    
    //
    sprintf( str, "SELECT PATH,NAME,USERNAME,PASSWORD FROM GIT_PROJ_TABLE" );
    
    sqlite3_stmt    *stmt;
    sqlite3_prepare_v2( db, str, strlen(str)+1, &stmt, 0 );
    
    while(true)
    {
        rc  =   sqlite3_step(stmt);
        if( rc == SQLITE_ROW )
        {
            data.path   =   (char*)sqlite3_column_text( stmt, 0 );
            data.name   =   (char*)sqlite3_column_text( stmt, 1 );
            proj_list.push_back(data);
        }
        else if( rc == SQLITE_DONE )
            break;
        else
            ERRLOG("db error. msg = %s", err_msg );
    }
    
    sqlite3_finalize(stmt);
    return  proj_list;
}



/*********************************************************************
	is_exist_path
**********************************************************************/
bool	DbManager::is_exist_path( string path )
{
	int		rc;
	char	str[HIGIT_DB_BUF_SIZE];
	int		result;

	sprintf( str, "SELECT COUNT(*) FROM GIT_PROJ_TABLE WHERE PATH='%s'", path.c_str() );

	//
	sqlite3_stmt	*stmt;
	sqlite3_prepare_v2( db, str, strlen(str)+1, &stmt, 0 );

	rc	=	sqlite3_step(stmt);
	if( rc == SQLITE_ROW )
		result	=	sqlite3_column_int( stmt, 0 );
	else if( rc == SQLITE_DONE )
		result	=	-1;
	else
		ERRLOG("sqlite query fail.")

	sqlite3_finalize(stmt);

	if( result > 0 )
		return	true;
	else
		return	false;
}
