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
#include <deque>
#include <unordered_map>
#include <map>

//Global Processing Vector
std::vector<std::string> conversions = {"seed-to-soil", "soil-to-fertilizer", "fertilizer-to-water", "water-to-light", "light-to-temperature", "temperature-to-humidity", "humidity-to-location"};
//We are going to use a struct to represent the inervals
struct interval{
    long start;
    long end;
    int conversionNum;
    interval(long start, long end, int conversionNum): start(start), end(end), conversionNum(conversionNum){};
};

//Function Prototypes
int establishMaps(std::ifstream& file, std::unordered_map<std::string, std::map<long, std::pair<long, long>>>& conversionMap);
int solutionPartOne(const std::vector<long>& seeds, std::unordered_map<std::string, std::map<long, std::pair<long, long>>>& conversionMap);
int solutionPartTwo(std::deque<interval>& intervals, std::unordered_map<std::string, std::map<long, std::pair<long, long>>>& conversionMap);


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

    //This map will handle our entire mapping scheme -- std::map<source, std::pair<destination, length>>
    std::unordered_map<std::string, std::map<long, std::pair<long, long>>> conversionMap;

    //Parse our seeds
    std::string line;
    std::getline(file, line);
    int colonLocation = line.find(':');
    if(colonLocation == std::string::npos){
        std::cerr << "Error in file format\n";
        file.close();
        return 2;
    }

    //Now we need to tokenize the seeds
    std::vector<long> seeds;
    line = line.substr(colonLocation+2);
    std::stringstream ss(line);
    std::string seed;
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

    std::deque<interval> intervals;
    for(int i = 0; i < seeds.size()-1; i += 2){
        intervals.push_back(interval(seeds[i], seeds[i] + seeds[i+1], 0));
    }

    //Now that we have our intervals 
    if(solutionPartTwo(intervals, conversionMap) != 0){
        std::cerr << "Error in Part Two\n";
        file.close();
        return 5;
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
    for(auto& conversion: conversions){
        conversionMap[conversion] = std::map<long, std::pair<long, long>>();

        while(std::getline(file, line)){
            if(line.find(conversion) != std::string::npos){
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
            conversionMap[conversion][source] = std::make_pair(destination, length);
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

    //We are going to process each seed
    for(auto& seed: seeds){
        long sourceValue = seed;
        //For each seed we traverse the conversions
        for(auto& conversion: conversions){
            auto& processMap = conversionMap[conversion];
            //We now need to process the map
            for(auto& pair : processMap){
                if(sourceValue >= pair.first && sourceValue <= pair.first + pair.second.second-1){
                    sourceValue = pair.second.first + (sourceValue - pair.first);
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


/**
 * Solve part two of the problem
 * @param {std::deque<interval>&} intervals - The intervals that we will process
 * @param {std::unordered_map<std::string, std::map<long, std::pair<long, long>>>&} conversionMap - The map that we will use to process each seed
 * @return {int} - 0 if the function ran successfully
*/
int solutionPartTwo(std::deque<interval>& intervals, std::unordered_map<std::string, std::map<long, std::pair<long, long>>>& conversionMap){
    long lowestLocationNumber = -1;

    //The idea is that we are going to process each interval
    while(intervals.size() != 0){
        //Pop the front
        auto currentInterval = intervals.front();
        intervals.pop_front();

        //Now attempt to process this interval
        while(currentInterval.conversionNum < conversions.size()){
            //Grab the conversion map for this interval
            auto& processMap = conversionMap[conversions[currentInterval.conversionNum]];

            for(auto& pair: processMap){
                long destination = pair.second.first;
                long range = pair.second.second;
                long source = pair.first;

                if(currentInterval.start >= source+range || currentInterval.end <= source){
                    //The interval is not contained within the pair
                    continue;
                }else if(currentInterval.start >= source && currentInterval.end <= source + range){
                    //The interval is fully contained so transform it
                    currentInterval.start = destination + (currentInterval.start-source);
                    currentInterval.end = destination + (currentInterval.end-source);
                    break;
                }else if(currentInterval.start < source && currentInterval.end-1 >= source){
                    //The interval is partiall contained BUT we need to see if there are 3 intervals
                    intervals.push_back(interval(currentInterval.start, source, currentInterval.conversionNum));
                    currentInterval.start = source;
                    //Check if our end overruns the range
                    if(currentInterval.end > source + range){
                        //we need another interval for the portion that exceeds the range
                        intervals.push_back(interval(source+range, currentInterval.end, currentInterval.conversionNum));
                        currentInterval.end = source + range;
                    };
                    //Now just process the current interval
                    currentInterval.start = destination + (currentInterval.start-source);
                    currentInterval.end = destination + (currentInterval.end-source);
                    break;                    
                }
            }
            //Always increment the conversion number
            currentInterval.conversionNum++;
        }

        //Since the current interval has now been fully processed we only care about the smallest location
        if(lowestLocationNumber == -1){
            lowestLocationNumber = currentInterval.start;
        }else{
            lowestLocationNumber = std::min(lowestLocationNumber, currentInterval.start);
        }
    }


    std::cout << "Lowest Location Number: " << lowestLocationNumber << "\n";
    return 0;
}