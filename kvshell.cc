#include <iostream>
#include <string> 
#include <sstream> 

using namespace std; 

int main(int argc, char *argv[]){


string prompt = "k$ ";
    string cmdLine, inputRead; 
    int status; 

    while(true){
        cout << prompt;
        getline(cin, cmdLine);


        if (cmdLine == "quit"){

            cout << "exiting" << endl;
            exit(1);
        }

    }

    cout << "k$ " << cmdLine << endl; 



    return 0;
}


