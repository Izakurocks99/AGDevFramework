#pragma once

// includes the lua headers
#include <lua.hpp>
#include "Vector3.h"

class CLuaInterface
{
protected:
	static CLuaInterface *s_instance;
	CLuaInterface();

public:
	static CLuaInterface *GetInstance()
	{
		if (!s_instance)
			s_instance = new CLuaInterface;
		return s_instance;
	}
	static bool DropInstance()
	{
		if (s_instance)
		{
			// Drop the Lua Interface Class
			s_instance->Drop();

			delete s_instance;
			s_instance = NULL;
			return true;
		}
		return false;
	}
	virtual ~CLuaInterface();

	// Initialisation of the Lua Interface Class
	bool Init();

	// Run the Lua Interface Class
	void Run();

	// Drop the Lua Interface Class
	void Drop();

	// Get an integer value through the Lua Interface Class
	int getIntValue(const char* varName);
	// Get a float value through the Lua Interface Class
	float getFloatValue(const char* varName);

	// Save an integer value through the Lua Interface Class
	void saveIntValue(const char* varName, const int value, const bool bOverwrite = NULL);
	// Save a float value through the Lua Interface Class
	void saveFloatValue(const char* varName, const float value, const bool bOverwrite = NULL);
	// get char value through the lua interface class
	char getCharValue(const char* varName);
	// get Vector3 values through the Lua Interface Class
	Vector3 getVector3Values(const char* varName);
	//get distance square value through the lua interface class
	float getDistanceSquareValue(const char*varName, Vector3 source, Vector3 destination);
	//get variable number of values through the Lua Interface class
	int getVariableValues(const char*varName, int &a, int &b, int &c, int &d);
	//Extract a field form a table
	float GetField(const char*key);
	//Get error message using an error code
	void error(const char*errorCode);

	// Pointer to the Lua State
	lua_State *theLuaState;
	lua_State *theErrorState;

	// Key to move forward
	char keyFORWARD;
};

