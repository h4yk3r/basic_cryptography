#ifndef RSAUTILS_H
#define RSAYTILS_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <gmpxx.h>

using namespace ::std;

//接受一个seed, 得到cnt(-1)位的随机数，以string返回
string bbs_prng (unsigned long long s, unsigned int cnt);
//快速模幂, res = a^b mod c
void modpow(mpz_class a, mpz_class b, mpz_class c, mpz_class &res);
//素性测试
bool mr_isPrime (mpz_class n);
//gcd(a, b)
void easygcd (mpz_class a, mpz_class b, mpz_class &r);
//if (a, b) = 1, =1 return true
bool rp_jud (mpz_class a, mpz_class b);
//求a模n的乘法逆元（a, n互素的情况下）
void mul_inv (mpz_class a, mpz_class n, mpz_class &inverse);
//获取二进制位数
int get_len (mpz_class n);

#endif
