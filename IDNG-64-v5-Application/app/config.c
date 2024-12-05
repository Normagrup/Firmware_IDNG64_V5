/*

05/02/2019	SOPORTE FUNCIÓN CORRIDOR.

*/




#include "config.h"
#include "macros.h"

// 684 bytes RAM.

// p0
unsigned char _configIpAddress[4];
unsigned char _configSubmaskAddress[4];
unsigned char _configGatewayAddress[4];
unsigned char _configNombreEdificio[16];
unsigned char _configNombreLinea[16];
unsigned char _configAdminPass[16];
unsigned char _configMantenedorPass[16];
// p1
unsigned char _configDaliLoopInstalled[64];
unsigned char _configDaliLoopDeviceTypeValid[64];
unsigned char _configDaliLoopDeviceType[64];
// p2
unsigned char	_configTempActivo[16];
unsigned char	_configTempStartWeekday[16];
unsigned char	_configTempStartHour[16];
unsigned char	_configTempStartMinutes[16];
unsigned char	_configTempStartActionDest[16];
unsigned char	_configTempStartActionComand[16];
unsigned char	_configTempStopWeekday[16];
unsigned char	_configTempStopHour[16];
unsigned char	_configTempStopMinutes[16];
unsigned char	_configTempStopActionDest[16];
unsigned char	_configTempStopActionComand[16];
// p3
unsigned char _configTestGrupoActivo[16];
unsigned char _configTestFtWeekday[16];
unsigned char _configTestFtHour[16];
unsigned char _configTestFtMinutes[16];
unsigned char _configTestDtDay[16];
unsigned char _configTestDtMonth[16];
unsigned char _configTestDtHour[16];
unsigned char _configTestDtMinutes[16];
//p4
unsigned char _configDaliLoopGroupLow[64];
unsigned char _configDaliLoopGroupHigh[64];

BOOL _configSensoresIniciadaConfiguracion;

	unsigned short int _grupoShort;
	
	
unsigned char _configInterfacesIniciadaConfiguracion;


void fillBuffer(unsigned char *buffer, unsigned char *src, unsigned int ptr, unsigned int size)
{
	unsigned int n;
	for (n=0;n<size;n++)
	{
			buffer[(ptr+n)]=src[n];		
	}	
}





// D A L I   L O O P   I N S T A L L E D
void configDaliLoopInstalledSet (unsigned char *daliLoopInstalledByte, unsigned char storeInFlash)
{
	unsigned char n;
	for (n=0;n<64;n++)
	{
		_configDaliLoopInstalled[n]=daliLoopInstalledByte[n];		
	}	
	if (storeInFlash==0x01)
	{
		configStoreInFlash();
	}	
}
void configDaliLoopInstalledGet (unsigned char *daliLoopInstalledByte)
{
	unsigned char n;
	for (n=0;n<64;n++)
	{
		daliLoopInstalledByte[n]=_configDaliLoopInstalled[n];
	}		
}

























void configDaliLoopInstalledResetToFactorySettings (unsigned char storeInFlash)
{
	unsigned char daliLoopInstalled[64];
	unsigned char n;
	for (n=0;n<64;n++)
	{
		daliLoopInstalled[n]=0x00;
	}	
	configDaliLoopInstalledSet (&daliLoopInstalled[0], storeInFlash);	
}



// D A L I   L O O P   D E V I C E   T Y P E
void configDaliLoopDeviceTypeSet (unsigned char *daliLoopDeviceTypeValid, unsigned char *daliLoopDeviceType, unsigned char storeInFlash)
{
	unsigned char n;	
	for (n=0;n<64;n++)
	{
		_configDaliLoopDeviceType[n]=daliLoopDeviceType[n];		
		_configDaliLoopDeviceTypeValid[n]=daliLoopDeviceTypeValid[n];
	}	
	if (storeInFlash==0x01)
	{
		configStoreInFlash();
	}		
}

void configDaliLoopDeviceTypeGet (unsigned char *daliLoopDeviceTypeValid, unsigned char *daliLoopDeviceType)
{
	unsigned char n;
	for (n=0;n<64;n++)
	{
		daliLoopDeviceTypeValid[n]=_configDaliLoopDeviceTypeValid[n];
		daliLoopDeviceType[n]=_configDaliLoopDeviceType[n];
	}		
}


