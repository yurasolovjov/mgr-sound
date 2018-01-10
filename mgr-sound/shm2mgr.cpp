#include "shm2mgr.h"
#include <stdio.h>
#include <iostream>
#include <thread>
#include <signal.h>



Shm2Mgr::Shm2Mgr(std::string name, uint16_t size) {
	using namespace boost::interprocess;

	if (size > 0) {
		_buffer.resize(size);
		this->size = size;
		this->name = name;

		/*
		struct shm_remove {
			shm_remove() { shared_memory_object::remove("ShmMgrSound"); }
			~shm_remove() { shared_memory_object::remove("ShmMgrSound"); }
		} remover;
			*/

		try {
			std::shared_ptr<windows_shared_memory> tshm( new windows_shared_memory(create_only, name.c_str(), read_write, size*size));
			shm = tshm;
			
			mapped_region region(*shm, read_write);
			std::memset(region.get_address(), 0, region.get_size());
			this->_init = true;
		}
		catch(const interprocess_exception& ex){
			std::cout << " *** ERROR *** Shared memory not initalisation: " << ex.what() << std::endl;
			this->_init = false;
		}
	}	
}

Shm2Mgr::~Shm2Mgr() {
	
	if (this->_init == true) {
		shared_memory_object::remove(this->name.c_str());
	//	delete shm;
		std::cout << "Destroy class ~Shm2Mgr()" << std::endl;
	}
}


int32_t Shm2Mgr::read(uint32_t byte){
	
	uint32_t value = 0;
	mapped_region region(*shm, read_only);
	
	uint32_t *mem = static_cast<uint32_t*>(region.get_address());
	
	if (byte < 0) {
		return -1;
	}

	if (mem == NULL) {
		value = -1;
	}
	else {
		if (&mem[byte] != NULL) {
			value = mem[byte];
		}
		else value = -1;
	}
	return value;
}

bool Shm2Mgr::write(uint32_t value, uint8_t byte) {


	mapped_region region(*shm, read_write);

	uint32_t *mem = static_cast<uint32_t*>(region.get_address());
	
	if (byte < 0) {
		return false;			
	}

	if (mem != NULL) {
		if (&mem[byte] != NULL) {
			mem[byte] = value;
			return true;
		}
		else return false;
	}
	else {
		return false;
	}

	return true;
}

void Shm2Mgr::ReadData() {

	while (true) {

		for (uint8_t i = 0; i < size; i++) {

			int32_t value = this->read(i);

			if (value > -1) {
				_buffer[i] = (value);
			}
			else std::cout << "*** ERROR *** ReadData( ): value:  " <<std::hex<<value<< std::endl;
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}
}

bool Shm2Mgr::exec() {
	std::thread threadRead(&Shm2Mgr::ReadData,this);

	threadRead.detach();
	return true;
}
