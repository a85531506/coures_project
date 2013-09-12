#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

#define SERVER_PORT 5432
#define MAX_PENDING 5
#define MAX_LINE 256
fd_set readfds;
	char buf[MAX_LINE];
        char *putout;
	int len;
int main()
{
    struct sockaddr_in sin;
    struct timeval tv;

    int i;
	//char buf[MAX_LINE];
        //char *putout;
	//int len;
	int s, new_s;
        int seq;
        int nfds = 0;
     void handshake(int ndfs);   
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        FD_ZERO(&readfds);
	/* build address data structure */
	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(SERVER_PORT);
	
	/* setup passive open */
	if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0){
	  perror("simplex-talk: socket");
	  exit(1);
	  }
	if((bind(s, (struct sockaddr *)&sin, sizeof(sin))) <0){
	   perror("simplex-talk: bind");
	   exit(1);
	   }
	listen(s, MAX_PENDING);
	len = sizeof(struct sockaddr);
        i = 0;
	/* wait for connection, then receive and print text */
	while(1){
	  if((new_s = accept(s, (struct sockaddr *)&sin, &len)) < 0){
	    perror("simplex-talk: accept");
		exit(1);
		}
           FD_SET(new_s, &readfds);
           if(new_s > nfds)nfds = new_s + 1;
           switch(select(nfds, &readfds, &readfds, NULL, &tv))
           {
                case -1:exit(1);
                case 0:break;
                default:handshake(nfds);
           }
		//while(len = recv(new_s, buf, sizeof(buf), 0)){
                //  seq = atoi(buf);
                //  printf("HELLO %d\n", seq);
                //  seq++;
                //  snprintf(buf, 200, "%d\n", seq);
                //  send(new_s, buf, len, 0);
                //}
		}
}
void handshake(int ndfs)
{
   int i;
   //int len;
   int seq;
    for(i=0; i<ndfs; i++)
    {
       if(FD_ISSET(i, &readfds)){
         while(len = recv(i, buf, sizeof(buf), 0)){
        seq = atoi(buf);
        printf("HELLO %d\n", seq);
        seq++;
        snprintf(buf, 200, "%d\n", seq);
        send(i, buf, len, 0);
     }
       }
    }
}
    
