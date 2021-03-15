#include <iostream>
#include <string>
#include <vector>
#include <gmp.h>
#include <time.h>
using namespace ::std;

int main ()
{
    clock_t startTime, endTime;
    mpz_t length, i, j, k;
    mpz_init(length);
    mpz_init(i);
    mpz_init(j);
    mpz_init(k);
    //mpz_inits(length, i, j);
    gmp_scanf("%Zd", length);
    startTime = clock();
    if (mpz_cmp_ui(length ,2)<0) {
        return 0;
    }
    vector<bool> isprime;
    //isprime.reserve(1000000000);
    //填充vector
    while (mpz_cmp(i,length)<=0) {
        isprime.push_back(true);
        mpz_add_ui(i, i, 1);
    }
    //isprime.shrink_to_fit();
    //对vector进行筛选
    auto a = isprime.begin()+2;
    mpz_t s;
    mpz_init(s);
    mpz_sqrt(s, length);
    for (mpz_set_ui(j, 2); mpz_cmp(j, s) <= 0; mpz_add_ui(j, j, 1)) {
        if (*a) {
            auto leap = a - isprime.begin();
            auto p = a;
            while (p<isprime.end()) {
            p+=leap;
            *p = false;
        }
        }
        ++a;
    }
    mpz_set_ui(k, 2);
    for (auto t = isprime.begin()+2; t!=isprime.end(); ++t) {
        if (*t) {
            gmp_printf("%Zd ", k);
        }
        mpz_add_ui(k, k, 1);
    }
    cout << endl;
    mpz_clear(length);
    mpz_clear(i);
    mpz_clear(j);
    endTime = clock();
    cout << "total time: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
    system("pause");

    return 0;
}