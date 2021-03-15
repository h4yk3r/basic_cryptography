#include <iostream>
#include <time.h>
#include <gmp.h>
using namespace ::std;

void easygcd (mpz_t a, mpz_t b, mpz_t &r)
{
    if (mpz_cmp_ui(b, 0)==0) {
        mpz_set(r, a);
        return;
    }
    mpz_t tr;
    mpz_init(tr);
    mpz_mod(tr, a, b);// tr = a mod b
    easygcd(b, tr, r);
    
}

int main ()
{
    clock_t startTime, endTime;
    mpz_t a,b;
    mpz_init(a);
    mpz_init(b);
    gmp_scanf("%Zd %Zd", &a, &b);
    startTime = clock();
    mpz_t gcd;
    mpz_init(gcd);
    //gcd = easygcd(a, b);
    easygcd(a, b, gcd);
    gmp_printf("%Zd\n", gcd);
    endTime = clock();
    cout << "Total time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
    system("pause");

    return 0;
}