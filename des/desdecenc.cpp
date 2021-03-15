#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
using namespace ::std;
typedef bitset<64> b64;
typedef bitset<56> b56;
typedef bitset<48> b48;
typedef bitset<28> b28;
typedef bitset<32> b32;
typedef bitset<24> b24;
//133457799BBCDFF1

//初始置换表
const static unsigned int ip_table[64] = {
	58 , 50 , 42 , 34 , 26 , 18 , 10 , 2 , 60 , 52 , 44 , 36 , 28 , 20 , 12 , 4 ,
	62 , 54 , 46 , 38 , 30 , 22 , 14 , 6 , 64 , 56 , 48 , 40 , 32 , 24 , 16 , 8 ,
	57 , 49 , 41 , 33 , 25 , 17 , 9 , 1 , 59 , 51 , 43 , 35 , 27 , 19 , 11 , 3 ,
	61 , 53 , 45 , 37 , 29 , 21 , 13 , 5 , 63 , 55 , 47 , 39 , 31 , 23 , 15 , 7
} ;

//扩展置换，将数据从32位扩展为48位
static const unsigned int expa_perm[48] = {
	32 , 1 , 2 , 3 , 4 , 5 , 4 , 5 , 6 , 7 , 8 , 9 , 8 , 9 , 10 , 11 ,
	12 , 13 , 12 , 13 , 14 , 15 , 16 , 17 , 16 , 17 , 18 , 19 , 20 , 21 , 20 , 21 ,
	22 , 23 , 24 , 25 , 24 , 25 , 26 , 27 , 28 , 29 , 28 , 29 , 30 , 31 , 32 , 1
} ;

//S盒子代替
const static unsigned int sbox[8][64]={
       	{//S1盒子
	14 , 4 , 13 , 1 , 2 , 15 , 11 , 8 , 3 , 10 , 6 , 12 , 5 , 9 , 0 , 7 ,
       	0 , 15 , 7 , 4 , 14 , 2 , 13 , 1 , 10 , 6 , 12 , 11 , 9 , 5 , 3 , 8 ,
       	4 , 1 , 14 , 8 , 13 , 6 , 2 , 11 , 15 , 12 , 9 , 7 , 3 , 10 , 5 , 0 ,
       	15 , 12 , 8 , 2 , 4 , 9 , 1 , 7 , 5 , 11 , 3 , 14 , 10 , 0 , 6 , 13
} ,
	{//S2盒子
       	15 , 1 , 8 , 14 , 6 , 11 , 3 , 4 , 9 , 7 , 2 , 13 , 12 , 0 , 5 , 10 ,
       	3 , 13 , 4 , 7 , 15 , 2 , 8 , 14 , 12 , 0 , 1 , 10 , 6 , 9 , 11 , 5 ,
       	0 , 14 , 7 , 11 , 10 , 4 , 13 , 1 , 5 , 8 , 12 , 6 , 9 , 3 , 2 , 15 ,
       	13 , 8 , 10 , 1 , 3 , 15 , 4 , 2 , 11 , 6 , 7 , 12 , 0 , 5 , 14 , 9
} ,
	{//S3盒子
       	10 , 0 , 9 , 14 , 6 , 3 , 15 , 5 , 1 , 13 , 12 , 7 , 11 , 4 , 2 , 8 ,
       	13 , 7 , 0 , 9 , 3 , 4 , 6 , 10 , 2 , 8 , 5 , 14 , 12 , 11 , 15 , 1 ,
       	13 , 6 , 4 , 9 , 8 , 15 , 3 , 0 , 11 , 1 , 2 , 12 , 5 , 10 , 14 , 7 ,
       	1 , 10 , 13 , 0 , 6 , 9 , 8 , 7 , 4 , 15 , 14 , 3 , 11 , 5 , 2 , 12
} ,
	{//S4盒子
       	7 , 13 , 14 , 3 , 0 , 6 , 9 , 10 , 1 , 2 , 8 , 5 , 11 , 12 , 4 , 15 ,
       	13 , 8 , 11 , 5 , 6 , 15 , 0 , 3 , 4 , 7 , 2 , 12 , 1 , 10 , 14 , 9 ,
       	10 , 6 , 9 , 0 , 12 , 11 , 7 , 13 , 15 , 1 , 3 , 14 , 5 , 2 , 8 , 4 ,
       	3 , 15 , 0 , 6 , 10 , 1 , 13 , 8 , 9 , 4 , 5 , 11 , 12 , 7 , 2 , 14
} ,
       	{//S5盒子
	2 , 12 , 4 , 1 , 7 , 10 , 11 , 6 , 8 , 5 , 3 , 15 , 13 , 0 , 14 , 9 ,
       	14 , 11 , 2 , 12 , 4 , 7 , 13 , 1 , 5 , 0 , 15 , 10 , 3 , 9 , 8 , 6 ,
       	4 , 2 , 1 , 11 , 10 , 13 , 7 , 8 , 15 , 9 , 12 , 5 , 6 , 3 , 0 , 14 ,
       	11 , 8 , 12 , 7 , 1 , 14 , 2 , 13 , 6 , 15 , 0 , 9 , 10 , 4 , 5 , 3
} ,
       	{//S6盒子
	12 , 1 , 10 , 15 , 9 , 2 , 6 , 8 , 0 , 13 , 3 , 4 , 14 , 7 , 5 , 11 ,
       	10 , 15 , 4 , 2 , 7 , 12 , 9 , 5 , 6 , 1 , 13 , 14 , 0 , 11 , 3 , 8 ,
       	9 , 14 , 15 , 5 , 2 , 8 , 12 , 3 , 7 , 0 , 4 , 10 , 1 , 13 , 11 , 6 ,
       	4 , 3 , 2 , 12 , 9 , 5 , 15 , 10 , 11 , 14 , 1 , 7 , 6 , 0 , 8 , 13
} ,
	{//S7盒子
       	4 , 11 , 2 , 14 , 15 , 0 , 8 , 13 , 3 , 12 , 9 , 7 , 5 , 10 , 6 , 1 ,
       	13 , 0 , 11 , 7 , 4 , 9 , 1 , 10 , 14 , 3 , 5 , 12 , 2 , 15 , 8 , 6 ,
       	1 , 4 , 11 , 13 , 12 , 3 , 7 , 14 , 10 , 15 , 6 , 8 , 0 , 5 , 9 , 2 ,
       	6 , 11 , 13 , 8 , 1 , 4 , 10 , 7 , 9 , 5 , 0 , 15 , 14 , 2 , 3 , 12
} ,
	{//S8盒子
       	13 , 2 , 8 , 4 , 6 , 15 , 11 , 1 , 10 , 9 , 3 , 14 , 5 , 0 , 12 , 7 ,
       	1 , 15 , 13 , 8 , 10 , 3 , 7 , 4 , 12 , 5 , 6 , 11 , 0 , 14 , 9 , 2 ,
       	7 , 11 , 4 , 1 , 9 , 12 , 14 , 2 , 0 , 6 , 10 , 13 , 15 , 3 , 5 , 8 ,
       	2 , 1 , 14 , 7 , 4 , 10 , 8 , 13 , 15 , 12 , 9 , 0 , 3 , 5 , 6 , 11
}
} ;

