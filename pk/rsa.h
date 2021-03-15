#ifndef RSA_H
#define RSA_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <gmpxx.h>

using namespace ::std;

void menu ();

bool mr_isPrime (mpz_t n);

string bbs_prng (unsigned long long s, unsigned int cnt);

void easygcd (mpz_t a, mpz_t b, mpz_t &r);

bool rp_jud (mpz_t a, mpz_t b);

void mul_inv (mpz_t a, mpz_t n, mpz_t &inverse);

void modpow(mpz_t a, mpz_t b, mpz_t c, mpz_t &res);

#endif
