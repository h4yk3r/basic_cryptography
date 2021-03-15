#include <iostream>
#include <vector>
#include <gmpxx.h>
#include <time.h>
using namespace ::std;

void inverse (mpz_t a, mpz_t b, mpz_t &ia) 
{
    mpz_t y, gcd;
    mpz_init_set_ui(y, 0);
    mpz_init_set_ui(gcd, 0);
    mpz_gcdext(gcd, ia, y, a, b);
    cout << ia << "*" << a << "+" << y << "*" << b << "=" << gcd << endl;
    while (mpz_cmp_ui(ia, 0)<0) {
        mpz_add(ia, ia, b);
    }
    return;
}

int main ()
{
    clock_t startTime, endTime;
    //mpz_gcdext(gcd, x, y, a, b);
    mpz_t a[3], n[3], m[3], im[3], c[3];
    for (int i = 0; i < 3; ++i) {
        mpz_init_set_ui(a[i], 0);
        mpz_init_set_ui(n[i], 0);
        mpz_init_set_ui(m[i], 0);
        mpz_init_set_ui(im[i], 0);
        mpz_init_set_ui(c[i], 0);
    }
    mpz_t at, nt;
    mpz_init_set_ui(at, 0);
    mpz_init_set_ui(nt, 0);
    for (int i = 0; i < 3; ++i) {
        cin >> at >> nt;
        mpz_set(a[i], at);
        mpz_set(n[i], nt);
    }
    startTime = clock();
    mpz_t M;
    mpz_init_set_ui(M, 1);
    for (int i = 0; i < 3; ++i) {
        mpz_mul(M, M, n[i]);
    }
    for (int i = 0; i < 3; ++i) {
        mpz_fdiv_q(m[i], M, n[i]);
    }
    for (int i = 0; i < 3; ++i) {
        inverse(m[i], n[i], im[i]);
    }
    for (int i = 0; i < 3; ++i) {
        mpz_mul(c[i], m[i], im[i]);
    }
    mpz_t temp, res;
    mpz_init_set_ui(temp, 1);
    mpz_init_set_ui(res, 0);
    for (int i = 0; i < 3; ++i) {
        mpz_mul(temp, a[i], c[i]);
        mpz_add(res, res, temp);
    }
    cout << "the solution is " << res << " mod " << M << " (origin)" << endl;
    //大数不断缩小会十分耗时
    while (mpz_cmp(res, M) > 0 || mpz_cmp_ui(res, 0) < 0) {
        mpz_t mq;
        mpz_init_set_ui(mq, 0);
        if (mpz_cmp(res, M) > 0) {
            mpz_fdiv_q(mq, res, M);
            mpz_mul(mq, mq, M);
            mpz_sub(res, res, mq);
        }
        //其实不会有负数
        if (mpz_cmp_ui(res, 0) < 0) {
            mpz_add(res, res, M);
        }
    }
    cout << "the solution is " << res << " mod " << M << " (simplified)" << endl;
    endTime = clock();
    cout << "Total time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

    system("pause");

    return 0;
}