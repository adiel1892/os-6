#include<stdlib.h>


int main(){
    char * args[] = {"./ctest","Yarin",NULL};
    for (int i = 0; i <2 ; ++i) {
        if(!fork()){
            execvp(args[0],args);
        }
    }
    for (int i = 0; i < 2; ++i) {
        wait(NULL);
    }
    return 1;


}