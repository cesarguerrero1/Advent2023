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
int solutionPartOne(const std::vector<std::string>& engineRows, const std::map<std::pair<int,int>, std::vector<int>>& starMap);
int solutionPartTwo();

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

    //NOTE: You can't use a pair as a key in a map
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

    if(solutionPartTwo() != 0){
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
int solutionPartOne(const std::vector<std::string>& engineRows, const std::map<std::pair<int,int>, std::vector<int>>& starMap){

    //Iterate over the rows
    for(auto& row : engineRows){

        //Iterate over the characters in the row to find the numbers
        for(int i = 0; i < row.length(); i++){
            //If we find a digit we need to fully traverse the number to examine its bounds
            char character = row[i];
            //If the character is a digit we need to find the bounds of the number
            if(character >= 48 && character <= 57){
                int left = i;
                int right = i;
                ++i;
                //Keep going until we traverse the entirety
                while(i < row.length()){
                    if(row[i] < 48 || row[i] > 57){
                        right = i-1;
                        break;
                    }
                    ++i;
                }
                //Check if we reached the end of the line
                if(i == row.length()){
                    right = i-1;
                }

                //Now that we have the bounds of the number we need to check the neighbors
                int number = std::stoi(row.substr(left, right-left+1));

                

                //Check the neighbors
                /*
                    //Check if any of the neighbors are asterisks
                    alterMap(number, line, left, right, currentRow-1, currentRow+1);
                
                    if(checkNeighbors(line, left, right, currentRow-1, currentRow+1)){
                        //.substr(startIndex, lengthToTraverse);
                        sum += number;
                    }
                    continue;
                */
            }
        }
    }


    return 0;
}


/**
 * This runs the solution for part two
 * @return {int} - 0 if successful
*/
int solutionPartTwo(){
    return 0;
}




/*
//Function protopypes
void analyzeLine(const std::string&);
bool checkNeighbors(const std::string&, int, int, int, int);
bool checkSpecialCharacter(char);
void alterMap(int, const std::string&, int, int, int, int);

//Global variables
static int sum = 0;
static int productSum = 0;
static int currentRow = 0; 

//Entry point for the program
int main(){

    //Now that we have stored all of our rows, we can start analyzing them
    for(int i = 0 ; i < engineRows.size(); i++){
        analyzeLine(engineRows[i]);
        currentRow++;
    }
    
    std::cout << "The sum of all valid numbers is: " << sum << "\n";




    //Loop over our staMap and find the stars with only two numbers associated with them
    for(auto& star : starMap){
        if(star.second.size() == 2){
            productSum += star.second[0] * star.second[1];
        }
    }

    std::cout << "The sum of all the gear ratios is: " << productSum << "\n";

    //Always close your files!
    file.close();
    return sum;
}

//This function will check the neighbors of the number
bool checkNeighbors(const std::string& line, int leftBound, int rightBound, int topBound, int bottomBound){
    //Check the left side
    if(leftBound != 0){
        char character = line[leftBound-1];
        if(checkSpecialCharacter(character)){
            return true;
        }
    }

    //Check the right side
    if(rightBound != line.length()-1){
        char character = line[rightBound+1];
        if(checkSpecialCharacter(character)){
            return true;
        }
    }


    //Check the top row (rememebr the diagonals)
    if(topBound >= 0){
        for(int i = leftBound-1; i <= rightBound+1; i++){
            try{
                char character = engineRows[topBound].at(i);
                if(checkSpecialCharacter(character)){
                    return true;
                }
            }catch(std::out_of_range& e){
                //std::cout << "Out of range exception for top-row traversal \n";
            }
        }
    }

    //Check the bottom row (rememebr the diagonals)
    if(bottomBound <= engineRows.size()-1){
        for(int i = leftBound-1; i <= rightBound+1; i++){
            try{
                //Notice that we are using the .at() method so we can catch an exception
                char character = engineRows[bottomBound].at(i);
                if(checkSpecialCharacter(character)){
                    return true;
                }
            }catch(std::out_of_range& e){
                //std::cout << "Out of range exception for bottom-row traversal\n";
            }
        }
    }

    return false;
}

bool checkSpecialCharacter(char character){
    if(character != '.' && (character < 48 || character > 57)){
        //If the character is a symbol then this number is valid
        return true;
    }else{
        return false;
    }
}

void alterMap(int number, const std::string& line, int leftBound, int rightBound, int topBound, int bottomBound){

    //Check the left side to see if we find a star
    if(leftBound != 0){
        char character = line[leftBound-1];
        if(character == '*'){
            //We need to add this number to the vector of neighbors
            auto coordinates = std::make_pair(bottomBound-1, leftBound-1);
            starMap[coordinates].push_back(number);
        }
    }

    //Check the right side
    if(rightBound != line.length()-1){
        char character = line[rightBound+1];
        if(character == '*'){
            //We need to add this number to the vector of neighbors
            auto coordinates = std::make_pair(bottomBound-1, rightBound+1);
            starMap[coordinates].push_back(number);
        }
    }


    //Check the top row (rememebr the diagonals)
    if(topBound >= 0){
        for(int i = leftBound-1; i <= rightBound+1; i++){
            try{
                char character = engineRows[topBound].at(i);
                if(character == '*'){
                    //We need to add this number to the vector of neighbors
                    auto coordinates = std::make_pair(topBound, i);
                    starMap[coordinates].push_back(number);
                }
            }catch(std::out_of_range& e){
                //std::cout << "Out of range exception for top-row traversal \n";
            }
        }
    }

    //Check the bottom row (rememebr the diagonals)
    if(bottomBound <= engineRows.size()-1){
        for(int i = leftBound-1; i <= rightBound+1; i++){
            try{
                //Notice that we are using the .at() method so we can catch an exception
                char character = engineRows[bottomBound].at(i);
                if(character == '*'){
                    //We need to add this number to the vector of neighbors
                    auto coordinates = std::make_pair(bottomBound, i);
                    starMap[coordinates].push_back(number);
                }
            }catch(std::out_of_range& e){
                //std::cout << "Out of range exception for bottom-row traversal\n";
            }
        }
    }

    return;
}
*/