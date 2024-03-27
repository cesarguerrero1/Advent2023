/**
 * Cesar Guerrero
 * 03/26/24
 * 
 * Day 5: If you give a seed a fertilizer
 * 
 * Source-to-Destination Mapping is 3 numbers:
 * Destination Start, Source Start, Range Length
 * 
 * Example:
 * Seed 79
 * 
 * Seed-to-soil (Source-to-Destination) Mapping:
 * Destination, Source, Length
 * 50 98 2 -- In this case Seed 98 maps to Soil 50 and Seed 99 maps to Soil 51
 * 52 50 48 -- Same ideas above
 * 
 * If a mapping doesnt exist, then source = destination, no change!
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <utility>

//Function Prototypes
int establishMaps(std::ifstream& file, std::unordered_map<std::string, std::map<long, std::pair<long, long>>>& conversionMap);
int solutionPartOne(const std::vector<long>& seeds, std::unordered_map<std::string, std::map<long, std::pair<long, long>>>& conversionMap);

//Global Processing Vector
const std::vector<std::string> mapNames = {"seed-to-soil", "soil-to-fertilizer", "fertilizer-to-water", "water-to-light", "light-to-temperature", "temperature-to-humidity", "humidity-to-location"};

/**
 * Entry point into the program
 * @return {int} - 0 if the program ran successfully
*/
int main(){

    //Open the file
    std::ifstream file("fertilizer.txt");
    if(file.fail()){
        std::cerr << "Error opening file\n";
        return 1;
    }

    //This map will handle our entire mapping scheme
    std::unordered_map<std::string, std::map<long, std::pair<long, long>>> conversionMap;

    //Parse our seeds
    std::string line;
    std::getline(file, line);
    int colon = line.find(':');
    if(colon == std::string::npos){
        std::cerr << "Error in file format\n";
        file.close();
        return 2;
    }

    //Now we need to tokenize the seeds
    line = line.substr(colon+2);
    std::stringstream ss(line);
    std::string seed;
    std::vector<long> seeds;
    while(std::getline(ss, seed, ' ')){
        seeds.push_back(std::stol(seed));
    }
    //Remove this line entirely
    std::getline(file, line);

    //Establish the maps
    if(establishMaps(file, conversionMap) != 0){
        std::cerr << "Error establishing maps\n";
        file.close();
        return 3;
    }

    //Solve Part One
    if(solutionPartOne(seeds, conversionMap) != 0){
        std::cerr << "Error in Part One\n";
        file.close();
        return 4;
    }

    file.close();
    return 0;
}

/**
 * Establishes the maps that we will use to process the seeds
 * @param {std::ifstream&} file - The file that we will use to establish the maps
 * @param {std::unordered_map<std::string, std::map<long, std::pair<long, long>>>&} conversionMap - The map that we will use to process each seed
 * @return {int} - 0 if the function ran successfully
*/
int establishMaps(std::ifstream& file, std::unordered_map<std::string, std::map<long, std::pair<long, long>>>& conversionMap){
    std::string line;
    for(auto& mapName: mapNames){
        conversionMap[mapName] = std::map<long, std::pair<long, long>>();

        //We need to process each section of data
        while(std::getline(file, line)){
            if(line.find(mapName) != std::string::npos){
                //Don't process the section name
                continue;
            }else if(line.length() == 0){
                //Break when we reach an empty section
                break;
            }

            //We have hit data so need to process it
            std::stringstream ss(line);
            long destination, source, length;
            //Since we know the format of the data, we can just extract it
            ss >> destination >> source >> length;

            //Insert the data into the map
            conversionMap[mapName][source] = std::make_pair(destination, length);
        }
    }

    return 0;
}

/**
 * Solves part one of the problem
 * @param {const std::vector<long>&} seeds - The seeds that we will process
 * @param {std::unordered_map<std::string, std::map<long, std::pair<long, long>>>&} conversionMap - The map that we will use to process each seed
 * @return {int} - 0 if the function ran successfully
*/
int solutionPartOne(const std::vector<long>& seeds, std::unordered_map<std::string, std::map<long, std::pair<long, long>>>& conversionMap){
    long lowestLocationNumber = -1;
    for(auto& seed: seeds){
        //Now we need to process in order
        long sourceValue = seed;
        for(auto& mapName : mapNames){
            auto& processMap = conversionMap[mapName];
            //We now need to process the map
            for(auto& pair : processMap){
                long lowerBound = pair.first;
                long upperBound = lowerBound + pair.second.second - 1;
                if(sourceValue >= lowerBound && sourceValue <= upperBound){
                    sourceValue = pair.second.first + (sourceValue - lowerBound);
                    break;
                }
            }
        }
        //Finally we need to update our lowest location number
        if(lowestLocationNumber == -1){
            lowestLocationNumber = sourceValue;
        }else{
            lowestLocationNumber = std::min(lowestLocationNumber, sourceValue);
        }
    }

    std::cout << "Lowest Location Number: " << lowestLocationNumber << "\n";

    return 0;
}