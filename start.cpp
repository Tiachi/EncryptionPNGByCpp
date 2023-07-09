#include <bitset>
#include <string>
#include<fstream>
#include<iostream>
#include<map>
#include<zlib.h>
#include<vector>
#include<ctime>
#define endl '\n'
#define IOS ios::sync_with_stdio(0);cin.tie(0);
using namespace std;
int xx=1;
namespace Mypng{
    struct PNG{
        unsigned char *Data;
        unsigned int Length;
        unsigned char *ZipedIDAT;
        unsigned int ZipedIDATLength;
        unsigned char *UnzipedIDAT;
        unsigned int UnzipedIDATLength;
    };
}
using namespace Mypng;
namespace Des{
    bitset<64> key;                
    bitset<48> subKey[16];         
    int IP[] = {58, 50, 42, 34, 26, 18, 10, 2,
                60, 52, 44, 36, 28, 20, 12, 4,
                62, 54, 46, 38, 30, 22, 14, 6,
                64, 56, 48, 40, 32, 24, 16, 8,
                57, 49, 41, 33, 25, 17, 9,  1,
                59, 51, 43, 35, 27, 19, 11, 3,
                61, 53, 45, 37, 29, 21, 13, 5,
                63, 55, 47, 39, 31, 23, 15, 7};
    
    int IP_1[] = {40, 8, 48, 16, 56, 24, 64, 32,
                39, 7, 47, 15, 55, 23, 63, 31,
                38, 6, 46, 14, 54, 22, 62, 30,
                37, 5, 45, 13, 53, 21, 61, 29,
                36, 4, 44, 12, 52, 20, 60, 28,
                35, 3, 43, 11, 51, 19, 59, 27,
                34, 2, 42, 10, 50, 18, 58, 26,
                33, 1, 41,  9, 49, 17, 57, 25};
    
    int PC_1[] = {57, 49, 41, 33, 25, 17, 9,
                1, 58, 50, 42, 34, 26, 18,
                10,  2, 59, 51, 43, 35, 27,
                19, 11,  3, 60, 52, 44, 36,
                63, 55, 47, 39, 31, 23, 15,
                7, 62, 54, 46, 38, 30, 22,
                14,  6, 61, 53, 45, 37, 29,
                21, 13,  5, 28, 20, 12,  4}; 
    
    int PC_2[] = {14, 17, 11, 24,  1,  5,
                3, 28, 15,  6, 21, 10,
                23, 19, 12,  4, 26,  8,
                16,  7, 27, 20, 13,  2,
                41, 52, 31, 37, 47, 55,
                30, 40, 51, 45, 33, 48,
                44, 49, 39, 56, 34, 53,
                46, 42, 50, 36, 29, 32};
    
