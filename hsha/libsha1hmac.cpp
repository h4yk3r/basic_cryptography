#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <openssl/sha.h>
#include <bitset>
using ull = unsigned long long;

using namespace ::std;

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

string get_SHA (string str)
{
    unsigned char *ucstr = new unsigned char[str.size()];
    for (int i=0; i!=str.size(); ++i) {
        ucstr[i] = str[i];
    }
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(ucstr, str.size(), hash);
    string out = {};
    for (int i=0; i!=SHA_DIGEST_LENGTH; ++i) {
        char byte = hash[i];
        out += byte;
    }
    return out;
}

string hmac (string inkey, string inm)
{
    string key =inkey;
    string m = inm;
    string mac;
    bitset<8> bs_ipad_base(0x36);
    string ipad_base = bs_ipad_base.to_string();
    bitset<8> bs_opad_base(0x5c);
    string opad_base = bs_opad_base.to_string();
    vector<bitset<8>> ipad;
    vector<bitset<8>> opad;
    for (int i=0; i!=64; ++i) {
        ipad.push_back(bs_ipad_base);
        opad.push_back(bs_opad_base);
    }

    vector<bitset<8>> keyplus;
    int len = key.size();
    if (len > 64) {
        key = get_SHA(key);
    }
    if (len < 64) {
        //padding
        int gap = 64-len;
        for (int i=0; i!=gap; ++i) {
            key += (char)0x00;
        }
    }
    for (int i=0; i!=64; ++i) {
        bitset<8> temp(key[i]);
        keyplus.push_back(temp);
    }

    string si = {};
    for (int i=0; i!=64; ++i) {
        bitset<8> temp = (keyplus[i])^(ipad[i]);
        char tc = (char)(temp.to_ulong());
        si += tc;
    }
    string so = {};
    for (int i=0; i!=64; ++i) {
        bitset<8> temp = (keyplus[i])^(opad[i]);
        char tc = (char)(temp.to_ulong());
        so += tc;
    }

    //inner pad
    string innermessage = si + m;
    string innerhash = get_SHA(innermessage);
    string outermessage = so + innerhash;
    string outerhash = get_SHA(outermessage);
    mac = outerhash;

    return mac;
}

void manual ()
{
    cout << "   welcome to sha1 based mac" << endl;
    cout << "1: $./libsha1hmac" << endl;
    cout << "2: $./libsha1hmac -s string" << endl;
    cout << "3: $./libsha1hmac -f filename" << endl;
    cout << "try again" << endl;

    return ;
}

int main (int argc, char *argv[])
{
    string key;
    string message;
    if (argc==1) {
        cout << "Enter message:";
        getline(cin, message);
        cout << "read in message: " << message << endl;
        cout << "Enter key:";
        getline(cin, key);
        cout << "read in key: " << key << endl;
    } else if (argc==3) {
        string opstring = "-s";
        string opfile = "-f";
        if (argv[1]==opstring) {
            message = argv[2];
            cout << "Enter key:";
            getline(cin, key);
            cout << "read in key: " << key << endl;
        } else if (argv[1]==opfile) {
            message = readchar(argv[2]);
            cout << "Enter key:";
            getline(cin, key);
            cout << "read in key: " << key << endl;
        } else {
            //man
            manual();
            return 0;
        }
    } else {
        manual();
        return 0;
    }

    string res = hmac(key, message);

    cout << hex;
    for (auto beg = res.begin(); beg!=res.end(); beg++) {
        char c = *beg;
        bitset<8> temp(c);
        int i = (int)temp.to_ulong();
        if (i<16) {
            cout << 0;
        }
        cout << i;
    }
    cout << endl;
    return 0;
}
