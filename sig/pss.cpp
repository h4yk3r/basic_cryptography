#include <iostream>
#include <string>
#include <openssl/sha.h>
#include "rsautils.h"
using namespace ::std;

mpz_class p, q, n;
mpz_class e;
mpz_class d;
string salt;

string d2b (int a)
{
    string bin(32, '0');
    for (int i=0; i!=32; ++i) {
        int temp = a & 0x1;
        a = a >> 1;
        if (temp==1) {
            bin[31-i] = '1';
        } else if (temp==0) {
            bin[31-i] = '0';
        }
    }
    return bin;
}

string mask_xor (string db, string mask)
{
    string ret = {};
    int len = db.size();
    for (int i=0; i!=len; ++i) {
        char temp;
        if (db[i]==mask[i]) {
            temp = '0';
        } else if (db[i]!=mask[i]) {
            temp = '1';
        }
        ret += temp;
    }
    return ret;
}

string bin2char (string bin)
{
    int cast[bin.size()] = {};
    for (int i=0; i!=bin.size(); ++i) {
        char temp = bin[i];
        cast[i] = temp - '0';
    }
    string xchar = {};
    int length = (bin.size())/8;
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

void key_gen ()
{
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
        p = mpz_class (pstr.c_str(), 2);
    } while (!mr_isPrime(p));
    do {
        do {
            qs = rand()%100000;
        } while (((qs%383)==0) || ((qs%503)==0));
        qstr = bbs_prng(qs, primebit);
        q = mpz_class (qstr.c_str(), 2);
    } while ((!mr_isPrime(q)) || (p==q));
    n = p*q;
    mpz_class pm; pm = (p-1)*(q-1);

    string estr;
    cout << "select the para: e in binnary" << endl;
    cin >> estr;
    e = mpz_class(estr.c_str(), 2);
    while (!rp_jud(e, pm)) {
        cout << "wrong, select another e:" << endl;
        cin >> estr;
        e = mpz_class(estr.c_str(), 2);
    }

    mul_inv(e, pm, d);

    //crt key
    return ;
}

//(maskBits)bit mask number
string MGF1 (string X, int maskLen)
{
    int maskBits = 8*maskLen;
    //cout << "maskBits: " << maskBits << endl;
    string T = {};
    int k = (maskLen/SHA_DIGEST_LENGTH);
    for (int counter =0; counter<=k; ++counter) {
        //char temp = counter + '0';
        string temp = d2b(counter);
        string in = X + temp;
        T += sha1digist(in);// bin
        if (T.size() >= maskBits) {
            break;
        }
    }
    string out = {};
    for (int i=0; i!=maskBits; ++i) {
        out += T[i];
    }
    //cout << "T: " << T << endl;
    //cout << "len of out: " << out.size() << endl;
    //cout << "out of mask: " << out << endl;
    return out;
}

string sig (int emBits)
{
    string EM = {};
    int emLen = emBits/8;// Bytes
    int dbLen = emLen - SHA_DIGEST_LENGTH -1;// Bytes

    string mstr = readchar("intext");
    string mhash = sha1digist(mstr);// 20Bytes
    string m1str(64, '0');// 8Bytes
    m1str = m1str + mhash + salt;// 48Bytes
    //cout << "len of m1str: " << m1str.size() << endl;
    //cout << "m1str: " << m1str << endl;
    string m1in = bin2char(m1str);// 48Bytes
    string H = sha1digist(m1in);// 20Bytes -> bin
    string Hchar = bin2char(H);
    string mask = MGF1(Hchar, dbLen);
    //cout << "dbBits: " << dbLen*8 << endl;
    //cout << "len of mask: " << mask.size() << endl;
    //cout << "H in sig: " << H << endl;
    //cout << "mask in sig: " << mask << endl;
    //cout << "len of mask: " << mask.size() << endl;

    int pd2Bits = (emLen - SHA_DIGEST_LENGTH - SHA_DIGEST_LENGTH - 1) * 8;
    string DB(pd2Bits-8, '0');
    DB += "00000001";
    DB += salt;

    //cout << "DB in sig: " << DB << endl;
    //cout << "len of DB: " << DB.size() << endl;

    string maskedDB = mask_xor(DB, mask);
    int EMc = 8*emLen - emBits;
    for (int i=0; i!=EMc; ++i) {
        maskedDB[i] = '0';
    }

    //cout << "maskedDB in sig: " << maskedDB << endl;

    EM = maskedDB + H + "10111100";

    //cout << "len of maskedDB: " << maskedDB.size() << endl;
    //cout << "len of H: " << H.size() << endl;

    return EM;
}

