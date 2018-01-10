#include "fmodsystem.h"

FmodSystem::FmodSystem() {
}

FmodSystem::FmodSystem(std::string path /*../media/*/, float startValueParameter /*=600*/, int driver, bool info) {

	FMOD_RESULT result;
	if (!path.empty()) {
		this->_pathToStore = path;
	}
	else this->_pathToStore = "../media/";

	void *extraDriverData = NULL;
//	Common_Init(&extraDriverData);

	if (driver > 0) {
		this->_driver = driver;
	}

	result = FMOD::System_Create(&lowLevelSystem);

	if (result != FMOD_OK) {
		std::cerr << "FMOD Can`t create low level system " << FMOD_ErrorString(result) << std::endl;
	}


	if (info) {
		int numDrivers = -2;

		lowLevelSystem->getNumDrivers(&numDrivers);
		
		std::cout << "==========================================================================================" << std::endl;
		for (int i = 0; i < numDrivers; i++) {
			char buf[300] = { 0 };
			lowLevelSystem->getDriverInfo(i, buf, sizeof(buf), NULL, NULL, NULL, NULL);
			printf("ID Driver:[ %d ]: %s \n", i, buf);
			memset(buf, 0, sizeof(buf));
		}
		std::cout << "==========================================================================================" << std::endl;
	
	}
	
	result = lowLevelSystem->setOutput(FMOD_OUTPUTTYPE_DSOUND);

	if (result != FMOD_OK) {
		std::cerr << "FMOD Can`t set output " << FMOD_ErrorString(result) << std::endl;
	}


	result = lowLevelSystem->init(1024, FMOD_INIT_NORMAL, extraDriverData);

	if (result != FMOD_OK) {
		std::cerr << "FMOD Can`t init low level system " << FMOD_ErrorString(result) << std::endl;
	}
	
	
	lowLevelSystem->update();

	FMOD::Studio::System::create(&this->system);

	system->getLowLevelSystem(&lowLevelSystem);

	this->lowLevelSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0);
	
	result = lowLevelSystem->setDriver(_driver);

	if (result != FMOD_OK) {
		std::cerr << "FMOD Can`t set driver " << FMOD_ErrorString(result) << std::endl;
	}



	result = system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData);

	if (result != FMOD_OK) {
		std::cerr << " *** ERROR *** Class system not initialisation. " << FMOD_ErrorString(result) << std::endl;
		this->_init = false;
	}
	else {
		this->_init = false;
	}

	this->setValue(startValueParameter);

	system->update();

}


FmodSystem::~FmodSystem()
{
}

bool FmodSystem::exec() {
	std::thread threadSound(&FmodSystem::playSound, this);
	threadSound.detach();

	return true;
}

void FmodSystem::playSound() {
	do
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	//	std::cout << this->_parameterName.c_str() << " : " << std::hex << this->getValue() << std::endl;
		setParameterValue(_parameterName, getValue());
		
		if (Common_BtnPress(BTN_LEFT))
		{
			attributes.position.x -= 1.0f;
			ERRCHECK(eventInstance->set3DAttributes(&attributes));
		}

		if (Common_BtnPress(BTN_RIGHT))
		{
			attributes.position.x += 1.0f;
			ERRCHECK(eventInstance->set3DAttributes(&attributes));
		}

		if (Common_BtnPress(BTN_UP))
		{
			attributes.position.z += 1.0f;
			ERRCHECK(eventInstance->set3DAttributes(&attributes));
		}

		if (Common_BtnPress(BTN_DOWN))
		{
			attributes.position.z -= 1.0f;
			ERRCHECK(eventInstance->set3DAttributes(&attributes));
		}

		ERRCHECK(system->update());

		//updateScreenPosition(attributes.position);


	} while (true);
}
bool FmodSystem::set3DAttributes(FMOD_3D_ATTRIBUTES* attributes) {
	if (attributes != NULL) {
		FMOD_RESULT result = this->eventInstance->set3DAttributes(attributes);
		return true;
	}
	return false;
}
bool FmodSystem::set3DPositionX(float pos) {
	this->attributes.position.x = pos;

	FMOD_RESULT result = this->eventInstance->set3DAttributes(&this->attributes);
	
	if (result != FMOD_OK) {
		std::cerr << "FMOD Can`t set 3D position X " << FMOD_ErrorString(result) << std::endl;
		return false;
	}

	return true;
}

bool FmodSystem::set3DPositionY(float pos) {
	this->attributes.position.y = pos;

	FMOD_RESULT result = this->eventInstance->set3DAttributes(&this->attributes);

	if (result != FMOD_OK) {
		std::cerr << "FMOD Can`t set 3D position Y " << FMOD_ErrorString(result) << std::endl;
		return false;
	}

	return true;
}

