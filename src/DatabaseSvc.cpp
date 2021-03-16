/*
 * DatabaseSvc.cpp
 *
 *  Created on: Mar 10, 2021
 *      Author: dvasudeva
 */
#include <sqlite3.h>
#include "DatabaseSvc.h"


DatabaseSvc::DatabaseSvc()
{
    char *errMsg = 0;
    int status;

    status = sqlite3_open("Database.db", &db);

    if (status != 0) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* create tables if it doesn't exist */
    const char *sql1 = "CREATE TABLE IF NOT EXISTS Tenants (tenantName varchar(64) NOT NULL, tenantPcdDevId tinyint(3) NOT NULL, PRIMARY KEY (tenantName))";

    /* Execute SQL statement */
    status = sqlite3_exec(db, sql1, NULL, 0, &errMsg);

    if( status != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        fprintf(stdout, "Table Tenants created successfully\n");
    }

    const char *sql2 = "CREATE TABLE IF NOT EXISTS TenantKeys (tenantName varchar(64) NOT NULL, keyName varchar(128) NOT NULL,"\
        		" keyValue blob NOT NULL, keyType varchar(64) NOT NULL, PRIMARY KEY (tenantName, keyName),"\
        		" FOREIGN KEY(tenantName) REFERENCES Tenants(tenantName), FOREIGN KEY(keyType) REFERENCES KeyTypes(keyType))";
    status = sqlite3_exec(db, sql2, NULL, 0, &errMsg);

    if( status != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        fprintf(stdout, "Table TenantKeys created successfully\n");
    }
}

bool DatabaseSvc::exec(const char* query)
{
	char *zErrMsg = 0;
	int status;
	status = sqlite3_exec(db, query, NULL, 0, &zErrMsg);

    if( status  != SQLITE_OK ){
    	fprintf(stderr, "SQL error: %s\n", zErrMsg);
    	sqlite3_free(zErrMsg);
    	return false;
    } else {
	  fprintf(stdout, "Records created successfully\n");
	  return true;
   }
}

bool DatabaseSvc::recordExist(const char* query)
{
	sqlite3_stmt *stmt;
	bool recordExist;
	int rc = sqlite3_prepare_v2(db, query,-1, &stmt, NULL);
	if(rc != SQLITE_OK){
		printf("error: %s!\n", sqlite3_errmsg(db));
			   return 0;
	}
	rc = sqlite3_step(stmt);

   if(rc == SQLITE_ROW)
	   recordExist = true;
   else
	   recordExist = false;

   sqlite3_finalize(stmt);
   return recordExist;
}

DatabaseSvc::~DatabaseSvc()
{
    int status = sqlite3_close(db);
    db = NULL;
    if (status == SQLITE_OK) {
        fprintf(stderr, "Can't close database: %s\n", sqlite3_errmsg(db));
    } else {
        fprintf(stderr, "Closed database successfully\n");
    }
}
