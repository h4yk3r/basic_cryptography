#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <ctime>
using namespace ::std;
int ip[8] = {2,6,3,1,4,8,5,7};
int inv_ip[8] = {4,1,3,5,7,2,8,6};
int p10[10] = {3,5,2,7,4,10,1,9,8,6};
int p8[8] = {6,3,7,4,8,5,10,9};
int EP[8] = {4,1,2,3,2,3,4,1};
int p4[4] = {2,4,3,1};
int s1[4][4] = {
    {1,0,3,2},
    {3,2,1,0},
    {0,2,1,3},
    {3,1,3,2}
};
int s2[4][4] = {
    {0,1,2,3},
    {2,0,1,3},
    {3,0,1,0},
    {2,1,0,3}
};

int p10_sub (int key)
{
    int o[11];
    for (int i=1; i<=10; ++i) {
        int n = 10-i;
        int shift = 1 << n;
        o[i] = (key & shift) >> n;
    }
    int sk = 0;
    for (int i=0; i!=10; ++i) {
        int temp = o[p10[i]];
        temp = temp << (9-i);
        sk += temp;
    }
    return sk;
}

int p8_sub (int key)
{
    int o[11];
    for (int i=1; i<=10; ++i) {
        int n = 10-i;
        int shift = 1 << n;
        o[i] = (key & shift) >> n;
    }
    int sk = 0;
    for (int i=0; i!=8; ++i) {
        int temp = o[p8[i]];
        temp = temp << (7-i);
        sk += temp;
    }
    return sk;
}

int ls_1 (int prt)
{
    int temp = 0;
    int temp1 = prt & 0b01111;
    temp1 = temp1 << 1;
    int temp2 = prt >> 4;
    temp = temp1 + temp2;
    return temp;
}

int ls_2 (int prt)
{
    int temp = 0;
    int temp1 = prt & 0b00111;
    temp1 = temp1 << 2;
    int temp2 = prt >> 3;
    temp = temp1 + temp2;
    return temp;
}

void key_expand (int key, int* sk)
{
    int temp = 0;
    int pk = p10_sub(key);
    //cout << "after p10: " << bitset<10>(pk) << endl;
    int l = (pk & 0b1111100000) >> 5;
    int r = pk & 0b11111;
    //cout << "L: " << bitset<5>(l) << endl;
    //cout << "R: " << bitset<5>(r) << endl;
    l = ls_1(l);
    r = ls_1(r);
    //cout << "after ls1: " << endl;
    //cout << bitset<5>(l) << endl;
    //cout << bitset<5>(r) << endl;

    temp = (l << 5) + r;
    sk[0] = p8_sub(temp);

    l = ls_2(l);
    r = ls_2(r);

    temp = (l << 5) + r;
    sk[1] = p8_sub(temp);
    return ;
}

int p4_sub (int text)
{
    int o[5];
    for (int i=1; i<=4; ++i) {
        int n = 4-i;
        int shift = 1 << n;
        o[i] = (text & shift) >> n;
    }
    int sk = 0;
    for (int i=0; i!=4; ++i) {
        int temp = o[p4[i]];
        temp = temp << (3-i);
        sk += temp;
    }
    return sk;
}

int ep_sub (int ori)
{
    int res = 0;
    int o[5];
    o[1] = (ori & 0b1000) >> 3;
    o[2] = (ori & 0b0100) >> 2;
    o[3] = (ori & 0b0010) >> 1;
    o[4] = ori & 0b0001;

    for (int i=0; i!=8; ++i) {
        int temp = o[EP[i]];
        temp = temp << (7-i);
        res += temp;
    }
    return res;
}

int s1_sub (int ori)
{
    int row = 0, col = 0;
    int fir = (0b1000 & ori) >> 3;
    int sec = (0b0100 & ori) >> 2;
    int tri = (0b0010 & ori) >> 1;
    int frz = 0b0001 & ori;

    row = (fir << 1) + frz;
    col = (sec << 1) + tri;

    return s1[row][col];
}

int s2_sub (int ori)
{
    int row = 0, col = 0;
    int fir = (0b1000 & ori) >> 3;
    int sec = (0b0100 & ori) >> 2;
    int tri = (0b0010 & ori) >> 1;
    int frz = 0b0001 & ori;

    row = (fir << 1) + frz;
    col = (sec << 1) + tri;

    return s2[row][col];
}

int f_function (int rin, int sk)
{
    int itm = ep_sub(rin);
    itm = itm ^ sk;
    int l = (itm & 0b11110000) >> 4;
    int r = itm & 0b1111;
    l = s1_sub(l);
    r = s2_sub(r);

    int temp = (l << 2) + r;
    int res = p4_sub(temp);
    return res;
}

int ip_sub (int text)
{
    int o[9];
    for (int i=1; i<=8; ++i) {
        int n = 8-i;
        int shift = 1 << n;
        o[i] = (text & shift) >> n;
    }
    int sk = 0;
    for (int i=0; i!=8; ++i) {
        int temp = o[ip[i]];
        temp = temp << (7-i);
        sk += temp;
    }
    return sk;
}

