void testDaliNotifyCb(unsigned int daliSentSenderPacketId, unsigned char daliSentFrameType, unsigned char daliSentDest, unsigned char *daliSentApply, unsigned char daliSentCommandHigh, unsigned char daliSentCommandLow, unsigned char daliReplyFrameType, unsigned char daliReplyValid, unsigned char *daliReplyValidArray, unsigned char daliReplyAnswer, unsigned char *daliReplyAnswerArray);									
void testSet (unsigned char grupo, unsigned char activo, unsigned char *testTime);
unsigned char testGet (unsigned char grupo, unsigned char *testTime);
void testInit (void);
void testRTCC_Tick (void);
void testDoTask (void);
