/*
 * tenantCmdlineSvc.cpp
 *
 *  Created on: Mar 10, 2021
 *      Author: dvasudeva
 */
#include <string>

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cJSON.h>

#include <fcntl.h>
#include <archive.h>
#include <archive_entry.h>
#include "TenantCmdlineSvc.h"

using namespace std;
TenantSvcCmdLine::TenantSvcCmdLine()
{}

void TenantSvcCmdLine::createRegistrationRequest(string tenantName)
{

	char* exp;
	const char* queryInsert = "INSERT INTO Tenants (tenantName,tenantPcdDevId) VALUES (%s, %d);";
	const char* queryRecExist = "SELECT * FROM Tenants WHERE tenantName = %s";
	const char* queryInsertTKeys = "INSERT INTO TenantKeys (tenantName,keyName,keyValue,keyType) VALUES (\"%s\",\"%s\",\"%s\",\"%s\");";

	exp = sqlite3_mprintf(queryRecExist,tenantName.c_str());
	cout << "Checking if tenant already exist."<< endl;
	bool tenantExist = databaseSvc.recordExist(exp);
	if(tenantExist)
	{
		cout << "Tenant already exist."<< endl;
		return;
	}

	exp = sqlite3_mprintf(queryInsert,tenantName.c_str(),0);
	cout << "Adding Tenant to database."<< endl;
	bool result = databaseSvc.exec(exp);
	if(!result)
		cout << "Failed to add tenant to database"<< endl;

	for (int i=0;i < 2;i++)
	{
		cout << "Generating key pair-."<< i+1<<endl;
		ECKey keyPair = openSSLSvc.createKeyPair();
		exp = sqlite3_mprintf(queryInsertTKeys,tenantName.c_str(),"EC", keyPair.private_key, "Private");
		cout << "Updating TenantKeys table with new keys"<< endl;
		bool result = databaseSvc.exec(exp);
		if(!result)
			cout << "Failed to add tenant to database"<< endl;

		exp = sqlite3_mprintf(queryInsertTKeys,tenantName.c_str(),"EC", keyPair.private_key, "Public");
		result = databaseSvc.exec(exp);
		if(!result)
			cout << "Failed to add tenant to database"<< endl;

		keyPairs[i] = keyPair;
	}

	createTenantRegRequest(tenantName.c_str(),keyPairs[0].public_key,keyPairs[1].public_key);
}

void TenantSvcCmdLine::createTenantRegRequest(const char* tenantName, const char* tenantPubKey,const char* tenantSignPubKey)
{
	  cJSON *root;
	  ofstream file;

	  root = cJSON_CreateObject();
	  cJSON_AddStringToObject(root, "tenantName", tenantName);
	  cJSON_AddStringToObject(root, "tenantPub", tenantPubKey);
	  cJSON_AddStringToObject(root, "tenantPubSig", tenantSignPubKey);

	  std::cout << cJSON_Print(root) << std::endl;
	  char* data = cJSON_Print(root);

	  cout << "Generating tar file."<<endl;
	  char* fileName = strcat(strdup(tenantName) , ".req.json");
	  file.open(fileName);
	  file<<data ;
	  file.close();
	  createTARFile(tenantName);
}

void TenantSvcCmdLine::createTARFile(const char* tenantName)
{
	  struct archive *a;
	  struct archive_entry *entry;
	  struct stat st;
	  char buff[8192];
	  int len;
	  int fd;
	  char* filename = strcat(strdup(tenantName) , "req.json");
	  char* outname = strcat(strdup(tenantName) , ".tar");

	  cout << "Generating tar file."<<endl;
	  a = archive_write_new();
	  archive_write_add_filter_gzip(a);
	  archive_write_set_format_pax_restricted(a);
	  archive_write_open_filename(a, outname);

	  stat(filename, &st);
	  entry = archive_entry_new();
	  archive_entry_set_pathname(entry, filename);
	  archive_entry_set_size(entry, st.st_size);
	  archive_entry_set_filetype(entry, AE_IFREG);
	  archive_entry_set_perm(entry, 0644);
	  archive_write_header(a, entry);
	  fd = open(filename, O_RDONLY);
	  len = read(fd, buff, sizeof(buff));
	  while ( len > 0 ) {
		  archive_write_data(a, buff, len);
		  len = read(fd, buff, sizeof(buff));
	  }
	  close(fd);
	  archive_entry_free(entry);
	  archive_write_close(a);
	  archive_write_free(a);
}

int main(int argc,char* argv[])
{
	cout << "!!!Register new Tenant!!" << endl;
	TenantSvcCmdLine cmdlineSvc ;
	if(argc > 0){
	char* tenantName = "tenant1";
	cmdlineSvc.createRegistrationRequest(tenantName);
	}
	else
		cout << "Please provide tenant name." <<endl;

	return 0;
}



