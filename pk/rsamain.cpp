#include <iostream>
#include <string>
#include <vector>
#include <gmpxx.h>
#include <ctime>
#include "rsa.h"
#include "fileutil.h"

using namespace ::std;

int len;
int declen;
mpz_t dp, dq;
mpz_t crtp, crtq;
mpz_t invq;

int get_len (mpz_t n)
{
    mpz_t res;
    int cnt = 1;
    mpz_init_set(res, n);
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

//加密
void rsa_enc (mpz_t e, mpz_t n)
{
    clock_t startTime, endTime;
    startTime = clock();

    string plain;//bin stream
    //plain = readbin("rsaencintext");
    plain = readbin("rsa");
    //cout << "plain in binary: " << plain << endl << plain.size() << endl;
    string cipher = {};//bin stream
    int plen = plain.size();
    //cout << "plain len: " << plen << endl;
    //分组
    auto pbeg = plain.begin();

    int cnt = 1;

    while (pbeg!=plain.end()) {
        string temp = {};
        for (int i=0; i!=len; ++i) {
            temp+=(*pbeg);
            ++pbeg;
            if (pbeg==plain.end()) {
                break;
            }
        }
        while (temp.size()!=len) {
            temp+='0';
        }
        //cout << cnt++ << ": " << temp << endl;
        //cout << cnt << " readin: " << temp.size() << endl;
        mpz_t m;
        mpz_t c;
        mpz_init_set_str(m, temp.c_str(), 2);

        //cout << "m: " << m << endl;

        mpz_init(c);

        modpow(m, e, n, c);

        //cout << "c: " << c << endl;

        int loc = get_len(c);

        char *cc = new char[loc];
        //cout << cnt++ << ": " << loc << endl;
        mpz_get_str(cc, 2, c);
        int ccc = declen - loc;
        for (int k=0; k!=ccc; ++k) {
            cipher += '0';
        }

        cipher += cc;
    }
    //cout << cipher.size() << " cipher: " << cipher << endl;
    writebin("rsaencouttext", cipher);
    //cout << cipher << endl;
    endTime = clock();
    cout << "enc time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

    return ;
}

//CRT实现的快速解密
void rsa_dec (mpz_t d, mpz_t n, mpz_t p, mpz_t q)
{
    clock_t startTime, endTime;
    startTime = clock();

    string cipher;//bin stream
    cipher = readbin("rsaencouttext");
    string plain = {};//bin stream
    /*
    cout << "cipher len: " << cipher.size() << endl;
    cout << "cipher: " << cipher << endl;
    */

    auto cbeg = cipher.begin();

    while (cbeg!=cipher.end()) {
        string temp;
        for (int i=0; i!=declen; ++i) {
            temp+=(*cbeg);
            ++cbeg;
            if (cbeg==cipher.end()) {
                break;
            }
        }
        /*
        cout << "dp: " << dp << endl;
        cout << "dq: " << dq << endl;
        */
        //cout << temp.size() << ": " << temp << endl;

        mpz_t c, m;
        mpz_init_set_str(c, temp.c_str(), 2);

        //cout << "c: " << c << endl;

        //test

        mpz_t mtest;
        mpz_init(mtest);
        modpow(c, d, n, mtest);
        //cout << "d: " << d << endl;
        //cout << "n: " << n << endl;
        //cout << "mtest: " << mtest << endl;


        //crt
        mpz_t m1, m2;
        mpz_init(m1);
        mpz_init(m2);
        modpow(c, dp, p, m1);
        modpow(c, dq, q, m2);
        mpz_init_set(m, m1);
        //h= (qInv*((m1 - m2)mod p)) mod p
        //m = m2 + h*q
        mpz_add(m, m, p);
        mpz_sub(m, m, m2);
        mpz_mod(m, m, p);
        mpz_mul(m, m, invq);
        mpz_mod(m, m, p);
        mpz_mul(m, m, q);
        mpz_add(m, m, m2);

        //cout << "m: " << m << endl;

        int lom = get_len(m);
        char *mm = new char[lom];

        mpz_get_str(mm, 2, m);

        /*
        if (lom%8!=0) {
            int mcc = 8-(lom%8);
            for (int j=0; j!=mcc; ++j) {
                plain+='0';
            }
        }
        */
        if (lom!=len) {
            int mcc = len - lom;
            for (int j=0; j!=mcc; ++j) {
                plain += '0';
            }
        }

        plain+=mm;
    }
    //cout << plain.size() << " plain: " << plain << endl;
    //cout << "deplain in binary: " << plain << endl << plain.size() << endl;
    writebin("rsadecouttext", plain);
    //cout << plain << endl;
    endTime = clock();
    cout << "dec time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

    return ;
}

void rsa_solve ()
{
    mpz_t p, q, n;
    mpz_init(p);
    mpz_init(q);
    mpz_init(n);

    int primebit = 512;

    string pstr, qstr;
    unsigned long long ps=0, qs=0;
    srand(int(time(0)));
    do {
        do {
            ps = rand()%100000;
        } while (((ps%383)==0) || ((ps%503)==0));
        pstr = bbs_prng(ps, primebit);
        //cout << pstr << endl;
        //pstr = "7";
        mpz_set_str(p ,pstr.c_str(), 2);
    } while (!mr_isPrime(p));
    do {
        do {
            qs = rand()%100000;
        } while (((qs%383)==0) || ((qs%503)==0));
        qstr = bbs_prng(qs, primebit);
        mpz_set_str(q ,qstr.c_str(), 2);
    } while ((!mr_isPrime(q)) || (mpz_cmp(p, q)==0));
    mpz_mul(n, p, q);
    len = get_len(n)-1;
    if (((len+1)%8)==0) {
        declen = len+1;
    } else if (((len+1)%8)!=0) {
        declen = ((len+1)/8)*8+8;
    }

    /*
    cout << p << " length of p: " << get_len(p) << endl;
    cout << q << " length of q: " << get_len(p) << endl;
    cout << "length of block: " << len << endl;
    */

    mpz_t pn;
    mpz_init_set(pn, n);
    mpz_sub(pn,pn,p);
    mpz_sub(pn,pn,q);
    mpz_add_ui(pn,pn,1);

    mpz_t e, d;
    mpz_init(e);
    mpz_init(d);
    string estr;
    //cout << "pk: n = " << n << ", select the para: e" << endl;
    cout << "select the para: e in binnary" << endl;
    cin >> estr;
    mpz_set_str(e, estr.c_str(), 2);
    while (!rp_jud(e, pn)) {
        cout << "wrong, select another e: " << endl;
        cin >> estr;
        mpz_set_str(e, estr.c_str(), 2);
    }
    mul_inv(e, pn, d);

    /*
    cout << "pn: " << pn << endl;

    cout << "p: " << p << endl;
    cout << "q: " << q << endl;
    */

    mpz_init(dp);
    mpz_init(dq);
    mpz_init_set(crtp, p);
    mpz_init_set(crtq, q);
    mpz_sub_ui(crtp, crtp, 1);
    mpz_sub_ui(crtq, crtq, 1);

    mpz_mod(dp, d, crtp);
    mpz_mod(dq, d, crtq);

    mpz_init(invq);
    mul_inv(q, p, invq);

    /*
    cout << "p: " << p << endl;
    cout << "q: " << q << endl;

    cout << "dp: " << dp << endl;
    cout << "dq: " << dq << endl;
    */

    //key generate finish

    char op;

    while (1) {
        menu();
        cin >> op;
        //cout << "ok" << endl;
        if (op=='d') {
            rsa_dec(d, n, p, q);
            cout << "end of dec" << endl;
        } else if (op=='e') {
            rsa_enc(e, n);
            cout << "end of enc" << endl;
        } else if (op=='q') {
            return ;
        }
    }
    return ;
}

int main ()
{
    cout << "welcome to rsa, simple steps to take to generate your keys" << endl;
    rsa_solve();
    return 0;
}
