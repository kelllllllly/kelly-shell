#include <iostream>
#include <string> 
#include <sstream> 
#include <cstdlib> 

using namespace std; 

void loopshell();

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

        if (cmdLine == "myprocesses"){
            system("ps");
        }
        
        cout << "k$ " << cmdLine << endl;
    }

     
}

