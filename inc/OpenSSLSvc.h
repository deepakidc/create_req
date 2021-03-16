/*
 * OpenSSLSvc.h
 *
 *  Created on: Mar 10, 2021
 *      Author: dvasudeva
 */
#include "common.h"
#ifndef OPENSSLSVC_H_
#define OPENSSLSVC_H_


class OpenSSLSvc{
public:
	ECKey createKeyPair();
};

#endif /* OPENSSLSVC_H_ */
