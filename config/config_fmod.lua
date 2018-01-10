
local fmod_sound = {}

vehicle = { 
			banks={ "Master Bank.bank", 
					"Master Bank.strings.bank", 
					"Airship.bank", 
				  };
			
						
			events={ { event="event:/Airship/SFrontRight"; soundDriver=0; parameter="RPM"; byteInShmem=0; sourceWBus="wBUS";};
					 { event="event:/Airship/SFrontLeft";  soundDriver=0; parameter="RPM"; byteInShmem=1; sourceWBus="wBUS";};
					 { event="event:/Airship/SRearRight";  soundDriver=0; parameter="RPM"; byteInShmem=2; sourceWBus="wBUS";};
					 { event="event:/Airship/SRearLeft";   soundDriver=0; parameter="RPM"; byteInShmem=3; sourceWBus="wBUS";};
					 
--					 { event="event:/Airship/MFrontRight"; soundDriver=2; parameter="RPM"; byteInShmem=4; sourceWBus="wBUS";};
	--				 { event="event:/Airship/MFrontLeft";  soundDriver=2; parameter="RPM"; byteInShmem=5; sourceWBus="wBUS";};
		--			 { event="event:/Airship/MRearRight";  soundDriver=2; parameter="RPM"; byteInShmem=6; sourceWBus="wBUS";};
			--		 { event="event:/Airship/MRearLeft";   soundDriver=0; parameter="RPM"; byteInShmem=7; sourceWBus="wBUS";};
					};
		
		};
   
vehicle8 = { 
			banks={ "Master Bank.bank", 
					"Master Bank.strings.bank", 
					"Airship.bank", 
				  };
			
						
			events={ { event="event:/Airship/SFrontRight"; soundDriver=0; parameter="RPM"; byteInShmem=0; sourceWBus="wBUS";};
					 { event="event:/Airship/SFrontLeft";  soundDriver=0; parameter="RPM"; byteInShmem=1; sourceWBus="wBUS";};
					 { event="event:/Airship/SRearRight";  soundDriver=0; parameter="RPM"; byteInShmem=2; sourceWBus="wBUS";};
					 { event="event:/Airship/SRearLeft";   soundDriver=0; parameter="RPM"; byteInShmem=3; sourceWBus="wBUS";};
					 
					 { event="event:/Airship/MFrontRight"; soundDriver=2; parameter="RPM"; byteInShmem=4; sourceWBus="wBUS";};
					 { event="event:/Airship/MFrontLeft";  soundDriver=2; parameter="RPM"; byteInShmem=5; sourceWBus="wBUS";};
					 { event="event:/Airship/MRearRight";  soundDriver=2; parameter="RPM"; byteInShmem=6; sourceWBus="wBUS";};
					 { event="event:/Airship/MRearLeft";   soundDriver=0; parameter="RPM"; byteInShmem=7; sourceWBus="wBUS";};
					};
		
		};
return rmod_sound;
