#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <gmpxx.h>
using namespace ::std;

const vector<string> bin {"0000", "0001", "0010", "0011",
                            "0100", "0101", "0110", "0111",
                            "1000", "1001", "1010", "1011",
                            "1100", "1101", "1110", "1111"};

mpz_class p, g;
mpz_class x, y;

string readbin (string infilename)
{
    ifstream fin;
    fin.open(infilename.c_str(), ios::in | ios::binary);

    fin.seekg(0, fin.end);
    unsigned long long length = fin.tellg();
    fin.seekg(0, fin.beg);

    char *buffer = new char[length];
    //cout << "ok" << endl;
    fin.read(buffer, length);
    fin.close();

    string ss = "";
    for (unsigned long long i=0; i!=length; ++i) {
        const char byte = buffer[i];
        ss+=bin[(byte & 0xF0)>>4];
        ss+=bin[(byte & 0x0F)>>0];
    }

    return ss;
}

//gcd(a, b)
void easygcd (mpz_class a, mpz_class b, mpz_class &r)
{
    if (b==0) {
        r=a;
        return ;
    }
    mpz_class tr;
    tr = a % b;
    easygcd(b, tr, r);
    return ;
}

//if (a, b) = 1, =1 return true
bool rp_jud (mpz_class a, mpz_class b)
{
    mpz_class r;
    easygcd(a, b, r);
    if (r==1) {
        return true;
    } else {
        return false;
    }
}

//互素的a和b，得到ax+by=gcd的形式
void exgcd (mpz_class a, mpz_class b, mpz_class &x, mpz_class &y, mpz_class &gcd)
{
    /*
    mpz_class ta, tb;
    ta = a;
    tb = b;
    */
    if (b==0) {
        x=1;y=0;gcd=a;
        return ;
    }

    mpz_class r, temp, q;
    r = a % b;
    exgcd(b, r, x, y, gcd);
    temp = y;
    q = (a/b)*y;
    y = x-q;
    x = temp;

    return ;
}

//求a模n的乘法逆元（a, n互素的情况下）
void mul_inv (mpz_class a, mpz_class n, mpz_class &inv)
{
    mpz_class x, y, gcd;
    x=y=gcd=0;
    exgcd(a, n, x, y, gcd);
    while (x<0) {
        x = x+n;
    }
    inv = x;
    return ;
}

//快速模幂, res = a^b mod c
void modpow (mpz_class a, mpz_class b, mpz_class c, mpz_class &res)
{
    mpz_class loga, power;
    loga = a; power = b; res = 1;
    while (power > 0) {
        mpz_class temp;
        temp = power % 2;
        if (temp==1) {
            res = res * loga;
            res = res % c;
        }
        loga = loga * loga;
        loga = loga % c;
        power = power / 2;
    }
    return ;
}

void sig ()
{
    mpz_class mp;
    mp = p-1;
    string mstr = readbin("intext");
    mpz_class m(mstr, 2);// 明文获取完毕

    //cout << "read in ok" << endl;
    // produce the random number k from Z_p
    mpz_class k;
    srand(int(time(0)));
    do {
        unsigned int ran = rand();
        k = ran;
        k = k % mp;
    } while (!rp_jud(k, mp));
    cout << "->k: " << k << endl;

    mpz_class s1, s2;
    modpow(g, k, p, s1);
    mpz_class invk;
    mul_inv(k, mp, invk);

    s2 = m - s1*x;
    s2 = invk * s2;
    s2 = s2 % mp;

    cout << "s1: " << s1 << endl
         << "s2: " << s2 << endl;
    return ;
}

void confirm ()
{
    string mstr = readbin("intext");
    mpz_class m(mstr, 2);

    cout << "*enter s1, s2" << endl;
    mpz_class s1, s2;
    cin >> s1 >> s2;

    mpz_class t1, t2;
    //t1 = y^s1 mod p
    modpow(y, s1, p, t1);
    //t2 = s1^s2 mod p
    modpow(s1, s2, p, t2);

    mpz_class ret;
    ret = (t1*t2)%p;
    mpz_class r2;

    modpow(g, m, p, r2);
    if (ret == r2) {
        cout << "same" << endl;
    } else {
        cout << "not same" << endl;
    }

    return ;
}

int main (int argc, char *argv[])
{
    if (argc < 3) {
        cout << "***ElGamal sig manual***" << endl
             << "three options for p, g and an x" << endl << endl
             << "**for example:" << endl
             << "$ ./elgamal 19 2 9" << endl
             << "stands for p=19, g=2, x=9" << endl << endl
             << "**choose three numbers and try again" << endl;
             return 0;
    }
    /*
    cout << "enter p, g: " << endl;
    cin >> p >> g;
    //p = 19; g = 2; x = 9;
    cout << "select an x in Z_p" << endl;
    cin >> x;
    */
    //string pstr, gstr, xstr;
    p = argv[1]; g = argv[2]; x = argv[3];
    //cout << pstr << " " << gstr << " " << xstr << endl;
    /*
    p = pstr.c_str();
    g = gstr.c_str();
    x = xstr.c_str();
    */
    // y = g^x mod p
    modpow(g, x, p, y);
    // public key: p, g, y; private key: x
    cout << "->publish the public key: (p, g, y)" << endl
         << p << "," << endl
         << g << "," << endl
         << y << endl;
    cout << "->key generate ok" << endl;

    char op;
    while (1) {
        cout << "s to sig, c to confirm, q to quit:" << endl;
        cin >> op;
        if (op=='s') {
            sig();
        } else if (op=='c') {
            confirm();
        } else if (op=='q') {
            break;
        }
    }

    return 0;
}