void configDaliLoopGroupLowSet (unsigned char *daliLoopGroupLowValid, unsigned char *daliLoopGroupLow, unsigned char storeInFlash)
{
	unsigned char n;
	for (n=0;n<64;n++)
	{
		_configDaliLoopGroupLow[n]=daliLoopGroupLow[n];
	}		
		if (storeInFlash==0x01)
	{
		configStoreInFlash();
	}	
}

void configDaliLoopGroupHighSet (unsigned char *daliLoopGroupHighValid, unsigned char *daliLoopGroupHigh, unsigned char storeInFlash)
{
	unsigned char n;
	for (n=0;n<64;n++)
	{
		_configDaliLoopGroupHigh[n]=daliLoopGroupHigh[n];
	}			
	if (storeInFlash==0x01)
	{
		configStoreInFlash();
	}		
}


void belongsGroupSet(unsigned char group, unsigned char *belongsGroup)
{
		unsigned char i;
		unsigned char j;
		unsigned char addressCnt;
	
		if (group<8)
		{
			for (addressCnt=0;addressCnt<64;addressCnt++)
			{
				i=(addressCnt/8);
				j=(addressCnt-(8*i));					
				if (BITVAL(belongsGroup[i],j)==0x01)
				{
					SETBIT(_configDaliLoopGroupLow[addressCnt],group);
				}
				else
				{
					CLEARBIT(_configDaliLoopGroupLow[addressCnt],group);
				}				
			}		
		}
		else			
		{
			for (addressCnt=0;addressCnt<64;addressCnt++)
			{
				i=(addressCnt/8);
				j=(addressCnt-(8*i));		
				if (BITVAL(belongsGroup[i],j)==0x01)
				{
					SETBIT(_configDaliLoopGroupHigh[addressCnt],(group-8));
				}
				else
				{
					CLEARBIT(_configDaliLoopGroupHigh[addressCnt],(group-8));		
				}		
			}
		}
}




void belongsGroupGet (unsigned char groupCnt, unsigned char *belongsGroup)
{
	unsigned char addressCnt;
		unsigned char groupByte;
	unsigned char g;
	unsigned char i;
	unsigned char j;
	
	for (addressCnt=0;addressCnt<64;addressCnt++)	
	{
		if (groupCnt<8)
		{
			groupByte=_configDaliLoopGroupLow[addressCnt];						
			g=groupCnt;
		}
		else
		{
			groupByte=_configDaliLoopGroupHigh[addressCnt];			
			g=(groupCnt-8);			
		}				
		i=(addressCnt/8);
		j=(addressCnt-(8*i));		
		if (BITVAL(groupByte, g)==0x01)
		{
			SETBIT(belongsGroup[i],j);
		}
		else
		{
			CLEARBIT(belongsGroup[i],j);
		}			
	}		
}


unsigned char configDaliLoopGroupLowGet (unsigned char address)
{
	return _configDaliLoopGroupLow[address];
}

unsigned char configDaliLoopGroupHighGet (unsigned char address)
{
	return _configDaliLoopGroupHigh[address];
}


void configDaliLoopGroupsGet (unsigned char address, unsigned char *valid, unsigned int *groups)
{
}








void configDaliLoopDeviceTypeResetToFactorySettings (unsigned char storeInFlash)
{
	unsigned char daliLoopDeviceTypeValid[64]={0};
	unsigned char daliLoopDeviceType[64]={0};
	configDaliLoopDeviceTypeSet (&daliLoopDeviceTypeValid[0],&daliLoopDeviceType[0],storeInFlash);		
}












