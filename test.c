#include<stdlib.h>


int main(){
    char * args[] = {"./ctest","Yarin",NULL};
    for (int i = 0; i <4 ; ++i) {
        if(!fork()){
            execvp(args[0],args);
        }
    }
    for (int i = 0; i < 4; ++i) {
        wait(NULL);
    }
    return 1;


}