//密钥压缩64置56
const static unsigned int key_table[56] = {
	57 , 49 , 41 , 33 , 25 , 17 , 9 , 1 ,
	58 , 50 , 42 , 34 , 26 , 18 , 10 , 2 ,
	59 , 51 , 43 , 35 , 27 , 19 , 11 , 3 ,
	60 , 52 , 44 , 36 , 63 , 55 , 47 , 39 ,
	31 , 23 , 15 , 7 , 62 , 54 , 46 , 38 ,
	30 , 22 , 14 , 6 , 61 , 53 , 45 , 37 ,
	29 , 21 , 13 , 5 , 28 , 20 , 12 , 4
} ;

//密钥压缩56置48
const static unsigned int comp_perm[48] = {
	14 , 17 , 11 , 24 , 1 , 5 , 3 , 28 ,
	15 , 6 , 21 , 10 , 23 , 19 , 12 , 4 ,
	26 , 8 , 16 , 7 , 27 , 20 , 13 , 2 ,
	41 , 52 , 31 , 37 , 47 , 55 , 30 , 40 ,
	51 , 45 , 33 , 48 , 44 , 49 , 39 , 56 ,
	34 , 53 , 46 , 42 , 50 , 36 , 29 , 32
} ;

//每轮移动的位数
const static unsigned int bit_shift[16] = {
	1 , 1 , 2 , 2 , 2 , 2 , 2 , 2 , 1 , 2 , 2 , 2 , 2 , 2 , 2 , 1
} ;

//P盒置换
const static unsigned int p_table[32] = {
	16 , 7 , 20 , 21 , 29 , 12 , 28 , 17 , 1 , 15 , 23 , 26 , 5 , 18 , 31 , 10 ,
	2 , 8 , 24 , 14 , 32 , 27 , 3 , 9 , 19 , 13 , 30 , 6 , 22 , 11 , 4 , 25
} ;

