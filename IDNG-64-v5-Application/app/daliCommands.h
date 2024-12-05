unsigned char daliCommandsBlockingBoolQueryControlGear (unsigned char addressByte);
unsigned char daliCommandsBlockingFailureStatusGet (unsigned char address);
unsigned char daliCommandsBlockingStartFunctionTest (unsigned char group);
unsigned char daliCommandsBlockingStartDurationTest (unsigned char group);
unsigned char daliCommandsBlockingDoAction (unsigned char dest, unsigned char comand);
//unsigned char daliCommandsBlockingDoActionPrueba (unsigned char num);
void daliCommandsBlockingQueryDeviceType(unsigned char addressByte, unsigned char *valid, unsigned char *value);

