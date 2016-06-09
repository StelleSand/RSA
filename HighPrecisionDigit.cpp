//
// Created by AILance on 2016/5/25.
//

#include "HighPrecisionDigit.h"

vector<int> HighPrecisionDigit::smallNumberList;
vector<HighPrecisionDigit> HighPrecisionDigit::binaryPowerList;

// <!------构造析构函数
HighPrecisionDigit::HighPrecisionDigit(){
};
HighPrecisionDigit::HighPrecisionDigit(const long base) {
    HighPrecisionDigit();
    this->longToNumber(base);
}
HighPrecisionDigit::HighPrecisionDigit(const string base, bool isCHAR_NUMBER ) {
    HighPrecisionDigit();
    if(isCHAR_NUMBER)
        this->CHAR_Number = base;
    else
        this->hexStringToNumber(base);
    this->zeroCut();
}
HighPrecisionDigit::HighPrecisionDigit(const HighPrecisionDigit& other) {
    HighPrecisionDigit();
    this->CHAR_Number = other.CHAR_Number;
    this->negative = other.negative;
}
HighPrecisionDigit::~HighPrecisionDigit() {
}
// ------构造析构函数-->


//<!-------公共函数


const string& HighPrecisionDigit::getHEXNumber() {
    this->HEX_Number = "";
    int index = 0;
    HPD_CHAR tempChar;
    while(index < this->CHAR_Number.length()){
        tempChar = this->CHAR_Number[index];
        this->HEX_Number = this->numberToChar(tempChar >> 4) + (this->numberToChar(tempChar & 0xf) + this->HEX_Number);
        index++;
    }
    return this->HEX_Number;
}

const string& HighPrecisionDigit::getCharString() {
    return this->CHAR_Number;
}

HighPrecisionDigit HighPrecisionDigit::generatePrime(int bits) {
    HighPrecisionDigit result = HighPrecisionDigit::generateRandom(bits);
    while(!HighPrecisionDigit::primeTest(result)) {
        result = result + HighPrecisionDigit(2);
        cout << "randomNumber into test! is:" << result.getHEXNumber() << endl;
    }
    return result;
}

HighPrecisionDigit HighPrecisionDigit::gcd(HighPrecisionDigit A, HighPrecisionDigit B) {
    HighPrecisionDigit temp;
    while(B != HighPrecisionDigit(0)){
        //cout<<"A is:"<<A.getHEXNumber()<<endl;
        //cout<<"B is:"<<B.getHEXNumber()<<endl;
        temp = B;
        B = A % B;
        A = temp;
    }
    return A;
}

HighPrecisionDigit HighPrecisionDigit::gcd_E(HighPrecisionDigit A, HighPrecisionDigit B, HighPrecisionDigit &X,
                                             HighPrecisionDigit &Y) {
    if(B == HighPrecisionDigit(0)){
        X = HighPrecisionDigit(1);
        Y = HighPrecisionDigit(0);
        return A;
    }
    HighPrecisionDigit Remainder = HighPrecisionDigit::gcd_E(B, A % B, X, Y);
    HighPrecisionDigit Temp = Y;
    Y = X - (A / B) * Y;
    X = Temp;
    return Remainder;
}

int HighPrecisionDigit::compare(const HighPrecisionDigit &other) {
    if(this->isNegative() && !other.isNegative())
        return -1;
    if(!this->isNegative() && other.isNegative())
        return 1;
    if(this->isNegative() && other.isNegative()){
        HighPrecisionDigit A = (*this), B = other;
        A.setNegative(false);
        B.setNegative(false);
        return B.compare(A);
    }
    if(this->CHAR_Number.length() > other.CHAR_Number.length())
        return 1;
    if(this->CHAR_Number.length() < other.CHAR_Number.length())
        return -1;
    for(int i = this->CHAR_Number.length() - 1; i >= 0 ; i-- ){
        if((HPD_CHAR)this->CHAR_Number[i] > (HPD_CHAR)other.CHAR_Number[i])
            return 1;
        if((HPD_CHAR)this->CHAR_Number[i] < (HPD_CHAR)other.CHAR_Number[i])
            return -1;
    }
    return 0;
}

