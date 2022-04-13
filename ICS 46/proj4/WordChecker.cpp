// WordChecker.cpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"

WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
	return words.contains(word);
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
	std::vector<std::string> suggestions;
	std::string alphabets = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	// 1) delete
	for(int i=0; i<word.size(); i++)
	{	
		std::string temp = word;
		temp = temp.erase(i,1);
		if(wordExists(temp) && std::find(suggestions.begin(), suggestions.end(), temp) == suggestions.end())
			suggestions.push_back(temp);
	}

	// 2) Addition
	for(int i=0; i<word.size()+1; i++)
	{
		for(int j=0; j<alphabets.size(); j++)
		{
			std::string temp = word;
			temp.insert(i,alphabets.substr(j,1));
			if(wordExists(temp) && std::find(suggestions.begin(), suggestions.end(), temp) == suggestions.end())
				suggestions.push_back(temp);
		}
	}

	// 3) Substitution
	for(int i=0; i<word.size(); i++)
	{
		for(int j=0; j<alphabets.size(); j++)
		{
			std::string temp = word;
			temp.replace(i, 1, alphabets.substr(j,1));
			if(wordExists(temp) && std::find(suggestions.begin(), suggestions.end(), temp) == suggestions.end())
				suggestions.push_back(temp);
		}
	}

	// 4) Swap
	for(int i=0; i<word.size()-1; i++)
	{
		for (int j=i+1; j<word.size(); j++)
		{
			std::string temp = word;
			std::swap(temp[i],temp[j]);
			if(wordExists(temp) && std::find(suggestions.begin(), suggestions.end(), temp) == suggestions.end())
				suggestions.push_back(temp);
		}
	}

	// 5) Giving a space
	for(int i=1; i<word.size(); i++)
	{
		std::string temp = word;
		temp.insert(i," ");
		if(wordExists(temp) && std::find(suggestions.begin(), suggestions.end(), temp) == suggestions.end())
				suggestions.push_back(temp);
	}

    return suggestions;
}