// T E M P O R I Z A C I O N E S
void configTempSet (unsigned char num, unsigned char *tempData,  unsigned char storeInFlash)
{
	if (num<16)
	{	
		_configTempActivo[num]=tempData[0];
		_configTempStartWeekday[num]=tempData[1];
		_configTempStartHour[num]=tempData[2];
		_configTempStartMinutes[num]=tempData[3];
		_configTempStartActionDest[num]=tempData[4];
		_configTempStartActionComand[num]=tempData[5];
		_configTempStopWeekday[num]=tempData[6];
		_configTempStopHour[num]=tempData[7];
		_configTempStopMinutes[num]=tempData[8];
		_configTempStopActionDest[num]=tempData[9];
		_configTempStopActionComand[num]=tempData[10];	
	}
	if (storeInFlash==0x01)
	{
		configStoreInFlash();
	}		
}

void configTempGet (unsigned char num, unsigned char *tempData)
{
	if (num<16)
	{	
		tempData[0]=_configTempActivo[num];
		tempData[1]=_configTempStartWeekday[num];
		tempData[2]=_configTempStartHour[num];
		tempData[3]=_configTempStartMinutes[num];
		tempData[4]=_configTempStartActionDest[num];
		tempData[5]=_configTempStartActionComand[num];
		tempData[6]=_configTempStopWeekday[num];
		tempData[7]=_configTempStopHour[num];
		tempData[8]=_configTempStopMinutes[num];
		tempData[9]=_configTempStopActionDest[num];
		tempData[10]=_configTempStopActionComand[num];
	}
}






























void configTempInit(void)
{
	unsigned char n;
	unsigned char tempData[11];
	for (n=0;n<16;n++)
	{
		m25pReadBytes(0,2,(n*16), &tempData[0],11);		
		configTempSet (n, &tempData[0],  0x00);
	}	
}

void configTempResetToFactorySettings (unsigned char storeInFlash)
{
	unsigned char tempData[11]={0};
	unsigned char n;	
	for (n=0;n<16;n++)
	{
		configTempSet (n, &tempData[0],0x00);		
	}
	if (storeInFlash==0x01) configStoreInFlash();	
}




























// T E S T
void configTestSet (unsigned char num, unsigned char *testData,  unsigned char storeInFlash)
{
	if (num<16)
	{	
		_configTestGrupoActivo[num]=testData[0];
		_configTestFtWeekday[num]=testData[1];
		_configTestFtHour[num]=testData[2];
		_configTestFtMinutes[num]=testData[3];
		_configTestDtDay[num]=testData[4];
		_configTestDtMonth[num]=testData[5];
		_configTestDtHour[num]=testData[6];
		_configTestDtMinutes[num]=testData[7];		
		if (storeInFlash==0x01) {configStoreInFlash();}	
	}
}

void configTestGet (unsigned char num, unsigned char *testData)
{
	testData[0]=_configTestGrupoActivo[num];
	testData[1]=_configTestFtWeekday[num];
	testData[2]=_configTestFtHour[num];
	testData[3]=_configTestFtMinutes[num];
	testData[4]=_configTestDtDay[num];
	testData[5]=_configTestDtMonth[num];
	testData[6]=_configTestDtHour[num];
	testData[7]=_configTestDtMinutes[num];			
}








void configTestInit (void)
{
	unsigned char n;
	unsigned char testData[8];
	for (n=0;n<16;n++)
	{
		m25pReadBytes(0,3,(n*16), &testData[0],8);		
		configTestSet (n, &testData[0],  0x00);
	}		
}















void configTestResetToFactorySettings (unsigned char storeInFlash)
{
	unsigned char testData[8]={0};
	unsigned char n;	
	for (n=0;n<16;n++)
	{
		configTestSet (n, &testData[8],0x00);		
	}
	if (storeInFlash==0x01) configStoreInFlash();	
}









BOOL configSensoresIniciadaConfiguracionGet (void)
{
	return _configSensoresIniciadaConfiguracion;
}


BOOL configInterfacesConfiguracionGet(unsigned char interfazAddress, unsigned char *cfg)
{
	BOOL retVal;	
	if (_configInterfacesIniciadaConfiguracion==FALSE)
	{		
		m25pReadBytes(7,(0+interfazAddress),0,&cfg[0],14);
		if (cfg[0]<=1)
		{		
			retVal=TRUE;
		}
		else
		{
			retVal=FALSE;
		}			
	}
	else
	{
		retVal=FALSE;
	}	
	return retVal;
}

