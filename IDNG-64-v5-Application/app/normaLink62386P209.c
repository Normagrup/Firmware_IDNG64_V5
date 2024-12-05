#include "normalink.h"
#include "daliLoop.h"
#include "systick.h"
#include "dataTypes.h"
#include "dali.h"
#include "macros.h"
#include "actualLevel.h"

#define INC_SENSORES_GRUPOS_REGULADORES_LUZ_PRT
#define INC_SENSORES_GRUPOS_REGULADORES_LUZ_ESTADOS_E
	#include "sensoresGrupos/sensoresGruposReguladoresLuz.h"
#undef INC_SENSORES_GRUPOS_REGULADORES_LUZ_PRT
#undef INC_SENSORES_GRUPOS_REGULADORES_LUZ_ESTADOS_E

#define INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA_PRT
#define INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA_ESTADOS_E
	#include "sensoresGrupos/sensoresGruposPulsadoresAccionTemporizada.h"
#undef INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA_PRT
#undef INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA_ESTADOS_E

#define INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_PRT
#define INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_ESTADOS_E
	#include "sensoresGrupos/sensoresGruposDetectoresPresencia.h"
#undef INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_PRT
#undef INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_ESTADOS_E

#define INC_SENSORES_GRUPOS_PRT
	#include "sensoresGrupos/sensoresGrupos.h"
#undef INC_SENSORES_GRUPOS_PRT

#define CIRC_ESTADO_NO_OPERATIVO	0x00
#define CIRC_ESTADO_FUNCIONANDO		0x01
#define CIRC_ESTADO_DETENIDO			0x02


void normaLink62386209T4(unsigned char comL);

extern unsigned char	recibireth[320];		// Búffer para paquete entrante
extern unsigned char	ipDest[4];	//Ip del host que me envía el paquete
UINT16 								_circMatrizGruposInhibidos[2];		// Circadiano inhibido [0] nivel de luz [1] tc.
unsigned char					_circEstado[16];
unsigned char					_circGrupoCnt;

void circGruposInhibidosSet (unsigned char grupo, unsigned char nivTemp)
{
	if ((grupo<16) && (nivTemp<=1))
	{	
		SETBIT(_circMatrizGruposInhibidos[nivTemp], grupo);	
	}
}

void circGruposInhibidosClear (unsigned char grupo, unsigned char nivTemp)
{
	if ((grupo<16) && (nivTemp<=1))
	{	
		CLEARBIT(_circMatrizGruposInhibidos[nivTemp], grupo);	
	}
}
	
// Se le llama desde el pulsador de activacion / desactivación.
// Si la inhibición de luz o tc está activada desactiva.
// Si ninguna inhibición está activada entonces activa las dos.
void circGruposInhibidosToggle (unsigned char grupo)
{		
	if (grupo<16)
	{	
		if ((BITVAL(_circMatrizGruposInhibidos[0], grupo)==1) || (BITVAL(_circMatrizGruposInhibidos[1], grupo)==1))
		{
			CLEARBIT(_circMatrizGruposInhibidos[0], grupo);
			CLEARBIT(_circMatrizGruposInhibidos[1], grupo);		
		}
		else
		{
			SETBIT(_circMatrizGruposInhibidos[0], grupo);
			SETBIT(_circMatrizGruposInhibidos[1], grupo);		
		}
	}
}

void circActualizar (void)
{	
	unsigned char grupo;	
	grupo=_circGrupoCnt;	
	if (_circEstado[grupo]==CIRC_ESTADO_FUNCIONANDO)
	{
		if ((_sensoresGruposAlgunEquipoApagadoGet(grupo)==TRUE) || (sensoresGruposDetectoresPresenciaEstadoGet(grupo)==DETECTORES_PRESENCIA_ESTADO_VACANCY) || (sensoresGruposPulsadoresAccionTemporizadaEstadoGet(grupo)==PULSADORES_ACCION_TEMPORIZADA_ESTADO_DETENIDO))
		{
			_circEstado[grupo]=CIRC_ESTADO_DETENIDO;			
		}		
	}	
	else if (_circEstado[grupo]==CIRC_ESTADO_DETENIDO)
	{
		if (!((_sensoresGruposAlgunEquipoApagadoGet(grupo)==TRUE) || (sensoresGruposDetectoresPresenciaEstadoGet(grupo)==DETECTORES_PRESENCIA_ESTADO_VACANCY) || (sensoresGruposPulsadoresAccionTemporizadaEstadoGet(grupo)==PULSADORES_ACCION_TEMPORIZADA_ESTADO_DETENIDO)))
		{
			_circEstado[grupo]=CIRC_ESTADO_FUNCIONANDO;
			CLEARBIT(_circMatrizGruposInhibidos[0], grupo);
			CLEARBIT(_circMatrizGruposInhibidos[1], grupo);				
		}		
	}	
	if (_circGrupoCnt<15)
	{
		_circGrupoCnt++;
	}
	else
	{
		_circGrupoCnt=0;
	}	
}



