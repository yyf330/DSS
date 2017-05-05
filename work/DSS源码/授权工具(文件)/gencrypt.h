/* 
 * File:   gencrypt.h
 * Author: root
 *
 * Created on 2010年5月27日, 下午9:31
 */

#ifndef _GENCRYPT_H
#define	_GENCRYPT_H

#include <iostream>
#include <string>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

using namespace std;

class GEncrypt
{
public:
    GEncrypt();
    virtual ~GEncrypt();
public:
    static string Encrypt(string iv, string key, string instr);
    static string Decrypt(string iv, string key, string instr);
    static string MD5Encrypt(string instr);
    static int do_encrypt(unsigned char *iv, unsigned char *key, unsigned char *inBuffer, int inLen, unsigned char *outBuffer, int *pOutLen);
    static int do_decrypt(unsigned char *iv, unsigned char *key, unsigned char *inBuffer, int inLen, unsigned char *outBuffer, int *pOutLen);
public:
    static unsigned char HexToDec(string str);
};

#endif	/* _GENCRYPT_H */

