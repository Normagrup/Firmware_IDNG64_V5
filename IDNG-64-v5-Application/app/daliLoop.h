#define DALI_LOOP_ERROR_COM_RETRIES		3


unsigned char daliLoopStatusGet (unsigned char address);
unsigned char daliLoopErrorComGet (unsigned char address);
unsigned char daliLoopInstalledArrayByteGet (unsigned char *arrayByte);
unsigned char daliLoopDeviceTypeMatchArrayByteGet (unsigned char *arrayByte, unsigned char deviceType);
unsigned char daliLoopDeviceBitFromByteArrayGet (unsigned char *byteArray, unsigned char device);
unsigned char daliLoopErrorComCntIncrease (unsigned char address);
unsigned char daliLoopErrorComCntDecrease (unsigned char address);
void daliLoopActualLevelValidClearAll (void);
unsigned char daliLoopInstalledGet (unsigned char address);

unsigned char daliLoopDeviceTypeGet (unsigned char address);

unsigned char daliLoopEmergencyModeGet (unsigned char address);
unsigned char daliLoopFailureStatusGet (unsigned char address);
unsigned char daliLoopActualLevelValidGet (unsigned char address);
unsigned char daliLoopActualLevelGet (unsigned char address);
void daliLoopBatchFrameProcess(unsigned char daliSentCommandHigh, unsigned char daliSentCommandLow, unsigned char *daliSentApply, unsigned char *validArray, unsigned char *answerArray);
unsigned char daliLoopValidByteGet(unsigned char op, unsigned char byte);


void daliLoopActualLevelApplyGet (unsigned char *apply);

unsigned char daliLoopGroupActualLevelGet (unsigned char group);


void daliLoopDoTask(void);
