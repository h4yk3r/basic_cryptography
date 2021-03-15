#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace ::std;

void tswap (int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
    return ;
}

int main ()
{
    cout << "enter the key: " << endl;
    string key;
    cin >> key;
    cout << "enter the infile: " << endl;
    string infile;
    cin >> infile;
    cout << "enter the outfile: " << endl;
    string outfile;
    cin >> outfile;
    int s[256];
    int t[256];
    for (int i=0; i!=256; ++i) {
        s[i] = i;
        t[i] = key[(i%key.size())];
    }

    int j=0;
    for (int i=0; i!=256; ++i) {
        j += (s[i]+t[i]);
        j %= 256;
        tswap(s[i], s[j]);
    }

    ifstream fin;
    fin.open(infile.c_str(), ios::in | ios::binary);

    fin.seekg(0, fin.end);
    unsigned long long length = fin.tellg();
    fin.seekg(0, fin.beg);

    char *buffer = new char[length];
    char *cast = new char[length];
    fin.read(buffer, length);
    fin.close();

    int i=0;
    j=0;

    for (int p=0; p!=length; ++p) {
        ++i;
        i%=256;
        j+=s[i];
        j%=256;
        tswap(s[i], s[j]);
        int t = s[i] + s[j];
        t%=256;
        char k = s[t];
        cast[p] = k ^ buffer[p];
    }

    ofstream fout;
    fout.open(outfile.c_str(), ios::out | ios::trunc | ios::binary);
    fout.write(cast, length);
    fout.close();

    return 0;
}
