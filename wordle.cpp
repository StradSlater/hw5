#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include <cstdlib>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
std::set<int> getCertainPositions(const std::string& in);
std::set<std::string> wordleHelper(
    std::string guess, 
    int pos,
    const std::set<int>& positions,
    const std::string& in,
    std::string floatings,
    const std::set<std::string>& dict,
    std::set<std::string>& final);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::set<std::string> final;
    if (in.empty()){
        return final;
    }
    std::set<int> positions = getCertainPositions(in);
    std::string guess = "";
    int pos = 0;
    return wordleHelper(guess, pos, positions, in, floating, dict, final);


}

// Define any helper functions here

std::set<int> getCertainPositions(const std::string& in){
    std::set<int> positions;
    for (int i = 0; i < in.size(); ++i){
        if (in[i] != '-'){
            positions.insert(i);
        }
    }
    return positions;

}


std::set<std::string> wordleHelper(
    std::string guess, 
    int pos,
    const std::set<int>& positions,
    const std::string& in,
    std::string floatings,
    const std::set<std::string>& dict,
    std::set<std::string>& final){
    
    if (pos == (in.size())){
        std::set<std::string>::iterator it = dict.find(guess);
        if (it != dict.end()){ 
            if (final.find(guess) != final.end()){
                return final;
            }
            final.insert(guess);
            return final;
        }
        else{
            return final;
        }
        
    }

    else{
        std::set<int>::iterator it = positions.find(pos);
        int pos_ = pos;
        for (int i = 0; i < guess.size(); ++i){
            if (positions.find(i) != positions.end()){
                pos_ -= 1;
            }
        }

        int remaining = (in.size() - positions.size())-pos_;
        if (it != positions.end()){
            std::string guess_ = guess;
            guess_ += in[pos];
            final = wordleHelper(guess_, pos+1, positions, in, floatings, dict, final);
        }
        else if (abs(remaining) == floatings.size() && floatings.size() > 0){
            for (int i = 0; i < floatings.size(); ++i){
                std::string guess_ = guess;
                guess_ += floatings[i];
                std::string floatings_ = floatings;
                floatings_.erase(i, 1);
                final = wordleHelper(guess_, pos+1, positions, in, floatings_, dict, final);
            }
        }

        else{
            for (int i = 97; i < 123; ++i){
                std::string guess_ = guess;
                guess_ += static_cast<char>(i);
                if (floatings.find(i) != std::string::npos){
                    std::string floatings_ = floatings;
                    int index = floatings_.find(char(i));
                    floatings_.erase(index, 1);
                    final = wordleHelper(guess_, pos+1, positions, in, floatings_, dict, final);
                }
                else{
                    final = wordleHelper(guess_, pos+1, positions, in, floatings, dict, final);
                }
            }
        }
        return final;



    }


}
