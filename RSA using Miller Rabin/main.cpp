//
//  main.cpp
//  RSA using Miller Rabin
//
//  Created by Avijeet on 05/02/17.
//  Copyright © 2017 Avijeet. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <random>
#include <time.h>


#define ull unsigned long long int

#define vuc vector<unsigned char>
#define uc unsigned char
#define ll unsigned long long int
using namespace std;

ull mulmod(ull a,ull b,ull c){
    ull x = 0,y= a%c ;
    while(b > 0){
        if(b%2 == 1){
            x = (x+y)%c;
        }
        y = (y*2)%c;
        b /= 2;
    }
    return x%c;
}

ull modulo(ull a,ull b,ull c){
    ull x=1,y=a;
    while(b > 0){
        if(b%2 == 1){
            x=mulmod(x, y, c);
        }
        y = mulmod(y, y, c);
        b /= 2;
    }
    return x%c;
    
}
bool miller_rabin(ull a, ull num){
    
    ull d = num-1;
    ull ctr = 0;
    
    while(d%2==0){
        d/=2;
        ctr++;
    }
    ull t = modulo(a,d,num);
    
    if(t == 1 || t== -1){
        return true;
    }
    
    for(int i=0; i < ctr ; i ++){
        t = modulo(t, 2, num);
        if(t==1)    return false;
        else if(t==-1)  return true;
    }
    
    return false;
}

ull generatePrime(int k){
    
    bool Prime = false;
    ull num, a;
    while(!Prime){
        ull temp = pow(2, k-1);
        num = temp + ( rand() % temp );
        
        if(num%2 == 0){
            num++;
        }
        Prime = true;
        for(int i = 0 ; i < 5 ; i ++){
            a = 2 + rand()% (temp-4);
            
            if(miller_rabin(a,num)==false) {
                Prime = false;
                break;
            }
        }
    }
    return num;
}

ull modInv(ull e, ull phi){
    long long int r1, r2, t1, t2, r, t, q;
    r1 = phi;
    r2 = e;
    t1 = 0;
    t2 = 1;
    
    while(r2 > 1){
        r = r1%r2;
        q = r1/r2;
        
        r1 = r2;
        
        r2 = r;
        
        t = t1 - (q*t2);
        t1 = t2;
        t2 = t;
    }
    while (t < 0){
        t += phi;
    }
    return t;
    
    
}
ull setText(vuc s, int in, int c){
    ull temp=0;
    
    for( int i=0 ; i+in<s.size() && i<c ; i++ ){
        temp = temp << 8;
        temp = temp|(int)s[i+in];
    }
    
    return temp;
    
}


int getCh( ull ch){
    ch = ch>> 56;
    int c = (int) ch;
    return c;
}
vuc encrypt(string msg, ull e, ull n){
    vuc plain;
    for(int i= 0 ; i < msg.size(); i++){
        plain.push_back(msg[i]);
    }
    
    vector<ull> text;
    ull t;
    
    for(int i = 0 ; i < plain.size(); i+=3){
        t = setText(plain, i , 3);
        text.push_back(t);
    }
    
    vector<ull> cipherText;
    
    for(ull i= 0 ; i < text.size(); i ++){
        ull temp = modulo(text[i],e,n);
        cipherText.push_back(temp);
    }
    
    vuc cipher;
    
    for(int i = 0 ; i < cipherText.size() ; i++){
        cipherText[i] = cipherText[i]<<32;
        while(cipherText[i]){
            int c = getCh(cipherText[i]);
            cipherText[i] = cipherText[i]<<8;
            cipher.push_back(c);
        }
    }
    return cipher;
    
}

string decrypt(vuc cipher, ull d, ull n){
    vector<ull> txt1;
    
    ull t;
    
    for(int i = 0 ; i < cipher.size(); i += 4){
        t = setText(cipher, i , 4);
        txt1.push_back(t);
    }
    vector<ull> txt2;
    
    for( ull i = 0 ; i < txt1.size() ; i++){
        ull temp = modulo(txt1[i],d,n);
        txt2.push_back(temp);
    }
    
    vuc msg;
    for(int i = 0 ; i < txt2.size() ; i++){
        txt2[i] = txt2[i]<<32;
        while(txt2[i]){
            int c = getCh(txt2[i]);
            txt2[i] = txt2[i]<<8;
            if(c!=0)
                msg.push_back(c);
        }
    }
    
    string msgstr;
    
    for(int i = 0 ; i < msg.size(); i ++){
        msgstr.push_back(msg[i]);
    }
    return msgstr;
}
int main(int argc, const char * argv[]) {
    
    
    int k=16;
    
    srand(time(NULL));
    
    ull p = generatePrime(k);
    ull q = generatePrime(k);
    
    
    while(p==q){
        q = generatePrime(k);
    }
    
    ull n = p*q;
    ull phi = (p-1)*(q-1);
    ull e = generatePrime(k-1);
    
    ull d = modInv(e,phi);
    
    string input;
    cout<<"Enter Plain Text : ";
    getline(cin,input);
    
    vuc cipher = encrypt(input, e, n);
    
    cout<<"Cipher Text : ";
    
    for(int i = 0 ; i < cipher.size() ; i ++){
        cout<<cipher[i];
    }
    cout<<endl;
    string msg = decrypt(cipher,d,n);
    cout<<"Decrypt Plain Text: "<<msg<<endl;
    
    
    
    return 0;
}
