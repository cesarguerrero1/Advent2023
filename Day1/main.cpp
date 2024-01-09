/**
 * Cesar Guerrero
 * 01/09/2024
 * 
 * Day 1 - Trebuchet
 * 
 * 
 * NOTE: Another solution would just have been iterating over the string and checking if each 
 * character was an alpha-numeric number.
 * 
 * I suspect that the built-in functions are more efficient than a for loop, considering they are
 * likely using pointers to iterate over the string.
*/

#include <iostream>
#include <string>
#include <fstream>
std::string calfile = "calibration.txt";

//Function prototypes
int get_raw_num(std::string, int);
std::vector<int> get_word_num(std::string, int);

int main(){

    std::ifstream file;
    file.open(calfile);
    //Best practice to always look for errors
    if(file.fail()){
        std::cout << "Error opening file\n";
        return 1;
    }

    //We need a way to store data from each line
    std::string line;
    std::string num;

    //We need a way to keep track of the first and and last numbers so we can short circuit the search
    int firstPos;
    int lastPos;

    //We need a vector to hold our generated numbers
    int sum = 0;

    while(!file.eof()){
        std::getline(file, line);
        int lineSize = line.size();
        std::vector<int> wordNumResponse; //This is used to store the response from the get_word_num function
        int position;

        //First check for first occurrence of raw integers
        position = get_raw_num(line, 0);
        if(position != -1){
            if(position == 0){
                //Since we found a raw integer at the beginning of the line we can short-circuit the search
                num += line[position];
            }else{
                //Store the position of the first raw number occurrence so we can now check if it needs to be replaced by a word number
                firstPos = position;
                wordNumResponse = get_word_num(line, 0);
                if(wordNumResponse[0] != -1 && wordNumResponse[0] < firstPos){
                    num += std::to_string(wordNumResponse[1]);
                }else{
                    num += line[firstPos];
                }
            }

            //We now shift to the last occurrence
            position = get_raw_num(line, 1);
            if(position == lineSize-1){
                //We found a raw integer at the very end of the line therefore we can short-circuit the search
                num += line[position];
                sum += std::stoi(num);
                num.clear();
                continue;
            }else{
                //We need to check for the last occurrence of a word num as it could be after the last raw integer
                lastPos = position;
                wordNumResponse = get_word_num(line, 1);
                if(wordNumResponse[0] != -1 && wordNumResponse[0] > lastPos){
                    num += std::to_string(wordNumResponse[1]);
                }else{
                    num += line[lastPos];
                }
                sum += std::stoi(num);
                num.clear();
                continue;
            }
        }else{
            //There are no raw integers so we only need to check for word numbers
            wordNumResponse = get_word_num(line, 0);
            if(wordNumResponse[0] != -1){
                //There are word numbers so just add the first and last one
                num += std::to_string(wordNumResponse[1]);
                num += std::to_string(get_word_num(line, 1)[1]);
                sum += std::stoi(num);
                num.clear();
            }
        }

    }

    //We now just need to sum up the numbers
    std::cout << "Sum: " << sum << std::endl;
    return 0;
}


/**
 * This function will look for raw integers in a string and return either the first or last one
 * depending on the mode.
 * @param {std::string} line - The string to search
 * @param {int} mode - 0 for first, 1 for last
 * @return {int} - The first or last raw integer found.. Else -1 if none are found
*/
int get_raw_num(std::string line, int mode){

    //It turns out that the string library provides two good functions:
    //.find_first_of() and .find_last_of()
    std::string nums = "0123456789";
    int position;
    if(mode == 0){
        position = line.find_first_of(nums);
    }else{
        position = line.find_last_of(nums);
    }

    //Now we just need to check if we found a match
    if(position != std::string::npos){
        return position;
    }else{
        return -1;
    }
}

/**
 * This function will look for word numbers in a string and return either the first or last one
 * depending on the mode.
 * @param {std::string} line - The string to search
 * @param {int} mode - 0 for first, 1 for last
 * @return {int} - The first or last word number found.. Else -1 if none are found
*/
std::vector<int> get_word_num(std::string line, int mode){

    //To solve Part 2 we need to be able to analyze the words
    std::vector<std::string> word_nums = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

    int optimalPosition = -1;
    int currentPosition;
    int changeIndex = -1; //This will be used to keep track of which word number was used to find the optimal position

    for(int i = 0; i < word_nums.size(); i++){
        if(mode == 0){
            //First occurrence
            currentPosition = line.find(word_nums[i]);
        }else{
            //Last occurrence
            currentPosition = line.rfind(word_nums[i]);
        }
        //Check that we found something and see if it now optimal
        if(currentPosition == std::string::npos){
            continue;
        }

        //If we are on our first iteration then we need to set optimal to the first position
        if(optimalPosition == -1){
            optimalPosition = currentPosition;
            changeIndex = i;
            continue;
        }

        //Based on the mode we need to check if the current position is optimal
        if(mode == 0){
            if(currentPosition < optimalPosition){
                optimalPosition = currentPosition;
                changeIndex = i;
            }
        }else{
            if(currentPosition > optimalPosition){
                optimalPosition = currentPosition;
                changeIndex = i;
            }
        }
    }

    //Since we are working with numbers we need to convert the word number to an integer
    switch(changeIndex){
        case 0:
            return {optimalPosition, 0};
        case 1:
            return {optimalPosition, 1};
        case 2:
            return {optimalPosition, 2};
        case 3:
            return {optimalPosition, 3};
        case 4:
            return {optimalPosition, 4};
        case 5:
            return {optimalPosition, 5};
        case 6:
            return {optimalPosition, 6};
        case 7:
            return {optimalPosition, 7};
        case 8:
            return {optimalPosition, 8};            
        case 9:
            return {optimalPosition, 9};
        default:
            return {-1, -1};
    }

}