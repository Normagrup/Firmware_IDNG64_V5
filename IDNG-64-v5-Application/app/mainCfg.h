// Debuging OPTIONS

	//#define ETH_IGNORE_CRC
	//#define NO_BOOTLOADER
	//#define NO_WDT
	//#define DESACTIVAR_IED2
	//#define DEBUG
	//#define IGNORE_SUBNET_FILTER
	

#ifdef	DEBUG
	#include "debug.h"
#endif
	
	
#define MODEL_H						8
#define MODEL_L						0
#define FW_VERSION_MAJOR	(5)
#define FW_VERSION_MINOR	(3)
#define WEBSERVER_TEXT		"[IDNG64 5.3 build 17/07/2024].\r\n"

#ifdef	DEBUG
	#define WEBSERVER_TEXT		"[Fw: v5.0 dbg].\r\n"	
#endif