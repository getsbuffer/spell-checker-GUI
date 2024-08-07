#include <iostream>
#include <vector>
#include <fstream>
#include <functional>
#include <algorithm>
#include <list>
#include <cmath>
#include <wx/wx.h>
#include <wx/dcclient.h>
#include <wx/popupwin.h>
#include "hashtable.h"

#ifndef COP4530_HASHTABLE_HPP
#define COP4530_HASHTABLE_HPP

using namespace std;

template<typename T>
cop4530::HashTable<T>::HashTable(size_t size) 
{
	currentSize = 0;
	theList.resize(prime_below(size));
}

template<typename T>
cop4530::HashTable<T>::~HashTable() {}

template<typename T>
bool cop4530::HashTable<T>::contains(const T &x) const
{
	auto & whichList = theList[myhash(x)];
	return find(begin(whichList), end(whichList), x) != end(whichList);
}
template<typename T>
bool cop4530::HashTable<T>::insert(const T &x) 
{
	auto & whichList = theList[myhash(x)];
	if (find(begin(whichList), end(whichList), x) != end(whichList))
		return false;
	
	whichList.push_back(x);

	//rehash
	if (++currentSize > theList.size())
		rehash();
	return true;
}
template<typename T>
bool cop4530::HashTable<T>::insert(const T &&x) 
{
	auto & whichList = theList[myhash(x)];
	if (find(begin(whichList), end(whichList), x) != end(whichList))
		return false;
	
	whichList.push_back(move(x));

	//rehash
	if (++currentSize > theList.size())
		rehash();
	return true;

}
template<typename T>
bool cop4530::HashTable<T>::remove(const T &x) 
{
	auto & whichList = theList[myhash(x)];
	auto itr = find(begin(whichList), end(whichList), x);
	if (itr == end(whichList))
		return false;

	whichList.erase(itr);
	--currentSize;
	return true;
}
template<typename T>
void cop4530::HashTable<T>::clear() 
{
	makeEmpty();
	currentSize = 0;
}
template<typename T>
bool cop4530::HashTable<T>::load(const char* filename) 
{
	T token;
	ifstream input(filename);
	if (!input.is_open()) 
		return false;

	while (getline(input, token)) 
		insert(token);

	input.close();
	return true;
	

}
template<typename T>
void cop4530::HashTable<T>::dump() const
{
	int i = 0;
	for (auto & x : theList) 
	{
		cout << "v[" << i++ << "] ";
		for (auto & y : x) 
			cout << y << " ";
		cout << "\n";
	}
}
template<typename T>
bool cop4530::HashTable<T>::write_to_file(const char* filename) 
{
	ofstream output(filename);
	if (!output.is_open()) 
		return false;
	
	for (auto & x : theList) 
	{
		for (auto & y : x)
			output << y << "\n";
	}
	output.close();
	return true;
}
template<typename T>
void cop4530::HashTable<T>::makeEmpty() 
{
	for (auto & x : theList)
		x.clear();
}
template<typename T>
void cop4530::HashTable<T>::rehash() 
{
	vector<list<T>> oldList = theList;

	// Create new double-sized, empty table
	theList.resize(nextPrime(2 * theList.size()));
	clear();

	// Copy table over
	currentSize = 0;
	for (auto & x : oldList) 
		for (auto & y : x)
			insert(move(y));
}
template<typename T>
size_t cop4530::HashTable<T>::myhash(const T &x) const
{
	static hash<T> hf;
	return hf(x) % theList.size();
}
template<typename T>
unsigned long cop4530::HashTable<T>::prime_below(long n) 
{
	if (n > max_prime)
	{
		cerr << "** input too large for prime_below()\n";
		return 0;
	}	
	if (n == max_prime) 
		return max_prime;
	if (n <= 1) 
	{
		cerr << "** input too small\n";
	}
	vector<long> v(n+1);
	setPrimes(v);
	while (n > 2) 
	{
		if (v[n] == 1)
			return n;
		--n;
	}
	return 2;
}
template<typename T>
void cop4530::HashTable<T>::setPrimes(vector<long> &vprimes)
{
	int i = 0, j = 0;
	vprimes[0] = 0;
	vprimes[1] = 0;
	int n = vprimes.capacity();
	for (i = 2; i < n; i++)
		vprimes[i] = 1;
	for (i = 2; i * i < n; i++) 
	{
		if (vprimes[i] == 1)
			for (j = i + i; j < n; j+= i)
				vprimes[j] = 0;
	}
}
template<typename T>
int cop4530::HashTable<T>::getSize() const
{
	return currentSize;
}
template<typename T>
int cop4530::HashTable<T>::getCapacity() const
{
	return theList.size();
}
template <typename T>
int cop4530::HashTable<T>::nextPrime(int num) const
{
	if (num > 1) 
	{
		bool prime = false;
		while(!prime) 
		{
			prime = true;
			int sqrtNum = sqrt(num);
			for (int i = 2; i <= sqrtNum && prime; i++) 
				prime = (num % i != 0);
			if (prime)
				return num;
			num++;
		}
	}
	return 2;
}

template <typename T>
string cop4530::HashTable<T>::wxDump() const 
{
	string result = "";
	int i = 0;
	for (auto & x : theList) 
	{
		result += "v[";
		result += to_string(i++);
		result += "] ";
		for (auto & y : x)  
		{
			result += y;
			result += " ";
		}
		result += "\n";
	}
	return result;
}
#endif // COP4530_HASHTABLE_HPP
