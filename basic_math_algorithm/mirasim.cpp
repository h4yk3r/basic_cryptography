//最终优化版
#include <iostream>
#include <gmpxx.h>
#include <time.h>
using namespace ::std;

void seperate (mpz_t n, mpz_t &k, mpz_t &q)
{
    mpz_set_ui(k, 0);
    mpz_set(q, n);
    mpz_sub_ui(q, q, 1);
    mpz_t r;
    mpz_init(r);
    mpz_mod_ui(r, q, 2);
    while (mpz_cmp_ui(r, 0)==0) {
        mpz_fdiv_q_ui(q, q, 2);
        mpz_add_ui(k, k, 1);
        mpz_mod_ui(r, q, 2);
    }
    return;
}

bool seccheck (mpz_t a, mpz_t n, mpz_t k, mpz_t q)
{
    mpz_t s;
    mpz_init_set(s, n);
    mpz_sub_ui(s, s, 1);
    mpz_t j;
    mpz_init(j);
    mpz_t p;
    mpz_init_set(p, q);
    mpz_t r;
    mpz_init(r);
    for (mpz_set_ui(j, 0); mpz_cmp(j,k)<0; mpz_add_ui(j, j, 1)) {
        mpz_powm(r, a, p, n);
        mpz_mul_ui(p, p, 2);
        if (mpz_cmp(r,s)==0) {
            return true;
        }
    }
    return false;
}

int main ()
{
    clock_t startTime, endTime;
    mpz_t n;
    mpz_init(n);
    cout << "Enter a number for test: ";
    cin >> n;
    startTime = clock();
    mpz_t k;
    mpz_t q;
    mpz_init(k);
    mpz_init(q);
    seperate(n, k, q);

    mpz_t a;
    mpz_init_set(a, n);
    mpz_sub_ui(a, a, 1);
    mpz_fdiv_q_ui(a, a, 2);

    mpz_t temp;
    mpz_init(temp);
    mpz_powm(temp, a, q, n);

    bool isPrime = false;
    if (mpz_cmp_ui(temp, 1)==0) {
        isPrime = true;
    } else {
        //seccheck
        isPrime = seccheck(a, n, k, q);
    }
    if (isPrime) {
        cout << "is a prime" << endl;
    } else {
        cout << "is not" << endl;
    }
    endTime = clock();
    cout << "Total time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

    system("pause");
    return 0;
}