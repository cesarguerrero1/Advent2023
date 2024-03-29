/**
 * Cesar Guerrero
 * 03/29/24
 * 
 * Day 7
*/


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>

//We need to attach values to each card
std::unordered_map<char, int> cardValues = {
    {'2', 2},
    {'3', 3},
    {'4', 4},
    {'5', 5},
    {'6', 6},
    {'7', 7},
    {'8', 8},
    {'9', 9},
    {'T', 10},
    {'J', 11},
    {'Q', 12},
    {'K', 13},
    {'A', 14}
};

//We are using a struct to hold hand information
struct hand{
    std::vector<char> cards;
    std::unordered_map<char, int> cardMap;
    int handRank;
    int bidAmount;
};

//Function prototypes
void assignNormalHandRank(hand& h);
int solutionPartOne(std::vector<hand>& hands);
int solutionPartTwo(std::vector<hand>& hands);
void assignJokerHandRank(hand& h);


/**
 * Entry point of the program
 * @return {int} - 0 if the program runs successfully
*/
int main(){

    std::ifstream file("input.txt");
    if(file.fail()){
        std::cerr << "Failed to open file\n";
        return 1;
    }

    //We are going to group together hands of the same rank
    std::vector<hand> hands;
    std::string camelHand;
    std::stringstream ss;
    while(std::getline(file, camelHand)){
        std::string cards;
        int bidAmount;
        //We are reusing the stringstream so need to clear it
        ss.clear();
        ss.str(camelHand);
        ss >> cards >> bidAmount;

        //Now we need to create our hand object
        hand h;
        h.bidAmount = bidAmount;
        for(auto& c : cards){
            h.cards.push_back(c);
            h.cardMap[c]++;
        }
        hands.push_back(h);
    }

    //Now that we have our hands we can solve Part One and then Part Two
    if(solutionPartOne(hands) != 0){
        std::cout << "Failed Part One\n";
        return 2;
    }

    if(solutionPartTwo(hands) != 0){
        std::cout << "Failed Part Two\n";
        return 3;
    }

    return 0;
}


/**
 * Assigns a rank to a hand
 * @param {hand&} h - The hand to assign a rank to
*/
void assignNormalHandRank(hand& h){

    //We are going to go from best hand to worst hand
    //Find if we have multiples of each card AND check how many pairs we have
    int sameKindCount = 0;
    int pairCount = 0;
    for(auto& pair : h.cardMap){
        sameKindCount = std::max(sameKindCount, pair.second);
        if(pair.second == 2){
            pairCount++;
        }
    }

    //If the sameKindCount is greater than 3 it means we can just assign our rank
    if(sameKindCount > 3){
        h.handRank = sameKindCount+1;
        return;
    };

    //If there is <= 3 of kind we need to understand our pairs
    if(sameKindCount == 3){
        if(pairCount == 1){
            h.handRank = 4;
        }else{
            h.handRank = 3;
        }
        return;
    }

    //In this case our final checks are if we have pairs otherwise we have a high card
    if(pairCount == 2){
        h.handRank = 2;
    }else if(pairCount == 1){
        h.handRank = 1;
    }else{
        h.handRank = 0;
    }
    return;
}

/**
 * Solves part one of the problem
 * @param {<std::vector<hand>} hands - Array of all the hands
 * @return {int} - 0 if the solution is correct
*/
int solutionPartOne(std::vector<hand>& hands){
    //We are going to group together hands of the same rank
    std::array<std::vector<hand>, 7> handGroups;

    for(int i = 0; i < handGroups.size(); i++){
        handGroups[i] = std::vector<hand>();
    }
    
    //Group the hands by rank
    for(auto& h: hands){
        assignNormalHandRank(h);
        handGroups[h.handRank].push_back(h);
    }

    //Sort each group of hands
    for(auto& group: handGroups){
        sort(group.begin(), group.end(), [](hand a, hand b){
            for(int i = 0; i < a.cards.size(); i++){
                if(cardValues[a.cards[i]] != cardValues[b.cards[i]]){
                    return cardValues[a.cards[i]] < cardValues[b.cards[i]];
                }
            }
            return false;
        });
    }

    //Now we caclulate the total winnings
    long totalWinnings = 0;
    int rank = 1;
    for(auto& group: handGroups){
        for(auto& h: group){
            totalWinnings += h.bidAmount * rank;
            rank++;
        }
    }

    std::cout << "Total Winnings: " << totalWinnings << std::endl;

    return 0;
}


