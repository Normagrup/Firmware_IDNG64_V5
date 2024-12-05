void qSNDoTask (void);
static void qSNDTRSend (unsigned char value);
static void qSNDTR1Send (unsigned char value);
static void qSNReadMemoryLocationSend (unsigned char dest);
void qSNDTRCb (unsigned char daliSentDest);
void qSNDTR1Cb (unsigned char daliSentDest);
void qSNReadMemoryLocationCb (unsigned char daliSentCommandLow, unsigned char daliSentReplyValid, unsigned char daliSentReplyAnswer);
void qSNRead (unsigned char address, unsigned char *origen);