BOOL configSensoresConfiguracionGet(unsigned char accion, unsigned char *cfg)
{
	BOOL retVal;	
	if (_configSensoresIniciadaConfiguracion==FALSE)
	{	
		m25pReadBytes(6,(16+accion),0,&cfg[0],14);
		if (cfg[0]<=1)
		{		
			retVal=TRUE;
		}
		else
		{
			retVal=FALSE;
		}		
	}
	else
	{
		retVal=FALSE;
	}
	return retVal;
}

BOOL configSensoresGruposConfiguracionGet(unsigned char grupo, unsigned char *cfg)
{
	BOOL retVal;	
	if (_configSensoresIniciadaConfiguracion==FALSE)
	{	
		m25pReadBytes(6,(80+grupo),0,&cfg[0],10);
		//if (cfg[0]<=1)
		//{		
			retVal=TRUE;
		//}
		//else
		//{
		//	retVal=FALSE;
		//}		
	}
	else
	{
		retVal=FALSE;
	}
	return retVal;
}

BOOL configSensoresIniciarConfiguracion (void)
{
	BOOL retVal=TRUE;
	unsigned char n;		
	unsigned char addressM;	
	unsigned char buffer[14];	
	unsigned char status;		
	//	>>	BORRAR EL BANCO 7
	//	m25pWriteEnable();
	//	m25pSectorErase (7,0,0);
	//	do
	//	{		
	//		status=m25pStatusRead();
	//	}while((status&0x01)==0x01);
	//	<<	BORRAR EL BANCO 7	
	//	>> COPIAR	DE BANCO 6 A BANCO 7
	
	//for (n=0;n<16;n++)		// COPIAR INTERFACES
	//{		
	//	addressM=0+n;			
	//	m25pReadBytes(6,addressM,0,&buffer[0],13);
	//	m25pWriteEnable();		
	//	m25pWriteBytes(7,addressM,0,&buffer[0],13);	
	//	do
	//	{		
	//		status=m25pStatusRead();
	//	}while((status&0x01)==0x01);				
	//}	
	//for (n=0;n<64;n++)		// COPIAR ACCIONES
	//{		
	//	addressM=16+n;			
	//	m25pReadBytes(6,addressM,0,&buffer[0],14);
	//	m25pWriteEnable();		
	//	m25pWriteBytes(7,addressM,0,&buffer[0],14);	
	//	do
	//	{		
	//		status=m25pStatusRead();
	//	}while((status&0x01)==0x01);	
	//}
	//for (n=0;n<16;n++)		// COPIAR GRUPOS
	//{		
	//	addressM=80+n;			
	//	m25pReadBytes(6,addressM,0,&buffer[0],8);
	//	m25pWriteEnable();		
	//	m25pWriteBytes(7,addressM,0,&buffer[0],8);	
	//	do
	//	{		
	//		status=m25pStatusRead();
	//	}while((status&0x01)==0x01);			
	//}
	//	<< COPIAR	DE BANCO 6 A BANCO 7		
	//	>>	BORRAR EL BANCO 6
		m25pWriteEnable();
		m25pSectorErase (6,0,0);
		do
		{		
			status=m25pStatusRead();
		}while((status&0x01)==0x01);
	//	<<	BORRAR EL BANCO 6	
	_configSensoresIniciadaConfiguracion=TRUE;
	return retVal;
}
BOOL configInterfacesConfiguracionSet(unsigned char interfazAddress, unsigned char *cfg)
{
	BOOL retVal;
	unsigned char status;		
	if (_configInterfacesIniciadaConfiguracion==TRUE)
	{	
		m25pWriteEnable();		
		m25pWriteBytes(7,(0+interfazAddress),0,&cfg[0],13);	
		do
		{		
			status=m25pStatusRead();
		}while((status&0x01)==0x01);	
		retVal=TRUE;
	}
	else
	{
		retVal=FALSE;
	}	
	return retVal;
}
BOOL configSensoresConfiguracionSet(unsigned char accion, unsigned char *cfg)
{

	BOOL retVal;
	unsigned char status;
	if (_configSensoresIniciadaConfiguracion==TRUE)
	{		
		m25pWriteEnable();				
		_grupoShort=sensoresGruposToMatrizGruposGet(cfg[7]);		
		cfg[10]=cfg[9];
		cfg[9]=cfg[8];	
		cfg[7]=(unsigned char)(_grupoShort/0x0100);
		cfg[8]=(unsigned char)(_grupoShort&0x00FF);									
		m25pWriteBytes(6,(16+accion),0,&cfg[0],14);	
		do
		{		
			status=m25pStatusRead();
		}while((status&0x01)==0x01);			
		retVal=TRUE;
	}
	else
	{
		retVal=FALSE;
	}	
	return retVal;
}
BOOL configSensoresGruposConfiguracionSet(unsigned char grupo, unsigned char *cfg)
{
	BOOL retVal;
	unsigned char status;	
	if (_configSensoresIniciadaConfiguracion==TRUE)
	{	
		m25pWriteEnable();		
		m25pWriteBytes(6,(80+grupo),0,&cfg[0],10);	
		do
		{		
			status=m25pStatusRead();
		}while((status&0x01)==0x01);	
		retVal=TRUE;
	}
	else
	{
		retVal=FALSE;
	}		
	return retVal;
}





