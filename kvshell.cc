#include <iostream>
#include <string> 
#include <sstream> 

using namespace std; 

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

    }

    cout << "k$ " << cmdLine << endl; 
}

