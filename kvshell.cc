// KELLY VEINTIMILLA
// CSCI 411 LAB 2 SIMPLE SHELl
// This program erplicates a simple linux CLI with a variety of commands. simply run ./compile.sh and type help to get started.
// current bug on hopper: have to type quit twice to exit program not sure why. runs fine on my own VM through virtualbox.

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

    string prompt = "k$ "; // My prompt
    string cmdLine, cmd, parameters;
    
    ofstream historyOut(historyFile, ios::app); // Open history file in append mode
    if (!historyOut.is_open()) {
        cerr << "cannot open history file." << endl;
        return 1;
    }
    // loops the shell & handles user input & printing the command history once the loop is finished. 
    while (true) {
        cout << prompt;
        // read the entire input line from the user
        if (!getline(cin, cmdLine)) {
            break;  // Exit on EOF or input error
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
        else if (cmd == "allprocesses") { // if cmd = allprocesses, then "ps"
            system("ps"); // executed using system
        }
        else if (cmd == "clr") { // clears screen
            system("clear"); // executed using system 
        }
        else if (cmd == "environ") { // lists enviorment variables if "environ" is entered
            system("env"); // executed using system 
        }
        else if (cmd == "chgd") {
            if (chdir(parameters.c_str()) != 0) { // changes the current directory to what was specified in parameters, will return 0 on sucess and -1 on fail 
                cerr << "directory not found" << endl;
            }
        }
        else if (cmd == "dir") { // if dir is entered, directory conents are displayed
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

    }

    // displays history when exiting loop
    historyOut.close(); // close file
    ifstream historyIn(historyFile); // opens file for reading
    if (historyIn.is_open()) { // checks if file is open 
        cout << "command history" << endl; 
        string line;
        while (getline(historyIn, line)) { // reads each line 
            cout << line << endl; // prints each line
        }
        historyIn.close(); // close file
    }

    return 0;
}

void signalHandler(int signum) {// handles when a user wants to exit the shell using crtl + c, also prints out command history in the event 
    ifstream history(historyFile);
    cout << "command history" << endl;
    string line;

    while (getline(history, line)) {
        cout << line << endl;
    }

    history.close();
    exit(signum); // exit the program 
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
    stringstream ss(parameters); // used to parse parameters 
    string word, fileName, stop; // word for the text that will be input, filename for the file its being redirected to, stop for implementation of ">"
    bool redirect = false; 

    while (ss >> stop){
        if(stop == ">"){ // detects the redirection symbol 
            redirect = true; 
            ss >> fileName; // reads file name 
            break;
        }
            if(!word.empty()){
                word += " "; // adds a space between words if empty 
    }
    word += stop; 
    }

    if(redirect){ // once ">" is read it will output to a file 
        if(fileName.empty()){
            cerr << "file not specified." << endl;
            return;
        }

        ofstream outFile(fileName); // open file 

        if(!outFile){ // error checking 
            cerr << "can not open file: " << fileName << endl; 
            return; 
        }
        outFile << word << endl; // write text to file 
        outFile.close(); 
        cout << "text was sent to " << fileName << endl;
    } else {
        cout << word << endl; // "echoes" if text was not redirected 
    }
    }

void hiMom(){ // for IPC using pipes w parent and child 

    int pip[2]; 
    char instring[128]; // buffer for storing message read from pipe 
    const char *msg = "Hi mom!"; // message child wants to send to parent

    if (pipe(pip) == -1){ // attempt to create pipe, -1 is returned if failed 
        cout << "pipe failed" << endl; 
        return; 
    }
    int pid = fork(); // creates child process 

    if (pid < 0){ // check if fork failed 
        cout << "fork failed" << endl;
        return; 
    }

    // child process
    if(pid == 0){ 
        close(pip[0]); // close read
        cout << "Child: sends message to parent" << endl; 
        write(pip[1], msg, strlen(msg) + 1); // here write message to pipe 
        close(pip[1]);    // close write 
    }
    else { // parent process
        close(pip[1]); // close write 
        read(pip[0], instring, sizeof(instring) - 1); // reads message from pipe into instring 
        cout << "Parent: hey kid, got your message! " << instring << endl;  // if this prints parent got the message! yay 
        close(pip[0]); // close read 

    }
} 
