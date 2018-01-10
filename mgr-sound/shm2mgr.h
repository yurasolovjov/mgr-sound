#ifndef SHM2MGR_H
#define SHM2MGR_H
#include <boost\interprocess\windows_shared_memory.hpp>
#include <boost\interprocess\shared_memory_object.hpp>
#include <boost\interprocess\mapped_region.hpp>
#include <cstring>
#include <cstdlib>
#include <string>
#include <windows.h>
#include <signal.h>

using namespace boost::interprocess;

class Shm2Mgr {
	public: 
		Shm2Mgr(std::string name, uint16_t size);
		~Shm2Mgr();
		int32_t read(uint32_t byte);
		bool write(uint32_t value, uint8_t byte);
		bool exec();
		std::vector<uint32_t> get() { return _buffer; };
		bool init() { return _init; };
	private:
		void ReadData();

	public:
	
		std::shared_ptr<windows_shared_memory> shm;
		std::vector<uint32_t> _buffer;
		std::string name;
		uint16_t size;
		bool _init;

		
};
#endif // SHM2MGR_H