bool HighPrecisionDigit::operator>(const HighPrecisionDigit &other) {
    return this->compare(other) == 1;
}

bool HighPrecisionDigit::operator<(const HighPrecisionDigit &other) {
    return this->compare(other) == -1;
}

bool HighPrecisionDigit::operator>=(const HighPrecisionDigit &other) {
    return this->compare(other) >= 0;
}

bool HighPrecisionDigit::operator<=(const HighPrecisionDigit &other) {
    return this->compare(other) <= 0;
}

bool HighPrecisionDigit::operator==(const HighPrecisionDigit &other) {
    return this->compare(other) == 0;
}

bool HighPrecisionDigit::operator!=(const HighPrecisionDigit &other) {
    return this->compare(other) != 0;
}

bool HighPrecisionDigit::isNegative() const {
    return this->negative;
}

void HighPrecisionDigit::setNegative(bool state) {
    this->negative = state;
}

const char& HighPrecisionDigit::operator[](const int index) const {
    if(index < 0) {
        cout<<"Error: Trying to get char of index \'-1\'.";
    }
    if(index < this->CHAR_Number.length())
        return this->CHAR_Number[index];
    else
        return NULL;
}

char& HighPrecisionDigit::operator[](const int index) {
    if(index < 0) {
        cout<<"Error: Trying to get char of index \'-1\'.";
    }
    if(index > this->CHAR_Number.length())
        cout<<"Warning: Trying to set char of index "<<index<<".";
    if(index == this->CHAR_Number.length())
        this->CHAR_Number += char (0);
    return this->CHAR_Number[index];
}

HighPrecisionDigit HighPrecisionDigit::operator+(const HighPrecisionDigit &other) {
    //cout<<"into plus\n";
    HighPrecisionDigit result, A = (*this), B = other;
    if(A.isNegative() && !B.isNegative())
    {
        A.setNegative(false);
        return B - A;
    }
    else if(!A.isNegative() && B.isNegative())
    {
        B.setNegative(false);
        return A - B;
    }
    else if(A.isNegative() && B.isNegative()){
        A.setNegative(false);
        B.setNegative(false);
        result = A + B;
        result.setNegative(true);
        return result;
    }
    int high = max(this->CHAR_Number.length(), other.CHAR_Number.length());
    HPD_CHAR upAdd = 0;
    HPD_CHAR tempCHAR;
    HPD_SHORT tempSHORT;
    for(int i = 0; i < high; i++) {
        tempSHORT = (HPD_SHORT)(upAdd + (HPD_CHAR)(*this)[i] + (HPD_CHAR)other[i]);
        result.CHAR_Number += tempSHORT % HPD_BASE;
        upAdd = tempSHORT / HPD_BASE ;
    }
    if(upAdd)
        result.CHAR_Number += (HPD_CHAR) upAdd;
    //cout<<"out plus!\n";
    return result;
}

HighPrecisionDigit HighPrecisionDigit::operator+(const long other) {
    HighPrecisionDigit obj(other);
    return (*this) + obj;
}


HighPrecisionDigit HighPrecisionDigit::operator-(const HighPrecisionDigit &other) {
    /*cout<<"into minus!\n";
    cout<<"A is:"<<this->getHEXNumber()<<endl;
    cout<<"B is:"<<HighPrecisionDigit(other).getHEXNumber()<<endl;*/
    HighPrecisionDigit result((*this)), A = (*this), B = other;
    if(A.isNegative() && B.isNegative())
    {
        //（-A） - （-B），返回B-A
        A.setNegative(false);
        B.setNegative(false);
        result = B - A;
        return result;
    }
    else if(A.isNegative() && !B.isNegative()){
        //(-A) - B型，返回-(A + B)
        A.setNegative(false);
        result = A + B;
        result.setNegative(true);
        return result;
    }
    else if(!A.isNegative() && B.isNegative()){
        //A - (-B),返回A + B;
        B.setNegative(false);
        result = A + B;
        return result;
    }
    //如果都不是，则是A-B,正常计算
    int high = result.CHAR_Number.length();
    int low  = other.CHAR_Number.length();
    if(high < low )
    {
        result = HighPrecisionDigit(other.CHAR_Number, true) - result;
        result.negative = true;
        //cout<<"Negative Number get!\n";
        return result;
    }
    int i;
    for(i = 0 ; i < high; i++){
        if((HPD_CHAR)result[i] >= (HPD_CHAR)other[i])
            result[i] = (HPD_CHAR)result[i] - (HPD_CHAR)other[i];
        else {
            result[i] = (HPD_CHAR)((HPD_SHORT)0x100 + (HPD_CHAR)result[i] - (HPD_CHAR)other[i]);
            //向前寻找第一位不为0的数，中间的位全置为0xff;
            int j = i;
            while(++j < high && result[j] == 0) {
                result[j] = (HPD_CHAR)0xff;
            }
            if(j < high)
                result[j] -= 1;
        }
    }
    result.zeroCut();
    /*cout<<"out minus!\n";*/
    return result;
}

