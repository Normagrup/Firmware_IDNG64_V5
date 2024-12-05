#include "dataTypes.h"

BOOL daliSensoresQuerySensorStatusEnviar(unsigned char sensorAddress);
void daliSensoresQuiereComunicar (unsigned char sensorAddress);
BOOL daliSensoresQuerySensorStatusEnviar(unsigned char sensorAddress);
BOOL daliSensoresRecallMaxLevelEnviar (unsigned char numeroGrupo);
BOOL daliSensoresOffEnviar (unsigned char numeroGrupo);
BOOL daliSensoresUpEnviar (unsigned char numeroGrupo);
BOOL daliSensoresDownEnviar (unsigned char numeroGrupo);
BOOL daliSensoresComandoArcoDirectoEnviar(unsigned char numeroGrupo, unsigned char nivel);
BOOL daliSensoresQuerySensorNivelEnviar(unsigned char sensorAddress);
void daliSensoresSensorStatusProcesar (unsigned char sensorAddress, unsigned char registroEstado);
void daliSensoresNivelLuzRecibidoProcesar (unsigned char sensorAddress, unsigned char nivel);
void daliSensoresDaliNotifyCb(unsigned int daliSentSenderPacketId, unsigned char daliSentFrameType, unsigned char daliSentDest, unsigned char *daliSentApply, unsigned char daliSentCommandHigh, unsigned char daliSentCommandLow, unsigned char daliReplyFrameType, unsigned char daliReplyValid, unsigned char *daliReplyValidArray, unsigned char daliReplyAnswer, unsigned char *daliReplyAnswerArray);
void daliSensoresPollingTmpDisable(void);
void daliSensoresPollingEndTmpDisable(void);
