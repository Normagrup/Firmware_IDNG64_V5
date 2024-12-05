#include "normalink.h"
#include "daliLoop.h"
#include "systick.h"
#include "dataTypes.h"


extern unsigned char recibireth[320];		// Búffer para paquete entrante
extern unsigned char ipDest[4];	//Ip del host que me envía el paquete
extern unsigned char _configDaliLoopGroupLow[64];
extern unsigned char _configDaliLoopGroupHigh[64];


#define BIT(x) 				(1 << (x))
#define SETBITS(x,y) 		((x) |= (y))
#define CLEARBITS(x,y) 		((x) &= (~(y)))
#define SETBIT(x,y) 		SETBITS((x), (BIT((y))))
#define CLEARBIT(x,y) 		CLEARBITS((x), (BIT((y))))
#define BITSET(x,y) 		((x) & (BIT(y)))
#define BITCLEAR(x,y) 		!BITSET((x), (y))
#define BITSSET(x,y) 		(((x) & (y)) == (y))
#define BITSCLEAR(x,y) 		(((x) & (y)) == 0)
#define BITVAL(x,y) 		(((x)>>(y)) & 1)

void Update_Groups(unsigned char com, unsigned char dest)
{
	if(com>=0x61 && com<=0x70)
	{
		if(com<=0x68)
		{
			if(dest==255)
			{
				for (unsigned char i=0;i<64;i++)
				{
					SETBIT(_configDaliLoopGroupLow[i],(com-0x61));
				}
			}
			else if (dest<=127)
			{
				SETBIT(_configDaliLoopGroupLow[((dest-1)>>1)],(com-0x61));
			}
		}
		else if(com>0x68)
		{
			if(dest==255)
			{
				for (unsigned char i=0;i<64;i++)
				{
					SETBIT(_configDaliLoopGroupHigh[i],(com-0x69));
				}
			}
			else if (dest<=127)
			{
				SETBIT(_configDaliLoopGroupHigh[((dest-1)>>1)],(com-0x69));
			}		
		}
	}
	else if(com>=0x71 && com<=0x80)
	{
		if(com<=0x78)
		{
			if(dest==255)
			{
				for (unsigned char i=0;i<64;i++)
				{
					CLEARBIT(_configDaliLoopGroupLow[i],(com-0x71));
				}
			}
			else if (dest<=127)
			{
				CLEARBIT(_configDaliLoopGroupLow[((dest-1)>>1)],(com-0x71));
			}
		}
		else if(com>0x78)
		{
		  if(dest==255)
			{
				for (unsigned char i=0;i<64;i++)
				{
					CLEARBIT(_configDaliLoopGroupHigh[i],(com-0x79));
				}
			}
			else if (dest<=127)
			{
				CLEARBIT(_configDaliLoopGroupHigh[((dest-1)>>1)],(com-0x79));
			}
		}
	}

}