    int shiftBits[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
    
    int E[] = {32,  1,  2,  3,  4,  5,
                4,  5,  6,  7,  8,  9,
                8,  9, 10, 11, 12, 13,
            12, 13, 14, 15, 16, 17,
            16, 17, 18, 19, 20, 21,
            20, 21, 22, 23, 24, 25,
            24, 25, 26, 27, 28, 29,
            28, 29, 30, 31, 32,  1};
    
    int S_BOX[8][4][16] = {
        {  
            {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},  
            {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},  
            {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0}, 
            {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13} 
        },
        {  
            {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},  
            {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5}, 
            {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},  
            {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}  
        }, 
        {  
            {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},  
            {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},  
            {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},  
            {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}  
        }, 
        {  
            {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},  
            {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},  
            {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},  
            {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}  
        },
        {  
            {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},  
            {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},  
            {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},  
            {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}  
        },
        {  
            {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},  
            {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},  
            {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},  
            {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}  
        }, 
        {  
            {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},  
            {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},  
            {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},  
            {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}  
        }, 
        {  
            {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},  
            {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},  
            {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},  
            {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}  
        } 
    };
    
    int P[] = {16,  7, 20, 21,
            29, 12, 28, 17,
                1, 15, 23, 26,
                5, 18, 31, 10,
                2,  8, 24, 14,
            32, 27,  3,  9,
            19, 13, 30,  6,
            22, 11,  4, 25 };
    
    bitset<32> f(bitset<32> R, bitset<48> k){
        bitset<48> expandR;
        for(int i=0; i<48; ++i)expandR[47-i] = R[32-E[i]];
        expandR = expandR ^ k;
        bitset<32> output;
        int x = 0;
        for(int i=0; i<48; i=i+6){
            int row = expandR[47-i]*2 + expandR[47-i-5];
            int col = expandR[47-i-1]*8 + expandR[47-i-2]*4 + expandR[47-i-3]*2 + expandR[47-i-4];
            int num = S_BOX[i/6][row][col];
            bitset<4> binary(num);
            output[31-x] = binary[3];
            output[31-x-1] = binary[2];
            output[31-x-2] = binary[1];
            output[31-x-3] = binary[0];
            x += 4;
        }
        bitset<32> tmp = output;
        for(int i=0; i<32; ++i)
            output[31-i] = tmp[32-P[i]];
        return output;
    }
    
    bitset<28> leftShift(bitset<28> k, int shift){
        bitset<28> tmp = k;
        for(int i=27; i>=0; --i){
            if(i-shift<0)
                k[i] = tmp[i-shift+28];
            else
                k[i] = tmp[i-shift];
        }
        return k;
    }
    
    void generateKeys() {
        bitset<56> realKey;
        bitset<28> left;
        bitset<28> right;
        bitset<48> compressKey;
        for (int i=0; i<56; ++i)
            realKey[55-i] = key[64 - PC_1[i]];
        for(int round=0; round<16; ++round) {
            for(int i=28; i<56; ++i)
                left[i-28] = realKey[i];
            for(int i=0; i<28; ++i)
                right[i] = realKey[i];
            left = leftShift(left, shiftBits[round]);
            right = leftShift(right, shiftBits[round]);
            for(int i=28; i<56; ++i)
                realKey[i] = left[i-28];
            for(int i=0; i<28; ++i)
                realKey[i] = right[i];
            for(int i=0; i<48; ++i)
                compressKey[47-i] = realKey[56 - PC_2[i]];
            subKey[round] = compressKey;
        }
    }
    
    bitset<64> charToBitset(const char s[8]){
        bitset<64> bits;
        for(int i=0; i<8; ++i)
            for(int j=0; j<8; ++j)
                bits[i*8+j] = ((s[i]>>j) & 1);
        return bits;
    }

    bitset<64> encrypt(bitset<64>& plain){
        bitset<64> cipher;
        bitset<64> currentBits;
        bitset<32> left;
        bitset<32> right;
        bitset<32> newLeft;
        for(int i=0; i<64; ++i)
            currentBits[63-i] = plain[64-IP[i]];
        for(int i=32; i<64; ++i)
            left[i-32] = currentBits[i];
        for(int i=0; i<32; ++i)
            right[i] = currentBits[i];
        for(int round=0; round<16; ++round){
            newLeft = right;
            right = left ^ f(right,subKey[round]);
            left = newLeft;
        }
        for(int i=0; i<32; ++i)
            cipher[i] = left[i];
        for(int i=32; i<64; ++i)
            cipher[i] = right[i-32];
        currentBits = cipher;
        for(int i=0; i<64; ++i)
            cipher[63-i] = currentBits[64-IP_1[i]];
        return cipher;
    }
    
    bitset<64> decrypt(bitset<64>& cipher){
        bitset<64> plain;
        bitset<64> currentBits;
        bitset<32> left;
        bitset<32> right;
        bitset<32> newLeft;
        for(int i=0; i<64; ++i)
            currentBits[63-i] = cipher[64-IP[i]];
        for(int i=32; i<64; ++i)
            left[i-32] = currentBits[i];
        for(int i=0; i<32; ++i)
            right[i] = currentBits[i];
        for(int round=0; round<16; ++round)
        {
            newLeft = right;
            right = left ^ f(right,subKey[15-round]);
            left = newLeft;
        }
        for(int i=0; i<32; ++i)
            plain[i] = left[i];
        for(int i=32; i<64; ++i)
            plain[i] = right[i-32];
        currentBits = plain;
        for(int i=0; i<64; ++i)
            plain[63-i] = currentBits[64-IP_1[i]];
        return plain;
    }
};
using namespace Des;
string k="2021103280";
int OP;
time_t start,endtm;
unsigned char Buffer[8];
ifstream Input;
ofstream Output;
PNG SoursePicture;
PNG ModifiedPicture;
unsigned int GetNum(unsigned char data[],int len);
void WriteUint32(ofstream& outputFile, unsigned int value);
void intToByteArray(int value, char* byteArray);
void GetThePicture();
void Close();
void Printinfo();
void GetIDAT();
void Debug();
void UnzipIDAT();
void EncryptThePicture();
void DecryptThePicture();
void ZipIDAT();
void ChangeData();
void OutputThePicture();
void debug();
void Welcome();
int main(void){
    IOS
    Welcome();
	GetThePicture();
    Printinfo();
    GetIDAT();
    //Debug();
    UnzipIDAT();
    if(OP==1) {
        start=time(0);
        EncryptThePicture();
        endtm=time(0);
    }
    else {
        start=time(0);
        DecryptThePicture();
        endtm=time(0);
    }
    ZipIDAT();
    ChangeData();
    OutputThePicture();
    //Close();
    cout<<"Solve time is "<<(endtm-start)<<" seconds"<<endl;
	return 0;
}
void Welcome(){
    cout<<"Input your number: ";
    cin>>k;
    cout<<"Chose to Encrypt or Decrypt"<<endl<<"1.Encrypt "<<endl<<"2.Decrypt"<<endl<<"Input your chose: ";
    cin>>OP;
    return;
}
unsigned int GetNum(unsigned char data[],int len){
    unsigned int res=0;
    for(int i=0;i<len;i++){
        res=(res<<8)+data[i];
    }
    return res;
}
void WriteUint32(ofstream& outputFile, unsigned int value) {
    outputFile.write(reinterpret_cast<const char*>(&value), sizeof(value));
}
void intToByteArray(int value, char* byteArray) {
    byteArray[0] = static_cast<char>((value >> 24) & 0xFF);
    byteArray[1] = static_cast<char>((value >> 16) & 0xFF);
    byteArray[2] = static_cast<char>((value >> 8) & 0xFF);
    byteArray[3] = static_cast<char>(value & 0xFF);
}
void longlongToByteArray(long long value, char* byteArray) {
    byteArray[0] = static_cast<char>((value >> 56) & 0xFF);
    byteArray[1] = static_cast<char>((value >> 48) & 0xFF);
    byteArray[2] = static_cast<char>((value >> 40) & 0xFF);
    byteArray[3] = static_cast<char>((value >> 32) & 0xFF);
    byteArray[4] = static_cast<char>((value >> 24) & 0xFF);
    byteArray[5] = static_cast<char>((value >> 16) & 0xFF);
    byteArray[6] = static_cast<char>((value >> 8) & 0xFF);
    byteArray[7] = static_cast<char>(value & 0xFF);
}
//sourse_img
//encrypted_img
//decrypt_img
void GetThePicture(){
    if(OP==1){
        Input.open("sourse_img.png",ios::binary);
        Output.open("encrypted_img.png", ios::binary);
    }
    else {
        Input.open("encrypted_img.png",ios::binary);
        Output.open("decrypt_img.png", ios::binary);
    }
    Input.seekg(0,Input.end);
    SoursePicture.Length=Input.tellg();
    SoursePicture.Data=new unsigned char[SoursePicture.Length];
    SoursePicture.ZipedIDAT=new unsigned char[SoursePicture.Length];
    SoursePicture.UnzipedIDAT=new unsigned char[SoursePicture.Length*10];
    Input.seekg(0,Input.beg);
    Input.read((char*)SoursePicture.Data,SoursePicture.Length);
    ModifiedPicture=SoursePicture;
    //cout<<SoursePicture.Length<<" "<<SoursePicture.BodyLength<<endl;
}
void Close(){
    delete [] SoursePicture.Data;
    delete [] ModifiedPicture.Data;
    delete [] SoursePicture.ZipedIDAT;
    delete [] ModifiedPicture.ZipedIDAT;
    delete [] SoursePicture.UnzipedIDAT;
    delete [] ModifiedPicture.UnzipedIDAT;
    Input.close();
    Output.close();
}

void Printinfo(){
    map<string,int>mp;
    int len=0;
    Input.seekg(8,Input.beg);
    while(Input.peek()!=EOF){
        cout<<"Idx is "<<Input.tellg()<<" ";
        Input.read((char*)Buffer,8);
        len=GetNum(Buffer,4);
        string s;for(int i=4;i<8;i++)s.push_back(Buffer[i]);
        mp[s]+=len;
        Input.seekg(len+4,Input.cur);
        cout<<"Part of "<<s<<" length is "<<len<<endl;
    }
    for(auto &x:mp){
        cout<<"The len of "<<x.first<<" is "<<x.second<<endl;
    }
    mp.clear();
}
void GetIDAT(){
    unsigned int idx=8;
    unsigned int len=0;
    while(idx<ModifiedPicture.Length){
        for(int i=0;i<8;i++){
            Buffer[i]=ModifiedPicture.Data[idx+i];
        }
        idx+=8;
        len=GetNum(Buffer,4);
        string s;for(int i=4;i<8;i++)s.push_back(Buffer[i]);
        //cout<<len<<" "<<s<<endl;
        if(s=="IDAT"){
            for(int i=0;i<len;i++){
                ModifiedPicture.ZipedIDAT[ModifiedPicture.ZipedIDATLength++]=ModifiedPicture.Data[i+idx];
            }
        }
        idx=idx+len+4;
    }
    cout<<"InitZipedIDATLength is "<<ModifiedPicture.ZipedIDATLength<<endl;
}
void UnzipIDAT(){
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in=ModifiedPicture.ZipedIDAT;
    strm.avail_in=ModifiedPicture.ZipedIDATLength;
    strm.next_out=NULL;
    strm.avail_out=0;
    int ret=inflateInit(&strm);
    if(ret!=Z_OK){
        cout<<"Failed to init zlib"<<endl;
        return;
    }
    const unsigned int Buffer2Length=1024;
    unsigned char Buffer2[Buffer2Length];
    do{
        strm.next_out=(Bytef*)Buffer2;
        strm.avail_out=Buffer2Length;
        ret=inflate(&strm,Z_NO_FLUSH);
        if(ret==Z_STREAM_ERROR||ret==Z_DATA_ERROR){
            cout<<"Failed to inflate"<<endl;
            inflateEnd(&strm);
            return;
        }
        unsigned int decompressedSize=Buffer2Length-strm.avail_out;
        for(int i=0;i<decompressedSize;i++)
            ModifiedPicture.UnzipedIDAT[ModifiedPicture.UnzipedIDATLength++]=Buffer2[i];
    }while(strm.avail_out==0);
    inflateEnd(&strm);
    cout<<"UnzipedIDATLength is "<<ModifiedPicture.UnzipedIDATLength<<endl;
}

char Buffer2[20];
void EncryptThePicture(){
    key = charToBitset(k.c_str());
    generateKeys();
    for(int i=0;i+8<ModifiedPicture.UnzipedIDATLength;i+=8){
        cout<<"Encrypt schedule : "<<(double)(i*100)/(double)ModifiedPicture.UnzipedIDATLength<<"% "<<endl;
        string s;
        for(int j=0;j<8;j++){
            s.push_back(ModifiedPicture.UnzipedIDAT[i+j]);
        }
        bitset<64>plain=charToBitset(s.c_str());
        bitset<64>cipher=encrypt(plain);//8*8 8*4*2
        unsigned long long tmp=cipher.to_ullong();
        longlongToByteArray(tmp,Buffer2);
        for(int j=0;j<8;j++)ModifiedPicture.UnzipedIDAT[i+j]=Buffer2[7-j];
    }
}
void DecryptThePicture(){
    key = charToBitset(k.c_str());
    generateKeys();
    for(int i=0;i+8<ModifiedPicture.UnzipedIDATLength;i+=8){
        cout<<"Decrypt schedule : "<<(double)(i*100)/(double)ModifiedPicture.UnzipedIDATLength<<"% "<<endl;
        string s;
        for(int j=0;j<8;j++){
            s.push_back(ModifiedPicture.UnzipedIDAT[i+j]);
        }
        bitset<64>plain=charToBitset(s.c_str());
        bitset<64>dipher=decrypt(plain);//8*8 8*4*2
        unsigned long long tmp=dipher.to_ullong();
        longlongToByteArray(tmp,Buffer2);
        for(int j=0;j<8;j++)ModifiedPicture.UnzipedIDAT[i+j]=Buffer2[7-j];
    }
}
void ZipIDAT() {
    ModifiedPicture.ZipedIDATLength=0;
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = ModifiedPicture.UnzipedIDAT;
    strm.avail_in = ModifiedPicture.UnzipedIDATLength;
    strm.next_out = NULL;
    strm.avail_out = 0;
    int ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        cout << "Failed to init zlib" << endl;
        return;
    }
    const unsigned int Buffer2Length = 1024;
    unsigned char Buffer2[Buffer2Length];
    do {
        strm.next_out = Buffer2;
        strm.avail_out = Buffer2Length;
        ret = deflate(&strm, Z_FINISH);
        if (ret == Z_STREAM_ERROR) {
            cout << "Failed to deflate" << endl;
            deflateEnd(&strm);
            return;
        }
        unsigned int compressedSize = Buffer2Length - strm.avail_out;
        for (int i = 0; i < compressedSize; i++)
            ModifiedPicture.ZipedIDAT[ModifiedPicture.ZipedIDATLength++] = Buffer2[i];
    } while (strm.avail_out == 0);
    deflateEnd(&strm);
    cout << "ZipedIDATLength is " << ModifiedPicture.ZipedIDATLength << endl;
}

