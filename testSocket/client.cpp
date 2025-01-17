
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <vector>


#include <algorithm>

#include "./include/global.h"
#include "./include/logger.h"


using namespace std;

//#define PORT2 "3491"
#define MSG_SIZE 256
//#define PORT "3490"
#define MAXDATASIZE 100
using namespace std;

// 取得 IPv4 或 IPv6 的 sockaddr：
void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
      return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// use like split_msg(msg," ", result);
void split_msg(string& src, const string& separator, vector<string>& dest)
{
    string str = src;
    string substring;
    string::size_type start = 0, index;
    dest.clear();
    index = str.find_first_of(separator,start);
    do
    {
        if (index != string::npos)
        {    
            substring = str.substr(start,index-start );
            dest.push_back(substring);
            start =index+separator.size();
            index = str.find(separator,start);
            if (start == string::npos) break;
        }
    }while(index != string::npos);

    //the last part
    substring = str.substr(start);
    dest.push_back(substring);
}

void log_IP(string ip){
string command = "IP";
cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
cse4589_print_and_log("IP:%s\n", ip.c_str());
cse4589_print_and_log("[%s:END]\n", command.c_str());
}

void log_AUTHOR() {
string command = "AUTHOR";
cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
string ubit_name_1 = "lchen76";
string ubit_name_2 = "ziangli";
cse4589_print_and_log("I, %s, %s, have read and understood the course academic integrity policy.\n", ubit_name_1.c_str(), ubit_name_2.c_str());
cse4589_print_and_log("[%s:END]\n", command.c_str());
}

void log_PORT(int port) {
string command = "PORT";
cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
cse4589_print_and_log("PORT:%d\n", port);
cse4589_print_and_log("[%s:END]\n", command.c_str());
}

bool cmp(string p[], string q[]) {
  int p1, p2;
  if (p[2] == "") {
    p1 = 100000;
  }
  else {
    p1 = stoi(p[2]);
  }
  if (p[2] == "") {
    p2 = 100000;
  }
  else {
    p2 = stoi(q[2]);
  }
  
  return p1 < p2;
}

void log_LIST(string list[][3]) {
string command = "LIST";
cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
string **res = new string*[4];
  for (int i = 0; i < 4; ++i) {
    res[i] = new string[3];
    res[i][0] = list[i][0];
    res[i][1] = list[i][1];
    res[i][2] = list[i][2];
  }


  sort(res, res + 4, cmp);
for(int i = 0; i < 4; ++i){
  if(res[i][0] == "") break;
  cse4589_print_and_log("%-5d%-35s%-20s%-8d\n", i, res[i][0].c_str(), res[i][1].c_str(), res[i][2].c_str());
}
cse4589_print_and_log("[%s:END]\n", command.c_str());
}

void log_EVENT(string client_ip, string msg) {
string command = "EVENT";
cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
cse4589_print_and_log("msg from:%s\n[msg]:%s\n", client_ip.c_str(), msg.c_str());
cse4589_print_and_log("[%s:END]\n", command.c_str());
}

