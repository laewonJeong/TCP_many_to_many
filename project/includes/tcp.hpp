#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <pthread.h>
#include <thread>
using namespace std;
class TCP{
    public:
        void Server();
        void Run(const char* iip);
        void Client(const char* iip);
        void check(const char* iip);
    private:
        const char* server[8] = {"192.168.1.101", "192.168.1.102",
                                "192.168.1.103", "192.168.1.104",
                                "192.168.1.105", "192.168.1.106",
                                "192.168.1.107", "192.168.1.108"};
        int sock;
        struct sockaddr_in serv_addr;
        pthread_t snd_thread, rcv_thread;
        void * thread_return;
        int serv_sock, clnt_sock;
        struct sockaddr_in serv_adr, clnt_adr;
        int clnt_adr_sz;
        pthread_t t_id;
        vector<thread> workers;
};