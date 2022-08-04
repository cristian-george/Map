#include "Map.h"

#pragma region First example
void Example1()
{
	Map<double, int> myMap;

	myMap[7.1] = 0;
	myMap[9.2] = 1;
	myMap[15.6] = 2;
	myMap[12.5] = 3;
	myMap[24.3] = 4;
	myMap[23.8] = 5;
	myMap[7.1]++;

	for (auto it = myMap.begin(); it != myMap.end(); ++it)
		std::cout << it->first << ' ' << it->second << std::endl;

	myMap.print();

	myMap.erase(15.6);

	for (auto [key, value] : myMap)
		std::cout << key << ' ' << value << std::endl;

	myMap.print();
}
#pragma endregion

#pragma region Second example
void Example2()
{
	Map<int, int, std::greater<int>> myMap;

	myMap[0] = 6;
	myMap[8] = 9;
	myMap[4] = 5;
	myMap[9] = 8;
	myMap[2] = 12;
	myMap[3] = 9;

	Map<int, int, std::greater<int>> myMap2(myMap);

	myMap2.insert(std::make_pair(97, 12));
	myMap2.insert(std::make_pair(65, 67));
	myMap2.insert(std::make_pair(99, 19));
	myMap2.insert(std::make_pair(7, 43));

	myMap = myMap2;

	myMap2.erase(0);
	myMap2.erase(4);
	myMap2.erase(99);
	myMap2.erase(8);

	myMap[8] = 9;
	myMap[24] = 5;
	myMap[88] = 56;
	myMap[98] = 21;

	std::cout << "myMap: \n";
	for (auto it = myMap.begin(); it != myMap.end(); it++)
		std::cout << it->first << ' ' << it->second << std::endl;
	std::cout << std::endl;

	std::cout << "myMap2: \n";
	for (auto it = myMap2.begin(); it != myMap2.end(); ++it)
		std::cout << it->first << ' ' << it->second << std::endl;
	std::cout << std::endl;

	auto it = myMap2.find(9);
	std::cout << (it--)->first << ' ' << (++it)->second << std::endl;
	std::cout << it->first << ' ' << it->second << std::endl;
}
#pragma endregion

#pragma region Third example
void Example3()
{
	Map<char, int, std::less<char>> map;
	map['e'] = 0;
	map['c'] = 1;
	map['a'] = 2;
	map['d'] = 3;
	map['b'] = 4;

	std::cout << map['f'] << std::endl;

	for (auto& it : map)
		std::cout << it.first << ' ' << it.second << std::endl;
}
#pragma endregion

#pragma region Fourth example
void Example4()
{
	Map<std::string, int> map;
	map["Christian"] = 21;
	map["John"] = 24;
	map["Mary"] = 28;
	map["Andy"] = 34;
	map["Simon"] = 17;

	std::cout << map["Andy"] << std::endl;

	for (auto& it : map)
		std::cout << it.first << ' ' << it.second << std::endl;
}
#pragma endregion

#pragma region Fifth example
class myComparator
{
public:
	bool operator()(const std::string& lhs, const std::string& rhs)
	{
		return getCharNumber(lhs, 'a') < getCharNumber(rhs, 'a');
	}

private:
	int getCharNumber(const std::string& str, char character)
	{
		int no = 0;

		for (auto c : str)
			if (isalpha(c) && tolower(c) == tolower(character))
				++no;

		return no;
	}
};

void Example5()
{
	Map<std::string, int, myComparator> myMap;

	myMap.insert({ "Alexa", 0 });
	myMap.insert({ "John", 1 });
	myMap.insert({ "Selena", 2 });
	myMap.insert({ "Anna", 3 });

	myMap.print();

	for (auto it = myMap.begin(); it != myMap.end(); ++it)
		std::cout << it->first << ' ' << it->second << std::endl;
}
#pragma endregion


#include <functional>
#include <vector>

int main()
{
	std::vector<std::function<void()>> examples
	{
		Example1,
		Example2,
		Example3,
		Example4,
		Example5
	};

	for (size_t index = 0; index < examples.size(); ++index)
	{
		std::cout << "-------------------- Example " << index + 1 << " --------------------";
		std::cout << std::endl;

		examples[index]();
		std::cout << std::endl;
	}
	return 0;
}