BOOL configSensoresFinalizarConfiguracion (void)
{
	BOOL retVal=TRUE;
	unsigned char n;		
	unsigned char addressM;	
	unsigned char buffer[14];	
	unsigned char status;	 
	unsigned char firstByte;	
	if (_configSensoresIniciadaConfiguracion==TRUE)
	{	
		//	>> COPIAR	DE BANCO 7 A BANCO 6
		//for (n=0;n<16;n++)		// COPIAR INTERFACES
		//{					
		//	addressM=0+n;						
		//	m25pReadBytes(6,addressM,0,&firstByte,1);
		//	if (firstByte==0xFF)
		//	{					
		//		m25pReadBytes(7,addressM,0,&buffer[0],13);						
		//		m25pWriteEnable();		
		//		m25pWriteBytes(6,addressM,0,&buffer[0],13);	
		//		do
		//		{		
		//			status=m25pStatusRead();
		//		}while((status&0x01)==0x01);				
		//	}			
		//}	
		//for (n=0;n<64;n++)		// COPIAR ACCIONES
		//{					
		//	addressM=16+n;						
		//	m25pReadBytes(6,addressM,0,&firstByte,1);
		//	if (firstByte==0xFF)
		//	{
		//		m25pReadBytes(7,addressM,0,&buffer[0],14);
		//		m25pWriteEnable();		
		//		m25pWriteBytes(6,addressM,0,&buffer[0],14);	
		//		do
		//		{		
		//			status=m25pStatusRead();
		//		}while((status&0x01)==0x01);	
		//	}
		//}
		//for (n=0;n<16;n++)		// COPIAR GRUPOS
		//{					
		//	addressM=80+n;									
		//	m25pReadBytes(6,addressM,0,&firstByte,1);
		//	if (firstByte==0xFF)
		//	{
				
		//		m25pReadBytes(7,addressM,0,&buffer[0],8);
		//		m25pWriteEnable();		
		//		m25pWriteBytes(6,addressM,0,&buffer[0],8);	
		//		do
		//		{		
		//			status=m25pStatusRead();
		//		}while((status&0x01)==0x01);			
		//	}
		//}
		//	<< COPIAR	DE BANCO 7 A BANCO 6		
		retVal=TRUE;
	}
	else
	{
		retVal=FALSE;
	}	
	_configSensoresIniciadaConfiguracion=FALSE;
	return retVal;
}







BOOL configInterfacesIniciarConfiguracion (void)
{
	BOOL retVal=TRUE;
	unsigned char status;			
	if (_configInterfacesIniciadaConfiguracion==FALSE)
	{
	
	//	>>	BORRAR EL BANCO 7
		m25pWriteEnable();
		m25pSectorErase (7,0,0);
		do
		{		
			status=m25pStatusRead();
		}while((status&0x01)==0x01);
	//	<<	BORRAR EL BANCO 7	
	_configInterfacesIniciadaConfiguracion=TRUE;
	}
	return retVal;
}

