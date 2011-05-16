/***********************************************************
 * AGS Array                                               *
 *                                                         *
 * Author: Ferry Timmers                                   *
 *                                                         *
 * Date: 16-05-11 13:48                                    *
 *                                                         *
 * Description: Test plugin, simply implements STL vector. *
 *                                                         *
 ***********************************************************/

#define MIN_EDITOR_VERSION 1
#define MIN_ENGINE_VERSION 18

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#if !defined(_WINDOWS_) && defined(_WIN32)
#define _WINDOWS_
#endif

#include "agsplugin.h"
#include "array.h"

//==============================================================================

// The standard Windows DLL entry point

BOOL APIENTRY DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved)
{
	switch(reason)
	{
		case DLL_PROCESS_ATTACH:
			break;
		
		case DLL_PROCESS_DETACH:
			break;
		
		case DLL_THREAD_ATTACH:
			break;
		
		case DLL_THREAD_DETACH:
			break;
	}
	
	return TRUE;
}

//==============================================================================

// ***** Plugin definition *****

IAGSEngine *engine; // Engine interface

//------------------------------------------------------------------------------

Array *ArrayCreate(int32 count, const char *str)
{
	Array *arr;
	
	if (count < 1)
		arr = new Array();
	else if (str == NULL)
		arr = new Array(count, "");
	else
		arr = new Array(count, str);
	
	engine->RegisterManagedObject(arr, &Array_Interface);
	
	return (arr);
}

//------------------------------------------------------------------------------

Array *ArrayCopy(Array *source)
{
	Array *arr;
	
	if (source == NULL)
		arr = new Array();
	else
		arr = new Array(source);
	
	engine->RegisterManagedObject(arr, &Array_Interface);
	
	return (arr);
}

//------------------------------------------------------------------------------

void ArraySwap(Array *a, Array *b)
{
	Array::swap(a, b);
}

//------------------------------------------------------------------------------

void ArrayClear(Array *arr)
{
	arr->clear();
}

//------------------------------------------------------------------------------

int32 ArrayEmpty(Array *arr)
{
	return arr->empty();
}

//------------------------------------------------------------------------------

void ArrayErase(Array *arr, int32 pos, int32 number)
{
	if (pos < 0)
		pos += arr->size();
	
	if (pos >= arr->size())
		pos = arr->size() - 1;
	
	if (number < 2)
		arr->erase(pos);
	else
	{
		number += pos;
		if (number < 0)
			number = 0;
		else if (number > arr->size())
			number = arr->size();
		
		arr->erase(pos, number);
	}
}

//------------------------------------------------------------------------------

void ArrayInsert(Array *arr, int32 pos, const char *str)
{
	if (pos < 0)
		pos += arr->size();
	
	if (pos > arr->size())
		pos = arr->size();
	
	if (str == NULL)
		arr->insert(pos, "");
	else
		arr->insert(pos, str);
}

//------------------------------------------------------------------------------

void ArrayInsertArray(Array *arr, int32 pos, Array *source)
{
	if (pos < 0)
		pos += arr->size();
	
	if (pos > arr->size())
		pos = arr->size();
	
	arr->insert(pos, source);
}

//------------------------------------------------------------------------------

