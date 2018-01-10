/*==============================================================================
Developer: Solovjov Yura
==============================================================================*/
#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "common.h"
#include "mgr-sound.h"
#include "shm2mgr.h"
#include "fmodsystem.h"
#include "argparser.h"
#include "configparser.h"
#include <stdio.h>
#include <iostream>
#include <dos.h>
#include <thread>
#include <boost\thread\thread.hpp>
#include <boost\interprocess\shared_memory_object.hpp>
#include <boost\interprocess\mapped_region.hpp>
#include <signal.h>


Shm2Mgr* destroyShm = NULL;

void signalHandler(int signo) {

	std::cout << "Signal Handler get signal: " << std::endl;

	switch( signo ) {
		case SIGABRT:
			std::cout << "SIGABRT" << std::endl;
		break;
		
		case SIGTERM:
			std::cout << "SIGTERM" << std::endl;
		break;
		
		case SIGINT:
			std::cout << "SIGINT" << std::endl;
		break;
		
	};

	if (destroyShm != NULL) {
		destroyShm->~Shm2Mgr();
		destroyShm = NULL;
	}
}

int main(int argc, char** argv)
{

	


	/* Разбор параметров командной строки*/
	ArgParser args(argc, argv);
	
	/* Проверка передачи параметров через командную строку*/
	if (!args.init()) return 0;

	/* Получение значений командной строки */
	std::string path2config  = args.getPathConfig();
	std::string table		 = args.getConfigName();
	std::string pathFModBank = args.getPathFmodBank();
	std::string nameShmem	 = args.getNameShmem();
	uint16_t	sizeShmem	 = args.getSizeShmem();
	uint16_t	timeout		 = args.getTimeout();
	bool		dbg			 = args.debug();

	if (dbg) {
		std::cout << " *** DEBUG *** Path to configuration file(lua): " << path2config.c_str() << std::endl;
		std::cout << " *** DEBUG *** Table lua in configuration file: " << table.c_str() << std::endl;
		std::cout << " *** DEBUG *** Path to FMOD banks: " << pathFModBank.c_str() << std::endl;
		std::cout << " *** DEBUG *** Name shared memory: " << nameShmem.c_str() << std::endl;
		std::cout << " *** DEBUG *** Size shared memory: " << sizeShmem << std::endl;
		std::cout << " *** DEBUG *** Timeout: " << timeout << std::endl;
	}
	
	/*Инициализация разделяемой памяти */
	Shm2Mgr sharedMemory(nameShmem,sizeShmem);

	if (sharedMemory.init()) {
		if (dbg) {
			std::cout << " *** DEBUG *** Shared memory has been initialisation " << std::endl;
		}
		sharedMemory.exec();
		
		destroyShm = &sharedMemory;

		signal(SIGABRT, signalHandler);
		signal(SIGTERM, signalHandler);
	//	signal(SIGINT,  signalHandler);
	}
	else return 0;

	/* Вектор музыкальных каналов */
	std::vector<FmodSystem> fsystems;		

	/* Разбор конфигурационного файла*/
	ConfigParser cfg(path2config, table);

	std::string table1 = "banks";
	std::string table2 = "events";
	std::string sDriver = "soundDriver";
	std::map<std::map<std::string, int>, std::map<std::string, int>> events;
	std::vector<std::string> banks;
	
	
	/* Получение банк-файлов из конфигурационного файла*/
	banks = cfg.parseTableNotParam(table1);
	
	/* Получение событий и параметров из конфигурационного файла*/
	cfg.parseTableWithParam(table2);
	
	/* Созданий музыкальных каналов класса fsystem */
	
	if (cfg.getSizeEvents() > 0) {
		events = cfg.getEvents();

		/* Инициализация музыкальных каналов класса FmodSystem */
		for (auto it = events.begin(); it != events.end(); ++it) {
			
			/* Получение табличны данных Event и Driver из конфигурационного файла Lua */
			for (auto itEv = it->first.begin(); itEv != it->first.end(); ++itEv){
				std::string even = itEv->first;
				int driver = itEv->second;

				/** Инициализация класса FmodSystem */
				FmodSystem fs(pathFModBank, 0, driver);
				
				for (auto itbank = banks.begin(); itbank != banks.end(); ++itbank) {
					fs.load(*itbank);
				}

				fs.loadEvent(even);
				
				/* Получение табличный данных о обрабатывемом параметре( содержится в Event) и байте-источнике из РП */
				for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {

					if (dbg) {
						std::cout << " *** DEBUG *** Event: " << even.c_str() << " Parameter: " << it2->first << " ByteShmem: " << it2->second << std::endl;
					}

					std::string param = it2->first;
					int byte = it2->second;

					fs.setParameterName(param);
					fs.setByteShmem(byte);
				}
				
				/* Добавление FmodSystem в обрабатывеммый вектор */
				fsystems.push_back(fs);
			}
		}
	}
	
	/* Запуск звуковых каналов */
	for (auto i = fsystems.begin(); i != fsystems.end(); ++i){
		float v = 0.0;
		i->setValue(v);
		i->start();
		i->exec();
	}

	/** Главный рабочий цикл программы */
	while (1) {
		Sleep(timeout);
		std::vector<uint32_t> buffer = sharedMemory.get();

		for (auto it = fsystems.begin(); it != fsystems.end(); ++it) {
			float value = buffer[it->getByteShmem()];
			it->setValue(value);

			if (dbg) {
				std::cout << " *** DEBUG *** Event: "<<it->getEventName()<<" Parameter event: " << it->getParameterName() << " set value: "<<value<<std::endl;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	};

	return 0;
}

