/**
 * Cesar Guerrero
 * 01/23/24
 * 
 * Advent of Code 2023: Day 4
 * 
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

//We need to open the file
std::string scratchFile = "scratch-data.txt";

int pointSum = 0;

//Function Prototypes
void analyzeCard(const std::string&);

int main(){

    //Open the file
    std::ifstream file;
    file.open(scratchFile);

    if(file.fail()){
        std::cout << "Error opening file\n";
        return 1;
    }

    std::string line;
    while(!file.eof()){
        std::getline(file, line);

        //Everything before the ":" is irrelevant
        int position = line.find(':'); //Recall that this is returning a size_t type
        if(position == std::string::npos){
            std::cout << "Error: No ':' found\n";
            return 2;
        }

        line = line.substr(position + 1);

        //Analyze the card
        analyzeCard(line);
    }

    //Close your file
    file.close();

    std::cout << "Point sum: " << pointSum << "\n";

    return 0;
}

//Best practice to PREVENT references from being modified
void analyzeCard(const std::string& card){

    //Create a map for each card
    std::map<std::string, int> cardMap;
    int cardPointValue = 0;
    
    std::string value;
    std::istringstream token;
    token.clear();
    token.str(card);

    //Keep tokenizing until there is nothing left
    while(std::getline(token, value, ' ')){
        if(value != ""){
            if(value == "|"){
                break;
            }

            cardMap.insert(std::pair<std::string, int>(value, 1));
        }

    }

    //Our map will be full
    while(std::getline(token, value, ' ')){
        if(value != ""){
            auto keyValuepair = cardMap.find(value);
            if(keyValuepair != cardMap.end()){
                //The number exists
                if(cardPointValue == 0){
                    cardPointValue++;
                }else{
                    cardPointValue *= 2;
                }
            }
        }
    }
    pointSum += cardPointValue;

    return;
}