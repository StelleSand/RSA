//
// Created by AILance on 2016/5/22.
//

#ifndef RSA_RSA_H
#define RSA_RSA_H

#include "HighPrecisionDigit.h"
#include <fstream>
#include <string>

using namespace std;

class RSA {
public:
    static string PubKeyFileName;
    static string PriKeyFileName;
private:
    string E_inputString;
    string E_outputString;
    string E_PubKeyFileName;
    string D_inputString;
    string D_outputString;
    string D_PriKeyFileName;
    HighPrecisionDigit P;
    HighPrecisionDigit Q;
    HighPrecisionDigit N;
    HighPrecisionDigit EulerValue;
    HighPrecisionDigit E;
    HighPrecisionDigit D;
public:
    void generateKey(int bits = 1024); //生成RSA公钥和私钥文件bits参数表示密钥的位数(P * Q 乘积的二进制下位数)
    void encrypt(string pubKeyFileName, string inputFileName, string outputFileName = "RSA_Encript_Output.txt");
    void decrypt(string priKeyFileName, string inputFileName, string outputFileName = "RSA_Decript_Output.txt");
    void test();
private:
    int getPublicKey(ifstream& pubKeyFile);
    int getPrivateKey(ifstream& priKeyFile);
    void generateDE();
};


#endif //RSA_RSA_H
