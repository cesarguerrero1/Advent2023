/**
 * Cesar Guerrero
 * 01/09/2024
 * 
 * Day 1 - Trebuchet
*/

#include <iostream>
#include <string>
#include <fstream>
std::string calfile = "calibration.txt";

int main(){

    //It turns out that the string library provides two good functions:
    //.find_first_of() and .find_last_of()
    std::string nums = "0123456789";

    std::ifstream file;
    file.open(calfile);
    //Best practice to always look for file.open() errors
    if(file.fail()){
        std::cout << "Error opening file\n";
        return 1;
    }

    //We need a way to store data from each line
    std::string line;
    std::string num;

    //We need a vector to hold our generated numbers
    int sum = 0;
    int count = 0;
    while(!file.eof()){
        std::getline(file, line);
        int pos = line.find_first_of(nums);
        //If there are no numbers in the line, we can skip it
        if(pos != line.npos){
            num += line[pos];
            //No need to error check because we know there is at least one number
            num += line[line.find_last_of(nums)];
            //As a last step we want to keep track of the sum
            sum += std::stoi(num);
        }
        num.clear();
    }

    //We now just need to sum up the numbers
    std::cout << "Sum: " << sum << std::endl;
    return 0;
}   

/**
 * NOTE: Another solution would just have been iterating over the string and checking if each 
 * character was an alpha-numeric number.
 * 
 * I suspect that the built-in functions are more efficient than a for loop, considering they are
 * likely using pointers to iterate over the string.
*/