/*
 * TenantCmdlineSvc.h
 *
 *  Created on: Mar 10, 2021
 *      Author: dvasudeva
 */
#include <string>

#include <DatabaseSvc.h>
#include <OpenSSLSvc.h>

using namespace std;

#ifndef TENANTCMDLINESVC_H_
#define TENANTCMDLINESVC_H_

class TenantSvcCmdLine{
public:
	TenantSvcCmdLine();
	void createRegistrationRequest(string tenantName);
private:
	void createTenantRegRequest(const char* tenantName, const char* tenantPubKey,const char* tenantSignPubKey);
	void createTARFile(const char* filename);
private:
	DatabaseSvc databaseSvc;
	OpenSSLSvc openSSLSvc;
	ECKey keyPairs[2];

};

#endif /* TENANTCMDLINESVC_H_ */

