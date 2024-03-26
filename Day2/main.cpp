/**
 * Cesar Guerrero
 * 03/26/2024
 * 
 * Day 2 - Cube Conundrum
 *
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>


//Function Prototypes
int solutionPartOne(std::ifstream& file);
int solutionPartTwo(std::ifstream& file);
bool verifySet(const std::string& set, std::unordered_map<std::string, int>& cubes);
void analyzeSet(const std::string& set, std::unordered_map<std::string, int>& minimumCubes);


/**
 * Entry point for the program
 * @return {int} - 0 if successful
*/
int main(){

    //Intake our file
    std::ifstream file("puzzle-input.txt");
    if(file.fail()){
        std::cerr << "Error opening file\n";
        return 1;
    }

    if(solutionPartOne(file) != 0){
        std::cerr << "Failed Part 1\n";
        file.close();
        return 2;
    }

    //Reset the file - We need to move the file pointer back to the beginning
    file.clear();
    file.seekg(0, std::ios::beg);
    if(solutionPartTwo(file) != 0){
        std::cerr << "Failed Part 2\n";
        file.close();
        return 3;
    }

    file.close();
    return 0;
}


/**
 * This runs the solution for part one
 * @param {std::ifstream&} file - The file to be read
 * @return {int} - 0 if successful
*/
int solutionPartOne(std::ifstream& file){

    //These are the set of cubes that each game is played with
    std::unordered_map<std::string, int> cubes = {
        {"red", 12},
        {"green", 13},
        {"blue", 14}
    };

    std::string line;
    std::stringstream ss;
    std::string set;
    bool validGame;
    int gameIDSum = 0;
    int gameID = 1;
    while(std::getline(file, line)){
        //We only want after the first colon
        int position = line.find(':');
        if(position == std::string::npos){
            std::cerr << "Error: File format error\n";
            return 1;
        }
        line = line.substr(position + 1);

        //We now need to start tokenizing on the semicolons
        ss.clear();
        ss.str(line);
        validGame = true;
        while(std::getline(ss, set, ';')){
            if(!verifySet(set, cubes)){
                validGame = false;
                break;
            }
        }

        if(validGame){
            gameIDSum += gameID;
        }
        gameID++;
    }
    
    std::cout << "Sum of Game IDs: " << gameIDSum << "\n";

    return 0;
}


/**
 * This runs the solution for part two
 * @param {std::ifstream&} file - The file to be read
 * @return {int} - 0 if successful
*/
int solutionPartTwo(std::ifstream& file){

    std::unordered_map<std::string, int> minimumCubes = {
        {"red", 0},
        {"green", 0},
        {"blue", 0}
    };

    std::string line;
    std::stringstream ss;
    std::string set;
    int gamePowerSum = 0;
    while(std::getline(file, line)){
        int position = line.find(':');
        if(position == std::string::npos){
            std::cerr << "Error: File format error\n";
            return 1;
        }
        line = line.substr(position + 1);

        //We now need to start tokenizing on the semicolons
        ss.clear();
        ss.str(line);
        while(std::getline(ss, set, ';')){
            //Analyze Set will update the minimumCubes map
            analyzeSet(set, minimumCubes);
        }

        //Record our answer and then reset the map
        gamePowerSum += (minimumCubes["red"] * minimumCubes["green"] * minimumCubes["blue"]);
        minimumCubes["red"] = 0;
        minimumCubes["green"] = 0;
        minimumCubes["blue"] = 0;
        
    }

    std::cout << "Game Power Sum: " << gamePowerSum << "\n";

    return 0;
}


/**
 * This function will verify that the set is valid
 * @param {std::string} set - The set to be analyzed
 * @param {std::unordered_map<std::string, int>} cubes - The set of cubes that each set is played with
 * @return {bool} - True if the set is valid, false otherwise
*/
bool verifySet(const std::string& set, std::unordered_map<std::string, int>& cubes){
    if(set.empty()){
        std::cerr << "Error: Empty set\n";
        return false;
    }

    //We need to further tokenize on the commas
    std::stringstream ss(set);
    std::string cubesShown;
    while(std::getline(ss, cubesShown, ',')){
        //We know that the format is "N color"
        int cubeColorPos = cubesShown.find_first_of("rgb");
        if(cubeColorPos == std::string::npos){
            std::cerr << "Error: Invalid cube color\n";
            return false;
        }

        //Recall that substr takes in the starting position and an optional length
        int cubeCount = std::stoi(cubesShown.substr(0, cubeColorPos-1));
        std::string cubeColor = cubesShown.substr(cubeColorPos);

        //Now we check against the map
        if(cubeCount > cubes[cubeColor]){
            return false;
        }
    }
    return true;
};


/**
 * This function will analyze the set to determine the minimum number of cubes needed to make the set valid
 * @param {std::string} set - The set to be analyzed
 * @param {std::unordered_map<std::string, int>} minimumCubes - The minimum number of cubes needed to make the set valid
*/
void analyzeSet(const std::string& set, std::unordered_map<std::string, int>& minimumCubes){
    if(set.empty()){
        std::cerr << "Error: Empty set\n";
        return;
    }

    //We need to further tokenize on the commas
    std::stringstream ss(set);
    std::string cubesShown;
    while(std::getline(ss, cubesShown, ',')){
        //We know that the format is "N color"
        int cubeColorPos = cubesShown.find_first_of("rgb");
        if(cubeColorPos == std::string::npos){
            std::cerr << "Error: Invalid cube color\n";
            continue;
        }

        //Recall that substr takes in the starting position and an optional length
        int cubeCount = std::stoi(cubesShown.substr(0, cubeColorPos-1));
        std::string cubeColor = cubesShown.substr(cubeColorPos);

        minimumCubes[cubeColor] = std::max(cubeCount, minimumCubes[cubeColor]);
    }

    return;
}