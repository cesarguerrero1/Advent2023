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
    int firstPos = -1;
    int lastPos = -1;

    //We need a vector to hold our generated numbers
    int sum = 0;
    int count = 0;

    while(!file.eof()){
        std::getline(file, line);
        std::cout << "Line: " << line << std::endl;
        int lineSize = line.size();
        std::vector<int> positionAndValue;
        int position;

        //First check for first occurrence of raw integers
        position = get_raw_num(line, 0);
        if(position != -1){
            if(position == 0){
                num += line[position];
            }else{
                //We can't short-circuit the search so we need to check for the first occurrence of a word num
                firstPos = position;
                positionAndValue = get_word_num(line, 0);
                if(positionAndValue[1] != -1 && positionAndValue[1] < firstPos){
                    num += std::to_string(positionAndValue[0]);
                }else{
                    num += line[firstPos];
                }
            }

            //We now shift to the last occurrence
            position = get_raw_num(line, 1);
            if(position == lineSize-1){
                num += line[position];
                sum += std::stoi(num);
                num.clear();
                continue;
            }else{
                //We need to check for the last occurrence of a word num as it could be after the last raw integer
                lastPos = position;
                positionAndValue = get_word_num(line, 1);
                if(positionAndValue[1] != -1 && positionAndValue[1] > lastPos){
                    num += std::to_string(positionAndValue[0]);
                }else{
                    num += line[lastPos];
                }
                sum += std::stoi(num);
                num.clear();
                continue;
            }
        }else{
            //There are no raw integers so we only need to check for word numbers
            positionAndValue = get_word_num(line, 0);
            if(positionAndValue[1] != -1){
                //There are word numbers so just add the first and last one
                num += std::to_string(positionAndValue[0]);
                num += std::to_string(get_word_num(line, 1)[0]);
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
    if(position != line.npos){
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
    int optimal = -1;
    int position;
    int optimalChangeIndex; //This will be used to keep track of which word number was used to find the optimal position
    for(int i = 0; i < word_nums.size(); i++){
        if(mode == 0){
            position = line.find(word_nums[i]);
            std::cout << "First Position: " << position << std::endl;
        }else{
            position = line.rfind(word_nums[i]);
            std::cout << "Last Position: " << position << std::endl;
        }
        //Check that we found something and see if it now optimal
        if(position == std::string::npos){
            std::cout << "Not found\n";
            continue;
        }

        //If we are on our first iteration then we need to set optimal to the first position
        if(optimal == -1){
            optimal = position;
            optimalChangeIndex = i;
            continue;
        }

        //Based on the mode we need to check if the current position is optimal
        if(mode == 0){
            if(position < optimal){
                //We found a better position
                optimal = position;
                optimalChangeIndex = i;
            }
        }else{
            if(position > optimal){
                optimal = position;
                optimalChangeIndex = i;
            }
        }
    }

    if(optimal == -1){
        return {-1, -1};
    }
    std::cout << "Optimal: " << optimal << std::endl;
    switch(optimalChangeIndex){
        case 0:
            return {0, optimal};
        case 1:
            return {1, optimal};
        case 2:
            return {2, optimal};
        case 3:
            return {3, optimal};
        case 4:
            return {4, optimal};
        case 5:
            return {5, optimal};
        case 6:
            return {6, optimal};
        case 7:
            return {7, optimal};
        case 8:
            return {8, optimal};
        case 9:
            return {9, optimal};
        default:
            return {-1, -1};
    }

}