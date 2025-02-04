#include <iostream>
#include <string> 

using namespace std; 

int main(int argc, char **argv){

    string cmdline; 

    cout << "k$ "; 
    getline(cin, cmdline); 

    cout << "k$ " << cmdline << endl; 

    return 0;
}