HighPrecisionDigit HighPrecisionDigit::operator-(const long other) {
    HighPrecisionDigit obj(other);
    return (*this) - obj;
}

HighPrecisionDigit HighPrecisionDigit::operator*(const HighPrecisionDigit &other) {
    HighPrecisionDigit result;
    int length = this->CHAR_Number.length() + other.CHAR_Number.length();
    HPD_INT* re = new HPD_INT[length];
    for(int i = 0; i < length; i++) {
        re[i] = 0;
    }
    for(int i = 0; i < this->CHAR_Number.length(); i++) {
        for(int j = 0; j < other.CHAR_Number.length(); j++){
            re[i + j] += (HPD_INT)(HPD_CHAR)(*this)[i] * (HPD_CHAR)other[j];
        }
    }
    for(int i = 0; i < length - 1; i++){
        re[i + 1] += re[i] / HPD_BASE;
        result[i] = (HPD_CHAR)(re[i] % HPD_BASE);
    }
    //单独处理最高位
    result[length - 1] = (HPD_CHAR)(re[length - 1] % HPD_BASE);
    delete[] re;
    result.zeroCut();
    result.setNegative(this->isNegative() ^ other.isNegative());
    return  result;
}

HighPrecisionDigit HighPrecisionDigit::operator/(const HighPrecisionDigit &other) {
    //cout<<"into divide!\n";
    if(other.CHAR_Number.length() == 0 ){
        cout<<"Error: trying to divide by zero!";
        return 0;
    }
    string result, remainder; //定义商result, remainder作为余数
    string dividend = this->CHAR_Number; //被除数
    HighPrecisionDigit divisor = other; //除数
    string  left; //left作为除法的对齐部分
    int index = dividend.length() - 1; //index指示除法进行到的对齐位数,从被除数最高位开始
    HPD_SHORT tempHigh, tempLow, tempMid; // 二分法求商临时结果位时的上下界和中间值
    HighPrecisionDigit tempMulResult; //除数与临时商位的临时结果
    while(true){
        // 每一轮除法首先获取必需的一位数，从被除数devidend获取到对其部分left。每取一位数，默认这一位的商数都是0
        if(index >= 0 )
        {
            left = dividend[ index-- ] + left ;
            result = char(0) + result;
        }
        //对于额外获取的位数，每获取一位，在商中添加一个0
        while(index >= 0 && divisor > HighPrecisionDigit(left, true)) {
            left = dividend[ index-- ] + left ;
            result = char(0) + result;
        }
        // 拿数完成，此时要么对齐部分已经大于除数了，要么dividend被使用完
        // 如果是left仍旧小于divisor的情况,则已经求到了余数，补全最后一位退出循环,否则继续进行除法
        if(divisor > HighPrecisionDigit(left, true)){
            remainder = left;
            break;
        }
        tempHigh = (HPD_SHORT)0x100;
        tempLow  = (HPD_SHORT)1;
        //二分法获得刚好小于left的第一个值，当达到结果时，刚好应取tempLow
        while(tempHigh != tempLow + 1) {
            tempMid = (tempHigh + tempLow) / 2;
            tempMulResult = divisor * tempMid;
            if(tempMulResult > HighPrecisionDigit(left, true))
                tempHigh = tempMid;
            else tempLow = tempMid;
        }
        //用获取的tempLow修改当前位的数字，当前位数字为最低位，应在0位置。
        result[0] = tempLow;
        /*cout<<"left is:"<<HighPrecisionDigit(left, true).getHEXNumber()<<endl;
        cout<<"tempLow is:"<<int(tempLow)<<endl;
        cout<<"result is:"<<HighPrecisionDigit(result, true).getHEXNumber()<<endl;*/
        //相减获取新的left值
        left = (HighPrecisionDigit(left, true) - (HighPrecisionDigit(divisor) * tempLow)).CHAR_Number;
        /*cout<<"new left is:"<<HighPrecisionDigit(left, true).getHEXNumber()<<endl;*/
    }
    HighPrecisionDigit returnValue(result, true);
    returnValue.zeroCut();
    returnValue.setNegative(this->isNegative() ^ other.isNegative());
    //cout<<"out divide!\n";
    return returnValue;
}