BOOL configInterfacesFinalizarConfiguracion (void)
{
	BOOL retVal=TRUE;
	if (_configInterfacesIniciadaConfiguracion==TRUE)
	{	
		retVal=TRUE;
	}
	else
	{
		retVal=FALSE;
	}	
	_configInterfacesIniciadaConfiguracion=FALSE;
	return retVal;
}






















// -- T E S T
void configInit (void)
{
	
	m25pReadBytes(0,0,0, &_configIpAddress[0],4);
	m25pReadBytes(0,0,4, &_configSubmaskAddress[0],4);
	m25pReadBytes(0,0,8, &_configGatewayAddress[0],4);
	m25pReadBytes(0,0,12, &_configNombreEdificio[0],16);
	m25pReadBytes(0,0,28, &_configNombreLinea[0],16);
	m25pReadBytes(0,0,44, &_configAdminPass[0],16);
	m25pReadBytes(0,0,60, &_configMantenedorPass[0],16);	
	m25pReadBytes(0,1,0, &_configDaliLoopInstalled[0],64);
	m25pReadBytes(0,1,64, &_configDaliLoopDeviceTypeValid[0],64);
	m25pReadBytes(0,1,128, &_configDaliLoopDeviceType[0],64);	
	
	//m25pReadBytes(0,4,0, &_configDaliLoopGroupLowValid[0],64);	
	m25pReadBytes(0,4,64, &_configDaliLoopGroupLow[0],64);	
	//m25pReadBytes(0,4,128, &_configDaliLoopGroupHighValid[0],64);	
	m25pReadBytes(0,4,192, &_configDaliLoopGroupHigh[0],64);	
	
	
	configTempInit();	
	configTestInit();
}

void configStoreInFlash (void)
{
	unsigned char status;	
	unsigned char bufferP0[76];
	unsigned char bufferP1[192];
	unsigned char bufferP2[256]={0xFF};
	unsigned char bufferP3[256]={0xFF};
	unsigned char bufferP4[256]={0xFF};
	unsigned char	bufferP5[2]={0x01,1};	//si está o no grabada y la versión.
	unsigned char tempData[11];
	unsigned char testData[8];
	unsigned char n;	
	fillBuffer(&bufferP0[0],&_configIpAddress[0],0,4);
	fillBuffer(&bufferP0[0],&_configSubmaskAddress[0],4,4);
	fillBuffer(&bufferP0[0],&_configGatewayAddress[0],8,4);
	fillBuffer(&bufferP0[0],&_configNombreEdificio[0],12,16);
	fillBuffer(&bufferP0[0],&_configNombreLinea[0],28,16);
	fillBuffer(&bufferP0[0],&_configAdminPass[0],44,16);
	fillBuffer(&bufferP0[0],&_configMantenedorPass[0],60,16);
	fillBuffer(&bufferP1[0],&_configDaliLoopInstalled[0],0,64);
	fillBuffer(&bufferP1[0],&_configDaliLoopDeviceTypeValid[0],64,64);
	fillBuffer(&bufferP1[0],&_configDaliLoopDeviceType[0],128,64);	
	
	//fillBuffer(&bufferP4[0],&_configDaliLoopGroupLowValid[0],0,64);	
	fillBuffer(&bufferP4[0],&_configDaliLoopGroupLow[0],64,64);	
	//fillBuffer(&bufferP4[0],&_configDaliLoopGroupHighValid[0],128,64);	
	fillBuffer(&bufferP4[0],&_configDaliLoopGroupHigh[0],192,64);
	
	
	
	for (n=0;n<16;n++)
	{	
		configTempGet (n, &tempData[0]);
		fillBuffer(&bufferP2[0],&tempData[0],(n*16),16);		
	}	
	for (n=0;n<16;n++)
	{	
		configTestGet (n, &testData[0]);
		fillBuffer(&bufferP3[0],&testData[0],(n*16),16);		
	}		
	// SECTOR 0 ERASE
	m25pWriteEnable();
	m25pSectorErase (0,0,0);
	do
	{		
		status=m25pStatusRead();
	}while((status&0x01)==0x01);			
	// WRITE PAGE 0
	m25pWriteEnable();
	m25pWriteBytes(0,0,0,&bufferP0[0],CONFIG_MEMORY_SIZE);	
	do
	{		
		status=m25pStatusRead();
	}while((status&0x01)==0x01);			
	// WRITE PAGE 1
	m25pWriteEnable();
	m25pWriteBytes(0,1,0,&bufferP1[0],192);	
	do
	{		
		status=m25pStatusRead();
	}while((status&0x01)==0x01);	
	// WRITE PAGE 2
	m25pWriteEnable();
	m25pWriteBytes(0,2,0,&bufferP2[0],256);	
	do
	{		
		status=m25pStatusRead();
	}while((status&0x01)==0x01);		
	// WRITE PAGE 3
	m25pWriteEnable();
	m25pWriteBytes(0,3,0,&bufferP3[0],256);	
	do
	{		
		status=m25pStatusRead();
	}while((status&0x01)==0x01);	
	// WRITE PAGE 4
	m25pWriteEnable();
	m25pWriteBytes(0,4,0,&bufferP4[0],256);	
	do
	{		
		status=m25pStatusRead();
	}while((status&0x01)==0x01);	
	
	// WRITE PAGE 5
	m25pWriteEnable();
	m25pWriteBytes(0,5,0,&bufferP5[0],2);	
	do
	{		
		status=m25pStatusRead();
	}while((status&0x01)==0x01);		
}


