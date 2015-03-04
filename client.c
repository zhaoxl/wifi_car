#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> /* netbd.h is needed for struct hostent =) */

#define PORT 10000 /* Open Port on Remote Host */
#define MAXDATASIZE 100 /* Max number of bytes of data */


int main()
{
  char ch;
  struct termios old_term;
  struct termios cur_term;
  int ret ;
  tcgetattr(STDIN_FILENO, &old_term);
  memcpy(&cur_term, &old_term, sizeof(cur_term));
  cur_term.c_lflag &= ~(ICANON);
  cur_term.c_cc[VMIN] = 1;
  cur_term.c_cc[VTIME] = 0;
  ret = tcsetattr(STDIN_FILENO, TCSANOW, &cur_term);
  if (ret < 0)
  {
    printf("Can't set keyboard\n");
    return 1;
  }
  int fd, numbytes; /* files descriptors */
  char recvbuf[MAXDATASIZE]; /* buf will store received text */
  char sendbuf[MAXDATASIZE];
  struct hostent *he; /* structure that will get information about remote host */
  struct sockaddr_in server,client; /* server's address information */

  he = gethostbyname("192.168.2.75");

  if ((fd=socket(AF_INET, SOCK_DGRAM, 0))==-1){ /* calls socket() */
    printf("socket() error\n");
    exit(1);
  }

  bzero(&server,sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT); /* htons() is needed again */
  server.sin_addr = *((struct in_addr *)he->h_addr); /*he->h_addr passes "*he"'s info to "h_addr" */

  socklen_t len;
  len = sizeof(struct sockaddr_in);
  
  while(1)
  {
    ret = read(STDIN_FILENO, &ch, 1);
    if (ret <= 0)
    {
      printf("Error\n");
    }
    else
    {
      printf("%d\n", (int)ch);
      sprintf(sendbuf, "%d", (int)ch);
      sendto(fd, sendbuf, strlen(sendbuf), 0, (struct sockaddr *)&server,len);
    }

    fflush(stdout);
  }
  
  tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
  close(fd);
  return 0;
}