int client_process(string MYPORT)
{

   int sockfd;//numbytes;
   //char buf[MAXDATASIZE];
    // close(sockfd);
    // return 0;
   struct addrinfo hints, *servinfo,*clientinfo, *p, *p2;
   int rv;
   char s[INET6_ADDRSTRLEN]={""};
   string blank = " ";

   vector<string> Msg;

   bool login_st = 0; // login state 0 means offline, 1 means online

   memset(&hints, 0, sizeof hints); // 确保 struct 为空
   hints.ai_family = AF_INET; // good for ipv4 and ipv6
   hints.ai_socktype = SOCK_STREAM;

   string myCLientInfo[3];

   string Clientlist[4][3];
   for(int i = 0; i < 4; ++i){
     for(int j = 0; j < 3; ++j) {
       Clientlist[i][j] = "";
     }
   }

   string msg="";
   char charmsg[50000];
   string myServerInfo[2]={"", ""};
   string myHostname;
   char Hostname_char[40];
   vector<string> BlockList;



   vector<string> msg_p;
   

    const char* charPORT = MYPORT.c_str();
    if ((rv = getaddrinfo(NULL, charPORT, &hints, &clientinfo)) != 0) { // return not zero value when error happen
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv)); // print error with gai_strerror
      return 1;
      }


   // if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) { // return not zero value when error happen
   //      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv)); // print error with gai_strerror
   //      return 1;
   //  }


    for(p = clientinfo; p != NULL; p = p->ai_next) {
      if ((sockfd = socket(p->ai_family, p->ai_socktype, 
          p->ai_protocol)) == -1) {
          perror("client: socket"); 
          continue;
        }
      if (bind(sockfd, p->ai_addr, p->ai_addrlen) < 0) {
         close(sockfd);
         continue;
      }
    }
    gethostname(Hostname_char, 40);
    myHostname = Hostname_char;

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);

   
    myCLientInfo[0] = myHostname;
    myCLientInfo[1] = s;
    myCLientInfo[2] = MYPORT;


    cout << "myCLientInfo is" << myCLientInfo << endl;

    

