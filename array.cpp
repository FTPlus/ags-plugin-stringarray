/***************************************************************
 * STL vector wrapper -- See header file for more information. *
 ***************************************************************/

#include "array.h"

//------------------------------------------------------------------------------

extern IAGSEngine *engine;

ArrayInterface Array_Interface;
ArrayReader Array_Reader;

const char *ArrayInterface::name = "Array";

//------------------------------------------------------------------------------

int ArrayInterface::Dispose(const char *address, bool force)
{
	delete ((Array *) address);
	return (1);
}

//------------------------------------------------------------------------------

int ArrayInterface::Serialize(const char *address, char *buffer, int bufsize)
{
	Array *arr = (Array *) address;
	char *ptr = buffer;
	
	std::vector<std::string>::iterator it;
	for (it = arr->data.begin(); it != arr->data.end(); ++it)
	{
		size_t length = it->length() + 1;
		
		if (ptr - buffer + length > bufsize)
			break;
		
		memcpy(ptr, it->c_str(), length);
		ptr += length;
	}
	
	return (ptr - buffer);
}

//------------------------------------------------------------------------------

void ArrayReader::Unserialize(int key, const char *serializedData, int dataSize)
{
	Array *arr = new Array();
	
	const char *ptr = serializedData;
	while (ptr - serializedData < dataSize)
	{
		arr->push(ptr);
		ptr += strlen(ptr) + 1;
	}
	
	engine->RegisterUnserializedObject(key, arr, &Array_Interface);
}

//..............................................................................
