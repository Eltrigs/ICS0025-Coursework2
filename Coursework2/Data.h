#pragma once
#include <map>
#include <set>
#include "Item.h"
#include <optional>
#include <string>
#include "Date.h"
#include <algorithm>
#include <ranges>
#include <iostream>
#include <stdexcept>


class Data
{
	/*STL = Standard Template Library*/
private:
	/*
	 It is a C++ map in which the Item members Group are the keys. The values are pointers to inner C++
	 maps in which the keys are Item members Subgroup and values are pointers to sets. The sets contain
	 items belonging to the specified group and subgroup. The members of a set must be unique.

	 Roughly speaking, its a map, which has a key of type char and a value of type std::map*
	 the map it points to itself has a key of int and a value of std::set*
	 the set has elements of type Item.
	*/
	std::map<char, std::map<int, std::set<Item>*>*> DataStructure;

public:
	/*Constructs the object and fills the container with n random items.*/
	Data(int n);

	/*Constructs the object with an empty container.*/
	Data();

	/*Destructs the object and releases all the memory occupied by the container and the items in it.*/
	~Data();

	/*
	 Prints all the items stored in the container in command prompt window in easily readable format
	 (see Appendix). Items from the same group and subgroup must be ordered by their names.
	*/
	void PrintAll();

	/*Returns the total number of items in the container.*/
	int CountItems();

	/*
	 Returns the pointer to map containing all the items from group c. If the group does not exist, 
	 returns nullptr.
	*/
	std::map<int, std::set<Item>* >* GetGroup(char c);

	/*
	 Prints all the items from group c in command prompt window in easily readable format.
	 Items from the same subgroup must be ordered by their names. If the group was not
	 found, throws invalid_argument_exception.
	*/
	void PrintGroup(char c);

	/*Returns the current number of items in group c. If the group does not exist, returns 0.*/
	int CountGroupItems(char c);

	/*
	 Returns the pointer to set containing all the items from subgroup I from group c. If the subgroup
	 does not exist, returns nullptr.
	*/
	std::set<Item>* GetSubgroup(char c, int i );

	/*
	 Prints all the items from subgroup i from group c in command prompt window in easily readable
	 format (see Appendix). Items must be ordered by their names. If the subgroup was not found,
	 throws invalid_argument_exception.
	*/
	void PrintSubgroup(char c, int i);

	/*
	 Returns the current number of items in subgroup i from group c. 
	 If the subgroup does not exist, returns 0.
	*/
	int CountSubgroupItems(char c, int i);

	/*
	 Returns the item specified by group c, subgroup i and name s. If the item was not found returns
	 nullopt.
	*/
	std::optional<Item> GetItem(char c, int i, std::string s);

	/*
	 Prints the item specified by group c, subgroup i and name s. If the item was not found throws
	 invalid_argument_exception.
	*/
	void PrintItem(char c, int i, std::string s);

	/*
	 Creates, inserts and returns the specified item. If the input parameters were not correct, returns
	 nullopt. If necessary, creates the missing group and subgroup. If Timestamp is not specified, use
	 Date::CreateRandomDate(Date, Date) to create. Parameter s cannot be empty string. Important: a
	 set cannot contain items that are absolutely identical.
	*/
	std::optional<Item> InsertItem(char c, int i, std::string s, std::optional<Date> d = std::nullopt);

	/*
	 Creates and inserts the specified subgroup. The initializer_list contains tuples specifying the Names
	 (cannot be empty strings) and Timestamps (if not specified use Data::CreateRandomDate(Date,
	 Date) to create) of new items. Returns the pointer to new subgroup. If the specified subgroup
	 already exists or the input parameters are not correct, returns nullptr. If necessary, creates the
	 missing group. Important: a set cannot contain items that are absolutely identical.
	*/
	std::set<Item>* InsertSubgroup(char c, int i, std::initializer_list < std::tuple<std::string, std::optional<Date>> > items);

	/*
	 Creates and inserts the specified group. The subgroups initializer_list presents the keys to be
	 included into the new group. The items initializer_list contains initializer_lists presenting tuples
	 specifying the Names (cannot be empty strings) and Timetamps (if not specified use
	 Data::CreateRandomDate(Date, Date) to create) of new items. The first initializer_list from items
	 corresponds to the first integer in subgroups. Returns the pointer to new group. If the specified
	 group already exists or the input parameters are not correct, returns nullptr. Important: a set cannot
	 contain items that are absolutely identical.
	*/
	std::map<int, std::set<Item>*>* InsertGroup(char c, 
												std::initializer_list<int> subgroups, 
												std::initializer_list<std::initializer_list<std::tuple<std::string, std::optional<Date>>>> items);

	/*
	 Removes the specified item. If after removing the subgroup has no members, remove it too. If after
	 that the group is empty, remove it also. All the not used memory must be released. Return value:
	 false if the item was not found, otherwise true.
	*/
	bool RemoveItem(char c, int i, std::string s);

	/*
	 Removes the specified subgroup. If after removing the corresponding group has no members,
	 remove it too. All the not used memory must be released. Return value: false if the subgroup was
	 not found, otherwise true.
	*/
	bool RemoveSubgroup(char c, int i);

	/*
	 Removes the specified group. All the not used memory must be released. Return value: false if the
	 group was not found, otherwise true.
	*/
	bool RemoveGroup(char c);
};

