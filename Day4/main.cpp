/**
 * Cesar Guerrero
 * 03/26/24
 * 
 * Advent of Code 2023: Day 4 - Scratch Card
 * 
*/


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <regex>

//Function Prototypes
int solutionPartOne(std::ifstream& file);
int solutionPartTwo(std::ifstream& file);
void analyzeCard(std::string& card, int& sum);


/**
 * Entry point into the program
 * @return 0 if the program ran successfully
*/
int main(){

    //Open the file
    std::ifstream file("scratch-data.txt");
    if(file.fail()){
        std::cerr << "Error opening file\n";
        return 1;
    }

    if(solutionPartOne(file) != 0){
        std::cerr << "Error in Part 1\n";
        file.close();
        return 2;
    }

    //Reset the file
    file.clear();
    file.seekg(0, std::ios::beg);
    if(solutionPartTwo(file) != 0){
        std::cerr << "Error in Part 2\n";
        file.close();
        return 3;
    }

    file.close();
    return 0;
}

/**
 * Solution for Part 1
 * @param {std::ifstream&} file - The file to read from
 * @return {int} - 0 if the solution ran successfully
*/
int solutionPartOne(std::ifstream& file){
    int sum = 0;
    std::string line;
    while(std::getline(file, line)){
        //Regex to format the line
        std::regex pattern("\\s+");
        line = std::regex_replace(line, pattern, " ");

        int position = line.find(':');
        if(position == std::string::npos){
            std::cerr << "Error: No ':' found\n";
            return 1;
        }
        line = line.substr(position+2);

        //Analyze the card
        analyzeCard(line, sum);
    }

    std::cout << "The sum of all the cards is: " << sum << std::endl;
    return 0;
}


/**
 * Solution for Part 2
 * @param {std::ifstream&} file - The file to read from
 * @return {int} - 0 if the solution ran successfully
*/
int solutionPartTwo(std::ifstream& file){
    int numCards = 0;

    std::unordered_map<int, int> cardMap;
    std::string line;
    int cardNumber = 1;

    while(std::getline(file, line)){
        //Regex to format the line
        std::regex pattern("\\s+");
        line = std::regex_replace(line, pattern, " ");
        cardMap[cardNumber]++;

        //Now that we have begun our map we can analyze the card
        std::unordered_set<std::string> winningSet;
        int winningNumbers = 0;

        //Tokenize the card
        std::stringstream ss(line);
        std::string token;
        while(std::getline(ss, token, ' ')){
            //Keep going until we hit the '|'
            if(token == "|"){
                break;
            }
            winningSet.insert(token);
        }

        //Now that we have the winning set we can analyze the rest of the card
        while(std::getline(ss, token, ' ')){
            if(winningSet.find(token) != winningSet.end()){
                ++winningNumbers;
            }
        }
        //Now that we have the number of winning numbers we need only see how many copies of our card exist
        int numberOfCopies = cardMap[cardNumber];
        for(int i = 0; i < numberOfCopies; i++){
            for(int j = 1; j <= winningNumbers; j++){
                cardMap[cardNumber+j]++;
            }
        }
        cardNumber++;
    }

    //Finally we count all the copies
    for(const auto& card: cardMap){
        if(card.first < cardNumber){
            numCards += card.second;
        }
    }

    std::cout << "The number of cards is: " << numCards << std::endl;
    return 0;
}


/**
 * Analyze the card
 * @param {std::string&} card - The card to analyze
 * @param {int&} sum -The running sum of all the cards
*/
void analyzeCard(std::string& card, int& sum){
    std::unordered_set<std::string> winningSet;

    int cardPointValue = 0;
    //Tokenize the card
    std::stringstream ss(card);
    std::string token;
    while(std::getline(ss, token, ' ')){
        //Keep going until we hit the '|'
        if(token == "|"){
            break;
        }
        winningSet.insert(token);
    }

    //Now that we have the winning set, we can analyze the rest of the card
    while(std::getline(ss, token, ' ')){
        if(winningSet.find(token) != winningSet.end()){
            if(cardPointValue == 0){
                cardPointValue = 1;
            }else{
                cardPointValue *= 2;
            }
        }
    }

    //Finally we add to our global sum
    sum += cardPointValue;
    return;
}