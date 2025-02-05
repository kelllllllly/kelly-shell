// Kelly Veintimilla
// CSCI 411 LAB 2 SIMPLE SHELL 
// This program replicates a simple Linux CLI with a variety of commands. simply run ./compile.sh and type help to get started. 
// This program is seperated into functions; with the main function being loopShell, which is run in main. 

#include <iostream>
#include <string> 
#include <sstream>
#include <fstream> 
#include <cstdlib>
#include <unistd.h> // pipes don't run in windows; but luckily run through hopper (linux CLI)! 
#include <stdlib.h> 
#include <stdio.h>
#include <cstring> 
#include <csignal> 

using namespace std; 

ofstream history; // history file 

void loopShell(); // loops the "k$" prompt & handles CL user input 
void userManual(); // shows all commands in the shell 
void repeat(const string &parameters); // directs user input to a file or echoes it to the command line 
void hiMom(); // forks a child process & communicates with parent
void signalHandler(int signum); 
void print(); // prints a history file once shell is exited. 

int main(){

    // opens history file 
    history.open("history.txt", ios::app);
    if(!history){
        cerr << "cannot open history file." << endl;
        return 1;
    }

    signal(SIGINT, signalHandler); // handles crtl + c, also prints history if needed. 
    loopShell(); // starts the shell & loops
    return 0;
}
void loopShell(){
    string prompt = "k$ "; // my prompt 
    string cmdLine, cmd, parameters;  

    while(true){
        cout << prompt;
        getline(cin, cmdLine);
        if(!cmdLine.empty()){
            history << cmdLine << endl;
        }
        // parsing command 
        stringstream ss(cmdLine);
        ss >> cmd; 

        getline(ss >> ws, parameters);
        // used to quit the shell 
        if (cmd == "quit"){
            cout << "quitting" << endl;
            exit(0);
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
        if (cmd == "pwd"){
            system("pwd"); // print working directory for user to see current directory
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
    cout << "pwd - prints working directory" << endl; 
    cout << "repeat abc > abc.txt - sends text to a specified file, or echos words" << endl;
    cout << "himom - creates a child process using a fork. Where child and parent communicate." << endl; 
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
    const char *msg = "Hi mom!";

    if (pipe(pip) == -1){
        cout << "pipe failed" << endl;
        return; 
    }
    int pid = fork(); // creates child process 

    if (pid < 0){
        cout << "fork failed" << endl;
        return; 
    }

    // child process
    if(pid == 0){
        close(pip[0]); // close read
        cout << "Child: sends message to parent" << endl; 
        write(pip[1], msg, strlen(msg) + 1);
        close(pip[1]);    
    }
    else { // parent process
        close(pip[1]);
        read(pip[0], instring, sizeof(instring));
        cout << "Parent: hey kid, got your message! " << instring << endl; 
        close(pip[0]);

    }
} 
void signalHandler(int signum){
    cout << "recieved " << signum << ", exiting shell" << endl;
    print(); 
    _exit(0);
}   

void print(){
    ifstream infile("history.txt");
    if(!infile){
        cerr << "cannot open history file" << endl;
        return;
    }

    string line; 
    cout << "command history" << endl;
    while(getline(infile, line)){
        cout << line << endl;
    }
    infile.close();
}