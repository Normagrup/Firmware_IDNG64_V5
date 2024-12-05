#include "normaLink62386P102.h"
#include "normaLink62386P202.h"
#include "normaLinkCfgInfoPasarela.h"
#include "normaLinkSingleLoop.h"
#include "normaLinkCmdPasarela.h"
#include "normaLinkDaliUtils.h"
#include "normalink.h"
#include "w5100/socket.h"
#include "w5100/w5100.h"
#include "mainCfg.h"

#define PORT 2234

// Received packet
unsigned char ipDest[4];					//Ip del host que me envía el paquete
unsigned int portFrom;						//Puerto del host que me envía el paquete
unsigned int portDest=PORT;				//Puerto del host que al que enviaré el paquete
unsigned int packetSize;					//Tamaño del paquete recibido
unsigned char packetHeader[8];		// Cabecera del paquete
unsigned char recibireth[320];		// Búffer para paquete entrante



extern unsigned char	_daliUtilsBlink;
extern unsigned char	_daliUtilsBlinkNotif;
extern unsigned char _daliUtilsBlinkDest;
extern unsigned char _broadcastAddress[4];	
extern unsigned char _configIpAddress[4];
extern unsigned char _configSubmaskAddress[4];
		


void ethSendFrame(unsigned char socket, unsigned char *buffer, unsigned int frameLenght, unsigned char *address, unsigned int port)
{
	unsigned char frame[300];
	unsigned int n;
	unsigned char crc=0;
	frame[0]=0x54;
	frame[1]=0x43;
	frame[2]=0x97;
	for (n=0;n<frameLenght;n++)
	{
		frame[n+3]=buffer[n];
		crc+=buffer[n];
	}	
	frame[frameLenght+3]=crc;	
	sendto(socket,&frame[0],(frameLenght+4),&address[0],port);		// Send Answer if needed				
}


void ethSendBlinkNotif (void)
{
		unsigned char respuesta[8];		
		respuesta[0]=0x04;
		respuesta[1]=0x00;
		respuesta[2]=0xe4;
		respuesta[3]=0x22;
		respuesta[4]=0xe4;
		respuesta[5]=0x22;
		respuesta[6]=0x00;
		respuesta[7]=_daliUtilsBlinkDest;	
		ethSendFrame(0,&respuesta[0],8,&_broadcastAddress[0], 2234);
}

void ethSendACK (unsigned char *packetId, unsigned char *ipDest)
{
		unsigned char respuesta[5];
		respuesta[0]=0x82;
		respuesta[1]=packetId[0];
		respuesta[2]=packetId[1];
		respuesta[3]=0x01;
		respuesta[4]=0x00;
		ethSendFrame(0,&respuesta[0],5,&ipDest[0], 2234);
}





