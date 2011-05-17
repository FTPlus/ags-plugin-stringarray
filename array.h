/*
  AGS Array plugin -- copyright (c) 2011 Ferry Timmers.
  All rights reserved.

  the AGS Array plugin is provided under the Artistic License 2.0
  http://www.opensource.org/licenses/artistic-license-2.0.php
*/

/*******************************************************
 * STL vector wrapper -- header file                   *
 *                                                     *
 * Author: Ferry Timmers                               *
 *                                                     *
 * Date: 14:06 16-5-2011                               *
 *                                                     *
 * Description: wrapper object for AGS                 *
 *******************************************************/

#ifndef _ARRAY_H
#define _ARRAY_H

#include <vector>
#include <string>

#include "agsplugin.h"

//------------------------------------------------------------------------------

// note: All datatypes in AGS are 32 bits (except maybe char).
// int32 is defined in agsplugin.h as long

struct Array
{
	int32 capacity;
	std::vector<std::string> data;
	
	// Excuse to use a member value
	void _c() { capacity = data.capacity(); }
	
	Array() : data() { _c(); }
	Array(Array *a) : data(a->data) { _c(); }
	Array(int32 num, const char *str) : data(num, str) { _c(); }
	
	void clear() { data.clear(); _c(); }
	
	int32 empty() const { return data.empty(); }
	
	void erase(int32 pos) { data.erase(data.begin() + pos); _c(); }
	
	void erase(int32 first, int32 last) { data.erase(data.begin() + first,
	                                                 data.begin() + last);
	                                      _c(); }
	
	void insert(int32 pos, const char *str) { data.insert(data.begin() + pos,
	                                                      str); _c(); }
	
	void insert(int32 pos, const Array *src) { data.insert(data.begin() + pos,
	                                                       src->data.begin(),
														   src->data.end());
	                                           _c(); }
	
	std::string &operator[](int32 pos) { return data[pos]; _c(); }
	
	std::string pop()
	{
		std::string str = *data.rbegin();
		data.pop_back();
		_c();
		return str;
	}
	
	void push(const char *str) { data.push_back(str); _c(); }
	
	int32 size() const { return data.size(); }
	
	void resize(int32 num) { data.resize(num); _c(); }
	
	void reserve(int32 num) { data.reserve(num); _c(); }
	
	// Excuse to use floating point values
	float avg_length()
	{
		int32 count = 0;
		std::vector<std::string>::iterator it;
		
		for (it = data.begin(); it != data.end(); ++it)
			count += it->length();
		
		return (float) ((double) count / (double) data.size());
	}
	
	// Excuse to use static
	static void swap(Array *a, Array *b)
	{
		a->data.swap(b->data);
		a->_c();
		b->_c();
	}
};

//------------------------------------------------------------------------------
// AGS interface instances

class ArrayInterface : public IAGSScriptManagedObject
{
	public:
	static const char *name;
	
	ArrayInterface() {};
		
	virtual int Dispose(const char *address, bool force);
	virtual const char* GetType() { return (name); }
	virtual int Serialize(const char *address, char *buffer, int bufsize);
	
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

class ArrayReader : public IAGSManagedObjectReader
{
	public:
	
	ArrayReader() {}
	
	virtual void Unserialize(int key, const char *serializedData, int dataSize);
	
};

//------------------------------------------------------------------------------

extern ArrayInterface Array_Interface;
extern ArrayReader Array_Reader;

//------------------------------------------------------------------------------

#endif /* _ARRAY_H */

//..............................................................................