unsigned char _auxEnvia1=0;
unsigned char _auxEnvia2=0;


void circDoTask (void)
{
	circActualizar();
	
	if (_auxEnvia1==1)
	{
		_auxEnvia1=0;		
		daliSensoresTcStepUpEnviar (3);
	}
	
	if (_auxEnvia2==1)
	{
		_auxEnvia2=0;
		daliSensoresTcStepDownEnviar (4);
	}
	
}

void circInit (void)
{
	unsigned char grupo;	
	for (grupo=0;grupo<16;grupo++)
	{
		_circEstado[grupo]=CIRC_ESTADO_FUNCIONANDO;		
	}	
}

void normaLink62386209T4(unsigned char comL)
{	
	unsigned short int 	packetId;
	unsigned short int 	lenghtRx;
	unsigned char 		 	dest;
	unsigned char 			mirekH[16];
	unsigned char 			mirekL[16];
	unsigned char 			arc[16];
	unsigned char				destArray[DALI_BUFFER_LENGHT];
	unsigned char				comArray[DALI_BUFFER_LENGHT];
	unsigned char 			n;	
	unsigned char				ptr=0;	
	unsigned char				scene;
	unsigned char				frameType;
	unsigned char				grupo;
	unsigned char				skip=1;
	packetId=recibireth[2]*0x0100+recibireth[3];	
	lenghtRx=recibireth[6];	
	dest=recibireth[7];	
	for (n=0;n<DALI_BUFFER_LENGHT;n++)		destArray[n]=0xFF;
	for (n=0;n<DALI_BUFFER_LENGHT;n++)		comArray[n]=0xFF;	
	for (n=0;n<16;n++)										mirekH[n]=0xFF;
	for (n=0;n<16;n++)										mirekL[n]=0xFF;
	for (n=0;n<16;n++)										arc[n]=0xFF;		
	// GOTO COLOR AND LEVEL
	// IND [4] {destino[1], Mirek[2], nivel[1]}
	// 16*{Mirek[2], nivel[1]}
	if (((comL==0xE7) || (comL==0xE8)) && ((lenghtRx==3) || (lenghtRx==47)))
	{
		// >> Get parameters.
			if (lenghtRx==3)
			{			
				dest=recibireth[7];				// Get Dest
				mirekH[0]=recibireth[8];	// Get MirekH
				mirekL[0]=recibireth[9];	// Get MirekL
				arc[0]=recibireth[10];		// Get arc				
				frameType=0x18;
			}
			else 
			{
				for (n=0;n<16;n++)
				{				
					mirekH[n]=recibireth[(7+(3*n)+0)];	
					mirekL[n]=recibireth[(7+(3*n)+1)];	
					arc[n]=recibireth[(7+(3*n)+2)];
				}
				frameType=0x19;
			}
		// << Get parameters.
			
		// >> Buffer.			
		if (lenghtRx==3)
		{			
			n=0;
			destArray[ptr]=mirekH[n];	// mirekH				
			ptr++;				
			destArray[ptr]=mirekL[n];	// mirekL				
			ptr++;					
			destArray[ptr]=dest;			// Set Temporary Tc Dest
			ptr++;
			destArray[ptr]=arc[n];		// Arc arc level.
			ptr++;
			// >> INHIBICIONES DEL COMANDO MANUAL.				
				if ((comL==0xE7) && ((dest&0xE1)==0x81))
				{					
					grupo=((dest&0x1E)>>1);					
					if (arc[n]!=0xFF)	
					{
						SETBIT(_circMatrizGruposInhibidos[0], grupo);						
						if (sensoresGruposReguladoresLuzEstadoGet(grupo)==REGULADORES_LUZ_ESTADO_FUNCIONANDO)
						{
							sensoresGruposReguladoresLuzInhibir(grupo);
						}						
						actualLevelCmdNotify((0x81|(n<<1)), 0x00, arc[n]);
					}					
					if ((mirekH[n]!=0xFF) || (mirekL[n]!=0xFF))
					{	
						SETBIT(_circMatrizGruposInhibidos[1], grupo);						
					}
					skip=0;					
				}
				else
				{
					actualLevelCmdNotify(dest, 0x00, arc[n]);
					skip=0;						
				}
			// << INHIBICIONES DEL COMANDO MANUAL.		
			
			// >> INHIBICIONES DEL CIRCADIANO.
				if ((comL==0xE8) && ((dest&0xE1)==0x81))
				{
					grupo=((dest&0x1E)>>1);												
					if (_circEstado[grupo]==CIRC_ESTADO_FUNCIONANDO)
					{
						// Inhibiciones tc.
						if (BITVAL(_circMatrizGruposInhibidos[1], grupo)==1)
						{
							mirekH[n]=0xFF;
							mirekL[n]=0xFF;					
						}								
						// Inhibiciones arco.				
						if (BITVAL(_circMatrizGruposInhibidos[0], grupo)==1)	
						{				
							arc[n]=0xFF;
						}
						skip=0;
					}
					else
					{
						arc[n]=0xFF;
						mirekH[n]=0xFF;
						mirekL[n]=0xFF;					
					}
				}
			// << INHIBICIONES DEL CIRCADIANO.			
		}
		else
		{
			for (n=0;n<16;n++)
			{	
				// >> INHIBICIONES DEL COMANDO MANUAL.				
					if (comL==0xE7)
					{					
						grupo=n;						
						if (arc[n]!=0xFF)	
						{
							SETBIT(_circMatrizGruposInhibidos[0], grupo);						
							if (sensoresGruposReguladoresLuzEstadoGet(grupo)==REGULADORES_LUZ_ESTADO_FUNCIONANDO)
							{
								sensoresGruposReguladoresLuzInhibir(grupo);
							}						
							actualLevelCmdNotify((0x81|(n<<1)), 0x00, arc[n]);
						}					
						if ((mirekH[n]!=0xFF) || (mirekL[n]!=0xFF))
						{	
							SETBIT(_circMatrizGruposInhibidos[1], grupo);						
						}
						skip=0;
					}				
				// << INHIBICIONES DEL COMANDO MANUAL.
				// >> INHIBICIONES DEL CIRCADIANO.
					if (comL==0xE8)
					{
						grupo=n;						
						if (_circEstado[grupo]==CIRC_ESTADO_FUNCIONANDO) 
						{
							// Inhibiciones arco.				
							if (BITVAL(_circMatrizGruposInhibidos[0], grupo)==1)	
							{				
								arc[n]=0xFF;
							}				
							// Inhibiciones tc.
							if (BITVAL(_circMatrizGruposInhibidos[1], grupo)==1)
							{
								mirekH[n]=0xFF;
								mirekL[n]=0xFF;					
							}							
							skip=0;
						}
						else
						{
							arc[n]=0xFF;
							mirekH[n]=0xFF;
							mirekL[n]=0xFF;					
						}
					}					
				// << INHIBICIONES DEL CIRCADIANO.			
				destArray[ptr]=mirekH[n];	// mirekH				
				ptr++;				
				destArray[ptr]=mirekL[n];	// mirekL	
				ptr++;
				destArray[ptr]=arc[n];		// Arc arc level.
				ptr++;					
			}	
			frameType=0x19;
		}		
	// << Buffer.				
		if (skip==0)
		{		
			normalinkBufferDaliSend(&ipDest[0],packetId,frameType, &destArray[0], &comArray[0]);					
		}
	}	
	
	// STORE COLOR SCENE
	// IND [5] {scene[1], destino[1], Mirek[2], nivel[1]}
	// scene[1], 16*{Mirek[2], nivel[1]}
	if (comL==0xE9)
	{
		if (lenghtRx==4)
		{
			// >> Get parameters.
				scene=recibireth[7];			// Get Scene
				dest=recibireth[8];				// Get Dest
				mirekH[0]=recibireth[9];	// Get MirekH
				mirekL[0]=recibireth[10];	// Get MirekL
				arc[0]=recibireth[11];		// Get arc
			// << Get parameters.			
			// >> Frame					
				destArray[ptr]=scene;								// scene
				ptr++;
				destArray[ptr]=dest;								// dest
				ptr++;
				destArray[ptr]=mirekH[0];						// mirekH
				ptr++;
				destArray[ptr]=mirekL[0];						// mirekL
				ptr++;
				destArray[ptr]=arc[0];							// arc
				ptr++;							
				frameType=0x20;			
			// << Frame				
			
		}
		else if (lenghtRx==48)
		{
			// >> Get parameters
				scene=recibireth[7];
				for (n=0;n<16;n++)
				{				
					mirekH[n]=recibireth[(8+(3*n)+0)];	
					mirekL[n]=recibireth[(8+(3*n)+1)];	
					arc[n]=recibireth[(8+(3*n)+2)];
				}
			// << Get parameters
			
			// >> Frame		
				destArray[ptr]=scene;							//	scene
				ptr++;				
				for (n=0;n<16;n++)
				{			
					destArray[ptr]=mirekH[n];				//	mirek H
					ptr++;
					destArray[ptr]=mirekL[n];				// 	mirel L
					ptr++;
					destArray[ptr]=arc[n];					//	arc
					ptr++;							
				}
				frameType=0x21;
			// << Frame					
		}
		normalinkBufferDaliSend(&ipDest[0],packetId,frameType, &destArray[0], &comArray[0]);		
	}
	
	if ((comL==0xD0) && (lenghtRx==0))
	{		
		unsigned char j;
		if (recibireth[7]==0xFF)
		{			
			for (j=0;j<16;j++)
			{			
				circGruposInhibidosSet(j, 0);
				circGruposInhibidosSet(j, 1);
			}
		}
		else if (recibireth[7]<16)
		{
			circGruposInhibidosSet(recibireth[7], 0);
			circGruposInhibidosSet(recibireth[7], 1);
		}		
	}
	
	if ((comL==0xD1) && (lenghtRx==0))
	{
		unsigned char j;
		if (recibireth[7]==0xFF)
		{			
			for (j=0;j<16;j++)
			{			
				circGruposInhibidosClear(j, 0);
				circGruposInhibidosClear(j, 1);
			}
		}
		else if (recibireth[7]<16)
		{
			circGruposInhibidosClear(recibireth[7], 0);
			circGruposInhibidosClear(recibireth[7], 1);
		}				
	}
	
	
	/*
	if (comL==0xE2)					//	ACTIVATE.
	{
		for (n=0;n<64;n++)		destArray[n]=0xFF;
		for (n=0;n<64;n++)		comArray[n]=0xFF;		
		dest=0x01;		
		destArray[1]=0xC1;
		comArray[1]=0x08;	
		destArray[0]=dest;
		comArray[0]=0xE2;		
		normalinkBufferDaliSend(&ipDest[0],packetId, &destArray[0], &comArray[0]);		
	}	
	
	if (comL==0xE7)	// Set temporary colour Tc 2 bytes [Activate YES/NO].
	{
		mirekH=recibireth[8];
		mirekL=recibireth[9];		
		activate=recibireth[10];					
		for (n=0;n<64;n++)		destArray[n]=0xFF;
		for (n=0;n<64;n++)		comArray[n]=0xFF;		
		dest=recibireth[7];			
		mirekH=recibireth[8];	
		mirekL=recibireth[9];	
		arc=recibireth[10];	
		activate=recibireth[11];	
		
		destArray[6]=0xC3;
		comArray[6]=mirekH;
		destArray[5]=0xA3;
		comArray[5]=mirekL;
		destArray[4]=0xC1;
		comArray[4]=0x08;		
		destArray[3]=dest;
		comArray[3]=0xE7;		
	
		if (activate==0x01)
		{
			destArray[2]=0xC1;
			comArray[2]=0x08;	
			destArray[1]=dest;
			comArray[1]=0xE2;			
		}
		destArray[0]=dest&0xFE;
		comArray[0]=arc;			
		normalinkBufferDaliSend(&ipDest[0],packetId, &destArray[0], &comArray[0]);		
	}
	*/
	if (comL==0xd8)	// tc step colder.
	{
		for (n=0;n<64;n++)		destArray[n]=0xFF;
		for (n=0;n<64;n++)		comArray[n]=0xFF;		
		dest=0x01;		
		destArray[3]=0xC1;
		comArray[3]=0x08;		
		destArray[2]=dest;
		comArray[2]=0xE8;
		frameType=0x17;	
		normalinkBufferDaliSend(&ipDest[0],packetId,frameType,&destArray[0],&comArray[0]);		
	}
	
	if (comL==0xd9)				// tc step warm
	{
		for (n=0;n<64;n++)		destArray[n]=0xFF;
		for (n=0;n<64;n++)		comArray[n]=0xFF;		
		dest=0x01;		
		destArray[3]=0xC1;
		comArray[3]=0x08;		
		destArray[2]=dest;
		comArray[2]=0xE9;
		frameType=0x17;		
		normalinkBufferDaliSend(&ipDest[0],packetId,frameType,&destArray[0],&comArray[0]);		
	}	
	
}
