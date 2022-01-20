#include "tcp.hpp"

int clnt_cnt=0;
int clnt_socks[256];
pthread_mutex_t mutx;
char name[NAME_SIZE];
char msg[BUF_SIZE];

void* TCP::handle_clnt(void* arg)
{
   int clnt_sock=*((int*)arg);
   int str_len=0, i;
   char msg[BUF_SIZE];
   
   while((str_len=read(clnt_sock, msg, sizeof(msg)))!=0)
      TCP::send_msg(msg, str_len);
   
   pthread_mutex_lock(&mutx);
   for(i=0; i<clnt_cnt; i++)
   {
      if(clnt_sock==clnt_socks[i])
      {
         while(i++<clnt_cnt-1)
            clnt_socks[i]=clnt_socks[i+1];
         break;
      }
   }
   clnt_cnt--;
   pthread_mutex_unlock(&mutx);
   close(clnt_sock);
   return NULL;
}
void TCP::send_msg(char* msg, int len)
{
   int i;
   //fputs(msg, stdout);
   pthread_mutex_lock(&mutx);
   for(i=0; i<clnt_cnt; i++)
      write(clnt_socks[i], msg, len);
   pthread_mutex_unlock(&mutx);
}

void * TCP::send_msgc(void * arg)
{
   int sock=*((int*)arg);
   char name_msg[NAME_SIZE+BUF_SIZE];
   while(1) 
   {
      fgets(msg, BUF_SIZE, stdin);
      if(!strcmp(msg,"exit\n")) 
      {
         close(sock);
         exit(0);
      }
      sprintf(name_msg,"%s %s",name, msg);
      //fputs(name_msg,stdout);
      for(int i=0; i<clnt_cnt; i++)
        write(clnt_socks[i],  name_msg, strlen(name_msg));
      //write(sock, name_msg, strlen(name_msg));
   }
   return NULL;
}
   
void * TCP::recv_msg(void * arg)
{
   int sock=*((int*)arg);
   char name_msg[NAME_SIZE+BUF_SIZE];
   int str_len;
   while(1)
   {
      str_len=read(sock, name_msg, NAME_SIZE+BUF_SIZE-1);
      if(str_len==-1) 
         return (void*)-1;
      name_msg[str_len]=0;
      fputs(name_msg, stdout);
   }
   return NULL;
}

void TCP::Server(){
   pthread_mutex_init(&mutx, NULL);
   serv_sock=socket(PF_INET, SOCK_STREAM, 0);

   memset(&serv_adr, 0, sizeof(serv_adr));
   serv_adr.sin_family=AF_INET;
   serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
   serv_adr.sin_port=htons(PORT);
   
   if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1){
      cout << "bind() error" << endl;
      exit(1);
   }
   if(listen(serv_sock, 5)==-1){
      cout << "listen() error" << endl;
      exit(1);
   }
   while(1)
   {
      clnt_adr_sz=sizeof(clnt_adr);
      clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr,(socklen_t*)&clnt_adr_sz);
      
      pthread_mutex_lock(&mutx);
      clnt_socks[clnt_cnt++]=clnt_sock;
      pthread_mutex_unlock(&mutx);

      thread handle = thread(&TCP::handle_clnt,TCP(),(void*)&clnt_sock);
      handle.detach();
      //pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
      //pthread_detach(t_id);
      printf("%s와 연결 성공 \n", inet_ntoa(clnt_adr.sin_addr));
   }
   close(serv_sock);
}
void TCP::check(const char* iip){
   for(int i=0;i<8;i++){
      if(strcmp(iip,server[i]) != 0){
         workers.push_back(thread(&TCP::Client,TCP(),server[i]));
      }
      else{
         name[0] = '[';
         name[1] = 'S';
         name[2] = 'N';
         name[3] = '0';
         name[4] = iip[12];
         name[5] = ']';
      }
   }
   for(int i=0;i<7;i++){
      workers[i].join();
   }
}
void TCP::Client(const char* iip){
   sock=socket(PF_INET, SOCK_STREAM, 0);
   
   memset(&serv_addr, 0, sizeof(serv_addr));
   serv_addr.sin_family=AF_INET;
   serv_addr.sin_addr.s_addr=inet_addr(iip);
   serv_addr.sin_port=htons(PORT);
    
   while(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1){
      if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1){
      }
      sleep(1);
   }
   thread snd_th = thread(&TCP::send_msgc,TCP(),(void*)&sock);
   thread rcv_th = thread(&TCP::recv_msg,TCP(),(void*)&sock);
   snd_th.join();
   rcv_th.join();
   close(sock);  
}
void TCP::Run(const char* iip){
   thread t1 = thread(&TCP::Server,TCP());
   TCP::check(iip);

   t1.join();
}
