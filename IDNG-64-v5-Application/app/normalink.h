void normaLinkTick (void);
unsigned char normaLinkDaliAccesoExclusivoABusEnable (unsigned char *address);
void normaLinkDaliAccesoExclusivoABusDisable (void);
void normalinkDaliSend (unsigned char *address, unsigned int packetId, unsigned char daliFrameType, unsigned char daliTransceiverDest, unsigned char daliTransceiverCommandHigh, unsigned char daliTransceiverCommandLow);
void normalinkDaliNotifyCb(unsigned int daliSentSenderPacketId, unsigned char daliSentFrameType, unsigned char daliSentDest, unsigned char *daliSentApply, unsigned char daliSentCommandHigh, unsigned char daliSentCommandLow, unsigned char daliReplyFrameType, unsigned char daliReplyValid, unsigned char *daliReplyValidArray, unsigned char daliReplyAnswer, unsigned char *daliReplyAnswerArray);
unsigned char normaLinkDestCorrecto (unsigned char _dest);
void normaLinkMasterPing (unsigned char *address);
void normalinkBufferDaliSend (unsigned char *address, unsigned int packetId, unsigned char frameType, unsigned char *dest, unsigned char *com);
