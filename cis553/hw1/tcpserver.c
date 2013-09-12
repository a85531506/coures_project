#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdlib.h>
#include<pthread.h>

#define SERVER_PORT 5432
#define MAX_PENDING 5
#define MAX_LINE 256
void *mythread(void *arg);
int main()
{
    struct sockaddr_in sin;
    pthread_t tids[50];
    int i;
    int id[50];
	char buf[MAX_LINE];
        char *putout;
	int len;
	int s, new_s;
        int seq;
        
	for(i=0; i<10; i++) id[i] = i;
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
	while(i<50){
	  if((new_s = accept(s, (struct sockaddr *)&sin, &len)) < 0){
	    perror("simplex-talk: accept");
		exit(1);
		}
            pthread_create(&tids[i], NULL, mythread, NULL);
            i++;
		/*while(len = recv(new_s, buf, sizeof(buf), 0)){
                  seq = atoi(buf);
                  printf("HELLO %d\n", seq);
                  seq++;
                  snprintf(buf, 200, "%d\n", seq);
                  send(new_s, buf, len, 0);
                }*/
            //pthread_join(tids[i], NULL);
		//close(new_s);
		}
}

void *mythread(void *arg){
     int seq;
     int new_s;
     char buf[MAX_LINE];
     //new_s = *((int *)arg);
     int len;
     while(len = recv(new_s, buf, sizeof(buf), 0)){
        seq = atoi(buf);
        printf("HELLO %d\n", seq);
        seq++;
        snprintf(buf, 200, "%d\n", seq);
        send(new_s, buf, len, 0);
     }
     pthread_exit(NULL);
}