void normaLink62386P102T1(unsigned char _comL, unsigned char _dest)
{
	unsigned int _packetId;
	_packetId=recibireth[2]*0x0100+recibireth[3];
	
	// [001] OFF
	if (_comL==0x01) 
	{			
		polling_PollActualLevel();
		actualLevelCmdNotify(_dest, _comL, 0xFF);		
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x00); 		
	}
	
	// [002] UP
	if (_comL==0x02)
	{				
		polling_PollActualLevel();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x01); 
	}
	
	// [003] DOWN
	if (_comL==0x03)
	{	
		polling_PollActualLevel();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x02); 
	}
	
	// [004] STEP UP
	if (_comL==0x04)
	{				
		polling_PollActualLevel();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x03); 
	}
			
	// [005] STEP DOWN
	if (_comL==0x05)
	{	
		polling_PollActualLevel();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x04); 
	}
				
	// [006] RECALL MAX LEVEL
	if (_comL==0x06)
	{
		polling_PollActualLevel();		
		if (((_dest&0xE1)==0x81) && (_sensoresGruposAlgunEquipoApagadoGet(((_dest&0x1E)>>1))==FALSE))
		{
			circGruposInhibidosSet (((_dest&0x1E)>>1),0);
			sensoresGruposReguladoresLuzInhibir((_dest&0x1E)>>1);
		}		
		actualLevelCmdNotify(_dest, _comL, 0xFF);				
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x05); 
	}
				
	// [007] RECALL MIN LEVEL
	if (_comL==0x07)
	{				
		polling_PollActualLevel();
		if ((_dest&0xE1)==0x81)
		{
			if (((_dest&0xE1)==0x81) && (_sensoresGruposAlgunEquipoApagadoGet(((_dest&0x1E)>>1))==FALSE))
			{
				circGruposInhibidosSet (((_dest&0x1E)>>1),0);
				sensoresGruposReguladoresLuzInhibir((_dest&0x1E)>>1);
			}						
		}	
		actualLevelCmdNotify(_dest, _comL, 0xFF);			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x06); 
	}
			
	// [008] STEP DOWN AND OFF
	if (_comL==0x08)
	{	
		polling_PollActualLevel();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x07); 
	}
				
	// [009] ON AND STEP UP
	if (_comL==0x09)
	{				
		polling_PollActualLevel();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x08); 
	}
	
	// [010] ENABLE DAPC SEQUENCE
	if (_comL==0x10)
	{				
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x09); 
	}	

	// [017] GOTO SCENE 00
	if (_comL==0x11)
	{	
		polling_PollActualLevel();
		//daliLoopActualLevelValidClearAll();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x10); 
	}
			
	// [018] GOTO SCENE 01
	if (_comL==0x12)
	{	
		polling_PollActualLevel();
		//daliLoopActualLevelValidClearAll();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x11); 
	}
			
	// [019] GOTO SCENE 02
	if (_comL==0x13)
	{				
		polling_PollActualLevel();
		//daliLoopActualLevelValidClearAll();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x12); 
	}
			
	// [020] GOTO SCENE 03
	if (_comL==0x14)
	{	
		polling_PollActualLevel();
		//daliLoopActualLevelValidClearAll();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x13); 
	}
			
	// [021] GOTO SCENE 04
	if (_comL==0x15)
	{				
		polling_PollActualLevel();
		//daliLoopActualLevelValidClearAll();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x14); 
	}
			
	// [022] GOTO SCENE 05
	if (_comL==0x16)
	{	
		polling_PollActualLevel();
		//daliLoopActualLevelValidClearAll();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x15); 
	}
			
	// [023] GOTO SCENE 06
	if (_comL==0x17)
	{	
		polling_PollActualLevel();
		//daliLoopActualLevelValidClearAll();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x16); 
	}
			
	// [024] GOTO SCENE 07
	if (_comL==0x18)
	{	
		polling_PollActualLevel();
		//daliLoopActualLevelValidClearAll();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x17); 
	}
			
	// [025] GOTO SCENE 08
	if (_comL==0x19)
	{	
		polling_PollActualLevel();
		//daliLoopActualLevelValidClearAll();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x18); 		
	}	
			
	// [026] GOTO SCENE 09
	if (_comL==0x1A)
	{	
		polling_PollActualLevel();
		//daliLoopActualLevelValidClearAll();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x19); 
	}
			
	// [027] GOTO SCENE 10
	if (_comL==0x1B)
	{				
		polling_PollActualLevel();
		//daliLoopActualLevelValidClearAll();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x1A); 
	}
			
	// [028] GOTO SCENE 11
	if (_comL==0x1C)
	{	
		polling_PollActualLevel();
		//daliLoopActualLevelValidClearAll();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x1B); 
	}
			
	// [029] GOTO SCENE 12
	if (_comL==0x1D)
	{	
		polling_PollActualLevel();
		//daliLoopActualLevelValidClearAll();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x1C); 
	}
			
	// [030] GOTO SCENE 13
	if (_comL==0x1E)
	{	
		polling_PollActualLevel();
		//daliLoopActualLevelValidClearAll();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x1D); 
	}
			
	// [031] GOTO SCENE 14
	if (_comL==0x1F)
	{	
		polling_PollActualLevel();
		//daliLoopActualLevelValidClearAll();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x1E); 
	}
			
	// [032] GOTO SCENE 15
	if (_comL==0x20)
	{	
		polling_PollActualLevel();
		//daliLoopActualLevelValidClearAll();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2B, 0x1F); 
	}
	
	if ((_comL>=0x11) && (_comL<=0x20))
	{
		if ((_dest&0xE1)==0x81)
		{
			circGruposInhibidosSet (((_dest&0x1E)>>1),0);
			circGruposInhibidosSet (((_dest&0x1E)>>1),1);
			sensoresGruposReguladoresLuzInhibir((_dest&0x1E)>>1);
		}
	}
			
	// [033] RESET
	if (_comL==0x21)
	{	
		//daliLoopActualLevelValidClearAll();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x20); 
		sysTickDelay (10);
	}
	
	// [034] STORE ACTUAL LEVEL IN THE DTR
	if (_comL==0x22)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x21); 
	}
	
	// [043] STORE DTR AS MAX LEVEL
	if (_comL==0x2B)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x2A); 
	}
			
	// [044] STORE DTR AS MIN LEVEL
	if (_comL==0x2C)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x2B); 
	}
			
	// [045] STORE DTR AS SYSTEM FAILURE LEVEL
	if (_comL==0x2D)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x2C); 
	}
			
	// [046] STORE DTR AS POWER ON LEVEL
	if (_comL==0x2E)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x2D); 
	}
			
	// [047] STORE DTR AS FADE TIME
	if (_comL==0x2F)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x2E); 
	}
		
	// [048] STORE DTR AS FADE RATE
	if (_comL==0x30)
	{																										
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x2F); 
	}
			
	// [065] STORE DTR AS SCENE 00 
	if (_comL==0x41)
	{																															
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x40); 
	}
			
	// [066] STORE DTR AS SCENE 01
	if (_comL==0x42)
	{																														
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x41); 
	}
		
	// [067] STORE DTR AS SCENE 02
	if (_comL==0x43)
	{																																					
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x42); 
	}
			
	// [068] STORE DTR AS SCENE 03
	if (_comL==0x44)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x43); 
	}
			
	// [069] STORE DTR AS SCENE 04
	if (_comL==0x45)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x44); 
	}
			
	// [070] STORE DTR AS SCENE 05
	if (_comL==0x46)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x45); 
	}
		
	// [071] STORE DTR AS SCENE 06
	if (_comL==0x47)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x46); 
	}
			
	// [072] STORE DTR AS SCENE 07
	if (_comL==0x48)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x47); 
	}
			
	// [073] STORE DTR AS SCENE 08 
	if (_comL==0x49)
	{				
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x48); 
	}
			
	// [074] STORE DTR AS SCENE 09
	if (_comL==0x4A)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x49); 
	}
			
	// [075] STORE DTR AS SCENE 10
	if (_comL==0x4B)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x4A); 
	}
			
	// [076] STORE DTR AS SCENE 11
	if (_comL==0x4C)
	{		
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x4B); 
	}
			
	// [077] STORE DTR AS SCENE 12
	if (_comL==0x4D)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x4C); 
	}
			
	// [078] STORE DTR AS SCENE 13
	if (_comL==0x4E)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x4D); 
	}
			
	// [079] STORE DTR AS SCENE 14
	if (_comL==0x4F)
	{				
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x4E); 
	}
	
	// [080] STORE DTR AS SCENE 15
	if (_comL==0x50)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x4F); 
	}
			
	// [081] REMOVE FROM SCENE 00
	if (_comL==0x51)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x50); 
	}
			
	// [082] REMOVE FROM SCENE 01
	if (_comL==0x52)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x51); 
	}
			
	// [083] REMOVE FROM SCENE 02
	if (_comL==0x53)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x52); 
	}
			
	// [084] REMOVE FROM SCENE 03
	if (_comL==0x54)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x53); 
	}
			
	// [085] REMOVE FROM SCENE 04
	if (_comL==0x55)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x54); 
	}
			
	// [086] REMOVE FROM SCENE 05
	if (_comL==0x56)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x55); 
	}
			
	// [087] REMOVE FROM SCENE 06
	if (_comL==0x57)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x56); 
	}
			
	// [088] REMOVE FROM SCENE 07
	if (_comL==0x58)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x57); 
	}
			
	// [089] REMOVE FROM SCENE 08
	if (_comL==0x59)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x58); 
	}
			
	// [090] REMOVE FROM SCENE 09
	if (_comL==0x5A)
	{				
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x59); 
	}
			
	// [091] REMOVE FROM SCENE 10
	if (_comL==0x5B)
	{				
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x5A); 
	}
			
	// [092] REMOVE FROM SCENE 11
	if (_comL==0x5C)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x5B); 
	}
			
	// [093] REMOVE FROM SCENE 12
	if (_comL==0x5D)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x5C); 
	}
		
	// [094] REMOVE FROM SCENE 13
	if (_comL==0x5E)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x5D); 
	}
			
	// [095] REMOVE FROM SCENE 14
	if (_comL==0x5F)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x5E); 
	}
			
	// [096] REMOVE FROM SCENE 15
	if (_comL==0x60)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x5F); 
	}
			
	// [097] ADD TO GROUP 00
	if (_comL==0x61)
	{	
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x60); 
	}
			
	// [098] ADD TO GROUP 01
	if (_comL==0x62)
	{		
		Update_Groups(_comL,_dest);		
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x61); 
	}
			
	// [099] ADD TO GROUP 02
	if (_comL==0x63)
	{	
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x62); 
	}
			
	// [100] ADD TO GROUP 03
	if (_comL==0x64)
	{	
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x63); 
	}
		
	// [101] ADD TO GROUP 04
	if (_comL==0x65)
	{
		Update_Groups(_comL,_dest);		
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x64); 
	}
			
	// [102] ADD TO GROUP 05
	if (_comL==0x66)
	{	
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x65); 
	}
			
	// [103] ADD TO GROUP 06
	if (_comL==0x67)
	{	
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x66); 
	}
			
	// [104] ADD TO GROUP 07
	if (_comL==0x68)
	{	
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x67); 
	}
			
	// [105] ADD TO GROUP 08
	if (_comL==0x69)
	{		
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x68);
	}
			
	// [106] ADD TO GROUP 09
	if (_comL==0x6A)
	{	
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x69);
	}
			
	// [107] ADD TO GROUP 10
	if (_comL==0x6B)
	{	
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x6A);
	}
			
	// [108] ADD TO GROUP 11
	if (_comL==0x6C)
	{	
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x6B);
	}
			
	// [109] ADD TO GROUP 12
	if (_comL==0x6D)
	{				
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x6C);
	}
			
	// [110] ADD TO GROUP 13
	if (_comL==0x6E)
	{	
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x6D);
	}
			
	// [111] ADD TO GROUP 14
	if (_comL==0x6F)
	{	
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x6E);
	}
			
	// [112] ADD TO GROUP 15
	if (_comL==0x70)
	{	
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x6F);
	}
			
	// [113] REMOVE FROM GROUP 00
	if (_comL==0x71)
	{				
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x70);
	}
			
	// [114] REMOVE FROM GROUP 01
	if (_comL==0x72)
	{				
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x71);
	}
			
	// [115] REMOVE FROM GROUP 02
	if (_comL==0x73)
	{	
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x72);
	}
			
	// [116] REMOVE FROM GROUP 03
	if (_comL==0x74)
	{	
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x73);
	}
			
	// [117] REMOVE FROM GROUP 04
	if (_comL==0x75)
	{				
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x74);
	}
			
	// [118] REMOVE FROM GROUP 05
	if (_comL==0x76)
	{				
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x75);
	}
			
	// [119] REMOVE FROM GROUP 06
	if (_comL==0x77)
	{				
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x76);
	}
			
	// [120] REMOVE FROM GROUP 07
	if (_comL==0x78)
	{				
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x77);
	}
			
	// [121] REMOVE FROM GROUP 08
	if (_comL==0x79)
	{
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x78);
	}
			
	// [122] REMOVE FROM GROUP 09
	if (_comL==0x7A)
	{				
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x79);
	}
			
	// [123] REMOVE FROM GROUP 10
	if (_comL==0x7B)
	{				
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x7A);
	}
			
	// [124] REMOVE FROM GROUP 11
	if (_comL==0x7C)
	{				
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x7B);
	}
			
	// [125] REMOVE FROM GROUP 12
	if (_comL==0x7D)
	{				
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x7C);
	}
			
	// [126] REMOVE FROM GROUP 13
	if (_comL==0x7E)
	{				
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x7D);
	}
			
	// [127] REMOVE FROM GROUP 14
	if (_comL==0x7F)
	{				
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x7E);
	}
			
	// [128] REMOVE FROM GROUP 15
	if (_comL==0x80)
	{				
		Update_Groups(_comL,_dest);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x7F);
	}
			
	// [129] STORE DTR AS SHORTADDRESS
	if (_comL==0x81)
	{				
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x80);
	}			
	
	// [130] ENABLE WRITE MEMORY
	if (_comL==0x82)
	{				
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2D, 0x81);
	}		
			
	// [145] QUERY STATUS
	if (_comL==0x91)
	{				
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0x90);
	}				
			
	// [146] QUERY CONTROL GEAR
	if (_comL==0x92)
	{				
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0x91);
	}
			
	// [147] QUERY LAMP FAILURE
	if (_comL==0x93)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0x92);
	}						
		
	// [148] QUERY LAMP POWER ON		
	if (_comL==0x94)
	{	
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0x93);
	}						
				
	// [149] QUERY LIMIT ERROR 
	if (_comL==0x95)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0x94);
	}						
			
	// [150] QUERY RESET STATE
	if (_comL==0x96)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0x95);
	}		
			
	// [151] QUERY MISSING SHORT ADDRESS
	if (_comL==0x97)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0x96);
	}
			
	// [152] QUERY VERSION NUMBER
	if (_comL==0x98)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0x97);
	}
		
	// [153] QUERY CONTENT DTR
	if (_comL==0x99)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0x98);
	}
			
	// [154] QUERY DEVICE TYPE
	if (_comL==0x9A)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0x99);
	}
		
	// [155] QUERY PHYSICAL MINIMUM LEVEL
	if (_comL==0x9B)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0x9A);
	}
			
	// [156] QUERY POWER FAILURE
	if (_comL==0x9C)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0x9B);
	}
			
	// [157] QUERY CONTENT DTR1
	if (_comL==0x9D)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0x9C);
	}
			
	// [158] QUERY CONTENT DTR2
	if (_comL==0x9E)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0x9D);
	}
					
	// [161] QUERY ACTUAL LEVEL
	if (_comL==0xA1)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xA0);
	}					

	// [162] QUERY MAX LEVEL
	if (_comL==0xA2)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xA1);
	}
					
	// [163] QUERY MIN LEVEL
	if (_comL==0xA3)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xA2);
	}
					
	// [164] QUERY POWER ON LEVEL
	if (_comL==0xA4)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xA3);
	}			
	
	// [165] QUERY SYSTEM FAILURE LEVEL
	if (_comL==0xA5)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xA4);
	}		
			
	// [166] QUERY FADE TIME/FADE RATE
	if (_comL==0xA6)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xA5);
	}
					
	// [177] QUERY SCENE LEVEL 00
	if (_comL==0xB1)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xB0);
	}
		
	// [178] QUERY SCENE LEVEL 01
	if (_comL==0xB2)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xB1);
	}
			
	// [179] QUERY SCENE LEVEL 02
	if (_comL==0xB3)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xB2);
	}
			
	// [180] QUERY SCENE LEVEL 03
	if (_comL==0xB4)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xB3);
	}
			
	// [181] QUERY SCENE LEVEL 04
	if (_comL==0xB5)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xB4);
	}		
			
	// [182] QUERY SCENE LEVEL 05
	if (_comL==0xB6)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xB5);
	}		
			
	// [183] QUERY SCENE LEVEL 06
	if (_comL==0xB7)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xB6);
	}		
			
	// [184] QUERY SCENE LEVEL 07
	if (_comL==0xB8)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xB7);
	}		
			
	// [185] QUERY SCENE LEVEL 08
	if (_comL==0xB9)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xB8);
	}
				
	// [186] QUERY SCENE LEVEL 09
	if (_comL==0xBA)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xB9);
	}		
			
	// [187] QUERY SCENE LEVEL 10
	if (_comL==0xBB)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xBA);
	}
			
	// [188] QUERY SCENE LEVEL 11
	if (_comL==0xBC)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xBB);
	}	
			
	// [189] QUERY SCENE LEVEL 12
	if (_comL==0xBD)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xBC);
	}
			
	// [190] QUERY SCENE LEVEL 13
	if (_comL==0xBE)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xBD);
	}		
			
	// [191] QUERY SCENE LEVEL 14
	if (_comL==0xBF)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xBE);
	}				
			
	// [192] QUERY SCENE LEVEL 15
	if (_comL==0xC0)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xBF);
	}						
			
	// [193] QUERY GROUPS 0-7
	if (_comL==0xC1)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xC0);
	}						
			
	// [194] QUERY GROUPS 8-15
	if (_comL==0xC2)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xC1);
	}								
			
	// [195] QUERY RANDOM ADDRESS (H)
	if (_comL==0xC3)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xC2);
	}									
			
	// [196] QUERY RANDOM ADDRESS (M)
	if (_comL==0xC4)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xC3);
	}									
			
	// [197] QUERY RANDOM ADDRESS (L)
	if (_comL==0xC5)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xC4);
	}		

	// [198] READ MEMORY LOCATION
	if (_comL==0xC6)
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2E, 0xC5);
	}		
			
	// [226] TERMINATE
	if ((_comL==0xE2) && (_dest==0xFF))
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, 0xA1, 0x2B, 0x00);
	}
	
	// [229] RANDOMISE
	if ((_comL==0xE5) && (_dest==0xFF))
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, 0xA7, 0x2D, 0x00);
		sysTickDelay (10);
	}									
			
	// [230] COMPARE
	if ((_comL==0xE6) && (_dest==0xFF))
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, 0xA9, 0x2E, 0x00);
	}								
			
	// [231] WITHDRAW
	if ((_comL==0xE7) && (_dest==0xFF))
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, 0xAB, 0x2B, 0x00);
	}					
	
	// [239] QUERY SHORT ADDRESS
	if ((_comL==0xEF) && (_dest==0xFF))
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, 0xBB, 0x2E, 0x00);
	}
	
}
void normaLink62386P102T3(unsigned char _comL, unsigned char _dest, unsigned char _data)
{
	unsigned int _packetId;
	_packetId=recibireth[2]*0x0100+recibireth[3];	
	
	// [000] DIRECT ARC POWER CONTROL
	if (_comL==0)
	{				
			//daliLoopActualLevelValidClearAll();		
			actualLevelCmdNotify(_dest, _comL, _data);			
			if ((_dest&0xE1)==0x81)
			{
				circGruposInhibidosSet (((_dest&0x1E)>>1),0);				
				sensoresGruposReguladoresLuzInhibir((_dest&0x1E)>>1);
			}		
			normalinkDaliSend(&ipDest[0],_packetId, 0x01, (_dest&0xFE), 0x2B, _data);
	}							
			
	// [227] DTR
	if ((_comL==0xE3) && (_dest==0xFF))
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, 0xA3, 0x2B, _data);
	}										
	
	// [228] INITIALISE
	if ((_comL==0xE4) && (_dest==0xFF))
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, 0xA5, 0x2D, _data);		
		sysTickDelay (10);		
	}					
			
	// [234] SEARCHADDRH
	if ((_comL==0xEA) && (_dest==0xFF))
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, 0xB1, 0x2B, _data);
	}											
			
	// [235] SEARCHADDRM
	if ((_comL==0xEB) && (_dest==0xFF))
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, 0xB3, 0x2B, _data);
	}								
		
	// [236] SEARCHADDRL
	if ((_comL==0xEC) && (_dest==0xFF))
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, 0xB5, 0x2B, _data);
	}			
	
	// [237] PROGRAM SHORT ADDRESS
	if ((_comL==0xED) && (_dest==0xFF))
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, 0xB7, 0x2B, _data);
	}									
				
	// [238] VERIFY SHORT ADDRESS
	if ((_comL==0xEE) && (_dest==0xFF))
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, 0xB9, 0x2E, _data);
	}							
	
	// [239] ENABLE DEVICE TYPE X
	if ((_comL==0xF1) && (_dest==0xFF))
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, 0xC1, 0x2B, _data);
	}							
	
	// [240] DTR1
	if ((_comL==0xF2) && (_dest==0xFF))
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, 0xC3, 0x2B, _data);
	}							
	
	// [241] DTR2
	if ((_comL==0xF3) && (_dest==0xFF))
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, 0xC5, 0x2B, _data);
	}			
	
	// [242] WRITE MEMORY LOCATION
	if ((_comL==0xF4) && (_dest==0xFF))
	{
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, 0xC7, 0x2B, _data);
	}		
}