void ChangeData(){
    delete [] ModifiedPicture.Data;
    ModifiedPicture.Data=new unsigned char[SoursePicture.Length];
    for(int i=0;i<54;i++)ModifiedPicture.Data[i]=SoursePicture.Data[i];
    intToByteArray(ModifiedPicture.ZipedIDATLength,(char*)ModifiedPicture.Data+54);
    for(int i=0;i<4;i++)ModifiedPicture.Data[i+54+4]=SoursePicture.Data[i+54+4];
    for(int i=0;i<ModifiedPicture.ZipedIDATLength;i++)ModifiedPicture.Data[i+54+8]=ModifiedPicture.ZipedIDAT[i];
    unsigned int crc = crc32(0, ModifiedPicture.ZipedIDAT, ModifiedPicture.ZipedIDATLength);
    intToByteArray(crc,(char*)ModifiedPicture.Data+54+8+ModifiedPicture.ZipedIDATLength);
    for(int i=0;i<12;i++)ModifiedPicture.Data[i+54+8+ModifiedPicture.ZipedIDATLength+4]=SoursePicture.Data[ModifiedPicture.Length-12+i];
    ModifiedPicture.Length=54+8+ModifiedPicture.ZipedIDATLength+4+12;
}
void OutputThePicture(){
    //cout<<"Look "<<endl;
    //debug();
    Output.write((char*)ModifiedPicture.Data,ModifiedPicture.Length);
}

void debug(){
    unsigned int idx=8;
    unsigned int len=0;
    while(idx<ModifiedPicture.Length){
        for(int i=0;i<8;i++){
            Buffer[i]=ModifiedPicture.Data[idx+i];
        }
        idx+=8;
        len=GetNum(Buffer,4);
        string s;for(int i=4;i<8;i++)s.push_back(Buffer[i]);
        cout<<len<<" "<<s<<endl;
        idx=idx+len+4;
    }
    //cout<<"ZipedIDATLength is "<<ModifiedPicture.ZipedIDATLength<<endl;
}
//Decompressed size: 786944 bytes