HighPrecisionDigit HighPrecisionDigit::operator/(const long other) {
    HighPrecisionDigit divisor(other);
    return (*this) / divisor;
}

HighPrecisionDigit HighPrecisionDigit::operator%(const HighPrecisionDigit &other) {
    if(other.CHAR_Number.length() == 0 ){
        cout<<"Error: trying to divide by zero!";
        return 0;
    }
    HighPrecisionDigit div = (*this);
    while(div.isNegative()){
        div = div + other;
    }
    string result, remainder; //定义商result, remainder作为余数
    string dividend = div.CHAR_Number; //被除数
    HighPrecisionDigit divisor = other; //除数
    string  left; //left作为除法的对齐部分
    int index = dividend.length() - 1; //index指示除法进行到的对齐位数,从被除数最高位开始
    HPD_SHORT tempHigh, tempLow, tempMid; // 二分法求商临时结果位时的上下界和中间值
    HighPrecisionDigit tempMulResult; //除数与临时商位的临时结果
    while(true){
        // 每一轮除法首先获取必需的一位数，从被除数devidend获取到对其部分left。每取一位数，默认这一位的商数都是0
        if(index >= 0 )
        {
            left = dividend[ index-- ] + left ;
            result = char(0) + result;
        }
        //对于额外获取的位数，每获取一位，在商中添加一个0
        while(index >= 0 && divisor > HighPrecisionDigit(left, true)) {
            left = dividend[ index-- ] + left ;
            result = char(0) + result;
        }
        // 拿数完成，此时要么对齐部分已经大于除数了，要么dividend被使用完
        // 如果是left仍旧小于divisor的情况,则已经求到了余数，补全最后一位退出循环,否则继续进行除法
        if(divisor > HighPrecisionDigit(left, true)){
            remainder = left;
            break;
        }
        tempHigh = (HPD_SHORT)0x100;
        tempLow  = (HPD_SHORT)1;
        //二分法获得刚好小于left的第一个值，当达到结果时，刚好应取tempLow
        while(tempHigh != tempLow + 1) {
            tempMid = (tempHigh + tempLow) / 2;
            tempMulResult = divisor * tempMid;
            if(tempMulResult > HighPrecisionDigit(left, true))
                tempHigh = tempMid;
            else tempLow = tempMid;
        }
        //用获取的tempLow修改当前位的数字，当前位数字为最低位，应在0位置。
        result[0] = tempLow;
        //相减获取新的left值
        left = (HighPrecisionDigit(left, true) - (HighPrecisionDigit(divisor) * tempLow)).CHAR_Number;
    }
    HighPrecisionDigit returnValue(remainder, true);
    returnValue.zeroCut();
    return returnValue;
}

HighPrecisionDigit HighPrecisionDigit::operator%(const long other) {
    HighPrecisionDigit divisor(other);
    return (*this) % divisor;
}

HighPrecisionDigit HighPrecisionDigit::operator^(const long other) {
    int times = other;
    HighPrecisionDigit result = (*this);
    while(--times){
        result = result * (*this);
    }
    return result;
}

HighPrecisionDigit HighPrecisionDigit::multiplyWithMod(const HighPrecisionDigit &other,
                                                       const HighPrecisionDigit &divisor) {
    return (*this)*other%divisor;
}

