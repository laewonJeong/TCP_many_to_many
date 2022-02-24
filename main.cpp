#include "tcp.hpp"

int main(int argc, char* argv[]){
  if(argc != 2)
  {
    std::cout << argv[0] << " MY IP" << std::endl;
    exit(1);
  }
  int socks_cnt;
  char NAME[6];
  vector<int> server_ip;
  int idx = 0;
  char msg[1024];
  char name_msg[1030];
  NAME[0]='[';
  NAME[1]='S';
  NAME[2]='N';
  NAME[3]='0';
  NAME[4]=argv[1][12];
  NAME[5]=']';

  TCP tcp = TCP();
  cout << "Server_t() 실행" <<endl;
  tcp.Server_t();
    
  sleep(2);
    
  cout << "Client_t() 실행"<<endl;
  tcp.Client_t(argv[1]);
  sleep(2);
  socks_cnt = tcp.Scnt();
  sleep(2);
  for(int i =0;i<socks_cnt;i++){
       if (idx+1 == argv[1][12]-'0'){
            idx++;
        }
        server_ip.push_back(idx+1);
        idx++;
  }
  while(1){
      fgets(msg,1024,stdin);
      if(strcmp(msg,"exit\n")!=0)
        sprintf(name_msg,"%s %s",NAME, msg);
      for(int i=0;i<socks_cnt;i++){
          tcp.Send_Msg(name_msg,server_ip[i]);
      }
      if(strcmp(msg,"exit\n")==0)
        break;
  }

  return 0;
}
