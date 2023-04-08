#pragma once
#include <string>
#include "Date.h"

class Item
{
private:
	char Group;     // Any from range 'A'...'Z'
	int Subgroup;   // Any from range 0...99
	std::string Name;    // Any, but not empty 
	Date Timestamp; // Any 
	
public:
	static const Date Begin;
	static const Date End;
	Item();
	Item(char, int, std::string, Date);
	Item(const Item &);
	~Item();
	bool operator==(const Item&) const;
	bool operator<(const Item&) const;
	char getGroup() const;
	int getSubgroup() const;
	std::string getName() const;
	Date getTimestamp() const;

// To do
};

