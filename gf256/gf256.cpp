#include <iostream>
#include <bitset>
#include <time.h>
#include <cmath>
#include <gmpxx.h>
using namespace ::std;
#define ORI 0b100011011 //ox11B
#define G 0b11 //x+1
int table[256];
int arc_table[256];
int inv_table[256];
int all_table[256];
/*
* 直接使用与运算和异或运算、位移运算进行操作
* 所有的运算数（幂次除外）都有范围限制
* 即不能超过256
*/

//加法与减法在有限群的意义下是等价的
int gf_add (int a, int b)
{
    //XOR
    int res = 0;
    res = a ^ b;
    return res;
}
//打表算法
void gf_table ()
{
    table[255] = 1;
    table[0] = 1;
    //经过尝试，生成元为x+1时计算速度很快，是生成元为x的情况的20倍
    table[1] = G;
    int temp = 0;
    for (int i = 2; i<255; ++i) {
        temp = table[i - 1] << 1;
        table[i] = temp ^ table[i - 1];
        //模去不可约多项式
        if (table[i] & 0b100000000) {
            table[i] = table[i] ^ ORI;
        }
    }
    return;
}

void gf_arc_table () 
{
    //反表不考虑0
    for (int i = 0; i<255; ++i) {
        arc_table[table[i]] = i;
    }
    return;
}

void gf_inv_table ()
{
    //0没有逆元
    for (int i = 1; i<255; ++i) {
        inv_table[i] = table[255 - i];
    }
    return;
}
//乘法
int gf_mul (int a, int b)
{
    if (!a || !b) {
        return 0;
    }
    int ap = arc_table[a];//得到a, b对应的幂次
    int bp = arc_table[b];
    int rp = ap + bp;
    rp %= 255;
    return table[rp];
}
/*
int gf_cdiv (int a, int b)
{
    int res = 0;
    if (!a || !b) {
        return 0;
    }
    if (a==b) {
        return 1;
    }
    if (a>b) {
        res = gf_mul(a, inv_table[arc_table[b]]);
    } else {
        res = gf_mul(b, inv_table[arc_table[a]]);
    }
    //整除
    return res;
}
*/
//带余除法，返回商
int gf_fdiv_q (int a, int b)
{
    int q = 0;
    int temp = 0;
    int r = 0;
    while(1) {
        temp = gf_mul(q, b);
        r = gf_add(a, temp);
        if (r>=b) {
            ++q;
        } else {
            return q;
        }
    }
}
//带余除法，返回余数
int gf_fdiv_r (int a, int b)
{
    int r = 0;
    int temp = 0;
    int q = 0;
    while (1) {
        temp = gf_mul(q, b);
        r = gf_add(a, temp);
        if (r>=b) {
            ++q;
        } else {
            return r;
        }
    }
}
//乘法幂次，int范围，可能偏小
int gf_binpow_mul (int a, int n)
{
    int res = 1;
    while (n>0) {
        if (n&1) {
            res = gf_mul(res, a);
            if (res & 0b100000000) {
                res ^= ORI;
            }
        }
        a = gf_mul(a, a);
        if (a & 0b100000000) {
            a ^= ORI;
        }
        n = n >> 1;
    }
    return res;
}
//乘法模幂，a的n次幂模上m
int gf_modpow_mul (int a, int n, int m)
{
    int res = 1;
    while (n>0) {
        if (n&1) {
            res = gf_mul(res, a);
            if (res & 0b100000000) {
                res ^= ORI;
            }
            res = gf_fdiv_r(res, m);
        }
        a = gf_mul(a, a);
        if (a & 0b100000000) {
            a ^= ORI;
        }
        a = gf_fdiv_r(a, m);
        n = n >> 1;
    }
    return res;
}
//GF256的欧几里得算法，求a, b的公因数
int gf_gcd (int a, int b)
{
    int r;
    while (b > 0) {
        r = gf_fdiv_r(a, b);
        a = b;
        b = r;
    }
    return a;
}
//GF256的扩展欧几里得算法，求模a的逆元
/* 会溢出，求逆元直接使用inv_table即可
int gf_exgcd (int a)
{
    int b = ORI;
    int x0 = 1, x1 = 0;
    int y0 = 0, y1 = 1;
    int q = gf_fdiv_q(a, b);
}
*/
//求逆元
int gf_get_inv (int a)
{
    if (a==1) {
        return 1;
    }
    return inv_table[arc_table[a]];
}

//乘法，广义，返回乘积，没有模
int gf_mul_ori (int a, int b)
{
    int len = 0;
    int sig = 1;
    int pos = 0;
    int res = 0;
    int temp = 0;
    len = (int)(log(b) / log(2));
    while (pos<=len) {
        if (b&sig) {
            temp = a << pos;
            //res += temp;
            res = gf_add(res, temp);
        }
        sig <<= 1;
        ++pos;
    }
    return res;
}

//除法，广义，a/b，返回商，没有模
int gf_div_ori_q (int a, int b)
{
    int ap = 0, qp = 0;
    int bp = (int)(log(b) / log(2));
    //int q = 1 << qp;
    int temp = 0;
    int q = 0;
    //int r = a;
    while (a>=b) {
        //temp = gf_mul_ori(q, b);
        ap = (int)(log(a) / log(2));
        qp = abs(ap - bp);
        temp = b << qp;
        a = gf_add(a, temp);
        qp = 1 << qp;
        q = gf_add(q, qp);
    }
    return q;
}

//返回余数，没有模
int gf_div_ori_r (int a, int b)
{
    int ap = 0, qp = 0;
    int bp = (int)(log(a) / log(2));
    //int q = 1 << qp;
    int temp = 0;
    int q = 0;
    //int r = a;
    while (a>=b) {
        //temp = gf_mul_ori(q, b);
        ap = (int)(log(a) / log(2));
        qp = abs(ap - bp);
        q = gf_add(q, qp);
        temp = b << qp;
        a = gf_add(a, temp);
    }
    return a;
}

