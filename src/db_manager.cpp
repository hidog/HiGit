#include "db_manager.h"

#include "sqlite3.h"
#include "def.h"

#include <string.h>
#include <iostream>
#include <cassert>
#include <list>

#include <QDir>
#include <QDebug>


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
{
	int		res		=	sqlite3_close_v2(db);
	if( res != SQLITE_OK )
		ERRLOG("sqlite close fail.")
}


/*******************************************************************
	DbManager
********************************************************************/
int		DbManager::generate_id()
{
    int     rc,	n;
    char    *err_msg    =   NULL;
    char    str[HIGIT_DB_BUF_SIZE];

	list<int>	list;
    
    //
    sprintf( str, "SELECT NID FROM GIT_PROJ_TABLE ORDER BY NID ASC" );
    
    sqlite3_stmt    *stmt;
    sqlite3_prepare_v2( db, str, strlen(str)+1, &stmt, 0 );
    
    while(true)
    {
        rc  =   sqlite3_step(stmt);
        if( rc == SQLITE_ROW )
        {
			n	=	sqlite3_column_int( stmt, 0 );
			list.push_back(n);
        }
        else if( rc == SQLITE_DONE )
            break;
        else
            ERRLOG("db error. msg = %s", err_msg );
    }
    
    sqlite3_finalize(stmt);

	//
	std::list<int>::iterator	itr;
	n	=	1;
	for( itr = list.begin(); itr != list.end(); ++itr )
	{
		if( n == *itr )
			n++;
	}

    return  n;
}


/*********************************************************************
	load_main_db
**********************************************************************/
void	DbManager::load_main_db()
{
	char	str[HIGIT_DB_BUF_SIZE];
	char	*err_msg;
	int		rc;

	// open main db
#ifndef _WIN32
    sprintf( str, "%s/.HiGit.db", QDir::homePath().toStdString().c_str() );
#else
    sprintf( str, "%s/HiGit.db", QDir::homePath().toStdString().c_str() );
#endif
    
	rc	=	sqlite3_open( str, &db );
	if( rc != SQLITE_OK )
		ERRLOG("open db fail.")

	// create main_table
	sprintf( str, "CREATE TABLE IF NOT EXISTS GIT_PROJ_TABLE(NID UNIQUE,PATH TEXT UNIQUE NOT NULL,NAME TEXT NOT NULL, USERNAME TEXT, PASSWORD TEXT, ORDER_VALUE INTEGER)" );
	rc	=	sqlite3_exec( db, str, 0, 0, &err_msg );
	if( rc != SQLITE_OK )
		ERRLOG("create table fail. msg = %s", err_msg )
}




/*********************************************************************
	add_proj
**********************************************************************/
bool	DbManager::add_proj( DbProj proj, int order )
{
	int		rc;
	char	*err_msg;	
	char	str[HIGIT_DB_BUF_SIZE];

	int		id	=	generate_id();

	// 
	sprintf( str, "INSERT OR IGNORE INTO GIT_PROJ_TABLE(NID,PATH,NAME,USERNAME,PASSWORD,ORDER_VALUE) VALUES(%d,'%s','%s','%s','%s',%d)" 
			, id, proj.path.c_str(), proj.name.c_str(), proj.username.c_str(), proj.password.c_str(), order );

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
	delete_proj
**********************************************************************/
void    DbManager::delete_proj( DbProj proj )
{
    int     rc;
    char    str[HIGIT_DB_BUF_SIZE];
    char    *err_msg;
    
    sprintf( str, "DELETE FROM GIT_PROJ_TABLE WHERE PATH='%s'", proj.path.c_str() );

    rc  =   sqlite3_exec( db, str, 0, 0, &err_msg );
    if( rc != SQLITE_OK )
        ERRLOG("delete proj fail, msg = %s", err_msg );
}

/*********************************************************************
	proj_count
**********************************************************************/
int		DbManager::proj_count()
{
	int		rc;
	int		count		=	0;
	char	*err_msg	=	NULL;
	char	str[HIGIT_DB_BUF_SIZE];

	// 
	sprintf( str, "SELECT COUNT(*) FROM GIT_PROJ_TABLE" );

	sqlite3_stmt    *stmt;
    sqlite3_prepare_v2( db, str, strlen(str)+1, &stmt, 0 );

	rc	=	sqlite3_step(stmt);
	if( rc == SQLITE_ROW )
		count	=	sqlite3_column_int( stmt, 0 );
	else
	{
		count	=	0;
		ERRLOG("count fail.")
	}

	return	count;
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
    sprintf( str, "SELECT PATH,NAME,USERNAME,PASSWORD FROM GIT_PROJ_TABLE ORDER BY ORDER_VALUE ASC" );
    
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
bool	DbManager::is_exist_proj( DbProj proj )
{
	int		rc;
	char	str[HIGIT_DB_BUF_SIZE];
	int		result;

	sprintf( str, "SELECT COUNT(*) FROM GIT_PROJ_TABLE WHERE PATH='%s'", proj.path.c_str() );

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
