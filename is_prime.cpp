/*

HOW TO COMPILE : g++ is_prime.cpp -o test
HOW TO RUN: ./test

*/

#include <iostream>
#include <fstream>

using namespace std;

/* This function calculates (ab)%c */
int modulo(int a,int b,int c){
    long long x=1,y=a; // long long is taken to avoid overflow of intermediate results
    while(b > 0){
        if(b%2 == 1){
            x=(x*y)%c;
        }
        y = (y*y)%c; // squaring the base
        b /= 2;
    }
    return x%c;
}

/* this function calculates (a*b)%c taking into account that a*b might overflow */
long long mulmod(long long a,long long b,long long c){
    long long x = 0,y=a%c;
    while(b > 0){
        if(b%2 == 1){
            x = (x+y)%c;
        }
        y = (y*2)%c;
        b /= 2;
    }
    return x%c;
}

/* Fermat's test for checking primality, the more iterations ,the more is accurate */
bool Fermat(long long p,int iterations){
    if(p == 1){ // 1 isn't prime
        return false;
    }
    for(int i=0;i<iterations;i++){
        // choose a random integer between 1 and p-1 ( inclusive )
        long long a = rand()%(p-1)+1;
        // modulo is the function we developed above for modular exponentiation.
        if(modulo(a,p-1,p) != 1){
            return false; /* p is definitely composite */
        }
    }
    return true; /* p is probably prime */
}

/* Miller-Rabin primality test, iteration signifies the accuracy of the test */
bool Miller(long long p,int iteration){
    if(p<2){
        return false;
    }
    if(p!=2 && p%2==0){
        return false;
    }
    long long s=p-1;
    while(s%2==0){
        s/=2;
    }
    for(int i=0;i<iteration;i++){
        long long a=rand()%(p-1)+1,temp=s;
        long long mod=modulo(a,temp,p);
        while(temp!=p-1 && mod!=1 && mod!=p-1){
            mod=mulmod(mod,mod,p);
            temp *= 2;
        }
        if(mod!=p-1 && temp%2==0){
            return false;
        }
    }
    return true;
}

//calculates Jacobian(a/n) n>0 and n is odd
int calculateJacobian(long long a,long long n){
    if(!a) return 0; // (0/n) = 0
    int ans=1;
    long long temp;
    if(a<0){
        a=-a;    // (a/n) = (-a/n)*(-1/n)
        if(n%4==3) ans=-ans; // (-1/n) = -1 if n = 3 ( mod 4 )
    }
    if(a==1) return ans; // (1/n) = 1
    while(a){
        if(a<0){
            a=-a;    // (a/n) = (-a/n)*(-1/n)
            if(n%4==3) ans=-ans;    // (-1/n) = -1 if n = 3 ( mod 4 )
        }
        while(a%2==0){
            a=a/2;    // Property (iii)
            if(n%8==3||n%8==5) ans=-ans;
        }
        swap(a,n);    // Property (iv)
        if(a%4==3 && n%4==3) ans=-ans; // Property (iv)
        a=a%n; // because (a/p) = (a%p / p ) and a%pi = (a%n)%pi if n % pi = 0
        if(a>n/2) a=a-n;
    }
    if(n==1) return ans;
    return 0;
}

/* Iterations determine the accuracy of the test */
bool Solovoy(long long p,int iteration){
    if(p<2) return false;
    if(p!=2 && p%2==0) return false;
    for(int i=0;i<iteration;i++){
        long long a=rand()%(p-1)+1;
        long long jacobian=(p+calculateJacobian(a,p))%p;
        long long mod=modulo(a,(p-1)/2,p);
        if(!jacobian || mod!=jacobian){
            return false;
        }
    }
    return true;
}


int main()
{
int choice,iterations;
long long p;
bool isPrime;
const char* bign = "big_number.txt";
cout<<"\nHow do you want to input the prime number?"<<endl;
cout<<"----------------------------------"<<endl;
cout<<"1) From keyboard"<<endl;
cout<<"2) From the \"big_number.txt\" file"<<endl;
cout<<"----------------------------------"<<endl;
cout<<"Your choice:";
cin >> choice;
if (choice==1)
  {
    cout<<"Enter the number that you want to test: \n";
    cin>>p;
  }
else
  {
    ifstream inFile(bign);
    if(!inFile) {
      cout << endl << "Failed to open file " << bign;
      return 1;
    }
      inFile >> p;
      cout<<"P is: "<<p<<endl;
  }
cout<<"Enter the number of iterations that you want to be used: ";
cin>>iterations;
cout << "Which algorith do you want to use to test-primality of a number?"<<endl;
cout<<"----------------------------------"<<endl;
cout<<"1) Fermat primality-test"<<endl;
cout<<"2) Miller-Rabin primality-test"<<endl;
cout<<"3) Solovay-Strassen primality-test"<<endl;
cout<<"----------------------------------"<<endl;
cout<<"Your choice:";
cin >> choice;
switch(choice) {
case 1:
    isPrime = Fermat(p,iterations);
    break;
case 2:
    isPrime = Miller(p,iterations);
    break;
case 3:
    isPrime = Solovoy(p,iterations);
    break;
default:
    cout<<"You didn't input correct number, so we chose Miller-Rabin's algorithm"<<endl;
    isPrime = Miller(p,iterations);
  }
if (isPrime==true)
  {
    cout<<"The number you have chocen "<<p<<" is probably prime."<<endl;
  }
else
  {
    cout<<"The number you have chocen "<<p<<" is composite."<<endl;
  }

return 0;

}
