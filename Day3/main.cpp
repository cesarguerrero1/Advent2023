/**
 * Cesar Guerrero
 * 03/26/2024
 * 
 * Day 3: Advent Calendar 2023
 * 
 * NOTE: My idea to solving this puzzle is just thinking about the problem as a giant multi-dimensional array.
 * 
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <utility>

//Function prototypes
int solutionPartOne(const std::vector<std::string>& engineRows, std::map<std::pair<int,int>, std::vector<int>>& starMap);
int solutionPartTwo(std::map<std::pair<int,int>, std::vector<int>>& starMap);
void alterMap(int number, std::map<std::pair<int,int>, std::vector<int>>& starMap, 
                const std::vector<std::string>& engineRows, int left, int right, int row);
bool checkNeighbors(const std::vector<std::string>& engineRows, int left, int right, int row);

/**
 * Entry point into the program
 * @return 0 if the program ran successfully
 */
int main(){

    //Open the file
    std::ifstream file("engine-numbers.txt");
    if(file.fail()){
        std::cout << "Error opening file\n";
        return 1;
    }

    //We want to vectorize the file
    std::vector<std::string> engineRows;
    std::string line;
    while(std::getline(file, line)){
        engineRows.push_back(line);
    }

    //NOTE: You can't use a pair as a key in an unordered_map
    std::map<std::pair<int,int>, std::vector<int>> starMap; 

    //Loop over all the rows and find the asterisks
    for(int i = 0; i < engineRows.size(); i++){
        std::string currentRow = engineRows[i];

        for(int j = 0; j < currentRow.length(); j++){
            char currentChar = currentRow[j];
            //If we find an asterisk put it in the map
            if(currentChar == '*'){
                starMap[std::make_pair(i,j)] = std::vector<int>();
            }
        }
    }

    if(solutionPartOne(engineRows, starMap) != 0){
        std::cerr << "Failed Part 1\n";
        file.close();
        return 2;
    }

    if(solutionPartTwo(starMap) != 0){
        std::cerr << "Failed Part 2\n";
        file.close();
        return 3;
    }

    file.close();
    return 0;
}


/**
 * This runs the solution for part one
 * @param {std::vector<std::string>&} engineRows - The rows of the engine
 * @param {std::map<std::pair<int,int>, std::vector<int>>&} starMap - The map of stars
 * @return {int} - 0 if successful
*/
int solutionPartOne(const std::vector<std::string>& engineRows, std::map<std::pair<int,int>, std::vector<int>>& starMap){

    //Iterate over the rows
    int sum = 0;
    for(int j = 0; j < engineRows.size(); j++){
        auto row = engineRows[j];

        //Iterate over the characters in the row to find the numbers
        for(int i = 0; i < row.length(); i++){
            //If we find a digit we need to fully traverse the number to examine its bounds
            char character = row[i];
            //If the character is a digit we need to find the bounds of the number
            if(character >= 48 && character <= 57){
                int left = i;
                int right = i;
                ++i;
                //Keep going until we traverse the entirety of the number we found
                while(i < row.length()){
                    //If we find a character that is not a digit we need to stop
                    if(row[i] < 48 || row[i] > 57){
                        right = i-1;
                        break;
                    }
                    ++i;
                }
                //Check if we reached the end of the line and the number is at the end
                if(i == row.length()){
                    right = i-1;
                }

                //Use our bounds to extract the number
                int number = std::stoi(row.substr(left, right-left+1));

                //Check if any of the neighbors are asterisks - This is for part wo
                alterMap(number, starMap, engineRows, left, right, j);

                //Check if any of the neighbors are symbols
                if(checkNeighbors(engineRows, left, right, j)){
                    sum += number;
                }
            }
        }
    }

    std::cout << "The sum of all the numbers is: " << sum << "\n";

    return 0;
}


