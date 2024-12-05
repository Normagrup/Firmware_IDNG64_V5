#include "macros.h"
#include "daliLoop.h"

#define	CMD_DAPC										0
#define	CMD_OFF											1
#define	CMD_RECALL_MAX_LEVEL				6
#define	CMD_RECALL_MIN_LEVEL				7

#define DEST_G0 										0x81		//10000001
#define DEST_G1 										0x83
#define DEST_G2 										0x85
#define DEST_G3 										0x87
#define DEST_G4 										0x89	
#define DEST_G5 										0x8B
#define DEST_G6 										0x8D
#define DEST_G7 										0x8F
#define DEST_G8 										0x91
#define DEST_G9 										0x93
#define DEST_G10 										0x95
#define DEST_G11										0x97
#define DEST_G12										0x99
#define DEST_G13										0x9B
#define DEST_G14										0x9D
#define DEST_G15										0x9F		//10011111
#define DEST_BC											0xFF

#define DEST_TIPO_NINGUNO						0x00
#define DEST_TIPO_INDIVIDUAL				0x01
#define DEST_TIPO_GRUPO							0x02
#define DEST_TIPO_BC								0x03


extern unsigned char _configDaliLoopGroupLow[64];
extern unsigned char _configDaliLoopGroupHigh[64];
extern unsigned char _actualLevelNotif;


extern unsigned char daliLoopActualLevel[64];


unsigned char actualLevelDestTipoGet(unsigned char dest)
{
	unsigned char destTipo=DEST_TIPO_NINGUNO;	
	if ((dest&0x81)==0x01)		destTipo=DEST_TIPO_INDIVIDUAL;
	if ((dest&0xE1)==0x81) 		destTipo=DEST_TIPO_GRUPO;
	if (dest==0xFF)						destTipo=DEST_TIPO_BC;
	return destTipo;	
}

unsigned char actualLevelGrupoGet (unsigned char dest)
{
	unsigned char grupo;
	if (dest==DEST_G0)							grupo=0;			
	if (dest==DEST_G1)							grupo=1;			
	if (dest==DEST_G2)							grupo=2;			
	if (dest==DEST_G3)							grupo=3;			
	if (dest==DEST_G4)							grupo=4;			
	if (dest==DEST_G5)							grupo=5;			
	if (dest==DEST_G6)							grupo=6;			
	if (dest==DEST_G7)							grupo=7;			
	if (dest==DEST_G8)							grupo=8;			
	if (dest==DEST_G9)							grupo=9;			
	if (dest==DEST_G10)							grupo=10;			
	if (dest==DEST_G11)							grupo=11;			
	if (dest==DEST_G12)							grupo=12;			
	if (dest==DEST_G13)							grupo=13;			
	if (dest==DEST_G14)							grupo=14;			
	if (dest==DEST_G15)							grupo=15;	
	return grupo;	
}




unsigned char actualLevelAddressPerteneceGrupoGet(unsigned char address, unsigned char grupo)
{
	unsigned char retVal=0x00;
	if (grupo<8)
	{		
		
		if (BITVAL(_configDaliLoopGroupLow[address], grupo)==0x01)		
		//if ((_configDaliLoopGroupLow[address]&(1<<grupo))==(1<<grupo))
		{		
			retVal=0x01;		
		}
	}
	else
	{
		if (BITVAL(_configDaliLoopGroupHigh[address], (grupo-8))==0x01)		
		//if ((_configDaliLoopGroupHigh[address]&(1<<(grupo-8)))==(1<<(grupo-8)))									
		{
			retVal=0x01;
		}
	}		
	return retVal;
}





unsigned char actualLevelLvlToStoreGet (unsigned char cmd, unsigned char level)
{
	unsigned char retVal;
	if (cmd==CMD_OFF)
	{	
		retVal=0;
	}	
	if (cmd==CMD_RECALL_MIN_LEVEL)
	{
		retVal=1;	//Aquí iría el nivel mínimo de la luminaria en cuestión.
	}	
	if (cmd==CMD_RECALL_MAX_LEVEL)
	{
		retVal=254;
	}	
	if (cmd==CMD_DAPC)
	{
		retVal=level;
	}	
	return retVal;	
}



























// Función que me devuelve cuantos grupos quedan afectados por un comando.
// Por ejemplo si envio broadcast quedarían afectados todos los grupos.
// Si envio un comando a un grupo sólo ese grupo.
// Si envio un comando a una luminaria en concreto podrían estar todos o ninguno.
// una vez que un grupo está afectado tendría que mirar en las acciones si hay alguna que tenga asociado basculante o regulacion y coincida el grupo.
// para a continuación emitir una notificación de cambio por cada grupo.
// Quizás esta función se puede unir a la anterior.
void actualLevelGruposPerteneceGet(unsigned char address, unsigned char *grupos)
{	
	unsigned char n;
	for (n=0;n<8;n++)
	{
		if (BITVAL(_configDaliLoopGroupLow[address], n)==0x01)
		//if ((_configDaliLoopGroupLow[address]&(1<<n))==(1<<n))
		{
			grupos[n]=0x01;
		}
		else
		{
			grupos[n]=0x00;
		}
	}
	for (n=8;n<16;n++)
	{
		if (BITVAL(_configDaliLoopGroupHigh[address], (n-8))==0x01)
		//if ((_configDaliLoopGroupHigh[address]&(1<<(n-8)))==(1<<(n-8)))
		{
			grupos[n]=0x01;
		}
		else
		{
			grupos[n]=0x00;
		}
	}	
}

