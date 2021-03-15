#include <iostream>
#include <string>
#include <gmpxx.h>
#include <vector>
#include <fstream>
#include <openssl/sha.h>

using namespace ::std;

mpz_class p;
int a, b;
mpz_class gx, gy;
mpz_class d;
mpz_class qx, qy;

//160bit hash
string sha1digist (string str)
{
    const vector<string> bin {"0000", "0001", "0010", "0011",
                            "0100", "0101", "0110", "0111",
                            "1000", "1001", "1010", "1011",
                            "1100", "1101", "1110", "1111"};

    unsigned char *ucstr = new unsigned char[str.size()];
    for (int i=0; i!=str.size(); ++i) {
        ucstr[i] = str[i];
    }
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(ucstr, str.size(), hash);

    string out = {};
    for (int i=0; i!=SHA_DIGEST_LENGTH; ++i) {
        char byte = hash[i];
        out+=bin[(byte & 0xF0)>>4];
        out+=bin[(byte & 0x0F)>>0];
    }

    return out;
}

//a curve of y^2=x^3+ax+b(mod p)
class elliptic {
private:
    mpz_class p; int a, b;//curve

    //快速模幂, res = a^b mod c
    void modpow (mpz_class a, mpz_class b, mpz_class c, mpz_class &res);

public:
    //constructor
    elliptic() = default;
    elliptic(mpz_class pin, int ain, int bin);

    void equallamda(mpz_class xp, mpz_class yp,
                    mpz_class xq, mpz_class yq, mpz_class &lamda);
    void unequallamda(mpz_class xp, mpz_class yp,
                    mpz_class xq, mpz_class yq, mpz_class &lamda);
    void eccplus(mpz_class xp, mpz_class yp,
                mpz_class xq, mpz_class yq, mpz_class &rx, mpz_class &ry);
    void eccmul(mpz_class n, mpz_class x, mpz_class y, mpz_class &rx, mpz_class &ry);
    bool econcurve(mpz_class x, mpz_class y);
    void ecco(mpz_class x, mpz_class y ,mpz_class &og);
    void ecqmul(mpz_class n, mpz_class x, mpz_class y, mpz_class &rx, mpz_class &ry);
};

elliptic::elliptic (mpz_class pin, int ain, int bin)
{
    p = pin;
    a = ain;
    b = bin;
}

void elliptic::equallamda(mpz_class xp, mpz_class yp, mpz_class xq, mpz_class yq, mpz_class &lamda)
{
    //up/down(mod p) = up*down^(p-2)(mod p)
    mpz_class nume;
    mpz_class deno;

    nume = 3*xp*xp+a;
    deno = 2*yp;

    mpz_class power;
    power = p-2;
    mpz_class invdeno;
    modpow(deno, power, p, invdeno);

    nume = nume % p;
    lamda = nume*invdeno;
    lamda = lamda % p;

    return ;
}

void elliptic::unequallamda(mpz_class xp, mpz_class yp, mpz_class xq, mpz_class yq, mpz_class &lamda)
{
    //up/down(mod p) = up*down^(p-2)(mod p)
    mpz_class nume;
    mpz_class deno;

    nume = yq - yp;
    deno = xq - xp;

    mpz_class power;
    power = p-2;
    mpz_class invdeno;
    modpow(deno, power, p, invdeno);

    nume = nume % p;
    lamda = nume*invdeno;
    lamda = lamda % p;

    return ;
}

void elliptic::eccplus(mpz_class xp, mpz_class yp, mpz_class xq, mpz_class yq, mpz_class &rx, mpz_class &ry)
{
    mpz_class lamda;
    if ( (xp==xq) && (yp==yq) ) {
        equallamda(xp, yp, xq, yq, lamda);
    } else {
        unequallamda(xp, yp, xq, yq, lamda);
    }

    mpz_class xtemp, ytemp;
    xtemp = lamda*lamda - xp - xq;
    xtemp = xtemp % p;
    ytemp = lamda*xp - lamda*xtemp -yp;
    ytemp = ytemp % p;

    rx = xtemp;
    while (rx<0) {
        rx = rx + p;
    }
    ry = ytemp;
    while (ry<0) {
        ry = ry + p;
    }

    return ;
}

void elliptic::eccmul(mpz_class n, mpz_class x, mpz_class y, mpz_class &rx, mpz_class &ry)
{
    mpz_class xtemp, ytemp;
    xtemp = x;
    ytemp = y;
    mpz_class i;
    for (i=1; i!=n; i=i+1) {
        eccplus(xtemp, ytemp, x, y, xtemp, ytemp);
    }
    rx = xtemp;
    while (rx<0) {
        rx = rx + p;
    }
    ry = ytemp;
    while (ry<0) {
        ry = ry + p;
    }

    return ;
}

