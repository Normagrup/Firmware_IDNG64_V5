#include "macros.h"
#include "daliCommands.h"
#include "historico.h"
#include "rtcc.h"
#include "fallos.h"


// 64 + 64 + 64 + 64 + 64 + 64 + 1			// 385 bytes 				49¿?

unsigned char fallosAutonomiaEstado[64];
unsigned char fallosBateriaEstado[64];
unsigned char fallosLamparaEstado[64];
unsigned char fallosNoFtEstado[64];
unsigned char fallosNoDtEstado[64];
unsigned char fallosComunicacionEstado[64];
unsigned char fallosFalloReset;


void fallosFalloResetSet(void)
{
	fallosFalloReset=0x01;
}

void fallosDoTask (void)
{
	if (fallosFalloReset==0x01)
	{
		Date date;
		Time time;
		date=rtccDateGet();
		time=rtccTimeGet();		
		historicoAdd (0xFF, 0xFF, date.day, date.month, date.year, time.hours, time.minutes, HISTORICO_INICIO_CENTRAL);
		fallosFalloReset=0x00;		
	}	
}

void fallosProcesar (unsigned char address, unsigned char deviceType, unsigned char failureOrStatusRegister)
{
	Date date;
	Time time;
	date=rtccDateGet();
	time=rtccTimeGet();		
	if (deviceType==0x01)		// E m e r g e n c i a s
	{
		// F a l l o     d e     a u t o n o m i a		
		if (BITVAL(failureOrStatusRegister,1)==0x01)
		{
			if (fallosAutonomiaEstado[address]!=FALLOS_ESTADO_MARCADO)	
			{			
				if (fallosAutonomiaEstado[address]==FALLOS_ESTADO_DETECTADO)
				{
					if (historicoAdd (0x00, address, date.day, date.month, date.year, time.hours, time.minutes, HISTORICO_FALLO_AUTONOMIA)==0x01)								
					{
						fallosAutonomiaEstado[address]=FALLOS_ESTADO_MARCADO;
					}
				}
				else
				{
					if (fallosAutonomiaEstado[address]==FALLOS_ESTADO_NO_MARCADO)
					{						
						fallosAutonomiaEstado[address]=FALLOS_ESTADO_DETECTADO;
					}
					else
					{
						fallosAutonomiaEstado[address]=FALLOS_ESTADO_MARCADO;
					}
				}
			}
		}
		else
		{
			if (fallosAutonomiaEstado[address]!=FALLOS_ESTADO_NO_MARCADO)
			{
				if (fallosAutonomiaEstado[address]==FALLOS_ESTADO_NO_DETECTADO)
				{
					if (historicoAdd (0x00, address, date.day, date.month, date.year, time.hours, time.minutes, HISTORICO_RECUPERACION_FALLO_AUTONOMIA)==0x01)
					{
						fallosAutonomiaEstado[address]=FALLOS_ESTADO_NO_MARCADO;
					}
				}
				else
				{
					if (fallosAutonomiaEstado[address]==FALLOS_ESTADO_MARCADO)
					{
						fallosAutonomiaEstado[address]=FALLOS_ESTADO_NO_DETECTADO;
					}
					else
					{
						fallosAutonomiaEstado[address]=FALLOS_ESTADO_NO_MARCADO;
					}
				}				
			}	
		}		
		
		
		// F a l l o     d e     b a t e r i a
		if (BITVAL(failureOrStatusRegister,2)==0x01)
		{
			if (fallosBateriaEstado[address]!=FALLOS_ESTADO_MARCADO)
			{
				if (fallosBateriaEstado[address]==FALLOS_ESTADO_DETECTADO)
				{
					if (historicoAdd (0x00, address, date.day, date.month, date.year, time.hours, time.minutes, HISTORICO_FALLO_BATERIA)==0x01)
					{
						fallosBateriaEstado[address]=FALLOS_ESTADO_MARCADO;
					}
				}
				else
				{
					if (fallosBateriaEstado[address]==FALLOS_ESTADO_NO_MARCADO)
					{
						fallosBateriaEstado[address]=FALLOS_ESTADO_DETECTADO;
					}
					else
					{
						fallosBateriaEstado[address]=FALLOS_ESTADO_MARCADO;
					}
				}
			}
		}
		else
		{
			if (fallosBateriaEstado[address]!=FALLOS_ESTADO_NO_MARCADO)
			{
				if (fallosBateriaEstado[address]==FALLOS_ESTADO_NO_DETECTADO)
				{
					if (historicoAdd (0x00, address, date.day, date.month, date.year, time.hours, time.minutes, HISTORICO_RECUPERACION_FALLO_BATERIA)==0x01)
					{
						fallosBateriaEstado[address]=FALLOS_ESTADO_NO_MARCADO;
					}
				}
				else
				{
					if (fallosBateriaEstado[address]==FALLOS_ESTADO_MARCADO)
					{
						fallosBateriaEstado[address]=FALLOS_ESTADO_NO_DETECTADO;
					}
					else
					{
						fallosBateriaEstado[address]=FALLOS_ESTADO_NO_MARCADO;
					}
				}
			}
		}
		
		// F a l l o     d e     l a m p a r a
		if (BITVAL(failureOrStatusRegister,3)==0x01)
		{
			if (fallosLamparaEstado[address]!=FALLOS_ESTADO_MARCADO)
			{
				if (fallosLamparaEstado[address]==FALLOS_ESTADO_DETECTADO)
				{
					if (historicoAdd (0x00, address, date.day, date.month, date.year, time.hours, time.minutes, HISTORICO_FALLO_LAMPARA)==0x01)
					{
						fallosLamparaEstado[address]=FALLOS_ESTADO_MARCADO;
					}
				}
				else
				{
					if (fallosLamparaEstado[address]==FALLOS_ESTADO_NO_MARCADO)
					{
						fallosLamparaEstado[address]=FALLOS_ESTADO_DETECTADO;
					}
					else
					{
						fallosLamparaEstado[address]=FALLOS_ESTADO_MARCADO;
					}
				}
			}
		}
		else
		{
			if (fallosLamparaEstado[address]!=FALLOS_ESTADO_NO_MARCADO)
			{
				if (fallosLamparaEstado[address]==FALLOS_ESTADO_NO_DETECTADO)
				{
					if (historicoAdd (0x00, address, date.day, date.month, date.year, time.hours, time.minutes, HISTORICO_RECUPERACION_FALLO_LAMPARA)==0x01)
					{
						fallosLamparaEstado[address]=FALLOS_ESTADO_NO_MARCADO;
					}
				}
				else
				{
					if (fallosLamparaEstado[address]==FALLOS_ESTADO_MARCADO)
					{
						fallosLamparaEstado[address]=FALLOS_ESTADO_NO_DETECTADO;
					}
					else
					{
						fallosLamparaEstado[address]=FALLOS_ESTADO_NO_MARCADO;
					}
				}
			}
		}	
		
		
		// F a l l o     d e     n o     F t
		if (BITVAL(failureOrStatusRegister,4)==0x01)
		{
			if (fallosNoFtEstado[address]!=FALLOS_ESTADO_MARCADO)
			{
				if (fallosNoFtEstado[address]==FALLOS_ESTADO_DETECTADO)
				{
					if (historicoAdd (0x00, address, date.day, date.month, date.year, time.hours, time.minutes, HISTORICO_FALLO_NO_FT)==0x01)
					{
						fallosNoFtEstado[address]=FALLOS_ESTADO_MARCADO;
					}
				}
				else
				{
					if (fallosNoFtEstado[address]==FALLOS_ESTADO_NO_MARCADO)
					{
						fallosNoFtEstado[address]=FALLOS_ESTADO_DETECTADO;
					}
					else
					{
						fallosNoFtEstado[address]=FALLOS_ESTADO_MARCADO;
					}
				}
			}
		}
		else
		{
			if (fallosNoFtEstado[address]!=FALLOS_ESTADO_NO_MARCADO)
			{
				if (fallosNoFtEstado[address]==FALLOS_ESTADO_NO_DETECTADO)
				{

						fallosNoFtEstado[address]=FALLOS_ESTADO_NO_MARCADO;

				}
				else
				{
					if (fallosNoFtEstado[address]==FALLOS_ESTADO_MARCADO)
					{
						fallosNoFtEstado[address]=FALLOS_ESTADO_NO_DETECTADO;
					}
					else
					{						
						fallosNoFtEstado[address]=FALLOS_ESTADO_NO_MARCADO;
					}
				}
			}
		}
	
	
	
	
	
	
			// F a l l o     d e     n o     D t
		if (BITVAL(failureOrStatusRegister,5)==0x01)
		{
			if (fallosNoDtEstado[address]!=FALLOS_ESTADO_MARCADO)
			{
				if (fallosNoDtEstado[address]==FALLOS_ESTADO_DETECTADO)
				{
					if (historicoAdd (0x00, address, date.day, date.month, date.year, time.hours, time.minutes, HISTORICO_FALLO_NO_DT)==0x01)
					{
						fallosNoDtEstado[address]=FALLOS_ESTADO_MARCADO;
					}
				}
				else
				{
					if (fallosNoDtEstado[address]==FALLOS_ESTADO_NO_MARCADO)
					{
						fallosNoDtEstado[address]=FALLOS_ESTADO_DETECTADO;
					}
					else
					{
						fallosNoDtEstado[address]=FALLOS_ESTADO_MARCADO;
					}
				}
			}
		}
		else
		{
			if (fallosNoDtEstado[address]!=FALLOS_ESTADO_NO_MARCADO)
			{
				if (fallosNoDtEstado[address]==FALLOS_ESTADO_NO_DETECTADO)
				{

						fallosNoDtEstado[address]=FALLOS_ESTADO_NO_MARCADO;
				}
				else
				{
					if (fallosNoDtEstado[address]==FALLOS_ESTADO_MARCADO)
					{
						fallosNoDtEstado[address]=FALLOS_ESTADO_NO_DETECTADO;
					}
					else
					{
						fallosNoDtEstado[address]=FALLOS_ESTADO_NO_MARCADO;
					}
				}
			}
		}	
	}		
	
	
	
	if (deviceType==0x06)		// LED
	{
				// F a l l o     d e     l a m p a r a
		if (BITVAL(failureOrStatusRegister,1)==0x01)
		{
			if (fallosLamparaEstado[address]!=FALLOS_ESTADO_MARCADO)
			{
				if (fallosLamparaEstado[address]==FALLOS_ESTADO_DETECTADO)
				{
					if (historicoAdd (0x00, address, date.day, date.month, date.year, time.hours, time.minutes, HISTORICO_FALLO_LAMPARA)==0x01)
					{
						fallosLamparaEstado[address]=FALLOS_ESTADO_MARCADO;
					}
				}
				else
				{
					if (fallosLamparaEstado[address]==FALLOS_ESTADO_NO_MARCADO)
					{
						fallosLamparaEstado[address]=FALLOS_ESTADO_DETECTADO;
					}
					else
					{
						fallosLamparaEstado[address]=FALLOS_ESTADO_MARCADO;
					}
				}
			}
		}
		else
		{
			if (fallosLamparaEstado[address]!=FALLOS_ESTADO_NO_MARCADO)
			{
				if (fallosLamparaEstado[address]==FALLOS_ESTADO_NO_DETECTADO)
				{
					if (historicoAdd (0x00, address, date.day, date.month, date.year, time.hours, time.minutes, HISTORICO_RECUPERACION_FALLO_LAMPARA)==0x01)
					{					
						fallosLamparaEstado[address]=FALLOS_ESTADO_NO_MARCADO;
					}
				}
				else
				{
					if (fallosLamparaEstado[address]==FALLOS_ESTADO_MARCADO)
					{
						fallosLamparaEstado[address]=FALLOS_ESTADO_NO_DETECTADO;
					}
					else
					{
						fallosLamparaEstado[address]=FALLOS_ESTADO_NO_MARCADO;
					}
				}
			}
		}	
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}


void fallosComunicacion(unsigned char address, unsigned char direction)
{	
	Date date;
	Time time;
	date=rtccDateGet();
	time=rtccTimeGet();	
	// F a l l o     d e     c o m u n i c a c i o n e s		
	if (direction==0x01)
	{
		if (fallosComunicacionEstado[address]!=FALLOS_ESTADO_MARCADO)	
		{			
			if (fallosComunicacionEstado[address]==FALLOS_ESTADO_DETECTADO)
			{
				if (historicoAdd (0x00, address, date.day, date.month, date.year, time.hours, time.minutes, HISTORICO_FALLO_COMUNICACION)==0x01)
				{
					fallosComunicacionEstado[address]=FALLOS_ESTADO_MARCADO;
				}
			}
			else
			{
				if (fallosComunicacionEstado[address]==FALLOS_ESTADO_NO_MARCADO)
				{						
					fallosComunicacionEstado[address]=FALLOS_ESTADO_DETECTADO;
				}
				else
				{
					fallosComunicacionEstado[address]=FALLOS_ESTADO_MARCADO;
				}
			}
		}
	}
	else
	{
		if (fallosComunicacionEstado[address]!=FALLOS_ESTADO_NO_MARCADO)
		{
			if (fallosComunicacionEstado[address]==FALLOS_ESTADO_NO_DETECTADO)
			{
				if (historicoAdd (0x00, address, date.day, date.month, date.year, time.hours, time.minutes, HISTORICO_RECUPERACION_FALLO_COMUNICACION)==0x01)
				{				
					fallosComunicacionEstado[address]=FALLOS_ESTADO_NO_MARCADO;
				}
			}
			else
			{
				if (fallosComunicacionEstado[address]==FALLOS_ESTADO_MARCADO)
				{
					fallosComunicacionEstado[address]=FALLOS_ESTADO_NO_DETECTADO;
				}
				else
				{
					fallosComunicacionEstado[address]=FALLOS_ESTADO_NO_MARCADO;
				}
			}				
		}	
	}			
}	
