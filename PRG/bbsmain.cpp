#include <iostream>
#include <gmpxx.h>
#include <vector>
using namespace ::std;

int main ()
{
    mpz_t p, q, n, s, cnt;
    mpz_init(p);
    mpz_init(q);
    mpz_init(n);
    mpz_init(s);
    mpz_init(cnt);

    cout << "enter P and Q: " << endl;
    gmp_scanf("%Zd %Zd", p, q);
    cout << "enter the seed: " << endl;
    gmp_scanf("%Zd", s);
    cout << "enter the number of bits: " << endl;
    gmp_scanf("%Zd", cnt);

    mpz_mul(n, p, q);

    mpz_t i, xp, xs, ss, b;
    mpz_init_set_ui(i, 0);
    mpz_init(xs);
    mpz_init(ss);
    mpz_init(b);
    mpz_init(xp);
    mpz_mul(ss, s, s);
    mpz_mod(xs, ss, n);
    //vector<int> keystring;

    for (; mpz_cmp(i, cnt)!=0; mpz_add_ui(i, i, 1)) {
        mpz_mul(xp, xs, xs);
        mpz_mod(xp, xp, n);
        mpz_mod_ui(b, xp, 2);
        //int temp = mpz_get_ui(b);
        //keystring.push_back(temp);
        gmp_printf("%Zd", b);
        mpz_set(xs, xp);
    }
    /*
    for (auto j : keystring) {
        cout << j;
    }
    */
    cout << endl;
    return 0;
}
