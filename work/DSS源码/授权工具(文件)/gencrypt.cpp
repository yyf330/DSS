/* 
 * File:   gencrypt.cpp
 * Author: root
 * 
 * Created on 2010年5月27日, 下午9:31
 */

#include "gencrypt.h"

GEncrypt::GEncrypt()
{
}


GEncrypt::~GEncrypt()
{
}

string GEncrypt::Encrypt(string iv, string key, string instr)
{
    int TempLen = 0;
    int OutLen = 0;
    unsigned char Iv[17];
    unsigned char Key[17];
    unsigned char OutBuffer[16];

    bzero(Iv, sizeof(Iv));
    bzero(Key, sizeof(Key));
    memcpy(Iv, iv.c_str(), iv.size());
    memcpy(Key, key.c_str(), key.size());
    
    EVP_CIPHER_CTX Ctx;

    EVP_CIPHER_CTX_init(&Ctx);
    EVP_EncryptInit_ex(&Ctx, EVP_aes_128_cbc(), NULL, Key, Iv);
    if (!EVP_EncryptUpdate(&Ctx, OutBuffer, &OutLen, (const unsigned char*)instr.c_str(), instr.size()))
    {
        return "";
    }

    if (!EVP_EncryptFinal_ex(&Ctx, OutBuffer + OutLen, &TempLen))
    {
        return "";
    }
    OutLen += TempLen;
    EVP_CIPHER_CTX_cleanup(&Ctx);

//    printf("out len = %d\n", OutLen);

    char MsgBuf[3];
    bzero(MsgBuf, sizeof(MsgBuf));

    string Str = "";
    int i = 0;
    while (i < OutLen)
    {
//        if ( i != 0 && i %4 == 0)
//            printf("-");
//        printf("%02X", OutBuffer[i]);
        sprintf(MsgBuf, "%02X\0", OutBuffer[i++]);
        Str += MsgBuf;
    }
    
    return Str;
}


string GEncrypt::Decrypt(string iv, string key, string instr)
{

    if (iv == "" || key == "" || instr.size() != 32)
        return "";

    int TempLen = 0;
    int OutLen = 0;

    unsigned char Iv[17];
    unsigned char Key[17];
    unsigned char OutBuffer[16];

    unsigned char InBuffer[16];

    bzero(Iv, sizeof(Iv));
    bzero(Key, sizeof(Key));
    memcpy(Iv, iv.c_str(), iv.size());
    memcpy(Key, key.c_str(), key.size());

    bzero(InBuffer, sizeof(InBuffer));

    for(int i = 0; i < sizeof(InBuffer); i++)
    {
        InBuffer[i] = HexToDec(instr.substr(i * 2, 2));
    }

    EVP_CIPHER_CTX Ctx;

    EVP_CIPHER_CTX_init(&Ctx);

    EVP_DecryptInit_ex(&Ctx, EVP_aes_128_cbc(), NULL, Key, Iv);

    if (!EVP_DecryptUpdate(&Ctx, OutBuffer, &OutLen, InBuffer, sizeof(InBuffer)))
    {
        return "";
    }
    if (!EVP_DecryptFinal_ex(&Ctx, OutBuffer + OutLen, &TempLen))
    {
        return "";
    }

    OutLen += TempLen;
    EVP_CIPHER_CTX_cleanup(&Ctx);

    string Str = "";
    int i = 0;
    while (i < OutLen)
    {
        //printf("%c", OutBuffer[i]);
        Str += (char)OutBuffer[i++];
    }
    return Str;
}



string GEncrypt::MD5Encrypt(string instr)
{
    const EVP_MD *md5 = EVP_md5();

    EVP_MD_CTX Ctx;

    EVP_DigestInit(&Ctx, md5);

    unsigned char OutBuffer[16];
    unsigned int OutLen = 0;

    if (!EVP_DigestUpdate(&Ctx, instr.c_str(), instr.size()))
    {
        return "";
    }

    if (!EVP_DigestFinal(&Ctx, OutBuffer, &OutLen))
    {
        return "";
    }

    char MsgBuf[3];
    bzero(MsgBuf, sizeof(MsgBuf));

    string Str = "";
    int i = 0;
    while (i < OutLen)
    {
//        if ( i != 0 && i %4 == 0)
//            printf("-");
//        printf("%02X", OutBuffer[i]);
        sprintf(MsgBuf, "%02X\0", OutBuffer[i++]);
        Str += MsgBuf;
    }
    return Str;
}


unsigned char GEncrypt::HexToDec(string str)
{
    int Size = str.size();
    int DecValue = 0;
    for(int i = 0; i < Size; i++)
    {
        unsigned char Val = 0;
        char Ch = str.substr(i,1).c_str()[0];
        switch(Ch)
        {
            case 'F':
                Val = 15;
                break;
            case 'E':
                Val = 14;
                break;
            case 'D':
                Val = 13;
                break;
            case 'C':
                Val = 12;
                break;
            case 'B':
                Val = 11;
                break;
            case 'A':
                Val = 10;
                break;
            default:
                Val = atoi(str.substr(i,1).c_str());
                break;
        }
        DecValue = DecValue + (int)Val * (int)pow(16, Size - i - 1);
    }
    return (unsigned char)DecValue;
}


int GEncrypt::do_encrypt(unsigned char *iv, unsigned char *key, unsigned char *inBuffer, int inLen, unsigned char *outBuffer, int *pOutLen)
{
    int tmplen;

    EVP_CIPHER_CTX ctx;

    EVP_CIPHER_CTX_init(&ctx);

    EVP_EncryptInit_ex(&ctx, EVP_aes_128_cbc(), NULL, key, iv);

    cout << "InLen=" << inLen << endl;

    if (!EVP_EncryptUpdate(&ctx, outBuffer, pOutLen, inBuffer, inLen))
    {
        return 1;
    }

    if (!EVP_EncryptFinal_ex(&ctx, outBuffer + *pOutLen, &tmplen))
    {
        return 1;
    }
    *pOutLen += tmplen;
    EVP_CIPHER_CTX_cleanup(&ctx);
    return 0;
}


int GEncrypt::do_decrypt(unsigned char *iv, unsigned char *key, unsigned char *inBuffer, int inLen, unsigned char *outBuffer, int *pOutLen)
{

    int tmplen;

    EVP_CIPHER_CTX ctx;

    EVP_CIPHER_CTX_init(&ctx);

    EVP_DecryptInit_ex(&ctx, EVP_aes_128_cbc(), NULL, key, iv);


    for(int i = 0; i < inLen; i++)
    {
        printf("%d\t", inBuffer[i]);
    }

    if (!EVP_DecryptUpdate(&ctx, outBuffer, pOutLen, inBuffer, inLen))
    {

        return 1;

    }
cout <<  *pOutLen  << endl;
    if (!EVP_DecryptFinal_ex(&ctx, outBuffer + *pOutLen, &tmplen))
    {
cout <<  *pOutLen  << endl;

        return 1;

    }
cout <<  *pOutLen  << endl;
cout <<  tmplen  << endl;

    *pOutLen += tmplen;

    EVP_CIPHER_CTX_cleanup(&ctx);

    return 0;
}
