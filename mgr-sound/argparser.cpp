#include "argparser.h"
#include "fmodsystem.h"


/* Значения по умолчанию*/
const std::string ArgParser::NameShmem="ShmMgrSound";
const std::string ArgParser::Path2Config="../config/config_fmod.lua";
const std::string ArgParser::ConfigName="vehicle";
const std::string ArgParser::Path2FModBank = "../media/";
const std::uint16_t ArgParser::SizeShmem = 16;
const std::uint16_t ArgParser::Timeout = 25;

namespace po = boost::program_options;

/** Класс разбора командной строки */
ArgParser::ArgParser(int argc, char** argv) {
	po::options_description desc("Program [mgr-sound]: Developer by Solovjov Yura");

	desc.add_options()
		("help,h", "help")
		("shmem", po::value<std::string>(), "Shared memory name, <string>")
		("size_shmem", po::value<int>(), "Size shared memory, <int>")
		("timeout", po::value<int>(), "Circle working mgr-sound, <int>")
		("path_config", po::value<std::string>(), "Path to config file(lua), <std::string>")
		("config_name", po::value<std::string>(), "Config name, <std::string>")
		("path_fmod_bank", po::value<std::string>(), "Path to catalog store fmod files, <std::string>")
		("debug", "Debug option")
		("driver_info", "Information about sound driver")
		;

	po::variables_map vm;

	try {
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);
		_init = true;
	}
	catch (po::error& e) {
		std::cerr << "*** ERROR *** Class ArgParser not intialisation" << std::endl;
		_init = false;
	}

	if (vm.count("help") || (argc == 1) ) {
		std::cout << desc << std::endl;
		this->_init = false;
		goto ReturnClass;
	}

	if (vm.count("driver_info")) {
		FmodSystem driverInfo("",0,0,true);
		this->_init = false;
		goto ReturnClass;
	}

	if (vm.count("debug")) {
		this->_debug = true;
	}
	else this->_debug = false;

	if (vm.count("shmem")) {
		this->_nameShmem = vm["shmem"].as<std::string>();
	}
	else {
		this->_nameShmem = ArgParser::NameShmem;
		std::cerr << " *** WARNING *** Not found param --shmem. Parameter has been set default value:"<< ArgParser::NameShmem<< std::endl;
	}

	if (vm.count("path_config")) {
	
		this->_path2config = vm["path_config"].as<std::string>();
	}
	else {
		this->_path2config = ArgParser::Path2Config;
		std::cerr << " *** WARNING *** Not found param --path_config. Parameter has been set default value:" << ArgParser::Path2Config << std::endl;
	}

	if (vm.count("config_name")) {
		this->_configName = vm["config_name"].as<std::string>();
	}
	else {
		this->_configName = ArgParser::ConfigName;
		std::cerr << " *** WARNING *** Not found param --config_name. Parameter has been set default value:" << ArgParser::ConfigName << std::endl;
	}



	if (vm.count("path_fmod_bank")) {
		this->_path2fmodbank = vm["path_fmod_bank"].as<std::string>();
	}
	else {
		this->_path2fmodbank = ArgParser::Path2FModBank;
		std::cerr << " *** WARNING *** Not found param --path_fmod_bank. Parameter has been set default value:" << ArgParser::Path2FModBank << std::endl;
	}


	if (vm.count("size_shmem")) {
		this->_sizeShmem = vm["size_shmem"].as<int>();
	}
	else {
		this->_sizeShmem = ArgParser::SizeShmem;
		std::cerr << " *** WARNING *** Not found param --size_shmem. Parameter has been set default value:" << ArgParser::SizeShmem << std::endl;
	}
	
	if (vm.count("timeout")) {
		this->_tout = vm["timeout"].as<int>();
	}
	else {
		this->_tout = ArgParser::Timeout;
		std::cerr << " *** WARNING *** Not found param --timeout. Parameter has been set default value:" << ArgParser::Timeout << std::endl;
	}

ReturnClass: return;
}



ArgParser::~ArgParser(){
	this->_init = false;
}
