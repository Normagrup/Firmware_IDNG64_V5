#include "normalink.h"
#include "historico.h"

extern unsigned char recibireth[320];		// Búffer para paquete entrante
extern unsigned char ipDest[4];					// Ip del host que me envía el paquete

void normaLink62386P202T1(unsigned char _comL, unsigned char _dest)
{
	unsigned int _packetId;
	_packetId=recibireth[2]*0x0100+recibireth[3];
	
	// [000] REST
	if (_comL==0x00) 
	{			
		polling_PollEmergencyMode();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2C, 0xE0);			
	}	
	
	// [001] INHIBIT
	if (_comL==0x01) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2C, 0xE1);			
	}	
	
	// [002] RE-LIGHT/RESET INHIBIT
	if (_comL==0x02) 
	{			
		polling_PollEmergencyMode();
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2C, 0xE2);			
	}	
	
	// [003] START FUNCTION TEST
	if (_comL==0x03) 
	{			
		polling_PollEmergencyMode();		
		polling_PollEmergencyStatus(0x00);
		historicoTest_PeticionManual(_dest, HISTORICO_SOLICITADO_FT);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2C, 0xE3);			
	}	
	
	// [004] START DURATION TEST
	if (_comL==0x04) 
	{			
		polling_PollEmergencyMode();
		polling_PollEmergencyStatus(0x01);
		historicoTest_PeticionManual(_dest, HISTORICO_SOLICITADO_DT);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2C, 0xE4);			
	}	
	
	// [005] STOP TEST
	if (_comL==0x05) 
	{			
		polling_PollEmergencyMode();
		polling_PollEmergencyStatus(0x00);
		historicoTest_PeticionManual(_dest, HISTORICO_STOP_TEST);
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2C, 0xE5);			
	}	
	
	// [006] RESET FUNCTION TEST DONE FLAG
	if (_comL==0x06) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2C, 0xE6);			
	}	
	
	// [007] RESET DURATION TEST DONE FLAG
	if (_comL==0x07) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2C, 0xE7);			
	}	
	
	// [008] RESET LAMP TIME
	if (_comL==0x08) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2C, 0xE8);			
	}	
	
	// [009] STORE DTR AS EMERGENCY LEVEL
	if (_comL==0x09) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2C, 0xE9);			
	}	
	
	// [010] STORE TEST DELAY HIGH BYTE
	if (_comL==0x0A) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2C, 0xEA);			
	}	
	
	// [011] STORE TEST DELAY LOW BYTE
	if (_comL==0x0B) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2C, 0xEB);			
	}	
	
	// [012] STORE FUNCTION TEST INTERVAL
	if (_comL==0x0C) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2C, 0xEC);			
	}	
	
	// [013] STORE DURATION TEST INTERVAL
	if (_comL==0x0D) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2C, 0xED);			
	}	
	
	// [014] STORE TEST EXECUTION TIMEOUT
	if (_comL==0x0E) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2C, 0xEE);			
	}	
	
	// [015] STORE PROLONG TIME
	if (_comL==0x0F) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2C, 0xEF);			
	}	
	
	// [016] START IDENTIFICATION
	if (_comL==0x10) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2C, 0xF0);			
	}	
	
	// [017] QUERY BATTERY CHARGE
	if (_comL==0x11) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x30, 0xF1);			
	}	
	
	// [018] QUERY TEST TIMING
	if (_comL==0x12) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x30, 0xF2);			
	}	
	
	// [019] QUERY DURATION TEST RESULT
	if (_comL==0x13) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x30, 0xF3);			
	}	
	
	// [019] QUERY LAMP EMERGENCY TIME
	if (_comL==0x14) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x30, 0xF4);			
	}	
	
	// [020] QUERY LAMP OPERATION TIME
	if (_comL==0x15) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x30, 0xF5);			
	}	
	
	// [021] QUERY EMERGENCY LEVEL
	if (_comL==0x16) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x30, 0xF6);			
	}	
	
	// [022] QUERY EMERGENCY MIN LEVEL
	if (_comL==0x17) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x30, 0xF7);			
	}	
	
	// [023] QUERY EMERGENCY MAX LEVEL
	if (_comL==0x18) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x30, 0xF8);			
	}	
	
	// [024] QUERY RATED DURATION
	if (_comL==0x19) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x30, 0xF9);			
	}	
	
	// [025] QUERY EMERGENCY MODE
	if (_comL==0x1A) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x30, 0xFA);			
	}	
	
	// [026] QUERY FEATURES
	if (_comL==0x1B) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x30, 0xFB);			
	}	
	
	// [026] QUERY FAILURE STATUS
	if (_comL==0x1C) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x30, 0xFC);			
	}	
	
	// [027] QUERY EMERGENCY STATUS
	if (_comL==0x1D) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x30, 0xFD);			
	}	
	
	// [028] PERFORM DTR SELECTED FUNCTION
	if (_comL==0x1E) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x2C, 0xFE);			
	}	
	
	// [029] QUERY EXTENDED VERSION NUMBER
	if (_comL==0x1F) 
	{			
		normalinkDaliSend(&ipDest[0],_packetId, 0x01, _dest, 0x30, 0xFF);			
	}	
}