bool FmodSystem::set3DPositionZ(float pos) {
	this->attributes.position.z = pos;

	FMOD_RESULT result = this->eventInstance->set3DAttributes(&this->attributes);

	if (result != FMOD_OK) {
		std::cerr << "FMOD Can`t set 3D position Z " << FMOD_ErrorString(result) << std::endl;
		return false;
	}

	return true;
}

bool FmodSystem::set3DPositionXYZ(float x, float y, float z){
	bool sx = false;
	bool sy = false;
	bool sz = false;

	if (this->set3DPositionX(x)) {
		sx = true;
	}

	if (this->set3DPositionY(y)) {
		sy = true;
	}
	
	if (this->set3DPositionZ(z)) {
		sz = true;
	}

	return (sx && sy && sz);
}

bool FmodSystem::setListenerAttributes(int listener, FMOD_3D_ATTRIBUTES* attributes){
	FMOD_RESULT result = this->system->setListenerAttributes(listener, attributes);

	if (result != FMOD_OK) {
		std::cerr << "FMOD Can`t set listener attributes( " <<listener<<" ). "<< FMOD_ErrorString(result) << std::endl;
		return false;
	}
	return true;
}
bool FmodSystem::setParameterValue(std::string& s, float value) {
	
	if (value == -1) {
		value = _startValueParameter;
	}
	else this->setValue( value );
	
	
	FMOD_RESULT result = this->eventInstance->setParameterValue(s.c_str(), this->getValue());
	
	if (result != FMOD_OK) {
		std::cerr << "FMOD Can`t set parameter value. " << FMOD_ErrorString(result) << std::endl;
		return false;
	}

	return true;
}

bool FmodSystem::load(std::string& bank) {
	
	if (!bank.empty()) {
		FMOD::Studio::Bank* Banks = NULL;
		std::string pathToBank = this->_pathToStore + bank;
		FMOD_RESULT result = system->loadBankFile(pathToBank.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &Banks);

		if (result != FMOD_OK) {
			std::cerr << " *** ERROR *** Can`t load bank-file: " << pathToBank.c_str() << " Message:   " << FMOD_ErrorString(result) << std::endl;
			return false;
		}
		return true;
	}

	return false;
}
bool FmodSystem::start() {
	FMOD_RESULT result = this->eventInstance->start();

	if (result != FMOD_OK) {
		std::cerr << "FMOD Can`t start. " << FMOD_ErrorString(result) << std::endl;
		return false;
	}

	return true;
}

bool FmodSystem::loadEvent(std::string& event) {
	FMOD_RESULT result;

	if (!event.empty()) {
		this->_eventName = event;
		result = system->getEvent(event.c_str(), &this->eventDescription);
		result = this->eventDescription->createInstance(&this->eventInstance);
		
		if (result != FMOD_OK){
			std::cerr << "*** ERROR *** Can`t load event: "<<event.c_str() << FMOD_ErrorString(result) << std::endl;
			return false;
		}

		return true;
	}
	else {
		return false;
	}

}

bool FmodSystem::defaultSettings() {
	std::string mbank = "Master Bank.bank";
	std::string msbank = "Master Bank.strings.bank";
	std::string vbank = "Vehicles.bank";
	std::string sEvent = "event:/Vehicles/Basic Engine";
	this->load(mbank);
	this->load(msbank);
	this->load(vbank);

	this->loadEvent(sEvent);

	std::string param = "RPM";
	this->setParameterValue(param, this->_startValueParameter);

	this->start();

	this->attributes.forward.z = 1.0f;
	this->attributes.up.y = 1.0f;

	this->setListenerAttributes(0, &this->attributes);


	this->attributes.position.z = 2.0f;

	this->set3DAttributes(&this->attributes);
	return true;
}

void FmodSystem::setValue(float& v) {

	if (v > -1) {
		this->_mutex.lock();
		this->_value = v;
		this->_mutex.unlock();
	}

}

FmodSystem::FmodSystem(const FmodSystem& fs){
		system = fs.system;
		lowLevelSystem = fs.lowLevelSystem;
		channel = fs.channel;
		eventDescription = fs.eventDescription;
		eventInstance = fs.eventInstance;
		attributes = fs.attributes;
		_pathToStore = fs._pathToStore;
		_init = fs._init;
		_startValueParameter = fs._startValueParameter;
		_value = fs._value;
		//_mutex = fs._mutex;
		_eventName = fs._eventName;
		_driver = fs._driver;
		_parameterName = fs._parameterName;
		_byteShmem = fs._byteShmem;
}