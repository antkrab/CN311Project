#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h> 

#define SUCCESS 0
#define ERROR   1
#define PORT1 12222
#define PORT2 22255
#define BUFFER_SIZE 1024

int rd();
int isDuplicate();
void *randLaddle();
void *randSnake();
void *randForward();
void *randReverse();
int checkDuplicates();
int checkWin();
int check_Position();

int arr_Laddle[5];
int arr_Snake[5];
int arr_Forward[5];
int arr_Reverse[5];
int player_1 = 1;
int player_2 = 1;
int win = 0;
int soc1,soc2,recvSoc1,recvSoc2;
char buffer1[BUFFER_SIZE];
char buffer2[BUFFER_SIZE];
char dummy1[BUFFER_SIZE];
char dummy2[BUFFER_SIZE];
struct sockaddr_in client_Addr, server_Addr_1, server_Addr_2;
socklen_t client_Lenght;

int main() {
    pthread_t thread1,thread2,thread3,thread4;
    memset(buffer1,0x0,BUFFER_SIZE);
    memset(buffer2,0x0,BUFFER_SIZE);
    memset(dummy1,0x0,BUFFER_SIZE);

    soc1 = socket(AF_INET, SOCK_STREAM, 0);
    soc2 = socket(AF_INET, SOCK_STREAM, 0);

    server_Addr_1.sin_family = AF_INET;
    server_Addr_1.sin_addr.s_addr = htonl(INADDR_ANY);
    server_Addr_1.sin_port = htons(PORT1);

    server_Addr_2.sin_family = AF_INET;
    server_Addr_2.sin_addr.s_addr = htonl(INADDR_ANY);
    server_Addr_2.sin_port = htons(PORT2);
    bind(soc1, (struct sockaddr *) &server_Addr_1, sizeof(server_Addr_1));
    bind(soc2, (struct sockaddr *) &server_Addr_2, sizeof(server_Addr_2));

    listen(soc1,0);
    listen(soc2,0);

    printf("Waiting for player\n");
    recvSoc1 = accept(soc1, (struct sockaddr *) &client_Addr, &client_Lenght);
    recvSoc2 = accept(soc2, (struct sockaddr *) &client_Addr, &client_Lenght);

    printf("starting....\n");
    
    pthread_create(&thread1, NULL, randLaddle, NULL);
    pthread_create(&thread2, NULL, randSnake, NULL);
    pthread_create(&thread3, NULL, randForward, NULL);
    pthread_create(&thread4, NULL, randReverse, NULL);
    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    pthread_join(thread3,NULL);
    pthread_join(thread4,NULL);
    while ((checkDuplicates(arr_Laddle,arr_Snake,5,5) == SUCCESS) || isDuplicate(arr_Laddle,5) == SUCCESS || isDuplicate(arr_Snake,5) == SUCCESS  ){
        printf("Detect!\n");
        pthread_create(&thread1, NULL, randLaddle, NULL);
        pthread_create(&thread2, NULL, randSnake, NULL);
        pthread_join(thread1,NULL);
        pthread_join(thread2,NULL);
    }
    printf("success\n");
    for(int i = 0; i < 5; i++){
        printf("Laddel %d : Forward %d\n",arr_Laddle[i],arr_Forward[i]);
    }
    for(int i = 0; i < 5; i++){
        printf("Snake %d : Reverst %d\n",arr_Snake[i],arr_Reverse[i]);
    }

    for (int i = 0; i < 5; i++)
    {
        char strbuff[BUFFER_SIZE];
        sprintf(strbuff,"Laddel %d : Forward %d\tSnake %d : Reverst %d\n",arr_Laddle[i],arr_Forward[i],arr_Snake[i],arr_Reverse[i]);
        strcat(buffer1, strbuff);
        strcat(buffer2, strbuff);
    }
    send(recvSoc1,"1",1,0);
    send(recvSoc2,"1",1,0);
    recv(recvSoc1,dummy1,BUFFER_SIZE,0);
    memset(dummy1,0x0,BUFFER_SIZE);

    recv(recvSoc2,dummy2,BUFFER_SIZE,0);
    memset(dummy2,0x0,BUFFER_SIZE);

    send(recvSoc1,buffer1,BUFFER_SIZE,0);
    send(recvSoc2,buffer2,BUFFER_SIZE,0);
    memset(buffer1,0x0,BUFFER_SIZE);
    memset(buffer2,0x0,BUFFER_SIZE);
    while (1){
        printf("waiting for player1 dice...\n");
        recv(recvSoc1, buffer1, BUFFER_SIZE, 0);
        int num_1 = strtol(buffer1, NULL, 10);
        memset(buffer1,0x0,BUFFER_SIZE);
        player_1 += num_1;
        player_1 = check_Position(player_1);
        printf("plyer1: %d\n",player_1);
        if(checkWin() == SUCCESS){
            printf("player 1 win!!!\n");
            char str_P1[10];
            char str_P2[10];
            sprintf(str_P1, "%d", player_1); 
            sprintf(str_P2, "%d", player_2); 
            send(recvSoc1,str_P1,10,0);
            recv(recvSoc1,dummy1,BUFFER_SIZE,0);
            memset(dummy1,0x0,BUFFER_SIZE);

            send(recvSoc1,str_P2,10,0);
            recv(recvSoc1,dummy1,BUFFER_SIZE,0);
            memset(dummy1,0x0,BUFFER_SIZE);

            send(recvSoc1,"1",10,0);
            send(recvSoc2,"2",10,0);
            break;
        }
        else{
            char str_P1[10];
            char str_P2[10];
            sprintf(str_P1, "%d", player_1); 
            sprintf(str_P2, "%d", player_2); 
            send(recvSoc1,str_P1,10,0);
            recv(recvSoc1,dummy1,BUFFER_SIZE,0);
            memset(dummy1,0x0,BUFFER_SIZE);

            send(recvSoc1,str_P2,10,0);
            recv(recvSoc1,dummy1,BUFFER_SIZE,0);

            send(recvSoc1,"0",10,0);

        }
        send(recvSoc2,"pass",10,0);

        printf("waiting for player2 dice...\n");
        recv(recvSoc2, buffer2, BUFFER_SIZE, 0);
        int num_2 = strtol(buffer2, NULL, 10);
        memset(buffer2,0x0,BUFFER_SIZE);
        player_2 += num_2;
        player_2 = check_Position(player_2);
        printf("plyer2: %d\n",player_2);
        if(checkWin() == SUCCESS){
            printf("player 2 win!!!\n");
            char str_P1[10];
            char str_P2[10];
            sprintf(str_P1, "%d", player_1); 
            sprintf(str_P2, "%d", player_2); 
            send(recvSoc2,str_P2,10,0);
            recv(recvSoc2,dummy2,BUFFER_SIZE,0);
            memset(dummy2,0x0,BUFFER_SIZE);

            send(recvSoc2,str_P1,10,0);
            recv(recvSoc2,dummy2,BUFFER_SIZE,0);
            memset(dummy2,0x0,BUFFER_SIZE);

            send(recvSoc2,"1",10,0);
            send(recvSoc1,"2",10,0);
            break;
        }
        else{
            char str_P1[10];
            char str_P2[10];
            sprintf(str_P1, "%d", player_1); 
            sprintf(str_P2, "%d", player_2); 
            send(recvSoc2,str_P2,10,0);
            recv(recvSoc2,dummy2,BUFFER_SIZE,0);
            memset(dummy2,0x0,BUFFER_SIZE);

            send(recvSoc2,str_P1,10,0);
            recv(recvSoc2,dummy2,BUFFER_SIZE,0);
            memset(dummy2,0x0,BUFFER_SIZE);

            send(recvSoc2,"0",10,0);
            send(recvSoc1,"pass",10,0);
        }
    }
    close(soc1);
    close(soc2);
    return 0;
}

