/**
 * Cesar Guerrero
 * 04/10/24
 * 
 * @file Day 9 - Mirage Maintenance
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

//Function Prototypes
int solutionPartOne(std::vector<std::vector<int>>& sequences);
int solutionPartTwo(std::vector<std::vector<int>>& sequences);

/**
 * Entry point of the program
*/
int main(){

    //Vector of vectors - All of the sequences
    std::vector<std::vector<int>> sequences;

    std::fstream file("input.txt");
    if(file.fail()){
        std::cerr << "Error opening file\n";
        return 1;
    }

    std::string line;
    while(std::getline(file, line)){
        std::vector<int> sequence;
        std::stringstream ss(line);
        int num;
        while(ss >> num){
            sequence.push_back(num);
        }
        sequences.push_back(sequence);
    }

    if(solutionPartOne(sequences) != 0){
        std::cout << "Failed part one\n";
        return 2;
    }

    if(solutionPartTwo(sequences) != 0){
        std::cout << "Failed part two\n";
        return 3;
    }
    
    return 0;
}

/**
 * Solution for part one of the problem
 * @param {std::vector<std::vector<int>>&} sequences - REFERENCE to vector of vectors of integers
 * @return {int} - 0 if successful, 1 if failed
*/
int solutionPartOne(std::vector<std::vector<int>>& sequences){
    int sumTotal = 0;
    //We need to go through each sequence
    for(auto& sequence: sequences){
        bool allZeroes = false;
        //Vectors to store the subsequence lengths as well as the numbers themselves
        std::vector<int> subSequenceLengths;
        std::vector<int> sequenceVec(sequence.begin(), sequence.end());

        //We need to keep track of where to start and how far to go
        int index = 0;
        int length = sequenceVec.size();
        while(!allZeroes){
            allZeroes = true;
            for(int i = index; i < length-1; i++){
                int result = sequenceVec[i+1] - sequenceVec[i];
                sequenceVec.push_back(sequenceVec[i+1] - sequenceVec[i]);
                if(result != 0){
                    allZeroes = false;
                }
            }
            subSequenceLengths.push_back(length);
            index = length; //This is the the new starting point
            length = sequenceVec.size();
        }
        //Now we need to loop through those subsequences and devise a running value
        int runningValue = 0;
        for(int i = subSequenceLengths.size()-1; i >=0; i--){
            runningValue += sequenceVec[subSequenceLengths[i]-1];
        }
        sumTotal += runningValue;
    }

    std::cout << "The sum total is: " << sumTotal << std::endl;

    return 0;
}

/**
 * Solution for part two of the problem
 * @param {std::vector<std::vector<int>>&} sequences - REFERENCE to vector of vectors of integers
 * @return {int} - 0 if successful, 1 if failed
*/
int solutionPartTwo(std::vector<std::vector<int>>& sequences){
    //Rather than doing the end we are going to do the beginning
    int sumTotal = 0;
    for(auto& sequence: sequences){
        bool allZeroes = false;

        //Vectors
        std::vector<int> subSequenceStarts;
        std::vector<int> sequenceVec(sequence.begin(), sequence.end());

        //Start and length to traverse
        int index = 0;
        int length = sequenceVec.size();

        while(!allZeroes){
            allZeroes = true;
            for(int i = index; i < length-1; i++){
                int result = sequenceVec[i+1] - sequenceVec[i];
                sequenceVec.push_back(sequenceVec[i+1] - sequenceVec[i]);
                if(result != 0){
                    allZeroes = false;
                }
            }
            subSequenceStarts.push_back(index);
            index = length;
            length = sequenceVec.size();
        }
        //Find the start values
        int runningValue = 0;
        for(int i = subSequenceStarts.size()-1; i >= 0; i--){
            runningValue = sequenceVec[subSequenceStarts[i]] - runningValue;
        }
        std::cout << "The running value is: " << runningValue << std::endl;
        sumTotal += runningValue;
    }

    std::cout << "The sum total is: " << sumTotal << std::endl;

    return 0;
}