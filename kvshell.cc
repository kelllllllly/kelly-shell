#include <iostream>
#include <string> 
#include <sstream> 

using namespace std; 

int main(){

kLoop(); 
    return 0;
}

void kLoop(){
     
    string prompt = "k$ ";
    string cmdLine, inputRead; 
    int status; 

    while(true){
        cout << prompt;
        getline(cin, cmdLine);
    }

    cout << "k$ " << cmdLine << endl; 


}

