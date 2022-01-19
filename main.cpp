#include<iostream>
#include "tcp.hpp"
using namespace std;

int main(int argc, char* argv[]){
    if(argc != 2)
    {
        printf("%s <MY IP>\n", argv[0]);
        exit(1);
    }
    TCP server = TCP();
    server.Run(argv[1]);
}