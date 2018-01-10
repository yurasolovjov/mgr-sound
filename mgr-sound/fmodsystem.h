#ifndef FMODSYSTEM_H
#define FMODSYSTEM_H
#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "fmod_errors.h"
#include "common.h"
#include <stdio.h>
#include <iostream>
#include <dos.h>
#include <thread>
#include <mutex>

class FmodSystem
{
public:
	FmodSystem();
	FmodSystem(std::string path, float startValueParameter = 600.0, int driver = 0, bool info = false);
	FmodSystem(const FmodSystem& fs);

	~FmodSystem();

	bool exec();
	bool set3DPositionX(float pos);
	bool set3DPositionY(float pos);
	bool set3DPositionZ(float pos);
	bool set3DPositionXYZ(float x, float y, float z);
	bool set3DAttributes(FMOD_3D_ATTRIBUTES* attributes);
	bool setListenerAttributes(int listener, FMOD_3D_ATTRIBUTES* attributes);
	bool setParameterValue(std::string& s, float value = -1);
	bool load(std::string& bank);
	bool loadEvent(std::string& event );
	bool start();
	bool init() { return _init; };
	bool update() { system->update(); };
	bool defaultSettings();
	void setValue(float& v);
	void setParameterName(std::string s) { this->_parameterName = s; };
	std::string getParameterName() { return _parameterName;};
	float getValue() { return _value; };
	void setByteShmem(int& b) { _byteShmem = b; };
	int  getByteShmem() { return _byteShmem; };
	std::string getEventName() { return _eventName; };


	//FmodSystem& operator=(FmodSystem& fs);
	

private:
	void playSound();

public:
		FMOD::Studio::System* system = NULL;
		FMOD::System* lowLevelSystem = NULL;
		FMOD::Channel* channel = NULL;
		FMOD::Studio::EventDescription* eventDescription = NULL;
		FMOD::Studio::EventInstance* eventInstance = NULL;
		FMOD_3D_ATTRIBUTES attributes = { { 0 } };
		std::string _pathToStore;
		bool _init;
		float _startValueParameter;
		float _value;
		std::mutex _mutex;
		std::string _eventName;
		std::string _parameterName;
		int _driver;
		int _byteShmem;
};

#endif 


/*
	Пример работы с классом FmodSystem;

	std::string mbank = "Master Bank.bank";
	std::string msbank = "Master Bank.strings.bank";
	std::string vbank = "Vehicles.bank";
	std::string sEvent = "event:/Vehicles/Basic Engine";
	std::string param = "RPM";
	FmodSystem fsystem("../media/", 600);
	
	fsystem.load(mbank);
	fsystem.load(msbank);
	fsystem.load(vbank);
	fsystem.loadEvent(sEvent);

	fsystem.setParameterValue(param);
	fsystem.setParameterName(param);

	fsystem.attributes.forward.z = 1.0f;
	fsystem.attributes.up.y = 1.0f;
	fsystem.setListenerAttributes(0, &fsystem.attributes);
	fsystem.attributes.position.z = 2.0f;
	fsystem.set3DAttributes(&fsystem.attributes);
	fsystem.start();

	fsystem.exec();

*/