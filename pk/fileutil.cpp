#include "fileutil.h"

const char hex_c_up[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
const char hex_c_low[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
const vector<string> bin {"0000", "0001", "0010", "0011",
                            "0100", "0101", "0110", "0111",
                            "1000", "1001", "1010", "1011",
                            "1100", "1101", "1110", "1111"};

/**
 * 读取任意一个文件，以string的形式返回文件的16进制，字节数是string.size()的一半。
 * 理论支持到16777216TB的文件。
 */
string readhex_up (string infilename)
{
    ifstream fin;
    fin.open(infilename.c_str(), ios::in | ios::binary);

    fin.seekg(0, fin.end);
    unsigned long long length = fin.tellg();
    fin.seekg(0, fin.beg);

    char *buffer = new char[length];
    fin.read(buffer, length);
    fin.close();

    string ss = "";
    for (unsigned long long i=0; i!=length; ++i) {
        const char byte = buffer[i];
        ss+=hex_c_up[(byte & 0xF0)>>4];
        ss+=hex_c_up[(byte & 0x0F)>>0];
    }

    return ss;
}

string readhex_low (string infilename)
{
    ifstream fin;
    fin.open(infilename.c_str(), ios::in | ios::binary);

    fin.seekg(0, fin.end);
    unsigned long long length = fin.tellg();
    fin.seekg(0, fin.beg);

    char *buffer = new char[length];
    fin.read(buffer, length);
    fin.close();

    string ss = "";
    for (unsigned long long i=0; i!=length; ++i) {
        const char byte = buffer[i];
        ss+=hex_c_low[(byte & 0xF0)>>4];
        ss+=hex_c_low[(byte & 0x0F)>>0];
    }

    return ss;
}

string readbin (string infilename)
{
    ifstream fin;
    fin.open(infilename.c_str(), ios::in | ios::binary);

    fin.seekg(0, fin.end);
    unsigned long long length = fin.tellg();
    fin.seekg(0, fin.beg);

    char *buffer = new char[length];
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

void writehex_up (string outfilename, string ss)
{
    ofstream fout;
    fout.open(outfilename.c_str(), ios::out | ios::trunc | ios::binary);

    unsigned long long length = (ss.size())/2;
    char *buffer = new char[length];
    int cast[ss.size()] = {};
    for (unsigned long long i=0; i!=ss.size(); ++i) {
        char temp = ss[i];
        if (temp >='0' && temp <='9') {
            cast[i] = temp-'0';
        } else if (temp >= 'A' && temp <= 'F') {
            cast[i] = temp-'A'+10;
        }
    }
    for (unsigned long long i=0; i!=length; ++i) {
        buffer[i] = (cast[i*2+0] << 4);
        buffer[i] += cast[i*2+1];
    }
    fout.write(buffer, length);
    fout.close();
    return ;
}

void writehex_low (string outfilename, string ss)
{
    ofstream fout;
    fout.open(outfilename.c_str(), ios::out | ios::trunc | ios::binary);

    unsigned long long length = (ss.size())/2;
    char *buffer = new char[length];
    int cast[ss.size()] = {};
    for (unsigned long long i=0; i!=ss.size(); ++i) {
        char temp = ss[i];
        if (temp >='0' && temp <='9') {
            cast[i] = temp-'0';
        } else if (temp >= 'a' && temp <= 'f') {
            cast[i] = temp-'a'+10;
        }
    }
    for (unsigned long long i=0; i!=length; ++i) {
        buffer[i] = (cast[i*2+0] << 4);
        buffer[i] += cast[i*2+1];
    }
    fout.write(buffer, length);
    fout.close();
    return ;
}

void writebin (string outfilename, string ss)
{
    ofstream fout;
    fout.open(outfilename.c_str(), ios::out | ios::trunc | ios::binary);

    unsigned long long length = (ss.size())/8;
    char *buffer = new char[length];
    int cast[ss.size()] = {};
    for (unsigned long long i=0; i!=ss.size(); ++i) {
        char temp = ss[i];
        cast[i] = temp - '0';
    }
    for (unsigned long long i=0; i!=length; ++i) {
        buffer[i] = (cast[i*8+0] << 7);
        buffer[i] += (cast[i*8+1] << 6);
        buffer[i] += (cast[i*8+2] << 5);
        buffer[i] += (cast[i*8+3] << 4);
        buffer[i] += (cast[i*8+4] << 3);
        buffer[i] += (cast[i*8+5] << 2);
        buffer[i] += (cast[i*8+6] << 1);
        buffer[i] += cast[i*8+7];
    }
    fout.write(buffer, length);
    fout.close();
    return ;
}
