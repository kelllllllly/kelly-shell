#include <iostream>
#include <string> 
#include <sstream>
#include <fstream> 
#include <cstdlib>
#include <unistd.h> // pipes don't run in windows; but luckily run through hopper! 
#include <stdlib.h> 
#include <stdio.h>

using namespace std; 

void loopShell();
void userManual();
void repeat(const string &parameters);
void hiMom(); 

int main(int argc, char *argv[]){
loopShell();

 
    return 0;
}
void loopShell(){
    string prompt = "k$ "; // my prompt 
    string cmdLine, cmd, parameters;  

    while(true){
        cout << prompt;
        getline(cin, cmdLine);

        // parsing command 
        stringstream ss(cmdLine);
        ss >> cmd; 

        getline(ss >> ws, parameters);
        // cout << cmd << endl;
        // cout << parameters << endl;
        // cout << pwd << endl; 
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
            repeat(parameters); 
        }
        if(cmd == "help"){
            userManual(); 
        }
        if (cmd == "chgd"){
            if(chdir(parameters.c_str()) != 0){
                cerr << "directory not found" << endl;
            } else{
                chdir(parameters.c_str()); 
            }
        }
        if (cmd == "dir"){
            system(("ls -l" + parameters).c_str());
        }

        if (cmd == "himom"){
            hiMom(); 
        }
        parameters = ""; // so it clears every loop, prevents dir from using the previous parased argument 

    }     
}
void userManual(){
    cout << "myprocess - shows current pid" << endl;
    cout << "allprocesses - shows all current processes running." << endl;
    cout << "clr - clears the screen" << endl;
    cout << "help - shows the usermanual" << endl;
    cout << "environ - list all enviroment settings" << endl;
    cout << "chgd <directory> - will change the current working directory" << endl;
    cout << "dir <directory> - will list the contets of the directory" << endl; 
    cout << "repeat abc > abc.txt- sends text to a specified file, or echos words" << endl; 
    cout << "quit - exit the shell" << endl; 
}
void repeat(const string &parameters){
    stringstream ss(parameters);
    string word, fileName, stop; // word for the text that will be input, filename for the file its being redirected to, stop for implementation of ">"
    bool redirect = false; 

    while (ss >> stop){
        if(stop == ">"){
            redirect = true; 
            ss >> fileName;
            break;
        }
            if(!word.empty()){
                word += " ";
    }
    word += stop; 
    }
    if(redirect){
        if(fileName.empty()){
            cerr << "file not specified." << endl;
            return;
        }

        ofstream outFile(fileName);
        if(!outFile){
            cerr << "can not open file: " << fileName << endl; 
            return; 
        }
        outFile << word << endl;
        outFile.close(); 
        cout << "text was sent to " << fileName << endl;
    } else {
        cout << word << endl; 
    }
    }
void hiMom(){
    int pip[2];
    char instring[20];
    const char* msg = "Hi mom!";

    if (pipe(pip) == -1){
        cout << "pipe failed" << endl;
        exit(1); 
    }
    
    int pid = fork(); // creates child process 

    if (pid < 0){
        cout << "fork failed" << endl;
        exit(2);
    }

    // child process
    if(pid == 0){
        close(pip[0]); // close read
        cout << "Child: sends message to parent"; 
        write(pip[1], msg, strlen(msg) + 1);
        close(pip[1]);    
    }
    else { // parent process
        close(pip[0]);
        read(pip[0], instring, sizeof(instring));
        cout << "Parent: hey kid, got your message! " << instring << endl; 
        close(pip[0]);

    }
} 
