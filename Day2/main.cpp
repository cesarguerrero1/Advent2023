/**
 * Cesar Guerrero
 * 01/09/2024
 * 
 * Day 2: Advent Calendar 2024
 * 
 * NOTE: To solve Part 2 rather than refactor the program or start all over, I just reused the same code from verifySet and added a few lines to analyzeSet
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

std::map<std::string, int> minimumCubes = {
    {"red", 0},
    {"green", 0},
    {"blue", 0}
};

//Function prototypes
bool verifySet(std::string);
void analyzeSet(std::string);

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
    int gamePowerSum = 0;
    while(!file.eof()){
        std::getline(file, fileLineEntry);
        std::cout << "\nGame: " << count << std::endl;
        //Reset the map
        minimumCubes["red"] = 0;
        minimumCubes["green"] = 0;
        minimumCubes["blue"] = 0;

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

            //We are adding new functionality to see how to make the game valid
            analyzeSet(set);

            //Only analyze the set if the game is still in a valid state
            if(validGame == true && verifySet(set) == false){
                //There is no point in further analyzing this game as a set is invalid
                validGame = false;
                std::cout << "Game " << count << " is invalid\n";
            }
        }

        if(validGame == true){
            std::cout << "Game " << count << " is valid\n";
            gameIDSum += count;
        }
        count ++;
        
        //Print out the minimum number of cubes needed to make the game valid
        gamePowerSum += minimumCubes["red"] * minimumCubes["green"] * minimumCubes["blue"];
    }

    //Always close your files
    file.close();

    std::cout << "\nSum of Game IDs: " << gameIDSum << "\n";
    std::cout << "Game Power Sum: " << gamePowerSum << "\n";

    return 0;
}


/**
 * This function will analyze the set to determine the minimum number of cubes needed to make the set valid
 * @param {std::string} set - The set to be analyzed
*/
void analyzeSet(std::string set){
    if(set.empty()){
        std::cout << "Error: Empty set\n";
        return;
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

        //Each game will have a map and we need to update it with each set
        if(numberCubes > minimumCubes[cubeColor]){
            minimumCubes[cubeColor] = numberCubes;
        }
    }
    return;
}



/**
 * This function will verify that the set is valid
 * @param {std::string} set - The set to be analyzed
 * @return {bool} - True if the set is valid, false otherwise
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
