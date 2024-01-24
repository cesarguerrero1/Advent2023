/**
 * Cesar Guerrero
 * 01/23/2024
 * 
 * Day 3: Advent Calendar 2023
 * 
 * 
 * NOTE: My idea to solving this puzzle is just thinking about the problem as a giant multi-dimensional array. For instance take the following example:
 * 
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

//File
std::string puzzleFile = "engine-numbers.txt";

//2D array of the engine schematic
std::vector<std::string> engineRows;

//Function protopypes
void analyzeLine(const std::string&);
bool checkNeighbors(const std::string&, int, int, int, int);
bool checkSpecialCharacter(char);

//Global variables
static int sum = 0;
static int currentRow = 0; 

//Entry point for the program
int main(){

    //Open the file
    std::ifstream file;
    file.open(puzzleFile);

    if(file.fail()){
        std::cout << "Error opening file\n";
        return 1;
    }

    std::string line;
    while(!file.eof()){
        //Append our file lines to the vector
        std::getline(file, line);
        engineRows.push_back(line);
    }

    //Now that we have stored all of our rows, we can start analyzing them
    for(int i = 0 ; i < engineRows.size(); i++){
        std::cout << "Analyzing row " << currentRow << "\n";
        analyzeLine(engineRows[i]);
        std::cout << "Current Sum: " << sum << "\n";
        currentRow++;
    }
    
    std::cout << "The sum of all valid numbers is: " << sum << "\n";
    return sum;
}


//Best practice dictates that anytime we pass by reference we make it a const to prevent side effects
void analyzeLine(const std::string& line){

    int lineSize = line.length();
    int index = 0;

    while(index < lineSize){
        char currentChar = line[index];
        //If we run into a number we need to define the bounds so we know what to check
        int left = index;
        int right = index;
        if(currentChar >= 48 && currentChar <= 57){
            index++;
            while(true){
                currentChar = line[index];
                if(currentChar < 48 || currentChar > 57){
                    //We have found the end of the number
                    right = index-1;
                    break;
                }
                index++;
                //We need to check if we have reached the end of the line
                if(index == lineSize){
                    right = index-1;
                    break;
                }
            }

            //We have found the bounds of the number so we need to check the neighbors
            int number = std::stoi(line.substr(left, right-left+1));
            if(checkNeighbors(line, left, right, currentRow-1, currentRow+1)){
                //.substr(startIndex, lengthToTraverse);
                std::cout << "VALID: " << number << "\n";
                sum += number;
            }else{
                std::cout << "INVALID: " << number << "\n";
            }
            continue;
        }
        index++;
    }

    return;
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

/**
 * 328,568
 * 785,664
*/