#include <iostream>
#include <vector>
#include <fstream>
#include <functional>
#include <algorithm>
#include <list>
#include <iostream>
#include <cmath>
#include <wx/wx.h>
#include <wx/dcclient.h>
#include <wx/popupwin.h>
#ifndef COP4530_HASHTABLE_H
#define COP4530_HASHTABLE_H

using namespace std;

namespace cop4530 
{
	template <class T>
	class HashTable 
	{
		public:
			HashTable(size_t size = 101); /* Size of vector set to prime_below(size) */
			~HashTable(); // Destructor
			bool contains(const T &x) const;
			bool insert(const T &x);
			bool insert(const T &&x);
			bool remove(const T &x);
			void clear();
			bool load(const char *filename);
			void dump() const;
			string wxDump() const;
			bool write_to_file(const char *filename);
			int getSize() const;
			int getCapacity() const;
		private:
			vector<list<T>> theList;
			long unsigned int currentSize;
			static const unsigned int default_capacity = 11;
			static const unsigned int max_prime = 1301081;
			void makeEmpty();
			void rehash();
			size_t myhash(const T &x) const;
			unsigned long prime_below(long);
			void setPrimes(vector<long> &);
			int nextPrime(int num) const;


	};
	#include "hashtable.hpp"
}
#endif // COP4530_HASHTABLE_H
