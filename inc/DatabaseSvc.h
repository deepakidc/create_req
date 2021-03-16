/*
 * DatabaseSvc.h
 *
 *  Created on: Mar 10, 2021
 *      Author: dvasudeva
 */
#include <string>
#include <sqlite3.h>

using namespace std;

#ifndef DATABASESVC_H_
#define DATABASESVC_H_

class DatabaseSvc{
public:
	DatabaseSvc();
	~DatabaseSvc();
public:
	bool exec(const char* query);
	bool recordExist(const char* query);

private:
   sqlite3 *db;
};



#endif /* DATABASESVC_H_ */
