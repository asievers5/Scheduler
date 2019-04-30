#include <stdio.h>

void firstComeFirstServe();
void shortestJobFirst();
void roundRobin();

int choice = 0;

int main(){
    switch(choice){
        case 1 : 
            firstComeFirstServe();
        case 2 :
            shortestJobFirst();
        case 3 :
            roundRobin();
    }

   return 0;
}

void firstComeFirstServe() {
    printf("First Come First Serve is running...");
}

void shortestJobFirst() {
    printf("Shortest Job First is running...");
}

void roundRobin(){
    printf("Round Robin is running...");
}