//末置换
const static unsigned int ipr_table[64] = {
	40 , 8 , 48 , 16 , 56 , 24 , 64 , 32 , 39 , 7 , 47 , 15 , 55 , 23 , 63 , 31 ,
	38 , 6 , 46 , 14 , 54 , 22 , 62 , 30 , 37 , 5 , 45 , 13 , 53 , 21 , 61 , 29 ,
	36 , 4 , 44 , 12 , 52 , 20 , 60 , 28 , 35 , 3 , 43 , 11 , 51 , 19 , 59 , 27 ,
	34 , 2 , 42 , 10 , 50 , 18 , 58 , 26 , 33 , 1 , 41 , 9 , 49 , 17 , 57 , 25
} ;

void exchange (b32 &left, b32 &right)
{
    b32 temp;
    decltype(temp.size()) i = 0;
    for (i = 0; i!=temp.size(); ++i) {
        temp[i] = left[i];
    }
    for (i = 0; i!=left.size(); ++i) {
        left[i] = right[i];
    }
    for (i = 0; i!=right.size(); ++i) {
        right[i] = temp[i];
    }
}

//初置换64置32-32
void initial_p (const b64 &block, b32 &left, b32 &right)
{
    decltype(left.size()) i = 0;
    for (i=0; i!=left.size(); ++i) {
        left[i] = block[ip_table[i] - 1];
    }
    for (i=0; i!=right.size(); ++i) {
        right[i] = block[ip_table[i + right.size()] - 1];
    }
    return;
}

//末置换32-32置64
void inverse_initial_p (const b32 &left, const b32 &right, b64 &block)
{
    decltype(block.size()) i = 0;
    for (i = 0; i!=block.size(); ++i) {
        if (ipr_table[i]<=32) {
            block[i] = right[ipr_table[i] - 1];
        } else {
            block[i] = left[ipr_table[i] - 32 - 1];
        }
    }
    return;
}

//获取第n轮的子密钥
b48 get_subkey (int n, const b64 &rawKey)
{
    b48 res;
    b56 temp;
    //取出校验位得到56位密钥:64-56 decltype(temp.size())
    for (int i=0; i!=56; ++i) {
        temp[i] = rawKey[key_table[i] - 1];
    }
    //cout << n << " 56bit: " << temp << endl;
    //<del>分开，并根据加密轮数n进行左移</del>全体右移
    b56 t56;

    for (int i = 0; i<=n; ++i) {
    //int i = n;
        int j = 0;
        for (j = 0; j < bit_shift[i]; ++j) {
            t56[j] = temp[j];
            t56[28 + j] = temp[28 + j];
        }
        temp >>= bit_shift[i];
        for (j=0; j<bit_shift[i]; ++j) {
            temp[28 - bit_shift[i] + j] = t56[j];
            temp[56 - bit_shift[i] + j] = t56[28 + j];
        }
    }
    //cout << n << " 56bit subkey: " << temp << endl;
    //48-56
    for (decltype(res.size()) i = 0; i != res.size(); ++i)
    {
        res[i] = temp[comp_perm[i] - 1];
    }
    //cout << n+1 << " 48bit subkey: " << res << endl;
    return res;
}

void des_turn (b32 &left, b32 &right, const b48 &subkey)
{
    b48 temp48;
    b32 temp32;
    //function:f begin
    //right:32-48
    decltype(temp48.size()) i = 0;
    for (i=0; i!=temp48.size(); ++i) {
        temp48[i] = right[expa_perm[i] - 1];
    }
    temp48 ^= subkey;
    //s-box
    bitset<2> row;//行
    bitset<4> col;//列
    for (int j=0; j!=8; ++j) {
        //6个一组
        row[0] = temp48[6 * j + 5];
        row[1] = temp48[6 * j + 0];
        col[0] = temp48[6 * j + 4];
        col[1] = temp48[6 * j + 3];
        col[2] = temp48[6 * j + 2];
        col[3] = temp48[6 * j + 1];
        auto srow = row.to_ulong() * 16;
        auto scol = col.to_ulong()+1;
        auto s = srow + scol;
        bitset<4> temp(sbox[j][s-1]);
        for (int k=0; k!=4; ++k) {
            temp48[4 * j + k] = temp[3-k];
        }
    }
    decltype(temp32.size()) q = 0;
    for (q=0; q<temp32.size(); ++q) {
        temp32[q] = temp48[p_table[q] - 1];
    }
    //function:f end
    left ^= temp32;
    return;
}

