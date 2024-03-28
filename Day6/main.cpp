/**
 * Cesar Guerrero
 * 03/28/24
 * 
 * Day 6 - Wait For It
 * 
 * Starting Speed = 0 mm/ms
 * 1 ms Holding Button = +1 mm/ms
 * 
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <vector>


//Function Prototypes
int solutionPartOne(const std::vector<int>& times, const std::vector<int>& distances);
int solutionPartTwo(long time, long distance);

/**
 * Entry point into the program
 * @return {int} - 0 if the program ran successfully
*/
int main(){

    //Read in our file
    std::ifstream file("times.txt");
    if(file.fail()){
        std::cerr << "File not found\n";
        return 1;
    }

    std::vector<int> times;
    std::vector<int> distances;

    //Grab the times
    std::string line;
    std::stringstream ss;
    std::getline(file, line);
    line = std::regex_replace(line, std::regex("\\s+"), " ");
    line = line.substr(line.find(':')+2);
    ss.str(line);
    while(std::getline(ss, line, ' ')){
        times.push_back(std::stoi(line));
    };

    //Grab the distances
    ss.clear();
    std::getline(file, line);
    line = std::regex_replace(line, std::regex("\\s+"), " ");
    line = line.substr(line.find(':')+2);
    ss.str(line);
    while(std::getline(ss, line, ' ')){
        distances.push_back(std::stoi(line));
    };

    if(solutionPartOne(times, distances) != 0){
        std::cerr << "Error in Part One\n";
        file.close();
        return 2;
    }

    //Since the time and distance are all just one value we need to reconfigure our vectors to produce a singular value
    std::string time = "";
    std::string distance = "";
    for(auto& raceTime: times){
        time += std::to_string(raceTime);
    }
    for(auto& recordDistance: distances){
        distance += std::to_string(recordDistance);
    }

    if(solutionPartTwo(std::stol(time), std::stol(distance)) != 0){
        std::cerr << "Error in Part Two\n";
        file.close();
        return 3;
    }


    //Close the file
    file.close();
    return 0;
}


/**
 * Determine the number of ways you could beat the record in each race then multiply all the numbers together
 * @param {const std::vector<int>&} times - Vector holding time for boat race
 * @param {const std::vector<int>&} distances - Vector holding record distance for that race
 * @return {int} - 0 if the program ran successfully
*/
int solutionPartOne(const std::vector<int>& times, const std::vector<int>& distances){
    int multiplication = 1;
    for(int i = 0; i < times.size(); i++){
        int raceTime = times[i];
        int recordDistance = distances[i];

        //Notice that the times that won't be the record only exist at the ends
        //March from front
        int counter = 0;
        int distance = 0;
        int speed = 0;
        int holdTime = 0;
        while(true){
            distance = speed * (raceTime - holdTime);
            //Shift our values
            speed++;
            holdTime++;
            //If we never beat the record we still need to stop
            if(holdTime > raceTime){
                break;
            }
            if(distance <= recordDistance){
                counter++;
            }else{
                break;
            }
        }

        //March from back
        distance = 0;
        speed = raceTime;
        holdTime = raceTime;
        while(true){
            distance = speed * (raceTime - holdTime);
            //Shift our values
            speed--;
            holdTime--;
            if(holdTime < 0){
                break;
            }
            if(distance <= recordDistance){
                counter++;
            }else{
                break;
            }
        }
        multiplication *= (raceTime+1) - counter;
    }

    std::cout << "Multiplication: " << multiplication << "\n";

    return 0;
}

/**
 * Determine the number of ways you could beat the record for a singular race
 * @param {long} time - Time alloted for the race
 * @param {long} distance - Record distance
 * @return {int} - 0 if the program ran successfully
*/
int solutionPartTwo(long raceTime, long recordDistance){

    //Same idea as Part One but now we are only doing one race
    long counter = 0;
    long distance = 0;
    long speed = 0;
    long holdTime = 0;

    //Analyze front
    while(true){
        distance = speed * (raceTime - holdTime);
        //Shift our values
        speed++;
        holdTime++;
        //If we never beat the record we still need to stop
        if(holdTime > raceTime){
            break;
        }
        if(distance <= recordDistance){
            counter++;
        }else{
            break;
        }
    }

    //March from back
    distance = 0;
    speed = raceTime;
    holdTime = raceTime;
    while(true){
        distance = speed * (raceTime - holdTime);
        //Shift our values
        speed--;
        holdTime--;
        if(holdTime < 0){
            break;
        }
        if(distance <= recordDistance){
            counter++;
        }else{
            break;
        }
    }

    std::cout << "Ways to beat the record: " << (raceTime+1) - counter << "\n";

    return 0;
}