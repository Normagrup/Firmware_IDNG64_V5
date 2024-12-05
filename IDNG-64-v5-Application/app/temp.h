#include "config.h"
#include "dali.h"
#include "daliLoop.h"
#include "macros.h"
#include "rtcc.h"
#include "m25p.h"

#define TEMP_ACTIVO											CONFIG_TEMP_ACTIVO
#define TEMP_START_WEEKDAY							CONFIG_TEMP_START_WEEKDAY
#define TEMP_START_HOUR									CONFIG_TEMP_START_HOUR
#define TEMP_START_MINUTES							CONFIG_TEMP_START_MINUTES
#define TEMP_START_ACTION_DEST					CONFIG_TEMP_START_ACTION_DEST					
#define TEMP_START_ACTION_COMAND				CONFIG_TEMP_START_ACTION_COMAND
#define TEMP_STOP_WEEKDAY								CONFIG_TEMP_STOP_WEEKDAY
#define TEMP_STOP_HOUR									CONFIG_TEMP_STOP_HOUR
#define TEMP_STOP_MINUTES								CONFIG_TEMP_STOP_MINUTES
#define TEMP_STOP_ACTION_DEST						CONFIG_TEMP_STOP_ACTION_DEST
#define TEMP_STOP_ACTION_COMAND					CONFIG_TEMP_STOP_ACTION_COMAND








void tempDaliNotifyCb(unsigned int daliSentSenderPacketId, unsigned char daliSentFrameType, unsigned char daliSentDest, unsigned char *daliSentApply, unsigned char daliSentCommandHigh, unsigned char daliSentCommandLow, unsigned char daliReplyFrameType, unsigned char daliReplyValid, unsigned char *daliReplyValidArray, unsigned char daliReplyAnswer, unsigned char *daliReplyAnswerArray)									;
unsigned char tempPacketIdGet(void);
void tempDoTask (void);
void tempSet (unsigned char numero, unsigned char activo, unsigned char *testTime);
unsigned char tempGet (unsigned char numero, unsigned char *testTime);
void tempInit (void);
void tempCheck (void);
void tempTick (void);





