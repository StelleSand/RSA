//
// Created by AILance on 2016/5/22.
//

#include "RSA.h"

string RSA::PubKeyFileName = "publicKey.txt";
string RSA::PriKeyFileName = "privateKey.txt";

void RSA::generateKey(int bits) {
    int bitsP, bitsQ;
    //P、Q的位数应不完全相同，设定为相差16 * [1, 3] * 2位二进制数
    bitsP = bits / 2 + (rand() % 3 + 1) * 16;
    bitsQ = bits - bitsP;
    //生成P, Q
    this->P = HighPrecisionDigit::generatePrime(bitsP);
    this->Q = HighPrecisionDigit::generatePrime(bitsQ);
    this->N = this->P * this->Q;
    this->EulerValue = (this->P - 1) * (this->Q - 1);
    //生成公钥私钥D,E
    this->generateDE();
    //写出公钥文件
    ofstream publicKeyFile(RSA::PubKeyFileName);
    publicKeyFile<<"E\n"<<this->E.getHEXNumber()<<endl;
    publicKeyFile<<"N\n"<<this->N.getHEXNumber()<<endl;
    publicKeyFile.close();
    cout<<"Public Key File Generated.\n";
    ofstream privateKeyFile(RSA::PriKeyFileName);
    privateKeyFile<<"P\n"<<this->P.getHEXNumber()<<endl;
    privateKeyFile<<"Q\n"<<this->Q.getHEXNumber()<<endl;
    privateKeyFile<<"D\n"<<this->D.getHEXNumber()<<endl;
    privateKeyFile<<"E\n"<<this->E.getHEXNumber()<<endl;
    privateKeyFile<<"EU\n"<<this->EulerValue.getHEXNumber()<<endl;
    privateKeyFile<<"N\n"<<this->N.getHEXNumber()<<endl;
    privateKeyFile.close();
    cout<<"Private Key File Generated.\n";
    cout<<"Key Generation Finished.\n";
}

void RSA::test() {

}

int RSA::getPublicKey(ifstream& publicKey) {
    string strIndex, strValue;
    while(publicKey>>strIndex){
        publicKey>>strValue;
        if(strIndex == "E")
            this->E = HighPrecisionDigit(strValue);
        if(strIndex == "N")
            this->N = HighPrecisionDigit(strValue);
    }
    if(this->E != 0 && this->N != 0)
        return 1;
    else return 0;
}

int RSA::getPrivateKey(ifstream& privateKey) {
    string strIndex, strValue;
    while(privateKey>>strIndex){
        privateKey>>strValue;
        if(strIndex == "P")
            this->P = HighPrecisionDigit(strValue);
        if(strIndex == "Q")
            this->Q = HighPrecisionDigit(strValue);
        if(strIndex == "D")
            this->D = HighPrecisionDigit(strValue);
        if(strIndex == "E")
            this->E = HighPrecisionDigit(strValue);
        if(strIndex == "N")
            this->N = HighPrecisionDigit(strValue);
        if(strIndex == "EU")
            this->EulerValue = HighPrecisionDigit(strValue);
    }
    if(this->P != 0 && this->Q != 0 && this->D != 0 && this->E != 0 && this->N != 0 && this->EulerValue != 0)
        return 1;
    else
        return 0;
}

void RSA::encrypt(string pubKeyFileName, string inputFileName, string outputFileName) {
    ifstream pubKeyFile(pubKeyFileName);
    if(!pubKeyFile || !this->getPublicKey(pubKeyFile) ){
        cout<<"Public file does not exist or illegal! Abort.\n";
        return;
    }
    pubKeyFile.close();
    ifstream input(inputFileName);
    ofstream output(outputFileName);
    string tempInput;
    char tempChar;
    int inputCount;
    while(true){
        inputCount = 0;
        tempInput = "";
        while(inputCount < 120 && !input.eof()){
            //input>>tempChar;
            input.get(tempChar);
            if(!input.eof())
                tempInput += tempChar;
            inputCount++;
        }
        //读取完一组字符串，进行加密并输出
        HighPrecisionDigit inputDigit(tempInput, true);
        HighPrecisionDigit outputDigit = inputDigit.powerWithMod(this->E, this->N);
        output<<outputDigit.getHEXNumber()<<endl;
        //如果都到了文件末尾，跳出循环，否则继续
        if(input.eof())
            break;
    }
    input.close();
    output.close();
}

void RSA::decrypt(string priKeyFileName, string inputFileName, string outputFileName) {
    ifstream priKeyFile(priKeyFileName);
    if(!priKeyFile || !this->getPrivateKey(priKeyFile)){
        cout<<"Private file does not exist or illegal! Abort.\n";
        return;
    }
    priKeyFile.close();
    ifstream input(inputFileName);
    ofstream output(outputFileName);
    string tempInput;
    while(input>>tempInput){
        HighPrecisionDigit cipherDigit(tempInput);
        HighPrecisionDigit plainDigit = cipherDigit.powerWithMod(this->D, this->N);
        output<<plainDigit.getCharString();
    }
    input.close();
    output.close();
}

void RSA::generateDE() {
    //生成最少256位,最多1024位的随机数，使满足temD^4 > this->N,且tempD < this->EulerValue
    HighPrecisionDigit tempD = HighPrecisionDigit::generateRandom(this->EulerValue, 256, 1024);
    HighPrecisionDigit X, Y;
    //如果不满足私钥条件，持续更新私钥
    HighPrecisionDigit gcd = HighPrecisionDigit::gcd_E(tempD, this->EulerValue, X, Y);
    while(gcd != HighPrecisionDigit(1)){
        tempD = HighPrecisionDigit::generateRandom(this->EulerValue, 256, 1024);
        gcd = HighPrecisionDigit::gcd_E(tempD, this->EulerValue, X, Y);
    }
    this->D = tempD;
    HighPrecisionDigit re = tempD * X + this->EulerValue * Y;
    this->E = X % this->EulerValue;
}