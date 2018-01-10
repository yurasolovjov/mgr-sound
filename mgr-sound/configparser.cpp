#include "configparser.h"



ConfigParser::ConfigParser(std::string config, std::string table) {
	
	
	luaL_openlibs(L);

	if (luaL_loadfile(L,config.c_str() )){
		lua_close(L);
	}
		
	lua_pcall(L, 0, 0, 0);

	lua_getglobal(L, table.c_str());

	if (!lua_istable(L, -1)){
		std::string msg = "";
		msg += config + " " ;
		msg += table;
		msg += " should be table!";
		std::cerr << "*** CONFIG ERROR *** MSG: " << msg.c_str() << std::endl;
	}
}


ConfigParser::~ConfigParser(){
}

std::string ConfigParser::getString(std::string& p) {
	std::string result = "";
	
	lua_pushstring(L, p.c_str());
	lua_gettable(L, -2);

	if (lua_isstring(L, -1)) {
		result = (lua_tostring(L, -1));
		lua_pop(L, 1);
		return result;
	}
	else {
		std::cout << "*** CONFIG ERROR *** should be string" << std::endl;
		lua_pop(L, 1);
		return result = "*** CONFIG ERROR *** should be string";
	}
}

int ConfigParser::getInteger(std::string& p) {

	lua_pushstring(L, p.c_str());
	lua_gettable(L, -2);

	if (lua_isnumber(L, -1)) {
		int result = lua_tonumber(L, -1);
		lua_pop(L, 1);
		return result;
	}
	else {
		std::cout << "*** CONFIG ERROR *** should be number" << std::endl;
		lua_pop(L, 1);
		return -1;
	}
}

std::vector<std::string> ConfigParser::parseTableNotParam(std::string& p) {
	
	lua_pushstring(L, p.c_str());
	lua_gettable(L,-2);
	
	if (lua_istable(L, -1)) {
		for (int i = 1;; i++){
			std::string    result = "";
			lua_pushinteger(L, i);
			lua_gettable(L, -2);

			if (lua_isstring(L, -1)) {
				result = lua_tostring(L, -1);
				//std::cout << "tostring() "<<result.c_str() << std::endl;
				lua_pop(L, 1);
				_banks.push_back(result);
			}
			else break;
		}
		lua_pop(L,1);
	}
	lua_pop(L, 1);
	return _banks;
}

bool ConfigParser::parseTableWithParam(std::string& p) {
	
	this->_events.clear();

	lua_pushstring(L, p.c_str());
	lua_gettable(L, -2);
	
	if (lua_istable(L, -1)) {
		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
					
			std::string  even = "";
			std::string	 param = "";
			int  byteShmem = -1;
			int  driver = 0;

			lua_pushstring(L, "event");
			lua_gettable(L, -2);

			if (lua_isstring(L, -1)) {
				even = lua_tostring(L,-1);
			}
			else{
				std::cerr << " *** CONFIG ERROR *** Parameter event should be string" << std::endl;
				lua_pop(L, 1);
				return false;
			}
		
			lua_pop(L,1);

			lua_pushstring(L, "soundDriver");
			lua_gettable(L, -2);

			if (lua_isnumber(L, -1)) {
				driver = lua_tonumber(L, -1);
			}
			else {
				std::cerr << " *** CONFIG ERROR *** Parameter soundDriver should be string" << std::endl;
				lua_pop(L, 1);
				return false;
			}

			lua_pop(L, 1);
			
			lua_pushstring(L, "parameter");
			lua_gettable(L, -2);

			if (lua_isstring(L, -1)) {
				param = lua_tostring(L, -1);
			}
			else {
				std::cerr << " *** CONFIG ERROR *** Parameter should be string" << std::endl;
				lua_pop(L, 1);
				return false;
			}

			lua_pop(L, 1);

			lua_pushstring(L, "byteInShmem");
			lua_gettable(L, -2);

			if (lua_isnumber(L, -1)) {
				byteShmem = lua_tonumber(L, -1);
			}
			else {
				std::cerr << " *** CONFIG ERROR *** Parameter byteInShmem should be number" << std::endl;
				lua_pop(L, 1);
				return false;
			}

			if (!even.empty() && !param.empty() ) {
				std::map<std::string, int> shParam;
				std::map<std::string, int> evParam;
				shParam.insert(std::pair<std::string, int>(param, byteShmem));
				evParam.insert(std::pair<std::string, int>(even, driver));
				this->_events.insert(std::pair<std::map<std::string, int>, std::map<std::string, int>>(evParam, shParam));
			}
				
			
			lua_pop(L, 1);
			lua_pop(L, 1);
		}
	}
	
	return true;
}