/**
 * Solves part two of the problem
 * @param {<std::vector<hand>&} hands - Array of all the hands
 * @return {int} - 0 if the solution is correct
*/
int solutionPartTwo(std::vector<hand>& hands){
    //We are going to group together hands of the same rank
    std::array<std::vector<hand>, 7> handGroups;

    for(int i = 0; i < handGroups.size(); i++){
        handGroups[i] = std::vector<hand>();
    }

    //We are changing the value of J
    cardValues['J'] = 1;

    //Group the hands by rank
    for(auto& h: hands){
        assignJokerHandRank(h);
        handGroups[h.handRank].push_back(h);
    }

    //Sort each group of hands
    for(auto& group: handGroups){
        sort(group.begin(), group.end(), [](hand a, hand b){
            for(int i = 0; i < a.cards.size(); i++){
                if(cardValues[a.cards[i]] != cardValues[b.cards[i]]){
                    return cardValues[a.cards[i]] < cardValues[b.cards[i]];
                }
            }
            return false;
        });
    }

    //Now we caclulate the total winnings
    long totalWinnings = 0;
    int rank = 1;
    for(auto& group: handGroups){
        for(auto& h: group){
            totalWinnings += h.bidAmount * rank;
            rank++;
        }
    }

    std::cout << "Total Winnings: " << totalWinnings << std::endl;

    return 0;
}

/**
 * Assigns a rank to a hand - Jokers can now increase hand rank
 * @param {hand&} h - The hand to assign a rank to
*/
void assignJokerHandRank(hand& h){
    //As before we are going to go from best hand to worst hand - However now need to account for Jokers
    int sameKindCount = 0;
    int pairCount = 0;
    for(auto& pair : h.cardMap){
        sameKindCount = std::max(sameKindCount, pair.second);
        if(pair.second == 2){
            pairCount++;
        }
    }

    if(sameKindCount == 5){
        h.handRank = 6;
        return;
    }

    if(sameKindCount == 4){
        if(h.cardMap['J'] == 4){
            h.handRank = 6;
        }else{
            if(h.cardMap['J'] > 0){
                h.handRank = 6;
            }else{
                h.handRank = 5;
            
            }
        }
        return;
    }

    //If there are no jokers we can just assign the rank as normal
    if(h.cardMap['J'] > 0){
        if(sameKindCount == 3){
            //There is a special case where we have 3 jacks
            if(h.cardMap['J'] == 3){
                if(pairCount == 1){
                    //This becomes 5 of a kind
                    h.handRank = 6;
                }else{
                    //This becomes 4 of a kind
                    h.handRank = 5;
                }
            }else if(h.cardMap['J'] == 2){
                //This becomes 5 of a kind
                h.handRank = 6;
            }else{
                //This becomes 4 of a kind
                h.handRank = 5;
            }
            return;
        }
        
        //Now we care about pairs
        if(pairCount == 2){
            if(h.cardMap['J'] == 2){
                //This becomes 4 of a kind
                h.handRank = 5;
            }else{
                //This becomes a full house
                h.handRank = 4;
            }
        }else if(pairCount == 1){
            h.handRank = 3;
        }else{
            //There are no pairs but there is a jack so we will always have a pair
            h.handRank = 1;
        }
        return;
    }else{
        //If there is <= 3 of kind we need to understand our pairs
        if(sameKindCount == 3){
            if(pairCount == 1){
                h.handRank = 4;
            }else{
                h.handRank = 3;
            }
            return;
        }

        //In this case our final checks are if we have pairs otherwise we have a high card
        if(pairCount == 2){
            h.handRank = 2;
        }else if(pairCount == 1){
            h.handRank = 1;
        }else{
            h.handRank = 0;
        }
        return;
    }
   
}