int rd(int x,int y){
    int range = y - x + 1;
    // srand(time(0));
    int random_num = x + rand() % range;
    return random_num;
}

int isDuplicate(int arr[], int size) {
    for(int i = 0; i < size; i++){
        int n = arr[i];
        int cnt = 0;
        for(int j = 0; j < size; j ++){
            if(n == j){
                cnt++;
                if(cnt > 1){
                    return SUCCESS;
                }
            }
        }
    }
    return ERROR;
}

void *randLaddle(){
    int n;
    for(int i = 0; i < 5; i++){
        if(i % 2 == 0){
            n = rd(2,20);
        }
        else{
            n = rd(21,42);
        }
        arr_Laddle[i] = n;
        // sleep(1);
    }
    pthread_exit(NULL);

}

void *randSnake(){
    int n;
    for(int i = 0; i < 5; i++){
        if(i % 2 == 0){
            n = rd(5,25);
        }
        else{
            n = rd(26,49);
        }

        arr_Snake[i] = n;
        // sleep(1);
    }
    pthread_exit(NULL);
}

void *randForward(){
    int n;
    for(int i = 0; i < 5; i++){
        if(i % 2 == 0){
            n = rd(5,20);
        }
        else{
            n = rd(1,10);
        }
        arr_Forward[i] = n;
        // sleep(1);
        
    }
    pthread_exit(NULL);
}

void *randReverse(){
    int n;
    for(int i = 0; i < 5; i++){        
        if( i % 2 == 0){
            n = rd(1,10);
        }
        else{
            n = rd(1,26);
        }
        arr_Reverse[i] = n;
        // sleep(1);
        
    }
    pthread_exit(NULL);
}

int checkDuplicates(int arr1[], int arr2[], int len1, int len2) {
    for(int i=0; i<len1; i++) {
        for(int j=0; j<len2; j++) {
            if(arr1[i] == arr2[j]) {
                return SUCCESS; 
            }
        }
    }
    return ERROR; 
}
int checkWin(){
    if(player_1 >= 50 || player_2 >= 50){
        return SUCCESS;
    }
    else{
        return ERROR;
    }
}

int check_Position(int p){
    for(int i = 0; i < 5; i++){
        if(p == arr_Laddle[i]){
            p += arr_Forward[i];
            printf("Forward to %d\n",p);
            return p;
        }
        else if (p == arr_Snake[i]){
            p -= arr_Reverse[i];
            printf("Reverse to %d\n",p);
            return p;
        }
    }
    return p;
}