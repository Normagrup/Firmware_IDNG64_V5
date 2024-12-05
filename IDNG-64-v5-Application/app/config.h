#include <string.h>
#include "m25p.h"
#include "dataTypes.h"

#define CONFIG_MEMORY_SIZE												76
//#define CONFIG_DEFAULT_IP_ADDRESS									{1,1,42,5}
//#define CONFIG_DEFAULT_SUBMASK_ADDRESS						{255,255,0,0}
#define CONFIG_DEFAULT_IP_ADDRESS									{192,168,1,254}
#define CONFIG_DEFAULT_SUBMASK_ADDRESS						{255,255,255,0}
#define CONFIG_DEFAULT_GATEWAY_ADDRESS						{0,0,0,0}
#define CONFIG_DEFAULT_NOMBRE_EDIFICIO						"NONAME"
#define CONFIG_DEFAULT_NOMBRE_LINEA								"NONAME"
#define CONFIG_DEFAULT_ADMIN_PASS									"A1234"
#define CONFIG_DEFAULT_MANTENEDOR_PASS						"M1234"

#define CONFIG_TEMP_ACTIVO												0x00
#define CONFIG_TEMP_START_WEEKDAY									0x01
#define CONFIG_TEMP_START_HOUR										0x02
#define CONFIG_TEMP_START_MINUTES									0x03
#define CONFIG_TEMP_START_ACTION_DEST							0x04
#define CONFIG_TEMP_START_ACTION_COMAND						0x05
#define CONFIG_TEMP_STOP_WEEKDAY									0x06
#define CONFIG_TEMP_STOP_HOUR											0x07
#define CONFIG_TEMP_STOP_MINUTES									0x08
#define CONFIG_TEMP_STOP_ACTION_DEST							0x09
#define CONFIG_TEMP_STOP_ACTION_COMAND						0x0A


void fillBuffer(unsigned char *buffer, unsigned char *src, unsigned int ptr, unsigned int size);
void configDaliLoopInstalledSet (unsigned char *daliLoopInstalledByte, unsigned char storeInFlash);
void configDaliLoopInstalledGet (unsigned char *daliLoopInstalledByte);
void configDaliLoopInstalledResetToFactorySettings (unsigned char storeInFlash);
void configDaliLoopDeviceTypeSet (unsigned char *daliLoopDeviceTypeValid, unsigned char *daliLoopDeviceType, unsigned char storeInFlash);
void configDaliLoopDeviceTypeGet (unsigned char *daliLoopDeviceTypeValid, unsigned char *daliLoopDeviceType);
void configDaliLoopDeviceTypeResetToFactorySettings (unsigned char storeInFlash);
void configTempSet (unsigned char num, unsigned char *tempData,  unsigned char storeInFlash);
void configTempGet (unsigned char num, unsigned char *tempData);
void configTempInit(void);
void cofigTempResetToFactorySettings (unsigned char storeInFlash);
void configTestSet (unsigned char num, unsigned char *testData,  unsigned char storeInFlash);
void configTestGet (unsigned char num, unsigned char *testData);
void configTestInit (void);
void configTestResetToFactorySettings (unsigned char storeInFlash);
void configInit (void);
void configStoreInFlash (void);
void configIpAddressSet(unsigned char *ipAddress, unsigned char storeInFlash);
void configSubmaskAddressSet(unsigned char *submaskAddress, unsigned char storeInFlash);
void configGatewayAddressSet(unsigned char *gatewayAddress, unsigned char storeInFlash);
void configNombreEdificioSet(unsigned char *nombreEdificio, unsigned char storeInFlash);
void configNombreLineaSet(unsigned char *nombreLinea, unsigned char storeInFlash);
void configAdminPassSet(unsigned char *adminPass, unsigned char storeInFlash);
void configMantenedorPassSet(unsigned char *mantenedorPass, unsigned char storeInFlash);
void configResetNetworkSettings(void);
void configResetToFactorySettings(void);
void configIpAddressGet(unsigned char *ipAddress);
void configSubmaskAddressGet(unsigned char *submaskAddress);
void configGatewayAddressGet(unsigned char *gatewayAddress);
void configNombreEdificioGet(unsigned char *nombreEdificio);
	
void configNombreLineaGet(unsigned char *nombreLinea);
void configAdminPassGet(unsigned char *adminPass);
void configMantenedorPassGet(unsigned char *mantenedorPass);

void configDaliLoopGroupLowSet (unsigned char *daliLoopGroupLowValid, unsigned char *daliLoopGroupLow, unsigned char storeInFlash);
void configDaliLoopGroupHighSet (unsigned char *daliLoopGroupHighValid, unsigned char *daliLoopGroupHigh, unsigned char storeInFlash);
void configDaliLoopGroupsGet (unsigned char address, unsigned char *valid, unsigned int *groups);


BOOL configSensoresIniciadaConfiguracionGet (void);
BOOL configInterfacesConfiguracionGet(unsigned char interfazAddress, unsigned char *cfg);
BOOL configSensoresConfiguracionGet(unsigned char accion, unsigned char *cfg);
BOOL configSensoresGruposConfiguracionGet(unsigned char grupo, unsigned char *cfg);
BOOL configSensoresIniciarConfiguracion (void);
BOOL configInterfacesConfiguracionSet(unsigned char interfazAddress, unsigned char *cfg);
BOOL configSensoresConfiguracionSet(unsigned char accion, unsigned char *cfg);
BOOL configSensoresGruposConfiguracionSet(unsigned char grupo, unsigned char *cfg);
BOOL configSensoresFinalizarConfiguracion (void);

void belongsGroupGet (unsigned char groupCnt, unsigned char *belongsGroup);
void belongsGroupSet(unsigned char group, unsigned char *belongsGroup);



BOOL configInterfacesIniciarConfiguracion (void);
BOOL configInterfacesFinalizarConfiguracion (void);