unsigned char ethProcessFrame (unsigned char *recibireth, unsigned int rxPacketSize)
{
	unsigned char _dest;
	unsigned char _data;		
	unsigned char _comL;
	unsigned char _comH;
	unsigned char _tipoDeFrame;
	_tipoDeFrame=recibireth[0];	
	if (_tipoDeFrame==0x00)
	{
		_comH=recibireth[4];
		_comL=recibireth[5];			
		
		if (_comH==0x00)
		{
			normaLinkCfgInfoPasarelaT0(_comL);				
		}			
		
		if (_comH==0x01)
		{
			normaLinkCmdPasarelaT0(_comL);				
		}			
	
		if (_comH==0x40)
		{
			normaLinkSingleLoopT0(_comL);				
		}		
	}
	
	
	if (_tipoDeFrame==0x01)
	{
		_dest=recibireth[4];	
		_comH=recibireth[5];
		_comL=recibireth[6];		
		if (normaLinkDestCorrecto(_dest)==0x01)
		{		
			if (_comH==0x63)
			{
				normaLink62386P102T1(_comL, _dest);	
			}
			
			if (_comH==0x64)
			{
				normaLink62386P202T1(_comL, _dest);	
			}
			
			if (_comH==0xE4)
			{
				normaLinkDaliUtilsT1(_comL, _dest);	
			}
		}	
	}
	
	if (_tipoDeFrame==0x03)
	{
		_dest=recibireth[4];	
		_comH=recibireth[5];
		_comL=recibireth[6];				
		_data=recibireth[7];	
		if (normaLinkDestCorrecto(_dest)==0x01)
		{		
			if (_comH==0x63)
			{
					normaLink62386P102T3(_comL,_dest, _data);
			}
			
			if (_comH==0xE4)
			{
				normaLinkDaliUtilsT3(_comL, _dest,_data);	
			}
		}	
	}

	if (_tipoDeFrame==0x05)
	{
		_comH=recibireth[4];
		_comL=recibireth[5];			
		
		if (_comH==0x01)		normaLinkCmdPasarelaT5(_comL);
	}
	
	
	if (_tipoDeFrame==0x04)
	{
		_comH=recibireth[4];
		_comL=recibireth[5];			
		
		if (_comH==0x00)
		{
			normaLinkCfgInfoPasarelaT4(_comL);				
		}					
		
		if (_comH==0x65) normaLink62386209T4(_comL);	
		
		
		
		if (_comH==0x40)
		{
			normaLinkSingleLoopT4(_comL);				
		}					
		
		if (_comH==0xE4)
		{
			normaLinkDaliUtilsT4(_comL);	
		}
		
		if (_comH==0x77)
		{
			normaLinkSensores(_comL);	
		}
		
		
		if ((_comH==0x78) && (_comL==0x01))
		{
			normaLinkMasterPing(&ipDest[0]);
		}
		
	}		
	return 1;	
}


void ethDoTask (void)
{
			unsigned int rsize=0;
			unsigned char rawEth[512];
			unsigned int n;
			unsigned char crc;
	
			if ((_daliUtilsBlink==1) && (_daliUtilsBlinkNotif==1))
			{
				_daliUtilsBlinkNotif=0;
				ethSendBlinkNotif();
			}
	
	
			rsize=getSn_RX_RSR(0);			
			if (rsize>8)
			{
				ethTransceiverTiempoSinRecibirCntRst();
				packetHeader[0]=0;
				packetHeader[1]=0;
				packetHeader[2]=0;
				packetHeader[3]=0;
				packetHeader[5]=0;
				packetHeader[6]=0;
				packetHeader[7]=0;
				recv(0,packetHeader,8);																	// Get packetHeader
				ipDest[0]=packetHeader[0];															// Get ip
				ipDest[1]=packetHeader[1];
				ipDest[2]=packetHeader[2];
				ipDest[3]=packetHeader[3];
				portFrom=(packetHeader[4]*256)+packetHeader[5];					// Get port
				packetSize=(packetHeader[6]*256)+packetHeader[7];				// Get packet size
				
				if (packetSize>512)	packetSize=512;
				
				recv(0,rawEth,packetSize);															// Get packet
				
				#ifdef IGNORE_SUBNET_FILTER
					if ((rawEth[0]==0x54) && (rawEth[1]==0x43) && (rawEth[2]==0x97))
				#else
					if ((rawEth[0]==0x54) && (rawEth[1]==0x43) && (rawEth[2]==0x97) &&	((_configIpAddress[0]&_configSubmaskAddress[0])==(ipDest[0]&_configSubmaskAddress[0])) && ((_configIpAddress[1]&_configSubmaskAddress[1])==(ipDest[1]&_configSubmaskAddress[1])) &&	((_configIpAddress[2]&_configSubmaskAddress[2])==(ipDest[2]&_configSubmaskAddress[2])) && ((_configIpAddress[3]&_configSubmaskAddress[3])==(ipDest[3]&_configSubmaskAddress[3])))				
				#endif			
				{
						crc=0;
						for (n=0;n<(packetSize-4);n++)
						{
							recibireth[n]=rawEth[n+3];
							crc+=recibireth[n];							
						}
						
						
						#ifdef ETH_IGNORE_CRC
							ethProcessFrame(&recibireth[0],(packetSize-4));
						#else						
							if (crc==rawEth[packetSize-1])
							{
								ethProcessFrame(&recibireth[0],(packetSize-4));
							}
							else
							{
								//CRC incorrecto
							}						
						#endif
						
				}
				else
				{
					//Preambulo Incorrecto
				}
			}
			// End of Ethernet Processing
}