const char *ArrayGetItems(Array *arr, int32 i)
{
	if ((i < 0) || (i > arr->size()))
		return (0);
	
	return engine->CreateScriptString((*arr)[i].c_str());
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

void ArraySetItems(Array *arr, int32 i, const char *str)
{
	if ((i < 0) || (i > arr->size()))
		return;
	
	(*arr)[i] = str;
}

//------------------------------------------------------------------------------

const char *ArrayPop(Array *arr)
{
	if (!arr->empty())
		return engine->CreateScriptString(arr->pop().c_str());
	else
		return (0);
}

//------------------------------------------------------------------------------

void ArrayPush(Array *arr, const char *str)
{
	arr->push(str == NULL ? "" : str);
}

//------------------------------------------------------------------------------

int32 ArrayGetSize(Array *arr)
{
	return arr->size();
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

void ArraySetSize(Array *arr, int32 size)
{
	arr->resize(size);
}

//------------------------------------------------------------------------------

void ArrayReserve(Array *arr, int32 number)
{
	arr->reserve(number);
}

//------------------------------------------------------------------------------

#define RETURN_AGS_FLOAT(x) return *((int32 *) &x)
// Note: I'm pretty sure this will break on some platforms

int32 ArrayAverageLength(Array *arr)
{
	float f = arr->avg_length();
	RETURN_AGS_FLOAT(f);
}

//==============================================================================

// ***** Design time *****

IAGSEditor *editor; // Editor interface

const char *ourScriptHeader =
  "managed struct Array {\r\n"
  "  readonly int Capacity;\r\n"
  "  /// Creates a new array.\r\n"
  "  import static Array *Create (int count = 0, String str = 0); // $AUTOCOMPLETESTATICONLY$\r\n"
  "  /// Copies an existing array.\r\n"
  "  import static Array *Copy (Array *source); // $AUTOCOMPLETESTATICONLY$\r\n"
  "  /// Swaps the values of two arrays.\r\n"
  "  import static void Swap (Array *a, Array *b); // $AUTOCOMPLETESTATICONLY$\r\n"
  "  /// Clears all values in the array.\r\n"
  "  import void Clear ();\r\n"
  "  /// Returns true when the array is empty.\r\n"
  "  import bool Empty ();\r\n"
  "  /// Removes specified value(s) from the array.\r\n"
  "  import void Erase (int pos, int number = 1);\r\n"
  "  /// Inserts a value at a specified place in the array.\r\n"
  "  import void Insert (int pos, String str);\r\n"
  "  /// Inserts all values of the given array a the specified place.\r\n"
  "  import void InsertArray (int pos, Array *source);\r\n"
  "  import attribute String Items[];\r\n"
  "  /// Removes the last item of the array and returns it.\r\n"
  "  import String Pop ();\r\n"
  "  /// Adds the specified value to the end of the array.\r\n"
  "  import void Push (String str);\r\n"
  "  import attribute int Size;\r\n"
  "  /// Increases the capacity of the array. (Note: you don't need to do this manually)\r\n"
  "  import void Reserve (int number);\r\n"
  "  /// Gets the average string length of the array values.\r\n"
  "  import float AverageLength ();\r\n"
  "};\r\n";

//------------------------------------------------------------------------------

int AGS_PluginV2()
{
	return (1);
}

//------------------------------------------------------------------------------

LPCSTR AGS_GetPluginName()
{
	return ("String array");
}

//------------------------------------------------------------------------------

int AGS_EditorStartup(IAGSEditor *lpEditor)
{
	// User has checked the plugin to use it in their game
	
	// If it's an earlier version than what we need, abort.
	if (lpEditor->version < MIN_EDITOR_VERSION)
		return (-1);
	
	editor = lpEditor;
	editor->RegisterScriptHeader(ourScriptHeader);
	
	// Return 0 to indicate success
	return (0);
}

//------------------------------------------------------------------------------

void AGS_EditorShutdown()
{
	// User has un-checked the plugin from their game
	editor->UnregisterScriptHeader(ourScriptHeader);
}

//------------------------------------------------------------------------------

void AGS_EditorProperties(HWND parent)                        //*** optional ***
{
	// User has chosen to view the Properties of the plugin
	// We could load up an options dialog or something here instead
	MessageBox(parent,
	           "String array, implementation of vector<string> mainly for testing purposes. by Wyz 2011",
	           "About",
			   MB_OK | MB_ICONINFORMATION);
}

//------------------------------------------------------------------------------

int AGS_EditorSaveGame(char *buffer, int bufsize)             //*** optional ***
{
	// Called by the editor when the current game is saved to disk.
	// Plugin configuration can be stored in [buffer] (max [bufsize] bytes)
	// Return the amount of bytes written in the buffer
	return (0);
}

//------------------------------------------------------------------------------

void AGS_EditorLoadGame(char *buffer, int bufsize)            //*** optional ***
{
	// Called by the editor when a game is loaded from disk
	// Previous written data can be read from [buffer] (size [bufsize]).
	// Make a copy of the data, the buffer is freed after this function call.
}

//==============================================================================

// ***** Run time *****

//------------------------------------------------------------------------------

#define REGISTER(x) engine->RegisterScriptFunction(#x, (void *) (x));
#define REG_CLASS(c,x,a) engine->RegisterScriptFunction(#c "::" #x "^" #a, (void *) (c ## x));
#define REG_ATTR(c,x) engine->RegisterScriptFunction(#c "::get_" #x, (void *) (c ## Get ## x)); \
                      engine->RegisterScriptFunction(#c "::set_" #x, (void *) (c ## Set ## x));
#define REG_ARR(c,x) engine->RegisterScriptFunction(#c "::geti_" #x, (void *) (c ## Get ## x)); \
                     engine->RegisterScriptFunction(#c "::seti_" #x, (void *) (c ## Set ## x));

#define STRINGIFY(s) STRINGIFY_X(s)
#define STRINGIFY_X(s) #s

void AGS_EngineStartup(IAGSEngine *lpEngine)
{
	engine = lpEngine;
	
	// Make sure it's got the version with the features we need
	if (engine->version < MIN_ENGINE_VERSION)
		engine->AbortGame("Plugin needs engine version " STRINGIFY(MIN_ENGINE_VERSION) " or newer.");
	
	// Register functions
	//engine->RegisterScriptFunction("", (void *) );
	
	engine->AddManagedObjectReader(ArrayInterface::name, &Array_Reader);
	REG_CLASS(Array, Create, 2)
	REG_CLASS(Array, Copy, 1)
	REG_CLASS(Array, Swap, 2)
	REG_CLASS(Array, Clear, 0)
	REG_CLASS(Array, Empty, 0)
	REG_CLASS(Array, Erase, 2)
	REG_CLASS(Array, Insert, 2)
	REG_CLASS(Array, InsertArray, 2)
	REG_ARR(Array, Items)
	REG_CLASS(Array, Pop, 0)
	REG_CLASS(Array, Push, 1)
	REG_ATTR(Array, Size)
	REG_CLASS(Array, Reserve, 1)
	REG_CLASS(Array, AverageLength, 0)
	
	// Request event hooks
	//engine->RequestEventHook(AGSE_);
}

//------------------------------------------------------------------------------

void AGS_EngineShutdown()
{
	// Called by the game engine just before it exits.
	// This gives you a chance to free any memory and do any cleanup
	// that you need to do before the engine shuts down.
}

//------------------------------------------------------------------------------
/*
int AGS_EngineOnEvent(int event, int data)                    //*** optional ***
{
	switch (event)
	{
		case AGSE_KEYPRESS:
		case AGSE_MOUSECLICK:
		case AGSE_POSTSCREENDRAW:
		case AGSE_PRESCREENDRAW:
		case AGSE_SAVEGAME:
		case AGSE_RESTOREGAME:
		case AGSE_PREGUIDRAW:
		case AGSE_LEAVEROOM:
		case AGSE_ENTERROOM:
		case AGSE_TRANSITIONIN:
		case AGSE_TRANSITIONOUT:
		case AGSE_FINALSCREENDRAW:
		case AGSE_TRANSLATETEXT:
		case AGSE_SCRIPTDEBUG:
		case AGSE_SPRITELOAD:
		case AGSE_PRERENDER:
		case AGSE_PRESAVEGAME:
		case AGSE_POSTRESTOREGAME:
		default:
			break;
	}
	
	// Return 1 to stop event from processing further (when needed)
	return (0);
}
*/
//------------------------------------------------------------------------------
/*
int AGS_EngineDebugHook(const char *scriptName,
                        int lineNum, int reserved)            //*** optional ***
{
	// Can be used to debug scripts, see documentation
}
*/
//------------------------------------------------------------------------------
/*
void AGS_EngineInitGfx(const char *driverID, void *data)      //*** optional ***
{
	// This allows you to make changes to how the graphics driver starts up.
	// See documentation
}
*/
//..............................................................................

/*
Copyright (c) 2011 Ferry Timmers

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
*/
