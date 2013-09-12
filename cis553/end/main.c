#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>

pthread_t thread;
pthread_mutex_t mutex;
int total_num = 0;
int total_value = 0;
int record[5] = {0,0,0,0,0};
int large = 0;
int small = 0;

void *compute()
{
     char *string = "please input an integer: ";
   while(1){
     sleep(10);
     pthread_mutex_lock(&mutex);
     printf("\nmaximum: %d\n", large);
     printf("minimum: %d\n", small);
     printf("average: %d\n", total_value/(total_num==0?1:total_num));
     printf("last five: %d, %d, %d, %d, %d\n", record[4], record[3], record[2], record[1], record[0]);
     printf("%s\n", string);
     pthread_mutex_unlock(&mutex);
   }
}

int main()
{
    int input, i, result, length, is;
    char *ret;
    char *q="q";
    //int is = 0;
    //ttt = 1;
    memset(&thread, 0, sizeof(thread));
    printf("please input an integer: \n");
    scanf("%s", ret);
    result = strcmp(ret, q);
    //printf("%d", result);
    if(result==0)exit(1);

    is = 1;
    //length = strlen(ret);
    /*for(i=0;i<length;i++){
       if(ret[i]<'0'||ret[i]>'9'){
          isint = 0;
          break;
       }
    }
    while(!isint){
         printf("please input an integer: \n");
         scanf("%s", ret);

         result = strcmp(ret, q);
         if(result==0)exit(1);
         isint =1;
         for(i=0;i<strlen(ret);i++){
           if(ret[i]<'0'||ret[i]>'9'){
              isint = 0;
              break;
            }
         }
    }
    input = atoi(ret);
    total_num++;
    total_value += input;
    large = input;
    small = input;
    record[0] = input;
    pthread_create(&thread, NULL, compute, NULL);
    pthread_mutex_init(&mutex, NULL);
    while(1){
           printf("please input an integer: \n");
    scanf("%s", ret);
    result = strcmp(ret, q);
    if(result==0)exit(1);
    isint = 1;
    for(i=0;i<strlen(ret);i++){
       if(ret[i]<'0'||ret[i]>'9'){
          isint = 0;
          break;
       }
    }
    while(!isint){
         printf("please input an integer: \n");
         scanf("%s", ret);
         result = strcmp(ret, q);
         if(result==0)exit(1);
         isint =1;
         for(i=0;i<strlen(ret);i++){
           if(ret[i]<'0'||ret[i]>'9'){
              isint = 0;
              break;
            }
         }
    }
       input = atoi(ret);
       total_num++;
       total_value += input;
       if(input > large)large = input;
       if(input < small)small = input;
       for(i = 4; i >0; i--){
          record[i] = record[i-1];
       }
       record[0] = input;
    }
*/
}
