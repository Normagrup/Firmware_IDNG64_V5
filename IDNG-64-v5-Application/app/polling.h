void pollingTick (void);
void pollingTmpDisable (void);
void polling_TmpGroupDisable (void);
void pollingEndTmpDisable(void);
unsigned char pollingStatusGet (void);
void pollingCntInc (void);
void pollingDaliNotifyCb(unsigned int daliSentSenderPacketId, unsigned char daliSentFrameType, unsigned char daliSentDest, unsigned char *daliSentApply, unsigned char daliSentCommandHigh, unsigned char daliSentCommandLow, unsigned char daliReplyFrameType, unsigned char daliReplyValid, unsigned char *daliReplyValidArray, unsigned char daliReplyAnswer, unsigned char *daliReplyAnswerArray, unsigned char sensorAddress);
void pollingDoTask (void);

void pollingRst (void);