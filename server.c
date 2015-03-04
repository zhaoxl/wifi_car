#include <stdio.h> /* These are the usual header files */
#include <string.h>
#include <unistd.h> /* for close() */
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <wiringPi.h>

#define PORT 10000 /* Port that will be opened */
#define MAXDATASIZE 100 /* Max number of bytes of data */
#define ACT_TOP 119
#define ACT_RIGHT_TOP 101
#define ACT_RIGHT 3
#define ACT_RIGHT_DOWN 100
#define ACT_DOWN 115
#define ACT_LEFT_DOWN 97
#define ACT_LEFT 7
#define ACT_LEFT_TOP 113

void action(char *act);
void top();
void right_top();
void right();
void right_down();
void down();
void left_down();
void left();
void left_top();
void reset_gpio();

int main()
{
  printf("initialze GPIO\n");
  if (-1 == wiringPiSetup()) {
    printf("Setup wiringPi failed!\n");
    return 1;
  }
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
    
  printf("start listen\n");
  int sockfd; /* socket descriptors */
  struct sockaddr_in server; /* server's address information */
  struct sockaddr_in client; /* client's address information */
  socklen_t sin_size;
  int num;
  char recvmsg[MAXDATASIZE]; /* buffer for message */
  char sendmsg[MAXDATASIZE];
  char condition[] = "quit";
  /* Creating UDP socket */
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    /* handle exception */
    perror("Creating socket failed.");
    exit(1);
  }
  printf("created socket\n");

  bzero(&server,sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = htonl (INADDR_ANY);
  if (bind(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) {
    /* handle exception */
    perror("Bind error.");
    exit(1);
  }
  printf("binded socket\n");

  sin_size = sizeof(struct sockaddr_in);
  while (1) {
    printf("listeing...\n");
    num = recvfrom(sockfd,recvmsg,MAXDATASIZE,0,(struct sockaddr *)&client,&sin_size);

    if (num < 0){
      perror("recvfrom error\n");
      exit(1);
    }

    recvmsg[num] = '\0';
    action(recvmsg);
    //printf("You got a message (%s) from %s\n",recvmsg,inet_ntoa(client.sin_addr) ); /* prints client's IP */
    //if(strcmp(recvmsg,condition)==0) break;
    //int i=0;
    //for(i = 0 ; i < num ; i ++)
    //{
    //  sendmsg[i] = recvmsg[num-1-i];
    //}
    //sendmsg[num]='\0';
    //sendto(sockfd,sendmsg,strlen(sendmsg),0,(struct sockaddr *)&client,sin_size);
  }

  close(sockfd); /* close listenfd */
}

void action(char* act)
{
  int int_act = atoi(act);
  switch(int_act)
  {
    case ACT_TOP: top(); break;
    case ACT_RIGHT_TOP: right_top(); break;
    case ACT_RIGHT: right(); break;
    case ACT_RIGHT_DOWN: right_down(); break;
    case ACT_DOWN: down(); break;
    case ACT_LEFT_DOWN: left_down(); break;
    case ACT_LEFT: left(); break;
    case ACT_LEFT_TOP: left_top(); break;
    default: printf("unknown action!\n");
  }
  delay(10);
  reset_gpio();
}

void reset_gpio()
{
  digitalWrite(0, LOW);
  digitalWrite(1, LOW);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
}

void top()
{
  printf("上\n");
  digitalWrite(2, HIGH);
}
void right_top()
{
  printf("右上\n");
  digitalWrite(1, HIGH);
  digitalWrite(2, HIGH);
}
void right()
{
  printf("右\n");
  digitalWrite(1, HIGH);
}
void right_down()
{
  printf("右下\n");
  digitalWrite(1, HIGH);
  digitalWrite(3, HIGH);
}
void down()
{
  printf("下\n");
  digitalWrite(3, HIGH);
}
void left_down()
{
  printf("左下\n");
  digitalWrite(0, HIGH);
  digitalWrite(3, HIGH);
}
void left()
{
  printf("左\n");
  digitalWrite(0, HIGH);
}
void left_top()
{
  printf("左上\n");
  digitalWrite(0, HIGH);
  digitalWrite(2, HIGH);
}

