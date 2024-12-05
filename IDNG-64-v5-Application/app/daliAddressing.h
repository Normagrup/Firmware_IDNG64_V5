void daliAddressingDaliNotifyCb(unsigned int daliSentSenderPacketId, unsigned char daliSentFrameType, unsigned char daliSentDest, unsigned char *daliSentApply, unsigned char daliSentCommandHigh, unsigned char daliSentCommandLow, unsigned char daliReplyFrameType, unsigned char daliReplyValid, unsigned char *daliReplyValidArray, unsigned char daliReplyAnswer, unsigned char *daliReplyAnswerArray);
void daliAddressingLineScanStart (unsigned char *address);
unsigned char daliAddressingGroupScanStart (void);
void daliAddressingNewInitialisationStart (unsigned char *address, unsigned char newInitType);
void daliAddressingSystemExtensionStart (unsigned char *address);
void daliAddressingTimeoutTick (void);
void daliAddressingDoTask (void);
unsigned char daliAddressingDefaultGroupsSetStart (void);