//y^2=x^3+ax+b(mod p)
bool elliptic::econcurve(mpz_class x, mpz_class y)
{
    mpz_class l,r;
    l = (y*y) % p;
    r = (x*x*x + a*x + b) % p;
    if (l==r) {
        return true;
    } else if (l!=r) {
        return false;
    }
    return true;
}

void elliptic::ecco(mpz_class x, mpz_class y ,mpz_class &og)
{
    mpz_class cnt = 1;
    mpz_class rx, ry;
    rx = 1; ry = 1;
    while (1) {
        eccmul(cnt, x, y, rx, ry);
        //cout << rx << " " << ry << endl;
        if (rx==x && ry+y==257) {
            break;
        }
        cnt = cnt + 1;
    }
    og = cnt;
    return ;
}

void elliptic::ecqmul(mpz_class n, mpz_class x, mpz_class y, mpz_class &rx, mpz_class &ry)
{
    mpz_class xtemp,ytemp;
    xtemp = x; ytemp = -y;
    /*
    n=og*og*og*og*...*og+r
    n*X=og*...*og*X+r*X

    */
    return ;
}

//快速模幂, res = a^b mod c
void elliptic::modpow (mpz_class a, mpz_class b, mpz_class c, mpz_class &res)
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

void sig ()
{
    string mstr;
    mstr = readchar("intext");
    string hstr = sha1digist(mstr);
    mpz_class h;
    h = mpz_class(hstr, 2);
    cout << "hash: " << h << endl;

    elliptic curve = elliptic(p, a, b);

    mpz_class k;
    mpz_class px, py;
    mpz_class invk;
    mpz_class r;
    mpz_class s;

    do {
        do {
            do {
                /*
                srand(int(time(0)*10));
                unsigned int ran = rand();
                k = ran; k = k % p;
                */
                cout << "select the private k: " << endl;
                cin >> k;
                curve.eccmul(k, gx, gy, px, py);
            } while ((k==0) || (!curve.econcurve(px, py)));

            r = px;
            r = r%p;
        } while (r==0);

        mul_inv(k, p, invk);

        s = invk*(h + d*r);
        s = s % p;
    } while (s==0);
    cout << "k: " << k << endl << "invk: " << invk << endl;

    cout << "signature: " << r << " " << s << endl;

    return ;
}

void confirm ()
{
    string mstr;
    mstr = readchar("intext");
    string hstr = sha1digist(mstr);
    mpz_class h;
    h = mpz_class(hstr, 2);
    cout << "hash: " << h << endl;

    elliptic curve = elliptic(p, a, b);

    mpz_class r, s;
    cout << "enter signature: " << endl;
    cin >> r >> s;

    if ( (r>=p)||(s>=p) ) {
        cout << "wrong signature!" << endl;
        cout << "enter signature: " << endl;
        cin >> r >> s;
    }

    mpz_class invs;
    mul_inv(s, p, invs);
    cout << "s: " << s << endl << "invs: " << invs << endl;

    mpz_class u1, u2;
    u1 = invs*h;
    u1 = u1 % p;
    u2 = invs*r;
    u2 = u2 % p;

    mpz_class x1x, x1y;
    mpz_class x2x, x2y;
    mpz_class xx, xy;
    curve.eccmul(u1, gx, gy, x1x, x1y);
    curve.eccmul(u2, qx, qy, x2x, x2y);
    /*
    curve.ecqmul(u1, gx, gy, x1x, x1y);
    curve.ecqmul(u2, qx, qy, x2x, x2y);
    */
    curve.eccplus(x1x, x1y, x2x, x2y, xx, xy);

    cout << xx << " " << xy << endl;

    mpz_class v;
    if (xx==0 && xy==0) {
        cout << "not same" << endl;
    } else {
        v = xx;
        v = v%p;
        if (v==r) {
            cout << "same" << endl;
        } else {
            cout << "not same" << endl;
        }
    }
    return ;
}

int main ()
{
    cout << "initializing..." << endl;
    cout << "select the base point G:" << endl;
    //cin >> gx >> gy;
    cout << "test mode" << endl; gx = 2; gy = 2;
    cout << "select the curve:" << endl;
    //cin >> a >> b >> p;
    cout << "test mode" << endl;
    a = 0; b = -4; p = 257;
    elliptic init = elliptic(p, a, b);
    cout << "generating keys..." << endl;
    do {
        srand(int(time(0)*10));
        unsigned int temp = rand();
        d = rand();
        d = d % p;
        init.eccmul(d, gx, gy, qx, qy);
    } while ( (d==0) || (!init.econcurve(qx, qy)) );
    cout << "pk: " << qx << " " << qy << endl;

    cout << endl;
    char op;
    while (1) {
        cout << "s to sig, c to confirm, q to quit:" << endl;
        cin >> op;
        if (op=='s') {
            sig();
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
