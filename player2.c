#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <time.h>

#define PORT 22255
#define ARR_SIZE 1024

char bord[50][3];
char YOU[] = "P2";
int possition;
int prepos = 1;
char p1[ARR_SIZE];

int rd();
void creat_Bord();
void show_Bord();

int main () {
    int soc1;
    char buffer[ARR_SIZE];
    char dummy[ARR_SIZE];
    char laddle_And_Snake[ARR_SIZE];
    struct sockaddr_in server_Addr;
    memset(buffer,0x0,ARR_SIZE);
    soc1 = socket(AF_INET, SOCK_STREAM, 0);
    server_Addr.sin_family = AF_INET;
    server_Addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_Addr.sin_port = htons(PORT);
    creat_Bord();
    connect(soc1, (struct sockaddr *) &server_Addr, sizeof(server_Addr));
    printf("waiting for creating...\n");
    recv(soc1,buffer,ARR_SIZE,1024);
    send(soc1,"pass",10,0);
    recv(soc1,laddle_And_Snake,ARR_SIZE,1024);
    memset(buffer,0x0,ARR_SIZE);
    show_Bord();
    printf("%s\n",laddle_And_Snake);
    printf("You here: %d\t player1 here: 1\n",prepos);

    while(1){
        printf("waiting player 1 dice...\n");
        recv(soc1,buffer,ARR_SIZE,1024);
        int checkwin = strtol(buffer, NULL, 10);

        if (checkwin == 2){
            printf("you lose\n");
            break;
        }
        memset(buffer,0x0,ARR_SIZE);
        printf("Your turn!!!!\n");
        printf("Enter: ");
        fgets(dummy,ARR_SIZE,stdin);
        int dice = rd(1,6);
        char strDice[10];
        sprintf(strDice, "%d", dice);
        send(soc1, strDice, 10, 0);
        memset(dummy,0x0,ARR_SIZE);
        
        recv(soc1,buffer,ARR_SIZE,0);
        possition = strtol(buffer, NULL, 10);
        memset(buffer,0x0,ARR_SIZE);
        send(soc1,"pass",10,0);
        recv(soc1,buffer,ARR_SIZE,0);
        strcpy(p1,buffer);
        memset(buffer,0x0,ARR_SIZE);
        send(soc1,"pass",10,0);
        recv(soc1,buffer,ARR_SIZE,1024);
        if(possition > 50){
            possition = 50;
            }
        for(int i = 0; i < 50; i++){
            if(i == (possition - 1)){
                strcpy(bord[i],YOU);
                break;
                }
            }
        char preposstr[3];
        sprintf(preposstr, "%d", prepos);
        strcpy(bord[prepos - 1],preposstr);
        prepos = possition;
        show_Bord();
        printf("%s\n",laddle_And_Snake);
        printf("You here: %d\t player1 here: %s\n",possition,p1);
        checkwin = strtol(buffer, NULL, 10);
        if(checkwin == 1){
            memset(buffer,0x0,ARR_SIZE);
            printf("you win\n");
            break;
        }
        else if(checkwin == 0){
            memset(buffer,0x0,ARR_SIZE);
            }
        else if (checkwin == 2){
            printf("you lose\n");
            break;
            }
        }
        close(soc1);                                   
        return 0;
}
int rd(int x,int y){
    int range = y - x + 1;
    srand(time(0));
    int random_num = x + rand() % range;
    return random_num;
}

void creat_Bord(){
    for(int i = 0; i < 50;i++){
        char n[3];
        if(i == 0){
            strcpy(bord[i],YOU);
        }
        else{
        sprintf(n, "%d", i+1);
        strcpy(bord[i],n);
        }
    }
}

void show_Bord(){
    for(int i = 49; i >= 0; i--){
        
        if((i+1) % 5 == 0){
            printf("\n");
        }
        printf("%s\t",bord[i]);
    }
    printf("\n");
}