#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <lua.hpp>
#include <lualib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>

class ConfigParser{

public:
	ConfigParser(std::string config, std::string table);
	~ConfigParser();

	std::string getString(std::string& p);
	int getInteger(std::string& p);
	std::vector<std::string> parseTableNotParam(std::string& p);
	bool parseTableWithParam(std::string& p);
	//std::map<std::string, std::map<std::string, int>> getEvents() { return _events;};
	std::map<std::map<std::string, int>, std::map<std::string, int>> getEvents() { return _events;};
	uint16_t getSizeEvents() { return _events.size(); };

private:
	lua_State *L = luaL_newstate();
	std::vector<std::string> _banks;
	//std::map<std::string, std::map<std::string, int> > _events;
	std::map<std::map<std::string, int>, std::map<std::string, int> > _events;
	int _soundDriver;
};

#endif
