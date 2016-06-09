#include <iostream>
#include "HighPrecisionDigit.h"
#include "RSA.h"
#include <fstream>
#include <getopt.h>

using namespace std;

int main(int argc, char** argv) {
    RSA rsa;
    int ch;
    int cmdType = 0;
    string paraKey, paraInput, paraOutput;
    while ((ch = getopt(argc, argv, "gGeEdDk:i:o:hH")) != EOF) {
        switch (ch) {
            case 'g':
            case 'G':
                cmdType = 'g'; break;
            case 'e':
            case 'E':
                cmdType = 'e'; break;
            case 'd':
            case 'D':
                cmdType = 'd'; break;
            case 'h':
            case 'H':
                cmdType = 'h';break;
            case 'i': paraInput = string(optarg) ; break;
            case 'o': paraOutput = string(optarg); break;
            case 'k': paraKey = string(optarg); break;
        }
    }
    switch (cmdType){
        case 'g': rsa.generateKey(1024);break;
        case 'h':
            cout<<"Pure handmake 1024-bits RSA algorithm with 24K gold.Version 1.0\n";
            cout<<"All By 13211103 AILance.\n";
            cout<<"BeiHang University Yeahhhhh!\n";
            cout<<"All rights go to hell, whatever.\n";
            cout<<"parameter rules:\n";
            cout<<"generate public key and private key: -g\n";
            cout<<"help: -h\n";
            cout<<"encrypt: -e -i [input file path] -o[output file path] -k[public key path]\n";
            cout<<"decrypt: -e -i [input file path] -o[output file path] -k[private key path]\n";
            cout<<"using 1024-bits way, public key and private key file are in the exe file's folder\n";
            cout<<"Have Fun!\n";
            break;
        case 'e':
            if(paraInput.empty() || paraOutput.empty() || paraKey.empty()){
                cout<<"not enough parameters. need help? type -h option.\n";
                break;
            }
            rsa.encrypt(paraKey, paraInput, paraOutput);
            break;
        case 'd':
            if(paraInput.empty() || paraOutput.empty() || paraKey.empty()){
                cout<<"not enough parameters. need help? type -h option.\n";
                break;
            }
            rsa.decrypt(paraKey, paraInput, paraOutput);
            break;
        case 0:
            cout<<"Illegal option. need help? type -h option.\n";
    }
    return 0;
}