void configIpAddressSet(unsigned char *ipAddress, unsigned char storeInFlash)
{
	unsigned char n;
	for (n=0;n<4;n++)
	{
		_configIpAddress[n]=ipAddress[n];
	}			
	board_i2c_write_ip_address (&_configIpAddress[0]);	
	if (storeInFlash==0x01)
	{
		configStoreInFlash();
	}
}
	
void configSubmaskAddressSet(unsigned char *submaskAddress, unsigned char storeInFlash)
{
	unsigned char n;
	for (n=0;n<4;n++)
	{
		_configSubmaskAddress[n]=submaskAddress[n];
	}	
	board_i2c_write_submask_address (&_configSubmaskAddress[0]);
	if (storeInFlash==0x01)
	{
		configStoreInFlash();
	}
}
	
void configGatewayAddressSet(unsigned char *gatewayAddress, unsigned char storeInFlash)
{
	unsigned char n;
	for (n=0;n<4;n++)
	{	
		_configGatewayAddress[n]=gatewayAddress[n];
	}	
	if (storeInFlash==0x01)
	{
		configStoreInFlash();
	}
}
	
void configNombreEdificioSet(unsigned char *nombreEdificio, unsigned char storeInFlash)
{
	unsigned char n;
	for (n=0;n<16;n++)
	{
		_configNombreEdificio[n]=nombreEdificio[n];
	}
	if (storeInFlash==0x01)
	{
		configStoreInFlash();
	}
}
	
void configNombreLineaSet(unsigned char *nombreLinea, unsigned char storeInFlash)
{
	unsigned char n;
	for (n=0;n<16;n++)
	{
		_configNombreLinea[n]=nombreLinea[n];
	}
	if (storeInFlash==0x01)
	{
		configStoreInFlash();
	}
}
	
void configAdminPassSet(unsigned char *adminPass, unsigned char storeInFlash)
{
	unsigned char n;
	for (n=0;n<16;n++)
	{
		_configAdminPass[n]=adminPass[n];
	}
	if (storeInFlash==0x01)
	{
		configStoreInFlash();
	}
}
	
void configMantenedorPassSet(unsigned char *mantenedorPass, unsigned char storeInFlash)
{
	unsigned char n;
	for (n=0;n<16;n++)
	{
		_configMantenedorPass[n]=mantenedorPass[n];
	}
	if (storeInFlash==0x01)
	{
		configStoreInFlash();
	}
}









