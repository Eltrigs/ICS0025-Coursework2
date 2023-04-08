#include "Main.h"

extern default_random_engine Generator;
extern uniform_int_distribution<int> GroupDistribution;
extern uniform_int_distribution<int> SubgroupDistribution;
const Date Item::Begin = Date(1, 1, 2018), Item::End = Date(31, 12, 2018);

Item::Item() 
{  // creates a pseudo-random item
	Group = (char)GroupDistribution(Generator);
	Subgroup = SubgroupDistribution(Generator);
	Name = ::CreateRandomName();
	Timestamp = Date::CreateRandomDate(Begin, End);
}
Item::Item(char c, int i, std::string s, Date d)
{
	this->Group = c;
	this->Subgroup = i;
	this->Name = s;
	this->Timestamp = d;
}
Item::Item(const Item& original)
{
	Group = original.Group;
	Subgroup = original.Subgroup;
	Name = original.Name;
	Timestamp = original.Timestamp;
}
Item::~Item()
{
	
}
bool Item::operator==(const Item& other) const
{
	if (this->Group == other.Group
		&& this->Subgroup == other.Subgroup
		&& this->Name == other.Name)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool Item::operator<(const Item& other) const
{
	if (this->Group < other.Group)
	{
		return true;
	}
	if (this->Subgroup < other.Subgroup)
	{
		return true;
	}
	return this->Name < other.Name;
}
char Item::getGroup() const
{
	return this->Group;
}
int Item::getSubgroup() const
{
	return this->Subgroup;
}
std::string Item::getName() const
{
	return this->Name;
}
Date Item::getTimestamp() const
{
	return this->Timestamp;
}
