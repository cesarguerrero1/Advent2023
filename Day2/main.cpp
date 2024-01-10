/**
 * Cesar Guerrero
 * 01/09/2024
 * 
 * Day 2: Advent Calendar 2024
 * 
*/

#include <iostream>
#include <string>
//fstream is a library for reading and writing files
#include <fstream>
//The reason we include this is we want to treat a string as a stream.
#include <sstream>
//To make life easier we are going to use a hash-map
#include <map>

//File
std::string puzzleFile = "puzzle-input.txt";

//These are the set of cubes that each game is played with
std::map<std::string, int> cubes = {
    {"red", 12},
    {"green", 13},
    {"blue", 14}
};

//Function prototypes
bool verifySet(std::string);

//Entry point for the program
int main(){

    //Open the file
    std::ifstream file;
    file.open(puzzleFile);

    if(file.fail()){
        std::cout << "Error opening file\n";
        return 1;
    }

    //We want to read the file line by line, and tokenize each line as we go
    std::string fileLineEntry;
    std::istringstream fileLine;
    std::string set;
    int gameIDSum = 0;
    bool validGame;
    int count = 1;
    while(!file.eof()){
    
        std::getline(file, fileLineEntry);
        std::cout << "\nGame: " << count << std::endl;
        //Remove everything up to and including the colon in each line
        int position = fileLineEntry.find(':');
        if(position == std::string::npos){
            std::cout << "Error: File format error\n";
            return 2;
        }
        fileLineEntry = fileLineEntry.substr(position + 1);

        //We need to clear the stream so that we can use it again
        fileLine.clear();
        //Initialize the stream with the new string
        fileLine.str(fileLineEntry);

        //We can now analyze each set of a game
        validGame = true;
        while(std::getline(fileLine, set, ';')){
            if(verifySet(set) == false){
                //There is no point in further analyzing this game as a set is invalid
                validGame = false;
                std::cout << "Game " << count << " is invalid\n";
                break;
            }
        }

        if(validGame == true){
            std::cout << "Game " << count << " is valid\n";
            gameIDSum += count;
        }
        count ++;
    }

    //Always close your files
    file.close();

    std::cout << "\nSum of Game IDs: " << gameIDSum << "\n";

    return 0;
}


/**
 * This function will verify that the set is valid
 * 
*/
bool verifySet(std::string set){
    if(set.empty()){
        std::cout << "Error: Empty set\n";
        return false;
    }

    std::istringstream setLine;
    setLine.clear();
    setLine.str(set);
    std::string cubesShown; 
    while(std::getline(setLine, cubesShown, ',')){
        //We now have cube information
        int cubeColorPos = cubesShown.find_first_of("rgb");

        //Extract our information
        std::string cubeColor = cubesShown.substr(cubeColorPos, cubesShown.length()-1);
        int numberCubes = std::stoi(cubesShown.substr(0, cubeColorPos-1));

        //Check against the map
        if(numberCubes > cubes[cubeColor]){
            std::cout << "Error: The set indicated " << numberCubes << " " << cubeColor << " cubes, but only " << cubes[cubeColor] << " are available\n";
            return false;
        }
    }
    return true;
}