int inv_ip_sub (int text)
{
    int o[9];
    for (int i=1; i<=8; ++i) {
        int n = 8-i;
        int shift = 1 << n;
        o[i] = (text & shift) >> n;
    }
    int sk = 0;
    for (int i=0; i!=8; ++i) {
        int temp = o[inv_ip[i]];
        temp = temp << (7-i);
        sk += temp;
    }
    return sk;
}

int s_des_enc (int key, int plain)
{
    int sk[2];
    key_expand(key, sk);
    //cout << "sk1: " << bitset<8>(sk[0]) << endl;
    //cout << "sk2: " << bitset<8>(sk[1]) << endl;
    //cout << "********************" << endl;
    int pri = ip_sub(plain);
    //cout << "result of ip: " << bitset<8>(pri) << endl;
    int l0 = (0b11110000 & pri) >> 4;
    int r0 = (0b1111 & pri);
    int l1 = r0;
    int temp = f_function(r0, sk[0]);
    int r1 = l0 ^ temp;
    //cout << "first round: " << bitset<4>(l1) << " " << bitset<4>(r1) << endl;
    int r2 = r1;
    temp = f_function(r1, sk[1]);
    int l2 = l1 ^ temp;
    //cout << "second round: " << bitset<4>(l2) << " " << bitset<4>(r2) << endl;
    int res = (l2 << 4) + r2;
    res = inv_ip_sub(res);
    return res;
}

int s_des_dec (int key, int cipher)
{
    int sk[2];
    key_expand(key, sk);
    int pri = ip_sub(cipher);
    int l2 = (0b11110000 & pri) >> 4;
    int r2 = (0b1111 & pri);
    int r1 = r2;
    int temp = f_function(r2, sk[1]);
    int l1 = l2 ^ temp;
    int r0 = l1;
    temp = f_function(l1, sk[0]);
    int l0 = r1 ^ temp;
    int res = (l0 << 4) + r0;
    res = inv_ip_sub(res);
    return res;
}

int tri_sdes_enc (int k1, int k2, int plaintext)
{
    int s1=0, s2=0, c=0;
    s1 = s_des_enc(k1, plaintext);
    s2 = s_des_dec(k2, s1);
    c = s_des_enc(k1, s2);
    return c;
}

int tri_sdes_dec (int k1, int k2, int cipher)
{
    int s1=0, s2=0, p=0;
    s2 = s_des_dec(k1, cipher);
    s1 = s_des_enc(k2, s2);
    p = s_des_dec(k1, s1);
    return p;
}

int k1table[1024][2];

int testcipher[256];

bool tri_pass_test (int k1, int k2)
{
    for (int i=0; i!=256; ++i) {
        int temp = tri_sdes_enc(k1, k2, i);
        if (temp!=testcipher[i]) {
            return false;
        }
    }
    return true;
}

void mitma (int plain, int cipher)
{
    for (int i=0; i!=1024; ++i) {
        k1table[i][0] = s_des_enc(i, plain);
        k1table[i][1] = s_des_dec(i, cipher);
    }
    /*
    for (int i=0; i!=1024; ++i) {
        cout << "s1:" << bitset<8>(k1table[i][0]) << " s2:" << bitset<8>(k1table[i][1]) << endl;
    }
    */
    int cnt = 0;
    for (int i=0; i!=1024; ++i) {
        for (int j=0; j!=1024; ++j) {
            int stemp = s_des_enc(i, k1table[j][1]);
            if (stemp==k1table[j][0]) {
                if (tri_pass_test(j, i)) {
                    ++cnt;
                    cout << cnt << " possible key:" << endl;
                    cout << "k1: " << bitset<10>(j) << endl;
                    cout << "k2: " << bitset<10>(i) << endl;
                    return ;
                }
            }
        }
    }
    return ;
}

int main ()
{
    int key1 = 0b0111011101;
    int key2 = 0b1110010001;
    int plaintext = 0b01010110;
    int cipher = 0b10010010;
    cout << "*********************************" << endl;
    cout << "3 S-DES meet in the middle attack" << endl;
    cout << "*********************************" << endl;
    cout << "test info:" << endl;
    cout << "key1: " << bitset<10>(key1) << endl;
    cout << "key2: " << bitset<10>(key2) << endl;
    cout << "known plaintext: " << bitset<8>(plaintext) << endl;
    cout << "known cipher: " << bitset<8>(cipher) << endl;
    cout << "*********************************" << endl;
    clock_t start_t, end_t;
    cout << "start timing" << endl;
    start_t = clock();
    for (int i=0; i!=256; ++i) {
        testcipher[i] = tri_sdes_enc(key1, key2, i);
    }
    mitma(plaintext, cipher);
    end_t = clock();
    cout << "stop timing" << endl;
    //cout << endl;
    cout << "attack time: " << (double)(end_t - start_t)*1000 / CLOCKS_PER_SEC << "ms" << endl;
    cout << "*********************************" << endl;

    return 0;
}

/*
    int key1 = 0b0111011101;
    int key2 = 0b1110010001;
    int plaintext = 0b01010110;
    int cipher = 0b10010010;
    int ret;
    //ret = tri_sdes_enc(key1, key2, plaintext);
    ret = tri_sdes_dec(key1, key2, cipher);
    cout << bitset<8>(ret) << endl;
*/
