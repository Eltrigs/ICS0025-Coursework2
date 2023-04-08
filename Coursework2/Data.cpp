#include "Data.h"

inline bool checkGroupInput(char c)
{
	if (c >= 'A' && c <= 'Z')
	{
		return true;
	}
	return false;
}

inline bool checkSubgroupInput(int i)
{
	if (i >= 0 && i <= 99)
	{
		return true;
	}
	return false;
}

inline bool checkNameInput(std::string s)
{
	if (!s.empty() && s != "")
	{
		return true;
	}
	return false;
}

Data::Data(int n)
{
	//Start adding n objects of type Item.
	for (int i = 0; i < n; i++)
	{
		//Create a new random item using it's constructor
		Item item = Item();
		//Insert it like any other Item object
		//std::cout << item.getGroup() << "\t" << item.getSubgroup() << "\t" << item.getName() << "\t" << item.getTimestamp()<< std::endl;
		this->InsertItem(item.getGroup(), item.getSubgroup(), item.getName(), item.getTimestamp());
	}
}

Data::Data()
{
}

Data::~Data()
{
	//Iterate through the DataStructure i.e. groups
	for (auto dataStructureIterator = DataStructure.begin(); dataStructureIterator != DataStructure.end(); ++dataStructureIterator) {
		
		//Take the value of the current group
		auto currentGroup = dataStructureIterator->second;

		//Then iterate through the current group's every subgroup
		for (auto subGroupIterator = currentGroup->begin(); subGroupIterator != currentGroup->end(); ++subGroupIterator) {
			//Iterate through the current subgroup's every item and delete it
			for_each(subGroupIterator->second->begin(), subGroupIterator->second->end(), [](Item item) {item.~Item(); });
			//After every subgroup element has been deleted, delete the subgroup itself
			delete subGroupIterator->second;
		}
		//After every subgroup has been deleted, delete the current group itself
		delete currentGroup;
	}
	//After every group has been deleted, clear the DataStructure
	DataStructure.clear();
}

void Data::PrintAll()
{
	//This lambda is for taking an Item object and printing it's relevant contents
	auto printLambda = [](const Item item) { std::cout << item.getName() + " " + item.getTimestamp().ToString() << std::endl; };

	//Iterate through all groups
	for (auto groupIterator = DataStructure.begin(); groupIterator != DataStructure.end(); ++groupIterator) {
		auto group = groupIterator->second;
		std::cout << groupIterator->first << ":" << std::endl; // print group key

		//Iterate through all subgroups
		for (auto subgroupIterator = group->begin(); subgroupIterator != group->end(); ++subgroupIterator) {
			std::cout << subgroupIterator->first << ": "; // print subgroup key

			// Print every item in the subrgoup using the print lambda
			std::for_each(subgroupIterator->second->begin(), subgroupIterator->second->end(), printLambda);
		}

		std::cout << std::endl;
	}
}

int Data::CountItems()
{
	int itemCount = 0;
	if (DataStructure.empty()) {
		return itemCount;
	}
	for (auto dataStructureIterator = DataStructure.begin(); dataStructureIterator != DataStructure.end(); ++dataStructureIterator) {
		auto group = dataStructureIterator->second;

		for (auto groupIterator = group->begin(); groupIterator != group->end(); ++groupIterator) {
			std::for_each(groupIterator->second->begin(), groupIterator->second->end(), [&itemCount](Item item) { itemCount++; });
		}
	}
	return itemCount;
}

std::map<int, std::set<Item>*>* Data::GetGroup(char c)
{
	auto groupIterator = DataStructure.find(c);
	if (groupIterator == DataStructure.end()) //The group does not exist
	{
		return nullptr;
	}
	else //The group exists, return it
	{
		return groupIterator->second;
	}
}