//finish initialize here

  fd_set readfds;   // for select
  int fdmax = sockfd;
  // core loop
  while (1) {
   if(login_st == 0){
    FD_ZERO(&readfds);
    FD_SET(fileno(stdin), &readfds);
    if (FD_ISSET(fileno(stdin), &readfds)) {
      read(fileno(stdin), charmsg, sizeof(msg));
      msg = charmsg;
      fflush(stdin);
      split_msg(msg," ", msg_p);

      int mark;
      if(msg_p[0] == "LOGIN"){
          mark =1;
      }
      if(msg_p[0] == "EXIT"){
          mark =2;
      }
      if(msg_p[0] == "IP"){
          mark =3;
      }
      if(msg_p[0] == "AUTHOR"){
          mark =4;
      }
      if(msg_p[0] == "PORT"){
          mark =5;
      }



      switch (mark){
            case 1:{
               cse4589_print_and_log("[%s:SUCCESS]\n", msg_p[0]);
                  //need change
                  myServerInfo[0] = msg_p[1]; //ip
                  myServerInfo[1] = msg_p[2]; //port
                      
                      //need change form of data here
                      if ((rv = getaddrinfo(myServerInfo[0].c_str(), myServerInfo[1].c_str(), &hints, &clientinfo)) != 0) { // return not zero value when error happen
                          fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv)); // print error with gai_strerror
                          cse4589_print_and_log("[%s:ERROR]\n", msg_p[0]);
                           break;
                           }
                       for(p2 = servinfo; p2 != NULL; p2 = p2->ai_next) {
                         if (connect(sockfd, p2->ai_addr, p2->ai_addrlen) == -1) {
                            close(sockfd);
                            perror("client: connect");
                            continue;
                          }
                          break;
                        }
                        if (p2 == NULL) {
                        fprintf(stderr, "client: failed to connect\n");
                        cse4589_print_and_log("[%s:ERROR]\n", msg_p[0]);
                        break;
                        }

                     inet_ntop(p2->ai_family, get_in_addr((struct sockaddr *)p2->ai_addr), s, sizeof s);
           
                     printf("client: connecting to %s\n", s);
                     freeaddrinfo(servinfo); // 全部皆以这个 structure 完成
                     string temp_num = "1 ";
                     msg = temp_num + myCLientInfo[0] + blank + myCLientInfo[1] + blank +myCLientInfo[2];
                     if(send(sockfd, (const char*)msg.c_str(), msg.length(), 0) == msg.length())
                        printf("Done!\n");
                  cse4589_print_and_log("[%s:END]\n", msg_p[0]);
               } 
            case 2:{
        cse4589_print_and_log("[%s:SUCCESS]\n", msg_p[0]);
        string temp_num = "5 ";
        msg = temp_num +  blank + myCLientInfo[1];
        if(send(sockfd, (const char*)msg.c_str(), msg.length(), 0) == msg.length())
              printf("Done!\n");
        cse4589_print_and_log("[%s:END]\n", msg_p[0]);
        break;
      }

      case 3:{
          log_IP(myCLientInfo[1]);
          break;
      }
      case 4:{
          log_AUTHOR();
          break;
      }
      case 5:{
          log_PORT(stoi(MYPORT));
          break;
      }
      }
   }else{
    FD_ZERO(&readfds);
    FD_SET(fileno(stdin), &readfds);
    FD_SET(sockfd, &readfds);
    fdmax = sockfd;
    select(fdmax + 1, &readfds, NULL, NULL, NULL);
    // handle commands
    if (FD_ISSET(fileno(stdin), &readfds)) {
      read(fileno(stdin), charmsg, sizeof charmsg);
      msg = charmsg;
      fflush(stdin);
      split_msg(msg," ", msg_p);


      int mark;
      if(msg_p[0] == "LOGOUT"){
          mark =1;
      }
      if(msg_p[0] == "EXIT"){
          mark =2;
      }
      if(msg_p[0] == "IP"){
          mark =3;
      }
      if(msg_p[0] == "AUTHOR"){
          mark =4;
      }
      if(msg_p[0] == "PORT"){
          mark =5;
      }
      if(msg_p[0] == "LIST"){
          mark =6;
      }
      if(msg_p[0] == "REFRESH"){
          mark =7;
      }
      if(msg_p[0] == "BROADCAST"){
          mark =8;
      }
      if(msg_p[0] == "BLOCK"){
          mark =9;
      }
      if(msg_p[0] == "UNBLOCK"){
          mark =10;
      }
      if(msg_p[0] == "SEND"){
          mark =11;
      }

      switch(mark){
      case 1:{
        cse4589_print_and_log("[%s:SUCCESS]\n", msg_p[0]);
        string temp_num = "4 ";
        msg = temp_num + myCLientInfo[0] + blank + myCLientInfo[1] + blank + myCLientInfo[2];
        if(send(sockfd, (const char*)msg.c_str(), msg.length(), 0) == msg.length())
              printf("Done!\n");
        cse4589_print_and_log("[%s:END]\n", msg_p[0]);
        break;
      }
      
      case 2: {
        cse4589_print_and_log("[%s:SUCCESS]\n", msg_p[0]);
        string temp_num = "5 ";
        msg = temp_num + myCLientInfo[0] + blank + myCLientInfo[1] + blank + myCLientInfo[2];
        if(send(sockfd, (const char*)msg.c_str(), msg.length(), 0) == msg.length())
              printf("Done!\n");
        cse4589_print_and_log("[%s:END]\n", msg_p[0]);
        break;
      }

      case 3: {
          log_IP(myCLientInfo[1]);
          break;
      }
      case 4:{
          log_AUTHOR();
          break;
      }
      case 5:{
          log_PORT(stoi(MYPORT));
          break;
      }
      case 6: {
          log_LIST(Clientlist);
          break;     
      }
      case 7:{
        cse4589_print_and_log("[%s:SUCCESS]\n", msg_p[0]);
          msg = "7";
        if(send(sockfd, (const char*)msg.c_str(), msg.length(), 0) == msg.length())
          printf("Done!\n");
        cse4589_print_and_log("[%s:END]\n", msg_p[0]);
        break;
      }
      case 8:{
        cse4589_print_and_log("[%s:SUCCESS]\n", msg_p[0]);
        string temp_num = "6 ";
        msg = temp_num + blank + myCLientInfo[1] + (string)blank + msg;
        if(send(sockfd, (const char*)msg.c_str(), msg.length(), 0) == msg.length())
          printf("Done!\n");
        cse4589_print_and_log("[%s:END]\n", msg_p[0]);
        break;
      }
      case 9: {
          cse4589_print_and_log("[%s:SUCCESS]\n", msg_p[0]);
          vector<string>::iterator ret;
          ret = find(BlockList.begin(), BlockList.end(), msg_p[1]);
          if(ret == BlockList.end()) {
            BlockList.push_back(msg_p[1]);
            string temp_num = "2 ";
            msg = temp_num + myCLientInfo[1] + blank + msg_p[1];
            if(send(sockfd, (const char*)msg.c_str(), msg.length(), 0) == msg.length())
              printf("Done!\n");
            cse4589_print_and_log("[%s:SUCCESS]\n", msg_p[0]);
            break;
          }else{
            cse4589_print_and_log("[%s:ERROR]\n", msg_p[0]);
            break;
          }
      }
      case 10:{
        cse4589_print_and_log("[%s:SUCCESS]\n", msg_p[0]);
        vector<string>::iterator ret;
        ret = find(BlockList.begin(), BlockList.end(), msg_p[1]);
        if(ret == BlockList.end()) {
          cse4589_print_and_log("[%s:ERROR]\n", msg_p[0]);
          break;
        }
        else {
          BlockList.erase(ret);
          string temp_num = "3 ";
          msg = temp_num + myCLientInfo[1] + blank + msg_p[1];
          if(send(sockfd, (const char*)msg.c_str(), msg.length(), 0) == msg.length())
            printf("Done!\n");
          cse4589_print_and_log("[%s:SUCCESS]\n", msg_p[0]);
          break;
        }
      }
      case 11:{
        cse4589_print_and_log("[%s:SUCCESS]\n", msg_p[0]);
        msg = "";
        for(int i = 2; i < msg_p.size(); ++i){
          msg = msg +blank+ msg_p[i];
        }
        string temp_num = "0 ";
        msg = temp_num + msg_p[1] + blank + msg;
        if(send(sockfd, (const char*)msg.c_str(), msg.length(), 0) == msg.length())
            printf("Done!\n");
          cse4589_print_and_log("[%s:SUCCESS]\n", msg_p[0]);
          break;
      }

      }
      //handleCommands(msg, 1);
    }  // handle new client connection
    else if (FD_ISSET(sockfd, &readfds)) {
      if(recv(sockfd, charmsg, sizeof charmsg, 0) == 0){
        close(sockfd);
        sockfd = 0;
      }else{
      msg = charmsg;
      split_msg(msg," ", msg_p);
      switch (stoi(msg_p[0])){

        //message
          case 0:{
          
          msg = msg_p[3];
          for(int i = 4; i < msg_p.size(); ++i){
            msg = msg +blank+ msg_p[i];
          }
          log_EVENT(msg_p[1],msg);
          cout<<msg_p[1]<<" "<<msg<<endl;
          break;
          }
        
        //update list
        case 1:{

          for(int i =0; i < (msg_p.size()-1)/3; i++){
            for(int j = 1; j < msg_p.size(); j++){
               Clientlist[i][(j-1) % 3] = msg_p[j];
            }
          }

          break;
        } 

        case 6:{
          msg = msg_p[2];
          for(int i = 3; i < msg_p.size(); ++i){
            string blank = " ";
            msg = msg +blank+ msg_p[i];
          }
          log_EVENT(msg_p[1], msg);
          cout<<msg_p[1]<<" "<<msg<<endl;
        }       
      }
        //handleClientEvents(msg);
      }
    }
  }

}




//send message here*****************************   
// char *msg = (char*) malloc(sizeof(char)*MSG_SIZE);
//     memset(msg, '\0', MSG_SIZE);
//         if(fgets(msg, MSG_SIZE-1, stdin) == NULL) //Mind the newline character that will be written to msg
//             exit(-1);
//         printf("\nSENDing it to the remote server ... ");
//         if(send(sockfd, msg, strlen(msg), 0) == strlen(msg))
//             printf("Done!\n");
//send message end*****************************    

//recv message start*****************************    
   // if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
   //     perror("recv");
   //     exit(1);
   //  }
   //  buf[numbytes] = '\0';
   //  printf("client: received '%s'\n",buf);
//recv message end*****************************    

    // close(sockfd);
    // return 0;
  
}
}
int main(int argc,char** argv){
  string MYPORT = argv[1];
  client_process(MYPORT);
}