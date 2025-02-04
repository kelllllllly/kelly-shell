#include <iostream>
#include <string> 
#include <sstream> 
#include <cstdlib>
#include <unistd.h> 

using namespace std; 

void loopshell();
void usermanual();

int main(int argc, char *argv[]){

loopshell();

    return 0;
}



void loopshell(){
    string prompt = "k$ ";
    string cmdLine, inputRead; 
    int status; 

    while(true){
        cout << prompt;
        getline(cin, cmdLine);

        if (cmdLine == "quit"){

            cout << "quitting" << endl;
            exit(1);
        }

        if (cmdLine == "myprocess"){
            cout << getpid() << endl;

        }

        if (cmdLine == "allprocesses"){
            system("ps");
        }

        if(cmdLine == "clr"){
            system("clear");
        }

        if(cmdLine == "help")
            usermanual(); 
        //cout << "k$ " << endl;
    }

     
}

void usermanual(){
    cout << "myprocess: shows current pid" << endl;
    cout << "allprocesses: shows all current processes running." << endl;
    cout << "clr: clears the screen" << endl;
    cout << "help: shows the usermanual" << endl;
}