void des_enc (b64 &block, b64 &rawKey)
{
    b32 left, right;
    initial_p(block, left, right);
    //cout << "initial: left: " << left << " right: " << right << endl;
    //16轮加密
    for (int i=0; i!=16; ++i) {
        //cout << "L" << i << ": " << left << " R" << i << ": " << right << endl;
        b48 subKey = get_subkey(i, rawKey);
        des_turn(left, right, subKey);
        //cout << i << " out: left: " << left << " right: " << right << endl;
        if (i!=16) {//面向数据编程，再看一下这里的L16R16的顺序问题，到底要不要交换
            exchange(left, right);
        }
    }
    //cout << "L16: " << left << " R16: " << right << endl;
    inverse_initial_p(left, right, block);
    return;
}

void des_dec (b64 &block, b64 &rawKey)
{
    b32 left, right;
    initial_p(block, left, right);
    //16轮
    for (int i=15; i>=0; --i) {
        b48 subKey = get_subkey(i, rawKey);
        des_turn(left, right, subKey);
        //if (i!=0) {
            exchange(left, right);
        //}
    }
    inverse_initial_p(left, right, block);
    return;
}

void hextobin (string hex, b64 &tar)
{
    int n = 0;
    b64 invtar;
    //hex to dec
    for (int i=0; i!=16; ++i) {
        if (hex[i]>='a' && hex[i]<='f') {
            n = hex[i] - 'a' + 10;
        } else if (hex[i]>='A' && hex[i]<='F') {
            n = hex[i] - 'A' + 10;
        } else if (hex[i]>='0' && hex[i]<='9') {
            n = hex[i] - '0';
        }
        bitset<4> temp = bitset<4>(n);
        //cout << temp << endl;
        for (int j=0; j!=4; ++j) {
            invtar[63 - 4 * i - j] = temp[3 - j];
        }
    }
    for (int i = 0; i!=64; ++i) {
        tar[i] = invtar[63 - i];
    }
    return;
}

string bintohex (b64 block)
{
    b64 invblock;
    for (int i=0; i!=64; ++i) {
        invblock[i] = block[63 - i];
    }
    string tar;
    char bin_table[16] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'a','b','c','d','e','f'
    };
    bitset<4> temp;
    for (int i=0; i!=16; ++i) {
        for (int j=0; j!=4; ++j) {
            temp[j] = invblock[60-4*i+j];
        }
        int n = temp.to_ulong();
        tar.push_back(bin_table[n]);
    }
    return tar;
}

//解密
void dec ()
{
    b64 block;
    b64 rawKey;
    string hexcipher;
    string hexkey;
    cout << "* enter the key in hex(0x......): " << endl;
    cin >> hexkey;
    cout << "* enter the ciphertext in hex(0x......): " << endl;
    cin >> hexcipher;
    cout << endl;
    for (int i=0; i<36; ++i) {
        cout << "-";
    }
    cout << "* the result is: " << endl;
    hextobin(hexkey, rawKey);
    hextobin(hexcipher, block);
    des_dec(block, rawKey);
    string res = bintohex(block);
    cout << "0x" << res;
    return;
}

//加密
void enc ()
{
    /*
    ifstream src("src.txt");
    ofstream tar("tar.txt");
    */
    b64 block;
    b64 rawKey;
    string hexplain;
    string hexkey;
    cout << "* enter the key in hex(0x......): " << endl;
    cin >> hexkey;
    hextobin(hexkey, rawKey);
    //cout << "rawKey: " << rawKey << endl;
    cout << "* enter the plaintext in hex(0x......): " << endl;
    cin >> hexplain;
    hextobin(hexplain, block);
    //cout << "block: " << block << endl;
    cout << endl;
    for (int i=0; i<36; ++i) {
        cout << "-";
    }
    cout << "* the result is: " << endl;
    des_enc(block, rawKey);
    string res = bintohex(block);
    cout << "0x" << res;
    //cout << block;
    return;
}

int main ()
{
    char option;
    for (int i=0; i!=48; ++i) {
        cout << '*';
    }
    cout << endl;
    cout << "       "
         << "---------------" << endl;
    cout << "       " << "| "
         << "DES enc&dec" << " |" << endl;
    cout << "       "
         << "---------------" << endl;
    cout << "* enter e to encrpt, d to decrypt:" << endl;
    cin >> option;
    if (option=='d') {//解密
        dec();
    } else if (option=='e') {//加密
        enc();
    }
    cout << endl;
    for (int i=0; i!=48; ++i) {
        cout << '*';
    }
    cout << endl;
    system("pause");
    return 0;
}