void actualLevelGruposAfectadosGet(unsigned char dest, unsigned char *gruposAfectados)
{	
	unsigned char destTipo;	
	unsigned char n;		
	unsigned char grupo;	
	destTipo=actualLevelDestTipoGet(dest);		
	if (destTipo==DEST_TIPO_BC)	
	{
		// Marco todos los grupos.
		for (n=0;n<16;n++)
		{
			gruposAfectados[n]=0x01;
		}
	}	
	if (destTipo==DEST_TIPO_GRUPO)	
	{
			// Marco sólo el grupo que toca.
			grupo=actualLevelGrupoGet(dest);
			for (n=0;n<16;n++)
			{				
				if (n==grupo)
				{
					gruposAfectados[n]=0x01;
				}
				else
				{
					gruposAfectados[n]=0x00;
				}			
			}
	}	
	if (destTipo==DEST_TIPO_INDIVIDUAL) 												
	{
		// Obtengo la matriz de grupos a los que pertenece ese equipo.
		actualLevelGruposPerteneceGet(((dest&0x7E)>>1),&gruposAfectados[0]);		
	}
}




void actualLevelCmdNotify (unsigned char dest, unsigned char cmd, unsigned char level)
{
	unsigned char grupo;
	unsigned char destTipo;	
	unsigned char levelToStore;
	unsigned char n;	
	
	unsigned char gruposAfectados[16];
	
	// El primer paso es analizar el tipo de comando que te llega y devolver si es un comando a EQUIPOS INDIVIDUALES, un comando a GRUPOS, ó un comando BROADCAST o no es nada.
	destTipo=actualLevelDestTipoGet(dest);	
	levelToStore=actualLevelLvlToStoreGet (cmd, level);	
	
	// >> GRABAR EN LA MATRIZ LOS NIVELES.	
		if (destTipo==DEST_TIPO_INDIVIDUAL) 												
		{
			daliLoopActualLevelSet(((dest&0x7E)>>1), levelToStore);
			daliLoopActualLevelDiscard(3);
		}
		if (destTipo==DEST_TIPO_GRUPO)	
		{
				grupo=actualLevelGrupoGet(dest);
				for (n=0;n<64;n++)
				{				
					if (actualLevelAddressPerteneceGrupoGet(n,grupo)==0x01)		
					{
						daliLoopActualLevelSet(n, levelToStore);

					}
				}
										daliLoopActualLevelDiscard(3);
		}
		if (destTipo==DEST_TIPO_BC)	
		{
			for (n=0;n<64;n++)
			{
				daliLoopActualLevelSet(n, levelToStore);
				
			}	
daliLoopActualLevelDiscard(3);			
		}		
	// << GRABAR EN LA MATRIZ LOS NIVELES.	
		
	
		
		actualLevelGruposAfectadosGet(dest, &gruposAfectados[0]);			//Obtengo los grupos afectados por el comando.
		sensoresCambioActualLevelNotificar(&gruposAfectados[0]);			//Notificará los NS de los interfaces afectados.
		
		
		
		
		
		
		
		
		
	
	
	
	
	
	_actualLevelNotif=0x01;
}


























// La matriz de grupos debe tener coherencia con los grupos actuales.
// ¿Equipos que no estén o fallo comunicación?.
void actualLevel_GrupoValoresGet (unsigned char grupo, unsigned char *valores)
{
	unsigned char 				equipos=0;
	unsigned char 				apagados=0;
	unsigned short int		avgAcu=0;	
	unsigned char					actualLevel;	
	unsigned char 				n;	
	for (n=0;n<64;n++)
	{
		if (actualLevelAddressPerteneceGrupoGet(n, grupo)==0x01)
		{			
			if (daliLoopActualLevelValidGet(n)==0x01)
			{
					equipos++;
					actualLevel=daliLoopActualLevelGet(n);				
			
			//		actualLevel=daliLoopActualLevel[n];
			
					avgAcu=avgAcu+actualLevel;				
					if (actualLevel==0)
					{
						apagados++;
					}							
			}			
		}		
	}		
	if (equipos>0)
	{	
		valores[0]=equipos;
		valores[1]=apagados;	
		valores[2]=(unsigned char)(avgAcu/equipos);
	}
	else
	{
		valores[0]=0;
		valores[1]=0;	
		valores[2]=0;	
	}
}	














