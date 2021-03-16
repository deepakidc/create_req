/*
 * OpenSSLSvc.cpp
 *
 *  Created on: Mar 10, 2021
 *      Author: dvasudeva
 */
#include <iostream>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/obj_mac.h>
#include <openssl/ec.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "OpenSSLSvc.h"
#include "common.h"


ECKey OpenSSLSvc::createKeyPair()
{
	EC_KEY* key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
	ECKey ecKey;
	if(!key)
	{
		std::cerr << "Error creating curve key" << '\n';
		return ecKey;
	}

	if(!EC_KEY_generate_key(key))
	{
		std::cerr << "Error generating curve key" << '\n';
		EC_KEY_free(key);
		return ecKey;
	}

	BIGNUM const* prv = EC_KEY_get0_private_key(key);
	if(!prv)
	{
		std::cerr << "Error getting private key" << '\n';
		EC_KEY_free(key);
		return ecKey;
	}

	char * priv_key = BN_bn2hex(prv);
	std::cout << "Private key: " << priv_key << '\n';

	EC_POINT const* pub = EC_KEY_get0_public_key(key);
	if(!pub)
	{
		std::cerr << "Error getting public key" << '\n';
		EC_KEY_free(key);
		return ecKey;
	}
	const EC_GROUP * group = EC_KEY_get0_group(key);
	point_conversion_form_t form = EC_KEY_get_conv_form(key);
	char * pub_key = EC_POINT_point2hex (group,pub,form,NULL);

	std::cout << "Public key: " << pub_key << '\n';

	EC_KEY_free(key);
	ecKey.private_key =  priv_key;
	ecKey.public_key = pub_key;
}