HighPrecisionDigit HighPrecisionDigit::powerWithMod(const HighPrecisionDigit &power,
                                                    const HighPrecisionDigit &divisor) {
    vector<HighPrecisionDigit> binaryBase = HighPrecisionDigit::getBinaryPowerList();//标志数组;
    vector<HighPrecisionDigit> remainderBase; //求余数组;
    remainderBase.push_back((*this) % divisor);//第一个余数A^1（mod n）加入余数数组
    int i = 0;
    //下标从0开始
    while(binaryBase[++i] < power){
        //如果当前power值仍不够大，则余数也继续由前一个余数平方取模生成
        //cout<<"BinaryBase "<<i<<" is "<<binaryBase[i].getHEXNumber()<<endl;
        remainderBase.push_back(remainderBase[i - 1].multiplyWithMod(remainderBase[i - 1], divisor));

        //cout<<"remainderBase "<<i<<" is "<<remainderBase[i].getHEXNumber()<<endl;
    }
    //cout<<"remainderBase end!\n";
    int index = binaryBase.size();
    HighPrecisionDigit rest = power, result = HighPrecisionDigit(1);
    while(rest > HighPrecisionDigit(0)){
        while(binaryBase[--index] > rest);//获取第一个小于rest的二进制值
        result = result.multiplyWithMod(remainderBase[index], divisor);//结果更新
        rest = rest - binaryBase[index];//指数更新
    }
    return result;
}
//-------公共函数-->



//<!------私有函数------
void HighPrecisionDigit::longToNumber(const long base) {
    int temp = base;
    while(temp != 0 ) {
        this->CHAR_Number += temp % HPD_BASE;
        temp /= HPD_BASE;
    }
}

//16进制字符串转内部char*结构,reverse表示是否逆序输入。默认是正序输入
//“0x123456789abcd”最左为高位表示正序，最右为高位表示逆序
void HighPrecisionDigit::hexStringToNumber(const string base) {
    int index = base.length() - 1;
    bool readOne = false;//标志位，表示是否已经读入第一位
    HPD_CHAR tempChar = 0;
    while(index >= 0 ){
        if(!readOne){
            tempChar = this->charToNumber(base[index]);
            readOne = true;
        }
        else{
            tempChar = tempChar + (this->charToNumber(base[index]) << 4);
            //读入两次，进行赋值
            this->CHAR_Number  += tempChar;
            readOne = false;
        }
        index--;
    }
    if(readOne)
        this->CHAR_Number  += tempChar;
}

HPD_CHAR HighPrecisionDigit::charToNumber(const char base) {
    if(base >= 48 && base <= 57)
        return char(base - 48);
    if(base <= 70) //大写F截至
        return char(base - 55);
    return char(base - 87); //小写f截至
}

char HighPrecisionDigit::numberToChar(HPD_CHAR base) {
    if(base < 10)
        return char(base + 48);
    return char(base + 55);
}
void HighPrecisionDigit::zeroCut() {
    unsigned int i = this->CHAR_Number.length() - 1;
    while(i >= 0 && this->CHAR_Number[i] == 0) i--;
    this->CHAR_Number = this->CHAR_Number.substr(0, i + 1);
}

vector<int> HighPrecisionDigit::getSmallNumberList() {
    vector<int> result =  HighPrecisionDigit::smallNumberList;
    if(!result.empty())
        return result;
    return HighPrecisionDigit::generateSmallNumberList();
}

vector<int> HighPrecisionDigit::generateSmallNumberList() {
    vector<int> result;
    bool mark = true;
    int  i,j;
    for(i = 2; i <= 1999; i++)
    {
        mark = true;
        for(j = 0; j < result.size(); j++){
            if(i % result[j] == 0){
                mark = false;
                break;
            }
        }
        if(mark)
            result.push_back(i);
    }
    HighPrecisionDigit::smallNumberList = result;
    return result;
}

vector<HighPrecisionDigit> HighPrecisionDigit::getBinaryPowerList() {
    vector<HighPrecisionDigit> result =  HighPrecisionDigit::binaryPowerList;
    if(!result.empty())
        return result;
    return HighPrecisionDigit::generateBinaryPowerList();
}