void Data::PrintGroup(char c)
{
	//This lambda is for taking an Item object and printing it's relevant contents
	auto printLambda = [](const Item item) { std::cout << item.getName() + " " + item.getTimestamp().ToString() << "; "; };

	//Find group
	auto group = GetGroup(c);
	if (group != nullptr)
	{
		std::cout << "Group " << c << ":" << std::endl;
		//Iterate through subgroups
		for (auto subgroupIterator = group->begin(); subgroupIterator != group->end(); ++subgroupIterator) {
			std::cout << subgroupIterator->first << ": "; // print the subgroup key

			// Print every item in the subrgoup using the print lambda
			std::for_each(subgroupIterator->second->begin(), subgroupIterator->second->end(), printLambda);
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	else
	{
		throw std::invalid_argument("Group " + std::string(1, c) + " does not exist\n");
	}
}

int Data::CountGroupItems(char c)
{	
	int groupCounter = 0;
	auto group = GetGroup(c);
	if (group != nullptr) {
		for_each(group->begin(), group->end(), [&groupCounter](auto subgroup) {groupCounter += subgroup.second->size(); });
	}
	return groupCounter;
}

std::set<Item>* Data::GetSubgroup(char c, int i)
{
	auto group = GetGroup(c);
	if (group != nullptr)
	{
		auto subgroupIterator = group->find(i);
		if (subgroupIterator == group->end()) //The subgroup does not exist
		{
			return nullptr;
		}
		else //The subgroup exists, return it
		{
			return subgroupIterator->second;
		}
	}
	return nullptr;
}

void Data::PrintSubgroup(char c, int i)
{
	//This lambda is for taking an Item object and printing it's relevant contents
	auto printLambda = [](const Item item) { std::cout << item.getName() + " " + item.getTimestamp().ToString() << std::endl; };

	//Find subgroup
	auto subgroup = GetSubgroup(c,i);
	if (subgroup != nullptr)
	{
		std::cout << i << ": " << std::endl; // print subgroup key
		std::for_each(subgroup->begin(), subgroup->end(), printLambda);
	}
	else
	{
		throw std::invalid_argument("Subgroup " + std::to_string(i) + " does not exist\n");
	}
	//Loop through all subgroups, printing them
}

int Data::CountSubgroupItems(char c, int i)
{
	auto subgroupSet = GetSubgroup(c, i);
	if (subgroupSet != nullptr) {
		return subgroupSet->size();
	}
	return 0;
}

std::optional<Item> Data::GetItem(char c, int i, std::string s)
{
	auto subgroup = GetSubgroup(c, i);
	if (subgroup != nullptr)
	{
		Item tempItem = Item(c, i, s, Date());
		for (const auto& item : *subgroup) {
		}
		auto itemIterator = subgroup->find(tempItem);
		if (itemIterator == subgroup->end()) //The Item does not exist
		{
			return std::nullopt;
		}
		else //The Item exists, return it
		{
			return *itemIterator;
		}
	}
	return std::nullopt;
}

void Data::PrintItem(char c, int i, std::string s)
{
	//Find item, then print it's timestamp
	std::optional<Item> item = GetItem(c, i, s);
	if (item != std::nullopt)
	{
		//Item from group Q subgroup 19 Mute Swan 23 Jun 2018
		std::cout << "Item from group " << item->getGroup() << " subgroup " << item->getSubgroup() << " " << item->getName() << " " << item->getTimestamp().ToString() << std::endl;
	}
	else
	{
		std::string errorString = "Item from group " + std::to_string(c) + " subgroup " + std::to_string(i) + " ID " + s + " does not exist\n";
		throw std::invalid_argument(errorString);
	}
}

std::optional<Item> Data::InsertItem(char c, int i, std::string s, std::optional<Date> d)
{
	//Check for empty string
	std::optional<Item> testItem = GetItem(c, i, s);
	if (!checkGroupInput(c) || !checkSubgroupInput(i) || !checkNameInput(s) || testItem != std::nullopt)
	{
		return std::nullopt;
	}

	//Create the new item with the given function parameters
	Item* newItem = new Item(c, i, s, d.value_or(Date::CreateRandomDate(Item::Begin, Item::End)));

	//First check if the group already exists
	auto groupIterator = DataStructure.find(c);

	if (groupIterator == DataStructure.end()) // The group doesn't exist
	{
		//Insert a new group using data from the newItem object
		InsertGroup(newItem->getGroup(), { newItem->getSubgroup() }, { {std::tuple<std::string, std::optional<Date>>(newItem->getName(), newItem->getTimestamp())} });
		return *newItem;
	}
	else //The group exists
	{
		//Check if the Subgroup exists for newItem
		auto currentGroup = groupIterator->second;
		auto subGroupIterator = currentGroup->find(i);

		if (subGroupIterator == currentGroup->end()) //Subgroup doesn't exist
		{
			//Create a new subgroup
			InsertSubgroup(newItem->getGroup(), newItem->getSubgroup(), { std::tuple<std::string, std::optional<Date>>(newItem->getName(), newItem->getTimestamp()) });
			return *newItem;
		}
		else //The subgroup exists
		{
			//Check if the exact same Item object exists
			auto currentSubgroup = subGroupIterator->second;
			auto itemIterator = subGroupIterator->second->find(*newItem);
			if (itemIterator == currentSubgroup->end()) // Item does not exist
			{
				currentSubgroup->insert(*newItem);
				return *newItem;
			}
			else //An exact match was found
			{
				return std::nullopt;
			}
		}
	}
}

std::set<Item>* Data::InsertSubgroup(char c, int i, std::initializer_list<std::tuple<std::string, std::optional<Date>>> items)
{
	//Check if the subgroup exists and if inputs are correct
	if (!checkGroupInput(c) || !checkSubgroupInput(i) || GetSubgroup(c, i))
	{
		return nullptr;
	}

	//Check the initializer list for duplicates and errors
	std::set<std::string> set;
	for (const auto& tuple : items) {
		if (std::get<0>(tuple) == "") //Empty string found
		{
			return nullptr;
		}
		auto insertionResult = set.insert(std::get<0>(tuple));
		if (!insertionResult.second) {
			return nullptr; // Duplicate found
		}
	}

	//Find the group that the subgroup is to be inserted into
	auto groupIterator = DataStructure.find(c);
	if (groupIterator == DataStructure.end()) // The Group itself does not exist
	{
		InsertGroup(c, { i }, { items });
		//Now that the group has been added, to return from this code find this new subgroup and return it
		groupIterator = DataStructure.find(c);
		auto subgroupIterator = groupIterator->second;
		auto subgroup = subgroupIterator->find(i)->second;
		return subgroup;
	}
	else //The Group exists, subgroup doesn't
	{
		auto currentGroupValue = groupIterator->second;
		//Create the std::set that is needed for a new subgroup:
		//????
		std::set<Item> *subGroupSet = new std::set<Item>();
		//Add a new subgroup to the map with key i and value subGroupSet
		currentGroupValue->insert({ i, subGroupSet });

		//Insert all the items into the newly emplaced empty subGroup
		for (std::tuple<std::string, std::optional<Date>> tuple : items)
		{
			InsertItem(c, i, std::get<0>(tuple), std::get<1>(tuple));
		}
		return DataStructure.find(c)->second->find(i)->second;
	}
}

std::map<int, std::set<Item>*>* Data::InsertGroup(char c, std::initializer_list<int> subgroups, std::initializer_list<std::initializer_list<std::tuple<std::string, std::optional<Date>>>> items)
{
	//Check if the group exists and if inputs are correct?
	if (GetGroup(c) || subgroups.size() != items.size() || !checkGroupInput(c))
	{
		return nullptr;
	}


	//Check the item initializer lists for duplicates and errors
	for (const auto& ilOuter : items) {
		std::set<std::string> set;
		for (const auto& tuple : ilOuter)
		{
			if (std::get<0>(tuple) == "") //Empty string found
			{
				return nullptr;
			}
			auto insertionResult = set.insert(std::get<0>(tuple));
			if (!insertionResult.second) {
				return nullptr; // Duplicate found
			}
		}
	}

	//Check subgroups initializer list for duplicates
	for (const auto& ilelement : subgroups)
	{
		std::set<int> set;
		if (!checkSubgroupInput(ilelement)) //Not an allowed subgroup number
		{
			return nullptr;
		}
		auto insertionResult = set.insert(ilelement);
		if (!insertionResult.second) {
			return nullptr; // Duplicate found
		}
	}

	//Create the new group where to fit all of the data
	std::map<int, std::set<Item>*>* newGroup = new std::map<int, std::set<Item>*>();

	//Insert this new empty group into the DataStructure
	DataStructure.emplace(c, newGroup);

	//Insert all subgroups into this new group by iterating through them at the same time
	auto subGroupIterator = subgroups.begin();
	auto itemilIterator = items.begin();
	for (subGroupIterator; subGroupIterator != subgroups.end(); subGroupIterator++, itemilIterator++)
	{
		InsertSubgroup(c, *subGroupIterator, *itemilIterator);
	}
	return DataStructure.find(c)->second;
	return nullptr;
}

bool Data::RemoveItem(char c, int i, std::string s)
{
	auto group = GetGroup(c);
	auto subgroup = GetSubgroup(c, i);
	auto item = GetItem(c, i, s);

	if (group != nullptr && subgroup != nullptr && item != std::nullopt) {
		//auto itemComparison = [&tempItem](const Item &x) { return (x.getName() == tempItem.getName() && x.getSubgroup() == tempItem.getSubgroup() && x.getGroup() == tempItem.getGroup());
		Item tempItem = Item(c, i, s, Date());
		for (auto iter = subgroup->begin(); iter != subgroup->end();)
		{
			if (iter->getName() == tempItem.getName() && iter->getSubgroup() == tempItem.getSubgroup() && iter->getGroup() == tempItem.getGroup())
			{
				subgroup->erase(iter++);
			}
			else
			{
				++iter;
			}
		}

		//If the subgroup is empty after removing the current item, remove the subgroup itself
		if (subgroup->size() == 0) {
			return RemoveSubgroup(c, i);
		}

		return true;
	}

	return false;
}

bool Data::RemoveSubgroup(char c, int i)
{
	auto group = GetGroup(c);
	auto subgroup = GetSubgroup(c, i);

	if (group && subgroup) {
		for (auto& item : *subgroup) {
			
		}
		delete subgroup;

		//Erase the subgroup in the current group
		group->erase(i);


		//If the subgroup is empty after removing the current item, remove the group itself
		if (group->size() == 0) {
			return RemoveGroup(c);
		}

		return true;
	}

	return false;
}

bool Data::RemoveGroup(char c)
{
	auto group = GetGroup(c);
	if (group != nullptr) {
		for (auto innerMapIterator = group->cbegin(); innerMapIterator != group->cend(); ++innerMapIterator) {
			auto set = innerMapIterator->second;
			set->clear();
			delete set;
		}
		delete group;
	}
	return DataStructure.erase(c);
}
