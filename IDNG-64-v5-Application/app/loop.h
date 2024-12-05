unsigned char loopInstalledGet (unsigned char address);









unsigned char loopActualLevelGet(unsigned char address);
unsigned char loopEmergencyModeGet(unsigned char address);
unsigned char loopFailureStatusGet(unsigned char address);
void loopActualLevelReceivedSet(unsigned char address);
void loopActualLevelSet(unsigned char address, unsigned char value);
void loopActualLevelTicksCntReset(unsigned char address);
unsigned char loopActualLevelShallBeGet(unsigned char address);
unsigned char loopActualLevelTransitionGet(unsigned char address);
void loopActualLevelTransitionReset(unsigned char address);
void loopEmergencyModeReceivedSet(unsigned char address);
void loopEmergencyModeSet(unsigned char address, unsigned char value);
unsigned char loopEmergencyModeShallBeGet(unsigned char address);
void loopEmergencyModeTicksCntReset(unsigned char address);
void loopEmergencyModeTransitionClear(unsigned char address);
unsigned char loopEmergencyModeTransitionGet(unsigned char address);
unsigned char loopFailureStatusLastGet(unsigned char address);
void loopFailureStatusLastSet(unsigned char address, unsigned char value);
void loopFailureStatusReceivedSet(unsigned char address);
void loopFailureStatusSet(unsigned char address, unsigned char value);
void loopFailureStatusTicksCntReset(unsigned char address);








