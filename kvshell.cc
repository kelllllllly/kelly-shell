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
#include <csignal>
#include <cstring> 

using namespace std;

const string historyFile = "history.txt";

void userManual(); // shows all commands in the shell
void repeat(const string &parameters); // directs user input to a file or echoes it to the command line
void hiMom(); // forks a child process & communicates with parent
void signalHandler(int signum); 

int main() {
    signal(SIGINT, signalHandler);  // handles crtl + c, also prints history if needed. 

    string prompt = "k$ "; // my prompt
    string cmdLine, cmd, parameters;
    
    ofstream historyOut(historyFile, ios::app); // open history file in append mode
    if (!historyOut.is_open()) {
        cerr << "cannot open history file." << endl;
        return 1;
    }
    // loops the shell & handles user input & printing the command history once the loop is finished. 
    while (true) {
        cout << prompt;
        // Read the entire input line from the user
        if (!getline(cin, cmdLine)) {
            break;  // exits on eof or an input error 
        }

        if (cmdLine.empty()) {
            continue;
        }

        historyOut << cmdLine << endl; // saves a command to history 
        
        stringstream ss(cmdLine);
        ss >> cmd;

        if (cmd == "quit") { // used to quit the shell 
            cout << "quitting" << endl;
            break;
        }

        getline(ss >> ws, parameters);

        if (cmd == "myprocess") { // if cmd = myprocess, getpid() prints
            cout << getpid() << endl;
        }
        else if (cmd == "allprocesses") { // if cmd = allprocesses, then "ps" is executed by system 
            system("ps"); // executed using system
        }
        else if (cmd == "clr") { // clears screen 
            system("clear"); // executed using system 
        }
        else if (cmd == "environ") { // shows all enviroment variables if user types environ 
            system("env"); // executed using system 
        }
        else if (cmd == "chgd") {
            if (chdir(parameters.c_str()) != 0) {
                cerr << "directory not found" << endl;
            }
        }
        else if (cmd == "dir") {
            system(("ls -l " + parameters).c_str()); // executed using system 
        }

        else if (cmd == "help"){ // executes userManual function, which shows all available commands 
            userManual(); 
        }
        else if(cmd == "repeat"){ // if cmd = repeats then repeat function will execute, with either writing to a file or 
            repeat(parameters);   //  echoing the input from user. 
        }
        else if(cmd == "himom"){ // executes my hiMom function 
              hiMom(); 
        }

        parameters.clear(); // so it clears every loop, prevents dir from using the previous parased argument 

    // displays history when exiting loop
    historyOut.close();
    ifstream historyIn(historyFile);
    if (historyIn.is_open()) {
        cout << "command history" << endl;
        string line;
        while (getline(historyIn, line)) {
            cout << line << endl;
        }
        historyIn.close();
    }

    return 0;
}
}

void signalHandler(int signum) { // handles when a user wants to exit the shell using crtl + c, also prints out command history in the event 
    ifstream history(historyFile);
    cout << "command history" << endl;
    string line;

    while (getline(history, line)) {
        cout << line << endl;
    }

    history.close();
    exit(signum);
}

void userManual(){ // user manual showing all available commands 
    cout << "myprocess - shows current pid" << endl;
    cout << "allprocesses - shows all current processes running." << endl;
    cout << "clr - clears the screen" << endl;
    cout << "help - shows the usermanual" << endl;
    cout << "environ - list all enviroment settings" << endl;
    cout << "chgd <directory> - will change the current working directory" << endl;
    cout << "dir <directory> - will list the contets of the directory" << endl;
    cout << "repeat abc > abc.txt - sends text to a specified file, or echos words" << endl;
    cout << "himom - creates a child process using a fork. Where child and parent communicate." << endl; 
    cout << "quit - exit the shell" << endl; 
}

void repeat(const string &parameters){ // repeats a string or redirects to a file 
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
    char instring[128];
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
        read(pip[0], instring, sizeof(instring) - 1);
        cout << "Parent: hey kid, got your message! " << instring << endl; 
        close(pip[0]);

    }
} 