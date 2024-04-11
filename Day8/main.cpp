/**
 * Cesar Guerrero
 * 04/10/24
 * 
 * @file Day 8 - Haunted Wasteland
 * 
*/

#include <iostream>
#include <deque> //We are going to use a queue since the instructions may need to be repeated
#include <unordered_map> //Store each location with the next instruction
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <vector>
#include <thread>

int solutionPartOne(std::deque<char> instructions, std::unordered_map<std::string, std::pair<std::string, std::string>>& directionsMap);
int solutionPartTwo(std::deque<char> instructions, std::unordered_map<std::string, std::pair<std::string, std::string>>& directionsMap);

/**
 * Entry point of the program
*/
int main(){

    //Input is maps of how to navigate the desert - left/right instructions, network of labeled nodes
    std::fstream file("input.txt");
    if(file.fail()){
        std::cerr << "Failed to open file\n";
        return 1;
    }
    std::string line;

    //We know the first line is the instructions
    std::deque<char> instructions;
    std::getline(file, line);
    for(auto c : line){
        instructions.push_back(c);
    }
    std::getline(file, line); //Skip the empty line

    //Now we create our map
    std::unordered_map<std::string, std::pair<std::string, std::string>> directionsMap;
    while(std::getline(file, line)){
        //We can use regex to remove specific characters
        line = std::regex_replace(line, std::regex("[=,()]"), "");
        line = std::regex_replace(line, std::regex("\\s+"), " ");

        //Split by string stream
        std::stringstream ss(line);
        std::string location, left, right;
        ss >> location >> left >> right;
        directionsMap[location] = std::make_pair(left, right);
    }
    if(solutionPartOne(instructions, directionsMap) != 0){
        std::cout << "Failed Part One\n";
        return 2;
    }

    if(solutionPartTwo(instructions, directionsMap) != 0){
        std::cout << "Failed Part Two\n";
        return 3;
    }


    return 0;
}

/**
 * Solution to Part One of the problem
 * @param {std::deque<char>} instructions - A copy of the instructions to follow
 * @param {std::unordered_map<std::string, std::pair<std::string, std::string>&} directionsMap - The map of locations and their directions
 * @return {int} - The number of steps it took to reach the end
*/
int solutionPartOne(std::deque<char> instructions, std::unordered_map<std::string, std::pair<std::string, std::string>>& directionsMap){

    int steps = 0;
    std::string currentLocation = "AAA";

    while(true){
        char instruction = instructions.front();
        instructions.pop_front();
        instructions.push_back(instruction); //We may need to repeat the instructions

        auto possibleLocations = directionsMap[currentLocation];
        if(instruction == 'L'){
            currentLocation = possibleLocations.first;
        }else{
            currentLocation = possibleLocations.second;
        }
        steps++;

        if(currentLocation == "ZZZ"){
            std::cout << "Number of steps: " << steps << std::endl;
            break;
        }
    }

    return 0;  
}

/**
 * Solution to Part Two of the problem
 * @param {std::deque<char>} instructions - A copy of the instructions to follow
 * @param {std::unordered_map<std::string, std::pair<std::string, std::string>&} directionsMap - The map of locations and their directions
*/
int solutionPartTwo(std::deque<char> instructions, std::unordered_map<std::string, std::pair<std::string, std::string>>& directionsMap){
    std::vector<int> stepsToZ;
    //We need to go through our map and find all the locations that end in 'A'
    for(auto location : directionsMap){
        if(location.first[2] == 'A'){
            int steps = 0;
            std::string newLocation = location.first;
            while(true){
                char instruction = instructions.front();
                instructions.pop_front();
                instructions.push_back(instruction);
                if(instruction == 'L'){
                    newLocation = directionsMap[newLocation].first;
                }else{
                    newLocation = directionsMap[newLocation].second;
                }
                steps++;
                if(newLocation[2] == 'Z'){
                    stepsToZ.push_back(steps);
                    break;
                }
            }
        }
    }

    //Least Common Multiple - Greatest Common Factor Method
    int gcf = 2;
    while(true){
        bool found = true;
        for(auto steps : stepsToZ){
            if(steps % gcf != 0){
                found = false;
                gcf++;
                break;
            }
        }
        if(found){
            break;
        }
    }
    
    long product = (stepsToZ[0]*stepsToZ[1])/gcf;
    std::cout << "Product: " << product << std::endl;
    for(int i = 2; i < stepsToZ.size(); i++){
        product = (product*stepsToZ[i])/gcf;
    }
  
    std::cout << "Number of steps: " << product << std::endl;

    return 0;
}