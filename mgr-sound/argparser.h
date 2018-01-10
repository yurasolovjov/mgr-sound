#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <boost/program_options.hpp>
#include <stdio.h>
#include <iostream>

class ArgParser
{
public:
	ArgParser(int argc, char** argv);
	~ArgParser();

	uint16_t getSizeShmem() { return _sizeShmem; };
	uint16_t getTimeout()	{  return _tout; };
	std::string getNameShmem() { return _nameShmem; };
	std::string getPathConfig() { return _path2config; };
	std::string getConfigName() { return _configName; };
	std::string getPathFmodBank() { return _path2fmodbank; };
	bool init(){ return _init; };
	bool debug() { return _debug; };

public:
	static const std::string NameShmem;
	static const std::string Path2Config;
	static const std::string ConfigName;
	static const std::string Path2FModBank;
	static const std::uint16_t SizeShmem;
	static const std::uint16_t Timeout;


private:
	uint16_t _sizeShmem;
	uint16_t _tout;
	std::string _nameShmem;
	std::string _path2config;
	std::string _configName;
	std::string _path2fmodbank;
	bool _init;
	bool _debug;
};

#endif 