/**
 * Cesar Guerrero
 * 03/26/2024
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
#include <fstream>
#include <string>

//Function Prototypes
int solutionPartOne(std::ifstream& file);
int solutionPartTwo(std::ifstream& file);
std::string findFirstWordNumber(const std::string& line, int currentNumIndex);
std::string findLastWordNumber(const std::string& line, int currentNumIndex);

/**
 * Main function
 * @return {int} - 0 if successful
*/
int main(){

    //The calibration has a line consisting of numbers and words
    std::ifstream file("calibration.txt");
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

    //Always close your files
    file.close();
    return 0;
}


/**
 * This runs the solution for Part 1 of Day 1
 * @param {std::ifstream&} file - The file stream to read from
 * @return {int} - 0 if successful
*/
int solutionPartOne(std::ifstream& file){
    int sum = 0;
    //We need to iterate over each line and parse it
    std::string line;
    std::string number;

    while(std::getline(file, line)){
        //We can use .find() to get the first number
        int firstNumIndex = line.find_first_of("0123456789");
        if(firstNumIndex == std::string::npos){
            //This means no numbers exist so do nothing
            continue;
        }
        //We don't have to worry about this every being invalid
        int lastNumIndex = line.find_last_of("0123456789");
        number += line[firstNumIndex];
        number += line[lastNumIndex];
        sum += std::stoi(number);
        //We are reusing this variable so we need to clear it
        number.clear();
    }

    std::cout << "Sum: " << sum << "\n";
    return 0;
};


/**
 * This runs the solution for Part 2 of Day 1
 * @param {std::ifstream&} file - The file stream to read from
 * @return {int} - 0 if successful
*/
int solutionPartTwo(std::ifstream& file){
    int sum = 0;
    //Now that we have words we need to convert them to numbers
    std::string line;
    std::string number;
    while(std::getline(file, line)){
        //We are using a boolean to check if we have a word number or an integer
        int firstNumIndex = line.find_first_of("123456789");
        if(firstNumIndex == std::string::npos){
            //No integers exist
            std::string temp = findFirstWordNumber(line, -1);
            if(temp == "-1"){
                continue;
            }else{
                number += temp; 
            }
        }else{
            if(firstNumIndex == 0){
                number += line[firstNumIndex];
            }else{
                //The word number may be before the integer
                std::string temp = findFirstWordNumber(line, firstNumIndex);
                if(temp != "-1"){
                    number += temp;
                }else{
                    number += line[firstNumIndex];
                }
            }
        }

        //Now we need to find the last number
        int lastNumIndex = line.find_last_of("123456789");
        if(lastNumIndex == std::string::npos){
            //No integers exist
            std::string temp = findLastWordNumber(line, line.size());
            //This will never be -1 because of our earlier logic check
            number += temp;
        }else{
            if(lastNumIndex == line.size() - 1){
                number += line[lastNumIndex];
            }else{
                //The word number may be after the integer
                std::string temp = findLastWordNumber(line, lastNumIndex);
                if(temp != "-1"){
                    number += temp;
                }else{
                    number += line[lastNumIndex];
                }
            }
        }
        sum += std::stoi(number);
        number.clear();
    }

    std::cout << "Sum: " << sum << "\n";
    
    return 0;
};


/**
 * Given a line, finds the first word number if it exists and is lower than the currentNumIndex
 * @param {const std::string&} line - The line to search
 * @param {int} currentNumIndex - The current index of the number
 * @return {std::string} - The number found if it exists and is lower than the currentNumIndex
*/
std::string findFirstWordNumber(const std::string& line, int currentNumIndex){
    std::vector<std::string> wordNums = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    int wordIndex = -1;
    for(int i = 0; i < wordNums.size(); i++){
        int temp = line.find(wordNums[i]);
        //If we find a valid word number and it is lower than the currentNumIndex then grab it
        if(temp != std::string::npos){
            if(currentNumIndex == -1){
                //This means we haven't found a number yet so we just grab the first one
                currentNumIndex = temp;
                wordIndex = i;
            }else{
                if(temp < currentNumIndex){
                    currentNumIndex = temp;
                    wordIndex = i;
                }
            }
        }
    }

    switch(wordIndex){
        case 0:
            return "1";
        case 1:
            return "2";
        case 2:
            return "3";
        case 3:
            return "4";
        case 4:
            return "5";
        case 5:
            return "6";
        case 6:
            return "7";
        case 7:
            return "8";
        case 8:
            return "9";
        default:
            return "-1";
    }
}


/**
 * Given a line, finds the last word number if it exists and is higher than the currentNumIndex
 * @param {const std::string&} line - The line to search
 * @param {int} currentNumIndex - The current index of the number
 * @return {std::string} - The number found if it exists and is higher than the currentNumIndex
*/
std::string findLastWordNumber(const std::string& line, int currentNumIndex){
    std::vector<std::string> wordNums = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    int wordIndex = -1;
    for(int i = 0; i < wordNums.size(); i++){
        int temp = line.rfind(wordNums[i]);
        if(temp != std::string::npos){
            if(currentNumIndex == line.size()){
                //This means we haven't found a number yet so we just grab the first one
                currentNumIndex = temp;
                wordIndex = i;
            }else{
                if(temp > currentNumIndex){
                    currentNumIndex = temp;
                    wordIndex = i;
                }
            }
        }
    }

    switch(wordIndex){
        case 0:
            return "1";
        case 1:
            return "2";
        case 2:
            return "3";
        case 3:
            return "4";
        case 4:
            return "5";
        case 5:
            return "6";
        case 6:
            return "7";
        case 7:
            return "8";
        case 8:
            return "9";
        default:
            return "-1";
    }
}