//大整数专场，没有限制，但是也没有取模
//加法
void gf_mp_add (mpz_t a, mpz_t b, mpz_t &res)
{
    mpz_xor(res, a, b);
}
//返回最高位幂次
int gf_mp_log2 (mpz_t a)
{
    mpz_t res;
    int cnt = 0;
    mpz_init_set(res, a);
    while (1) {
        if (mpz_cmp_ui(res, 2)<0) {
            return cnt;
        } else {
            mpz_fdiv_q_ui(res, res, 2);
            ++cnt;
        }
    }
    return 0;
}
//乘法
void gf_mp_mul (mpz_t a, mpz_t b, mpz_t &res)
{
    mpz_t sig, temp, flag;
    int len = 0, pos = 0;
    mpz_init_set(temp, a);
    mpz_init_set_ui(flag, 0);
    mpz_init_set_ui(sig, 1);
    mpz_set_ui(res, 0);
    len = gf_mp_log2(b);
    while (pos<=len) {
        mpz_and(flag, b, sig);
        if (flag) {
            for (int i = 0; i<pos; ++i) {
                mpz_mul_ui(temp, temp, 2);
            }
            gf_mp_add(res, temp, res);
        }
        mpz_mul_ui(sig, sig, 2);
        ++pos;
    }
    return;
}

void gf_mp_div_q (mpz_t a, mpz_t b, mpz_t &res)
{
    int bp = gf_mp_log2(b);
    int ap = 0, qp = 0;
    mpz_t temp;
    mpz_t r;
    mpz_t q;
    mpz_init_set_ui(q, 1);
    mpz_init_set(r, a);
    mpz_init_set(temp, b);
    while (mpz_cmp(r, b)>=0) {
        ap = gf_mp_log2(r);
        qp = abs(ap - bp);
        for (int i=0; i<qp; ++i) {
            mpz_mul_ui(temp, temp, 2);
            mpz_mul_ui(q, q, 2);
        }
        gf_mp_add(r, temp, r);
        gf_mp_add(res, q, res);
        mpz_set_ui(q, 1);
    }
}

void gf_mp_div_r (mpz_t a, mpz_t b, mpz_t &res)
{
    int bp = gf_mp_log2(b);
    int ap = 0, qp = 0;
    mpz_t temp;
    mpz_t r;
    mpz_init_set(r, a);
    mpz_init_set(temp, b);
    while (mpz_cmp(r, b)>=0) {
        ap = gf_mp_log2(r);
        qp = abs(ap - bp);
        mpz_set(temp, b);
        for (int i=0; i<qp; ++i) {
            mpz_mul_ui(temp, temp, 2);
        }
        gf_mp_add(r, temp, r);
    }
    mpz_set(res, r);
    return;
}

bool gf_mp_polynomial (int test)
{
    int n = 8;
    unsigned int m = 255;
    mpz_t M;
    mpz_init_set_ui(M, 0);
    mpz_ui_pow_ui(M, 2, 255);
    mpz_add_ui(M, M, 1);
    //cout << M << endl;

    mpz_t temp;
    mpz_init_set_ui(temp, test);
    mpz_t temp1;
    mpz_t temp2;
    mpz_init_set_ui(temp1, 0);
    mpz_init_set_ui(temp2, 0);

    mpz_t r, r1, r2;
    mpz_init_set_ui(r, 0);
    mpz_init_set_ui(r1, 0);
    mpz_init_set_ui(r2, 0);
    
    for (int i = 2; i<test; ++i) {
        mpz_set_ui(temp1, i);
        gf_mp_div_r(temp, temp1, r);
        if (mpz_cmp_ui(r, 0)==0) {
            return false;
        }
    }

    mpz_t fm;
    mpz_init_set_ui(fm, 0);
    gf_mp_div_r(M, temp, r1);
    if (mpz_cmp_ui(r1, 0)==0) {
        for (int p = n; p<m; ++p) {
            mpz_ui_pow_ui(fm, 2, p);
            mpz_add_ui(fm, fm, 1);
            gf_mp_div_r(fm, temp, r2);
            if (mpz_cmp_ui(r2, 0)==0) {
                return false;
            }
        }
    }
        return true;
}

void gf_mp_print ()
{
    for (int i = 256; i < 512; ++i) {
        if (gf_mp_polynomial(i)) {
            cout << bitset<16>(i) << endl;
        }
    }
    return;
}

int main ()
{
    clock_t startTime, endTime;
    bitset<16> bita;
    //int b;
    //bitset<16> bitb;
    int in = 1;
    while (in==1) {
        cin >> bita;
        startTime = clock();
        gf_table();
        gf_arc_table();
        gf_inv_table();
        
        int a;
        a = bita.to_ulong();
        //b = bitb.to_ulong();
        int res = 0;

        //加法
        //res = gf_add(a, b);
        //乘法
        //res = gf_mul(a, b);
        //除法
        /*
        int q = 0, r = 0;
        q = gf_fdiv_q(a, b);
        r = gf_fdiv_r(a, b);
        cout << bitset<8>(q) << endl;
        cout << bitset<8>(r) << endl;
        */
        //模幂
        //res = gf_modpow_mul(a, b, ORI);
        //gcd
        //res = gf_gcd(a, b);
        //逆元
        res = gf_get_inv(a);
        //本原多项式
        //gf_mp_print();

        cout << bitset<8>(res) << endl;
        endTime = clock();
        cout << "Total time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
        
        system("pause");
        cin >> in;
    }

    return 0;
}