void confirm (string EM)
{
    //1
    string mstr = readchar("intext");
    string mhash = sha1digist(mstr);// 20Bytes

    int emBits = EM.size();
    int emLen = emBits / 8;
    int hLen = SHA_DIGEST_LENGTH;
    int sLen = hLen;

    //2
    if (emLen < (hLen + sLen +2)) {
        cout << "s2 not same" << endl;
        return ;
    }

    //3
    string temp(8, '\0');
    for (int i=0; i!=8; ++i) {
        temp[7-i] = EM[emBits-1-i];
    }
    string bc = "10111100";
    if (!(temp == bc)) {
        cout << "s3 not same" << endl;
        return ;
    }

    //4
    string maskedDB = {};
    string H = {};
    int dbLen = emLen - hLen - 1;
    int dbBits = 8*dbLen;
    for (int i=0; i!=dbBits; ++i) {
        maskedDB += EM[i];
    }
    int hBits = 8*hLen;
    for (int i=0; i!=hBits; ++i) {
        H += EM[dbBits + i];
    }
    string Hchar = bin2char(H);

    //5
    int EMc = 8*emLen - emBits;
    for (int i=0; i!=EMc; ++i) {
        if (EM[i]!='0') {
            cout << "s5 not same" << endl;
            return ;
        }
    }

    //6
    string dbMask = MGF1(Hchar, dbLen);
    //cout << "mask in con: " << dbMask << endl;
    //cout << "H in con: " << H << endl;
    //cout << "len of mask: " << dbMask.size() << endl;
    //cout << "len of maskedDB: " << maskedDB.size() << endl;
    //7
    //cout << "maskedDB in con: " << maskedDB << endl;
    string DB = mask_xor(maskedDB, dbMask);
    //8
    for (int i=0; i!=EMc; ++i) {
        DB[i] = '0';
    }

    //cout << "DB in con: " << DB << endl;

    //9
    int pd2Bits = (emLen-sLen-hLen-1) * 8;
    for (int i=0; i!=(pd2Bits-8); ++i) {
        if (DB[i]!='0') {
            cout << "s9-1 not same" << endl;
            return ;
        }
    }
    string pd2temp = {};
    string pd2ex = "00000001";
    for (int i=(pd2Bits-8); i!=pd2Bits; ++i) {
        pd2temp += DB[i];
    }
    if (pd2temp != pd2ex) {
        cout << "s9-2 not same" << endl;
        return ;
    }

    //10
    int sBits = 8*sLen;

    string c_salt(sBits, '\0');
    for (int i=0; i!=sBits; ++i) {
        c_salt[sBits-1-i] = DB[dbBits-1-i];
    }

    string c_m1(64, '0');
    c_m1 = c_m1 + mhash + c_salt;
    //cout << "len of c_m1: " << c_m1.size() << endl;
    //cout << "c_m1: " << c_m1 << endl;
    string c_m1char = bin2char(c_m1);
    string H1 = sha1digist(c_m1char);
    if (H!=H1) {
        cout << "s10 not same" << endl;
        return ;
    }
    cout << "same" << endl;
    return ;
}

int main ()
{
    key_gen();
    cout << "p: " << p << endl
         << "q: " << q << endl
         << "n: " << n << endl
         << "e: " << e << endl
         << "d: " << d << endl;

    int nbit = get_len(n);
    cout << "emBits smaller than " << nbit << endl;

    int embits;
    cin >> embits;

    srand(int(time(0)));
    unsigned long long seed = rand()%100000;
    unsigned int sBits = SHA_DIGEST_LENGTH*8;
    salt = bbs_prng(seed, sBits);// 160bit 20Bytes
    //cout << "salt: " << salt << endl;

    mpz_class m, s, decm;

    char op;
    while (1) {
        cout << endl << "s to sig, c to confirm, q to quit" << endl;
        cin >> op;
        if (op=='s') {
            string EM = sig(embits);
            //cout << "length of EM: " << EM.size() << endl;
            m = mpz_class(EM.c_str(), 2);
            //cout << "EM: " << EM << endl;
            //cout << "m: " << m << endl;
            modpow(m, d, n, s);
            continue;

        } else if (op=='c') {
            modpow(s, e, n, decm);
            //cout << "decm: " << decm << endl;
            string decmstr = decm.get_str(2);
            string decstr = {};
            int cm = embits - decmstr.size();
            //cout << "cm: " << cm << endl;
            if (cm!=0) {
                for (int i=0; i!=cm; ++i) {
                    decstr += '0';
                }
                decstr += decmstr;
            } else {
                decstr += decmstr;
            }
            //cout << "decstr: " << decstr << endl;
            confirm(decstr);
            continue;

        } else if (op=='q') {
            break;
        }
    }

    //test RSA
    /*
    string mstr;
    cin >> mstr;
    mpz_class m(mstr, 0);
    mpz_class s;
    modpow(m, d, n, s);
    mpz_class decm;
    modpow(s, e, n, decm);
    cout << decm << endl;
    */

    return 0;
}