void configResetNetworkSettings(void)
{
	unsigned char ipAddress[4]=CONFIG_DEFAULT_IP_ADDRESS;
	unsigned char submaskAddress[4]=CONFIG_DEFAULT_SUBMASK_ADDRESS;
	unsigned char gatewayAddress[4]=CONFIG_DEFAULT_GATEWAY_ADDRESS;
	unsigned char nombreEdificio[16]=CONFIG_DEFAULT_NOMBRE_EDIFICIO;
	unsigned char nombreLinea[16]=CONFIG_DEFAULT_NOMBRE_LINEA;
	unsigned char adminPass[16]=CONFIG_DEFAULT_ADMIN_PASS;
	unsigned char mantenedorPass[16]=CONFIG_DEFAULT_MANTENEDOR_PASS;
	configIpAddressSet(&ipAddress[0], 0x00);
	configSubmaskAddressSet(&submaskAddress[0], 0x00);
	configGatewayAddressSet(&gatewayAddress[0], 0x00);
	configNombreEdificioSet(&nombreEdificio[0], 0x00);
	configNombreLineaSet(&nombreLinea[0], 0x00);
	configAdminPassSet(&adminPass[0], 0x00);
	configMantenedorPassSet(&mantenedorPass[0], 0x00);
	configStoreInFlash();	
}





void configResetToFactorySettings(void)
{
	unsigned char ipAddress[4]=CONFIG_DEFAULT_IP_ADDRESS;
	unsigned char submaskAddress[4]=CONFIG_DEFAULT_SUBMASK_ADDRESS;
	unsigned char gatewayAddress[4]=CONFIG_DEFAULT_GATEWAY_ADDRESS;
	unsigned char nombreEdificio[16]=CONFIG_DEFAULT_NOMBRE_EDIFICIO;
	unsigned char nombreLinea[16]=CONFIG_DEFAULT_NOMBRE_LINEA;
	unsigned char adminPass[16]=CONFIG_DEFAULT_ADMIN_PASS;
	unsigned char mantenedorPass[16]=CONFIG_DEFAULT_MANTENEDOR_PASS;
	configIpAddressSet(&ipAddress[0], 0x00);
	configSubmaskAddressSet(&submaskAddress[0], 0x00);
	configGatewayAddressSet(&gatewayAddress[0], 0x00);
	configNombreEdificioSet(&nombreEdificio[0], 0x00);
	configNombreLineaSet(&nombreLinea[0], 0x00);
	configAdminPassSet(&adminPass[0], 0x00);
	configMantenedorPassSet(&mantenedorPass[0], 0x00);
	configTempResetToFactorySettings (0x00);
	configTestResetToFactorySettings (0x00);	
	configStoreInFlash();	
}
	
void configIpAddressGet(unsigned char *ipAddress)
{
	unsigned char n;
	for (n=0;n<4;n++)
	{
		ipAddress[n]=_configIpAddress[n];
	}	
}
	
void configSubmaskAddressGet(unsigned char *submaskAddress)
{
	unsigned char n;
	for (n=0;n<4;n++)
	{
		submaskAddress[n]=_configSubmaskAddress[n];
	}	
}
	
void configGatewayAddressGet(unsigned char *gatewayAddress)
{
	unsigned char n;
	for (n=0;n<4;n++)
	{	
		gatewayAddress[n]=_configGatewayAddress[n];
	}	
}
	
void configNombreEdificioGet(unsigned char *nombreEdificio)
{
	unsigned char n;
	for (n=0;n<16;n++)
	{
		nombreEdificio[n]=_configNombreEdificio[n];
	}
}
	
void configNombreLineaGet(unsigned char *nombreLinea)
{
	unsigned char n;
	for (n=0;n<16;n++)
	{
		nombreLinea[n]=_configNombreLinea[n];
	}
}
	
void configAdminPassGet(unsigned char *adminPass)
{
	unsigned char n;
	for (n=0;n<16;n++)
	{
		adminPass[n]=_configAdminPass[n];
	}
}
	
void configMantenedorPassGet(unsigned char *mantenedorPass)
{
	unsigned char n;
	for (n=0;n<16;n++)
	{
		mantenedorPass[n]=_configMantenedorPass[n];
	}
}