/**
 * This runs the solution for part two
 * @param {std::map<std::pair<int,int>, std::vector<int>>&} starMap - The map of stars
 * @return {int} - 0 if successful
*/
int solutionPartTwo(std::map<std::pair<int,int>, std::vector<int>>& starMap){
    int productSum = 0;
    //Now that we have the map we can find the product of the two numbers associated with the stars
    for(auto& starPair: starMap){
        if(starPair.second.size() == 2){
            //We have a star with two numbers
            productSum += starPair.second[0] * starPair.second[1];
        }
    }

    std::cout << "The sum of all the gear ratios is: " << productSum << "\n";
    return 0;
}

/**
 * This function will attempt to map a number to a star
 * @param {int} number - The number to map
 * @param {std::map<std::pair<int,int>, std::vector<int>>&} starMap - The map of stars
 * @param {std::string&} line - The line of the engine schematic
 * @param {int} left - The left bound of the number
 * @param {int} right - The right bound of the number
 * @param {int} row - The current row of the engine schematic
*/
void alterMap(int number, std::map<std::pair<int,int>, std::vector<int>>& starMap, const std::vector<std::string>& engineRows, int left, int right, int row){
    
    //Check left
    try{
        //Recall .at() will throw an 'out_of_range' exception if the index is invalid
        if(engineRows[row].at(left-1) == '*'){
            starMap[std::make_pair(row, left-1)].push_back(number);
        }
    }catch(std::out_of_range& e){
        //We don't need to do anything here
        //std::cerr << "Out of range exception\n";
    }

    //Check right
    try{
        if(engineRows[row].at(right+1) == '*'){
            starMap[std::make_pair(row, right+1)].push_back(number);
        }
    }catch(std::out_of_range& e){
        //We don't need to do anything here
        //std::cerr << "Out of range exception\n";
    }

    //Check the top and bottom
    for(int i = left-1; i <= right+1; i++){
        //Check top
        try{
            if(engineRows[row-1].at(i) == '*'){
                starMap[std::make_pair(row-1, i)].push_back(number);
            }
        }catch(std::out_of_range& e){
            //std::cerr << "Out of range exception\n";
        }

        //Check bottom
        try{
            if(engineRows[row+1].at(i) == '*'){
                starMap[std::make_pair(row+1, i)].push_back(number);
            }
        }catch(std::out_of_range& e){
            //std::cerr << "Out of range exception\n";
        }
    }

    return;
}

/**
 * This function will attempt to check if there is at least one symbolic neighbor to the number
 * @param {std::vector<std::string>&} engineRows - The rows of the engine schematic
 * @param {int} left - The left bound of the number
 * @param {int} right - The right bound of the number
 * @param {int} row - The current row of the engine schematic
 * @return {bool} - True if there is at least one neighbor that is a symbol
*/
bool checkNeighbors(const std::vector<std::string>& engineRows, int left, int right, int row){
    //We need to check if there is at least one symbol that is a neighbor to our number

    //Check Left
    try{
        //Recall .at() will throw an 'out_of_range' exception if the index is invalid
        char symbol = engineRows[row].at(left-1);
        if(symbol != '.' && (symbol < 48 || symbol > 57)){
            return true;
        }
    }catch(std::out_of_range& e){
        //std::cerr << "Out of range exception\n";
    }

    //Check Right
    try{
        //Recall .at() will throw an 'out_of_range' exception if the index is invalid
        char symbol = engineRows[row].at(right+1);
        if(symbol != '.' && (symbol < 48 || symbol > 57)){
            return true;
        }
    }catch(std::out_of_range& e){
        //std::cerr << "Out of range exception\n";
    }
    
    //Check Top and Bottom
    for(int i = left-1; i <= right+1; i++){
        //Check top
        try{
            char symbol = engineRows[row-1].at(i);
            if(symbol != '.' && (symbol < 48 || symbol > 57)){
                return true;
            }
        }catch(std::out_of_range& e){
            //std::cerr << "Out of range exception\n";
        }

        //Check bottom
        try{
            char symbol = engineRows[row+1].at(i);
            if(symbol != '.' && (symbol < 48 || symbol > 57)){
                return true;
            }
        }catch(std::out_of_range& e){
            //std::cerr << "Out of range exception\n";
        }
    }
    return false;
}