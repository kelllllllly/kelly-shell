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
    string prompt = "k$ "; // my prompt 
    string cmdLine, cmd, parameters;  


    while(true){
        cout << prompt;
        getline(cin, cmdLine);

        // parsing command 
        stringstream ss(cmdLine);
        ss >> cmd; 

        //parameters = cmdLine.substr(cmd.length()+1); 
        if (cmd == "quit"){

            cout << "quitting" << endl;
            exit(1);
        }

        if (cmd == "myprocess"){
            cout << getpid() << endl;

        }

        if (cmd == "allprocesses"){
            system("ps"); // executed using system 
        }

        if(cmd == "clr"){
            system("clear"); // executed using system
        }
        
        if (cmd == "environ"){
            system("env"); // executed using system
        }

        if(cmd == "repeat"){
            
        }

        if(cmd == "help"){
            usermanual(); 
        }
    
  
        cout << prompt << cmd; 
    }

     
}

void usermanual(){
    cout << "myprocess - shows current pid" << endl;
    cout << "allprocesses - shows all current processes running." << endl;
    cout << "clr - clears the screen" << endl;
    cout << "help - shows the usermanual" << endl;
    cout << "environ - list all enviroment settings" << endl; 
}

void repeat(){

}