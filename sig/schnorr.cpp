#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <gmpxx.h>
using namespace :: std;
typedef unsigned long long ull;

mpz_class p, q, a;// globoal pk
mpz_class s;// sk
mpz_class v;// pk

string bin2char (mpz_class x)
{
    string xtemp = x.get_str(2);
    int xbit = xtemp.size();
    int xr = xbit % 8;
    string xbin = {};
    int xc = 8-xr;
    for (int i=0; i!=xc; ++i) {
        xbin += '0';
    }
    for (int i=0; i!=xbit; ++i) {
        xbin += xtemp[i];
    }
    int cast[xbin.size()] = {};
    for (int i=0; i!=xbin.size(); ++i) {
        char temp = xbin[i];
        cast[i] = temp - '0';
    }
    string xchar = {};
    int length = (xbin.size())/8;
    for (int i=0; i!=length; ++i) {
        xchar += '\0';
    }
    for (int i=0; i!=length; ++i) {
        xchar[i] = (cast[i*8+0] << 7);
        xchar[i] += (cast[i*8+1] << 6);
        xchar[i] += (cast[i*8+2] << 5);
        xchar[i] += (cast[i*8+3] << 4);
        xchar[i] += (cast[i*8+4] << 3);
        xchar[i] += (cast[i*8+5] << 2);
        xchar[i] += (cast[i*8+6] << 1);
        xchar[i] += cast[i*8+7];
    }
    return xchar;
}

string readchar (string infilename)
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

    string ss = {};
    for (unsigned long long i=0; i!=length; ++i) {
        ss+=buffer[i];
    }
    return ss;
}

ull MurmurHash64A ( const void * key, int len, unsigned int seed )
{
	const ull m = 0xc6a4a7935bd1e995;
	const int r = 47;

	ull h = seed ^ (len * m);

	const ull * data = (const ull *)key;
	const ull * end = data + (len/8);

	while(data != end)
	{
		ull k = *data++;

		k *= m;
		k ^= k >> r;
		k *= m;

		h ^= k;
		h *= m;
	}

	const unsigned char * data2 = (const unsigned char*)data;

	switch(len & 7)
	{
	case 7: h ^= ull(data2[6]) << 48;
	case 6: h ^= ull(data2[5]) << 40;
	case 5: h ^= ull(data2[4]) << 32;
	case 4: h ^= ull(data2[3]) << 24;
	case 3: h ^= ull(data2[2]) << 16;
	case 2: h ^= ull(data2[1]) << 8;
	case 1: h ^= ull(data2[0]);
	        h *= m;
	};

	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return h;
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

void sig (mpz_class r, mpz_class x)
{
    mpz_class e;
    // e = hash(m||x)
    string mstr = readchar("intext");
    string xstr = bin2char(x);
    string hstr = mstr + xstr;
    ull eull = MurmurHash64A(hstr.c_str(), hstr.size(), 7);
    string estr = to_string(eull);
    e = estr.c_str();

    mpz_class y;
    mpz_class etemp;
    etemp = e % q;
    y = r + s*etemp;
    y = y % q;
    cout << "(e, y):" << endl
         << "e: " << e << endl
         << "y: " << y << endl;
    return ;
}

void confirm ()
{
    mpz_class e, y;
    cout << "enter e and y:" << endl;
    cin >> e >> y;
    mpz_class v1, v2;
    mpz_class x1;
    modpow(a, y, p, v1);
    modpow(v, e, p, v2);
    x1 = v1 * v2;
    x1 = x1 % p;

    mpz_class e1;
    // e1 = hash(m||x1)
    string mstr = readchar("intext");
    string xstr = bin2char(x1);
    string hstr = mstr + xstr;
    ull e1ull = MurmurHash64A(hstr.c_str(), hstr.size(), 7);
    string e1str = to_string(e1ull);
    e1 = e1str.c_str();
    cout << "e': " << e1 << endl;

    if (e1==e) {
        cout << "same" << endl;
    } else {
        cout << "not same" << endl;
    }

    return ;
}

int main (int argc, char *argv[])
{
    if (argc < 3) {
        cout << "***Schnorr sig manual***" << endl
             << "three options for p, g and an x" << endl << endl
             << "**for example:" << endl
             << "$ ./schnorr 7879 101 170" << endl
             << "stands for p=7879, q=101, a=170" << endl << endl
             << "**choose three numbers and try again" << endl;
             return 0;
    }
    //cin >> p >> q;
    //cin >> a;
    //p = 7879; q = 101; a = 170;
    p = argv[1]; q = argv[2]; a = argv[3];

    mpz_class seed;
    srand(int(time(0)));
    unsigned int ran = rand();
    seed = ran;
    gmp_randclass rs (gmp_randinit_default);
    rs.seed(seed);
    do {
        s = rs.get_z_range(q);
    } while (s==0);
    //cout << "user sk s: " << s << endl;

    mpz_class ps;
    ps = -s;
    while (ps<0) {
        ps = ps + q;
    }

    modpow(a, ps, p, v);
    cout << "(a, p, q)" << endl
         << a << endl
         << p << endl
         << q << endl;
    cout << "user pk v: " << v << endl;

    mpz_class r, x;

    ran = rand();
    seed = ran;
    rs.seed(seed);
    do {
        r = rs.get_z_range(q);
    } while (r==0);

    modpow(a, r, p, x);

    char op;
    while (1) {
        cout << endl << "s to sig, c to confirm, q to quit:" << endl;
        cin >> op;
        if (op=='s') {
            sig(r, x);
            continue;
        } else if (op=='c') {
            confirm();
            continue;
        } else if (op=='q') {
            break;
        }
    }

    return 0;
}