vector<HighPrecisionDigit> HighPrecisionDigit::generateBinaryPowerList() {
    vector<HighPrecisionDigit>binaryBase;
    binaryBase.push_back(HighPrecisionDigit(1));
    int index = 0;
    while( ++index < HighPrecisionDigit::binaryPowerRound)
        binaryBase.push_back(binaryBase[binaryBase.size() - 1] * 2);
    HighPrecisionDigit::binaryPowerList = binaryBase;
    return binaryBase;
}

HighPrecisionDigit HighPrecisionDigit::generateRandom(int bits) {
    int times = bits / 8;//256进制下生成的位数
    HighPrecisionDigit result;
    srand((unsigned int)time(NULL));
    for(int i = 0 ; i < times ; i++){
        result[i] = (HPD_CHAR) (rand() % HPD_BASE);
    }
    if(result[0] % 2 == 0)
        result[0]  = result[0] - (HPD_CHAR)1;
    result.zeroCut();
    return result;
}

HighPrecisionDigit HighPrecisionDigit::generateRandom(HighPrecisionDigit High, int leastBits, int mostBits) {
    int factor = rand() % (mostBits - leastBits) + leastBits; //生成leastBits到mostBits位的随机数,指二进制下的位数
    return HighPrecisionDigit::generateRandom(factor) % High;
}

bool HighPrecisionDigit::primeTest(HighPrecisionDigit obj) {
    if(HighPrecisionDigit::primePreTest(obj) == false)
    {
        cout<<"preTest failed!\n";
        return false;
    }
    cout<<"preTest Passed!\n";
    HighPrecisionDigit objMinusOne = obj - 1; //计算n-1
    HighPrecisionDigit factorJ = objMinusOne; // 计算因子J
    HighPrecisionDigit randomFactor,testNumber;
    HighPrecisionDigit factorK = 0; //计算因子K
    bool isPrime = true;
    while(factorJ.operator%(2) == 0){
        factorK = factorK + HighPrecisionDigit(1);
        factorJ = factorJ.operator/(2);
    }
    int round;
    cout<<"get K is:"<<factorK.getHEXNumber()<<endl;
    cout<<"get baseJ is:"<<factorJ.getHEXNumber()<<endl;
    for(round = 0; isPrime && round < HighPrecisionDigit::MRTestRound; round++){
        cout<<"Into Miller Rabin round Test!No."<<round<<"\n";
        randomFactor = HighPrecisionDigit::generateRandom(objMinusOne);//获取(1, n-1)范围的随机数A
        cout<<"round Miller Rabin test Factor is:"<<randomFactor.getHEXNumber()<<endl;
        testNumber = randomFactor.powerWithMod(factorJ, obj);
        cout<<"A^J is:"<<testNumber.getHEXNumber()<<endl;
        //如果最开始就是+1或者-1，则是质数,直接进行下一轮测试
        if(testNumber == HighPrecisionDigit(1) || testNumber == objMinusOne) {
            isPrime = true;
            continue;
        }
        //平方根检测k-1次
        for(HighPrecisionDigit index(1); index < factorK; index = index + HighPrecisionDigit(1)){
            testNumber = testNumber.multiplyWithMod(testNumber, obj);
            //计算新的平方
            //如果新的平方等于1，则说明不是质数，跳出循环
            if(testNumber == HighPrecisionDigit(1)) {
                isPrime = false;
                break;
            }
            //如果新的平方等于-1，则一定是质数，跳出循环，开始下一轮测试
            else if(testNumber == objMinusOne)
                break;
        }
        //循环进行结束,且没有得到-1，则一定是合数
        if(testNumber != objMinusOne) {
            isPrime = false;
            cout<<"K-1 round end, not get -1, it's not a prime.\n";
        }
    }
    /*if(isPrime)
        cout<<"it's prime!\n";
    else
        cout<<"it's not prime!\n";*/
    return isPrime;
}

bool HighPrecisionDigit::primePreTest(HighPrecisionDigit obj) {
    vector<int> testArray = HighPrecisionDigit::getSmallNumberList();
    for(int i = 0; i < testArray.size(); i++){
        HighPrecisionDigit temp(testArray[i]);
        if(obj.operator%(temp) == 0)
            return false;
    }
    return true;
}
//------私有函数------>