#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace ::std;
using ull = unsigned long long;

unsigned int h[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};

const char hex_c_up[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

string len64 (ull len)
{
    string res = {};
    char c;
    ull val;
    ull mask = 0x8000000000000000;
    for (int i=0; i!=64; ++i) {
        val = len & mask;
        val = val >> (63-i);
        mask = mask >> 1;
        c = val + '0';
        res += c;
    }
    return res;
}

string bin2hex (string bin)
{
    string hexstr = {};
    int blen = bin.size();
    int hlen = blen / 4;
    for (int i=0; i!=hlen; ++i) {
        int temp = 0;
        for (int j=0; j!=4; ++j) {
            int val = bin[i*4+j] - '0';
            temp += (val << (3-j));
        }
        hexstr += hex_c_up[temp];
    }
    return hexstr;
}

//32bit int -> 32bit string
string int2bin (unsigned int num)
{
    string bin = {};
    char c;
    unsigned int val;
    unsigned int mask = 0x80000000;
    for (int i=0; i!=32; ++i) {
        val = num & mask;
        val = val >> (31-i);
        mask = mask >> 1;
        c = val + '0';

        bin += c;
    }
    return bin;
}

//32bit string -> 32bit int
unsigned int bin2int (string bin)
{
    unsigned int val = 0;
    for (int i=0; i!=32; ++i) {
        char c = bin[i];
        int temp = c - '0';
        val += (temp << (31-i));
    }
    return val;
}

string readbin (string infilename)
{
    const vector<string> bin {"0000", "0001", "0010", "0011",
                            "0100", "0101", "0110", "0111",
                            "1000", "1001", "1010", "1011",
                            "1100", "1101", "1110", "1111"};

    ifstream fin;
    fin.open(infilename.c_str(), ios::in | ios::binary);

    fin.seekg(0, fin.end);
    ull length = fin.tellg();
    fin.seekg(0, fin.beg);

    char *buffer = new char[length];
    fin.read(buffer, length);
    fin.close();

    string ss = "";
    for (ull i=0; i!=length; ++i) {
        const char byte = buffer[i];
        ss+=bin[(byte & 0xF0)>>4];
        ss+=bin[(byte & 0x0F)>>0];
    }

    return ss;
}

string padding (string src)
{
    ull len = src.size();
    ull q = len / 512; ull r = len % 512;
    ull idealen = 0;
    if (r < 448) {
        idealen = q*512 + 448;
    } else if (r >= 448) {
        idealen = (q+1)*512 + 448;
    }
    ull pd = idealen - len - 1;//pd >= 1
    string res = src + '1';
    for (ull i=0; i!=pd; ++i) {
        res += '0';
    }
    res += len64(len);
    //cout << res << endl;

    return res;
}

unsigned int leftrotate (unsigned int num, int n)
{
    unsigned int temp = num;
    unsigned int t1 = temp << n;
    unsigned int t2 = temp >> (32-n);
    unsigned int t = t1+t2;
    return t;
}

void hash_round (string chunk)
{
    unsigned int w[80];//0-15 16-79
    for (int i=0; i!=16; ++i) {
        string temp = {};
        for (int j=0; j!=32; ++j) {
            temp += chunk[32*i + j];
        }
        w[i] = bin2int(temp);
    }
    for (int i=16; i!=80; ++i) {
        unsigned int temp = w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16];
        w[i] = leftrotate(temp, 1);
    }
    unsigned int alp[5];
    for (int i=0; i!=5; ++i) {
        alp[i] = h[i];
    }

    //main loop
    for (int i=0; i!=80; ++i) {
        unsigned int k;
        unsigned int f;
        if (i>=0 && i<=19) {
            f=(alp[1]&alp[2])|((~alp[1])&alp[3]);
            k = 0x5A827999;
        } else if (i>=20 && i<=39) {
            f=alp[1]^alp[2]^alp[3];
            k = 0x6ED9EBA1;
        } else if (i>=40 && i<=59) {
            f=(alp[1]&alp[2])|(alp[1]&alp[3])|(alp[2]&alp[3]);
            k = 0x8F1BBCDC;
        } else if (i>=60 && i<=79) {
            f=alp[1]^alp[2]^alp[3];
            k = 0xCA62C1D6;
        }

        unsigned int temp = leftrotate(alp[0] ,5);
        temp = temp + f + alp[4] +k + w[i];
        alp[4] = alp[3];
        alp[3] = alp[2];
        alp[2] = leftrotate(alp[1], 30);
        alp[1] = alp[0];
        alp[0] = temp;
    }

    for (int i=0; i!=5; ++i) {
        h[i] += alp[i];
    }
    return ;
}

string get_SHA (string mstr)
{
    string m = padding(mstr);

    ull len = m.size();
    ull blockn = len / 512;
    vector<string> mvc;
    for (ull i=0; i!=blockn; ++i) {
        string temp = {};
        for (ull j=0; j!=512; ++j) {
            temp += m[i*512 + j];
        }
        mvc.push_back(temp);
    }

    for (ull i=0; i!=blockn; ++i) {
        hash_round(mvc[i]);
    }

    string digist = {};
    for (int i=0; i!=5; ++i) {
        digist += int2bin(h[i]);
    }

    return digist;
}

string char2bin (string buffer)
{
    const vector<string> bin {"0000", "0001", "0010", "0011",
                            "0100", "0101", "0110", "0111",
                            "1000", "1001", "1010", "1011",
                            "1100", "1101", "1110", "1111"};

    ull length = buffer.size();
    string ss = "";
    for (ull i=0; i!=length; ++i) {
        const char byte = buffer[i];
        ss+=bin[(byte & 0xF0)>>4];
        ss+=bin[(byte & 0x0F)>>0];
    }

    return ss;
}

void manual ()
{
    cout << "   welcome to SHA-1 digist" << endl;
    cout << "1: $./sha1main" << endl;
    cout << "2: $./sha1main -s string" << endl;
    cout << "3: $./sha1main -f filename" << endl;
    cout << "try again" << endl;

    return ;
}

int main (int argc, char *argv[])
{
    string mstr;
    if (argc==1) {
        string temp;
        cout << "Enter string: ";
        getline(cin, temp);
        cout << "read in: " << temp << endl;
        mstr = char2bin(temp);
    } else if (argc==3) {
        string opstring = "-s";
        string opfile = "-f";
        if (argv[1]==opstring) {
            string temp = argv[2];
            cout << "read in: " << temp << endl;
            mstr = char2bin(temp);
        } else if (argv[1]==opfile) {
            mstr = readbin(argv[2]);
        } else {
            //man
            manual();
            return 0;
        }
    } else {
        manual();
        return 0;
    }
    //string mstr = readbin("intext");
    //string mstr = "011000010110001001100011";
    string mhash = get_SHA(mstr);
    cout << bin2hex(mhash) << endl;
    return 0;
}
