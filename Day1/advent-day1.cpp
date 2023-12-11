/**
 * Cesar Guerrero
 * 12/7/2023
 * 
 * Advent of Code 2023 - Day 1
 * 
 * NOTE: I want to learn C++ and I figured doing Advent of Code would be a fun way to do this
*/

#include <iostream> //Header Library used for input and output in C++
#include <fstream> //Header Library used for file input and output in C++
#include <cstdio> //Used for printf

using namespace std; //Use names for objects and variables from the standard library

//Entry into our program
int main(){

    int sum = 0;
    string line;

    //The first thing we have to do is open the file
    fstream file("calibration.txt", ios::in);

    //Loop over each line of the file
    while(getline(file, line)){
        char temp[10];
        int index = 0;
        string num;
        //We don't want to interact with the non-numeric characters
        for(int i = 0; i < line.length(); i++){
            char character = line[i];
            if(character >= 48 && character <= 57){
                temp[index] = character;
                index++;
            }
        }
        num += temp[0];
        num += temp[index-1];
        cout << "Number: " << num << "\n";

        sum += stoi(num);
    }
    printf("Sum: %d\n", sum);

    //Always close your files
    file.close();

    return 0;
}