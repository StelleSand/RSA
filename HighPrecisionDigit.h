//
// Created by AILance on 2016/5/25.
//

#ifndef RSA_HIGHPRECISIONDIGIT_H
#define RSA_HIGHPRECISIONDIGIT_H

#define HPD_MAX_LENGTH 512
#define HPD_CHAR unsigned char
#define HPD_SHORT unsigned short
#define HPD_INT unsigned int
#define HPD_BASE 0x100 //进制 256

#include <string>
#include <iostream>
#include "stdio.h"
#include <vector>
#include <stdlib.h>
#include <time.h>
using namespace std;

class HighPrecisionDigit {
    //高精度数字
    //采用char数组存储256进制数字
    //0位为最低位数字
public:
    static const int BINARY = 1; //二进制
    static const int DECIMAL = 2; //十进制
    static const int HEX = 3; //十六进制
    static const int CHAR = 4; // 256进制
    static const int MRTestRound = 5;
    static vector<int> smallNumberList;
    static vector<HighPrecisionDigit> binaryPowerList;
    static const int binaryPowerRound = 4000; //设定binaryPowerList的长度,也即所求的2^X的最大X值
private:
    string  DEC_Number;//
    string  BINARY_Number;
    string  HEX_Number;
    string  CHAR_Number; //大数存储结构_256进制
    bool negative = false;
public:
    HighPrecisionDigit();
    HighPrecisionDigit(const long); //由long构造大数，可以使用隐式转换
    HighPrecisionDigit(const string, bool isCHAR_NUMBER = false); //由string构造大数,如果isCHAR_NUMBER为真，则直接赋值构造，不需要转换过程
    HighPrecisionDigit(const HighPrecisionDigit&);//复制构造
    ~HighPrecisionDigit();

    const string& getHEXNumber();
    const string& getCharString();
    static HighPrecisionDigit generatePrime(int bits = 512);//生成大素数，bits指明生成素数的位数,指2进制下的位数
    static HighPrecisionDigit generateRandom(HighPrecisionDigit High, int leastBits = 64, int mostBits = 512); //生成指定范围内的随机数
    static HighPrecisionDigit gcd(HighPrecisionDigit A, HighPrecisionDigit B);// 求两个数的最大公约数
    static HighPrecisionDigit gcd_E(HighPrecisionDigit A, HighPrecisionDigit B, HighPrecisionDigit& x , HighPrecisionDigit& y);
    // 求两个数的最大公约数,同时计算乘法逆元因子x,y


    bool isNegative() const;
    void setNegative(bool state);
    const char& operator[](const int index) const;
    char& operator[](const int index);
    int compare(const HighPrecisionDigit& other);
    bool operator>(const HighPrecisionDigit& other);
    bool operator<(const HighPrecisionDigit& other);
    bool operator>=(const HighPrecisionDigit& other);
    bool operator<=(const HighPrecisionDigit& other);
    bool operator==(const HighPrecisionDigit& other);
    bool operator!=(const HighPrecisionDigit& other);
    HighPrecisionDigit operator+(const HighPrecisionDigit& other);
    HighPrecisionDigit operator+(const long other);
    HighPrecisionDigit operator-(const HighPrecisionDigit& other);
    HighPrecisionDigit operator-(const long other);
    HighPrecisionDigit operator*(const HighPrecisionDigit& other);
    HighPrecisionDigit operator/(const HighPrecisionDigit& other);
    HighPrecisionDigit operator/(const long other);
    HighPrecisionDigit operator%(const HighPrecisionDigit& other);
    HighPrecisionDigit operator%(const long other);
    HighPrecisionDigit operator^(const long other);

    HighPrecisionDigit multiplyWithMod(const HighPrecisionDigit& other, const HighPrecisionDigit& divisor);//对结果取余的乘法
    HighPrecisionDigit powerWithMod(const HighPrecisionDigit& power, const HighPrecisionDigit& divisor);//对结果取余的次方运算，即求this^power(mod n)

    static bool primeTest(HighPrecisionDigit obj); // 测试一个数是否是素数，
private:
    void longToNumber(const long base); //long转内部char*结构
    void hexStringToNumber(const string base);
    //16进制字符串转内部char*结构,默认是正序输入
    //“0x123456789abcd”最左为高位表示正序，最右为高位表示逆序
    HPD_CHAR charToNumber(const char base);
    char numberToChar(const HPD_CHAR base);
    //字符类型转256进制数字
    void zeroCut(); //去除字符串高位多余的0；
    static vector<int> getSmallNumberList(); //获取小于2000的素数数组
    static vector<int> generateSmallNumberList(); //生成小于2000的素数数组,存储在类静态变量smallNumberList中
    static vector<HighPrecisionDigit> getBinaryPowerList(); //获取2的乘方数组,存储在类静态变量binaryPowerList中
    static vector<HighPrecisionDigit> generateBinaryPowerList(); //生成2的乘方数组
    static HighPrecisionDigit generateRandom(int bits);//生成大随机数，bits指明生成素数的位数,指2进制下的位数,leastBits表示最少生成位数
    static bool primePreTest(HighPrecisionDigit obj); //素数测试预测试
};


#endif //RSA_HIGHPRECISIONDIGIT_H
