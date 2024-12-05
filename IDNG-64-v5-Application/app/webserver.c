#include "stm32g0xx.h"
#include "w5100/socket.h"
#include "w5100/w5100.h"
#include "dali.h"
#include "wdt.h"
#include "ethTransceiver.h"
#include "config.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "config.h"
#include "systick.h"
#include "mainCfg.h"

unsigned char webServerEstado=0x01;

extern unsigned char _broadcastAddress[4];
extern unsigned char macAdrress[6];
extern unsigned char _normaLinkMasterIp[4];

void botoneraTestOn (unsigned char period);



int strindex(char *s,char *t)
{
	unsigned short int i,n;
	n=strlen(t);
	for(i=0;*(s+i); i++) {
		if (strncmp(s+i,t,n) == 0)
		return i;
	}
	return -1;
}


void printHexStringFromDec (unsigned char in, unsigned char *buffer)
{
	unsigned char digit[2];
	unsigned char out[2];
	unsigned char dec2hex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	digit[1]=in/16;
	digit[0]=in -(digit[1]*16);		
	out[0]=dec2hex[digit[1]];
	out[1]=dec2hex[digit[0]];	
	strcat((char*)(buffer),(char*)(out));
}

void webserverPrintMac (unsigned char *buffer)
{	
	printHexStringFromDec(macAdrress[0],buffer);
	strcat((char*)(buffer),(char*)(":"));
	printHexStringFromDec(macAdrress[1],buffer);
	strcat((char*)(buffer),(char*)(":"));
	printHexStringFromDec(macAdrress[2],buffer);
	strcat((char*)(buffer),(char*)(":"));
	printHexStringFromDec(macAdrress[3],buffer);
	strcat((char*)(buffer),(char*)(":"));
	printHexStringFromDec(macAdrress[4],buffer);
	strcat((char*)(buffer),(char*)(":"));
	printHexStringFromDec(macAdrress[5],buffer);	
}



unsigned char _webserverUtilsStringToIp (unsigned char *string, unsigned char *ip)
{
	unsigned char _n;
	unsigned char _charString[4][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
	unsigned char _dotCnt;
	unsigned char _strlenght;	
	unsigned char _digitCnt;
	unsigned char _dotLoc[3];
	_dotCnt=0;

	_strlenght=strlen((char *)(string));
	
	// Busqueda de puntos
	for (_n=0;_n<_strlenght;_n++)
	{
		if (string[_n]=='.')
		{
			_dotLoc[_dotCnt]=_n;
			if (_dotCnt<3)
			{
					_dotCnt++;				
			}							
			else
			{
				// Error Handler
			}
		}
	}	
	_digitCnt=0;
	for (_n=0;_n<_dotLoc[0];_n++)
	{
		_charString[0][_digitCnt]=string[_n];	
		_digitCnt++;		
	}	
	_digitCnt=0;
	for (_n=(_dotLoc[0]+1);_n<_dotLoc[1];_n++)
	{
		_charString[1][_digitCnt]=string[_n];		
		_digitCnt++;
	}	
	_digitCnt=0;
	for (_n=(_dotLoc[1]+1);_n<_dotLoc[2];_n++)
	{
		_charString[2][_digitCnt]=string[_n];		
		_digitCnt++;
	}	
	_digitCnt=0;
	for (_n=(_dotLoc[2]+1);_n<_strlenght;_n++)
	{
		_charString[3][_digitCnt]=string[_n];		
		_digitCnt++;
	}	
	ip[0]=atoi((char*)_charString[0]);
	ip[1]=atoi((char*)_charString[1]);
	ip[2]=atoi((char*)_charString[2]);
	ip[3]=atoi((char*)_charString[3]);
	return 0x01;	
}

















void _webserverPrintIp(unsigned char *ip, unsigned char *buffer)
{
	unsigned char _centenas[4]={0,0,0,0};
	unsigned char _decenas[4]={0,0,0,0};
	unsigned char _unidades[4]={0,0,0,0};
	unsigned char _ascii[3]={0,0,'0'};	
	_centenas[0]=ip[0]/100;
	_centenas[1]=ip[1]/100;
	_centenas[2]=ip[2]/100;
	_centenas[3]=ip[3]/100;
	_decenas[0]=(ip[0]/10)-(10*_centenas[0]);
	_decenas[1]=(ip[1]/10)-(10*_centenas[1]);
	_decenas[2]=(ip[2]/10)-(10*_centenas[2]);
	_decenas[3]=(ip[3]/10)-(10*_centenas[3]);
	_unidades[0]=ip[0]-(100*_centenas[0])-(10*_decenas[0]);
	_unidades[1]=ip[1]-(100*_centenas[1])-(10*_decenas[1]);
	_unidades[2]=ip[2]-(100*_centenas[2])-(10*_decenas[2]);						
	_unidades[3]=ip[3]-(100*_centenas[3])-(10*_decenas[3]);				
	if (_centenas[0]==0)
	{
		if (_decenas[0]==0)
		{
			_ascii[0]=48+_unidades[0];
			_ascii[1]=0;
			_ascii[2]=0;
		}
		else
		{
			_ascii[0]=48+_decenas[0];
			_ascii[1]=48+_unidades[0];
			_ascii[2]=0;
		}								
	}
	else
	{
		_ascii[0]=48+_centenas[0];	
		_ascii[1]=48+_decenas[0];
		_ascii[2]=48+_unidades[0];							
	}
	strcat((char*)(buffer),(char*)(_ascii));						
	strcat((char*)(buffer),".");							
	if (_centenas[1]==0)
	{
		if (_decenas[1]==0)
		{
			_ascii[0]=48+_unidades[1];
			_ascii[1]=0;
			_ascii[2]=0;
		}
		else
		{
			_ascii[0]=48+_decenas[1];
			_ascii[1]=48+_unidades[1];
			_ascii[2]=0;
		}								
	}
	else
	{
		_ascii[0]=48+_centenas[1];	
		_ascii[1]=48+_decenas[1];
		_ascii[2]=48+_unidades[1];							
	}
	strcat((char*)(buffer),(char*)(_ascii));						
	strcat((char*)(buffer),".");								
	if (_centenas[2]==0)
	{
		if (_decenas[2]==0)
		{
			_ascii[0]=48+_unidades[2];
			_ascii[1]=0;
			_ascii[2]=0;
		}
		else
		{
			_ascii[0]=48+_decenas[2];
			_ascii[1]=48+_unidades[2];
			_ascii[2]=0;
		}								
	}
	else
	{
		_ascii[0]=48+_centenas[2];	
		_ascii[1]=48+_decenas[2];
		_ascii[2]=48+_unidades[2];							
	}
	strcat((char*)(buffer),(char*)(_ascii));						
	strcat((char*)(buffer),".");							
	if (_centenas[3]==0)
	{
		if (_decenas[3]==0)
		{
			_ascii[0]=48+_unidades[3];
			_ascii[1]=0;
			_ascii[2]=0;
		}
		else
		{
			_ascii[0]=48+_decenas[3];
			_ascii[1]=48+_unidades[3];
			_ascii[2]=0;
		}								
	}
	else
	{
		_ascii[0]=48+_centenas[3];	
		_ascii[1]=48+_decenas[3];
		_ascii[2]=48+_unidades[3];							
	}							
	strcat((char*)(buffer),(char*)(_ascii));		
}











unsigned char _webserverUtilsStringFromBufferGet (unsigned char *buffr, unsigned char search, unsigned char *out)
{
	unsigned int _ipIndex=0;
	unsigned char _n;
	unsigned char _end=0x00;
	unsigned char _lenght;
	unsigned char _ip=0x00;

	for (_n=0;_n<32;_n++)
	{
			out[_n]=0;		
	}	

	if (search==0x01)
	{
		_lenght=8;
		_ip=0x01;
		_ipIndex=strindex((char *)buffr,"ipValue=")+_lenght;			
	}
	
	if (search==0x02)
	{
		_lenght=13;
		_ip=0x01;
		_ipIndex=strindex((char *)buffr,"submaskValue=")+_lenght;			
	}
	
	if (search==0x03)
	{
		_lenght=9;
		_ip=0x01;
		_ipIndex=strindex((char *)buffr,"gtwValue=")+_lenght;			
	}
	
	if (search==0x04)
	{
		_lenght=15;
		_ipIndex=strindex((char *)buffr,"edificioNombre=")+_lenght;			
	}
	
	if (search==0x05)
	{
		_lenght=12;
		_ipIndex=strindex((char *)buffr,"lineaNombre=")+_lenght;			
	}
	
	if (search==0x06)
	{
		_lenght=10;
		_ipIndex=strindex((char *)buffr,"adminPass=")+_lenght;			
	}
	if (search==0x07)
	{
		_lenght=13;
		_ipIndex=strindex((char *)buffr,"InputCheckBox")+_lenght;
		if (_ipIndex>0+_lenght)
			{
				out[0]=1;
				return 0x00;
			}
			else{
					out[0]=0;
				return 0x00;
			}
		}
			
	if ((_ipIndex>0) && ((_ipIndex+_lenght+32)<1024))
	{
		_n=0;
		while ((_n<32) && (_end==0x00))
		{			
			if (_ip==0x01)
			{
				if (((buffr[_ipIndex+_n]>=48) && (buffr[_ipIndex+_n]<=57)) || (buffr[_ipIndex+_n]=='.')) 
				{		
					out[_n]=buffr[_ipIndex+_n];
				}
				else
				{
					if (buffr[_ipIndex+_n]=='&')
					{
						_end=0x01;
					}			
				}		
			}
			else
			{
				if ((buffr[_ipIndex+_n]!='&') && (_n<16))
				{		
					out[_n]=buffr[_ipIndex+_n];
				}
				else
				{
					_end=0x01;						
				}				
			}		
			_n++;
		}
		if (_end==0x01)
		{		
			return 0x01;
		}
		else
		{
			return 0x00;
		}
	}
	else
	{
		return 0x00;
	}
}

void webServerDoTask (void)
{

	unsigned char _ipString[15];
	unsigned char _submaskString[15];
	//unsigned char _gtwString[32];
	unsigned char _adminPassString[32];	
	unsigned char _ipAdrress[4];
	unsigned char _subMask[4];
	//unsigned char _gtwAddress[4];
	unsigned char _edificioString[32];
	unsigned char _lineaString[32];
	unsigned char _gwadminPassString[16];
	
	unsigned char _socketEstado;
	int _postidx;	
	unsigned char _applyBtn=0x00;
	unsigned char _rebootBtn=0x00;
	unsigned char _maxBtn=0x00;
	unsigned char _offBtn=0x00;		
	
	unsigned char _bufferRx[1000];//768
	unsigned char _bufferTx[1000];//768
	
	unsigned char apply[8];
	unsigned int n;
	unsigned char _hea[400];
	strcpy((char*)(_hea),"HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<link rel=\"shortcut icon\" href=\"data:image/x-icon;,\" type=\"image/x-icon\"><html><body>\r\n<p><form method=\"POST\">");
	strcat((char*)(_hea),"<style>");
	strcat((char*)(_hea),"h2 {text-align:center;width:400px;border:4px solid #2E2E2E;border-radius:15px;padding:20px;};");

	strcat((char*)(_hea),"body{");
	strcat((char*)(_hea),"font-family:Arial,Helvetica,sans-serif;"  );
	strcat((char*)(_hea),"}"  );
	
	strcat((char*)(_hea),".left{");
	strcat((char*)(_hea),"padding:20px;");
	strcat((char*)(_hea),"border-radius:10px;");
	strcat((char*)(_hea),"width:450px;background-color:#aaa;");
	strcat((char*)(_hea),"}"  );
	strcat((char*)(_hea),"</style>");	
	
		
	if (webServerEstado==0x01)
	{		
		_socketEstado=getSn_SR(1);		
		switch (_socketEstado)
		{
			case SOCK_CLOSED:
			{
				// Crea un socket TCP en puerto 80
				if (socket(1,Sn_MR_TCP,80,0) > 0)
				{
					// Escuchar en el socket 0
					if (listen(1) <= 0)
					{
					}
				}
				break;
			}			
			case SOCK_ESTABLISHED:
			{
				unsigned int _rsize;
				_rsize=getSn_RX_RSR(1);
				if (_rsize>0)
				{
					ethTransceiverTiempoSinRecibirCntRst();
					send(1,_hea,strlen((char *)_hea));						
					
					for (n=0;n<1000;n++)
					{
						_bufferRx[n]=0x00;
						_bufferTx[n]=0x00;
					}
							

					configIpAddressGet(&_ipAdrress[0]);
					configSubmaskAddressGet(&_subMask[0]);
					//configGatewayAddressGet(&_gtwAddress[0]);	
					configNombreEdificioGet(&_edificioString[0]);
					configNombreLineaGet(&_lineaString[0]);
					configAdminPassGet(&_gwadminPassString[0]);

					if (_rsize<1000)
					{
						recv(1,_bufferTx,_rsize);
					}
					
					_postidx=strindex((char *)_bufferTx,"POST /");					
					if (_postidx >= 0) 
					{
						if (strindex((char *)_bufferTx,"ApplyBtn=Apply") > 0)
						{
							_applyBtn=0x01;
						}				

						if (strindex((char *)_bufferTx,"RebootBtn=Reboot") > 0)
						{
							_rebootBtn=0x01;
						}					
						
						if (strindex((char *)_bufferTx,"MaxBtn=Max") > 0)
						{
							_maxBtn=0x01;								
						}

						if (strindex((char *)_bufferTx,"OffBtn=Off") > 0)
						{
							_offBtn=0x01;
						}										

						if (_applyBtn==0x01)
						{			

							_webserverUtilsStringFromBufferGet(_bufferTx,1,_ipString);
							_webserverUtilsStringFromBufferGet(_bufferTx,2,_submaskString);				
							//_webserverUtilsStringFromBufferGet(_bufferTx,3,_gtwString);
							_webserverUtilsStringFromBufferGet(_bufferTx,4,_edificioString);					
							_webserverUtilsStringFromBufferGet(_bufferTx,5,_lineaString);
							_webserverUtilsStringFromBufferGet(_bufferTx,6,_adminPassString);
							 	
								
							//Cambios 19/01/23
								
								_webserverUtilsStringToIp (_ipString,_ipAdrress);
								_webserverUtilsStringToIp (_submaskString,_subMask);
								//_webserverUtilsStringToIp (_gtwString,_gtwAddress);				
						}
											
						
						if ((_rebootBtn==0x01) || (_maxBtn==0x01) || (_offBtn==0x01))
						{
							_webserverUtilsStringFromBufferGet(_bufferTx,6,_adminPassString);	
						}
						

					}	
					
					strcpy((char*)(_bufferRx),"<div class=\"left\">"); 
					strcat((char*)(_bufferRx),"<h2>IDNG64 v5 Network Configuration</h2>");

										
					strcat((char*)(_bufferRx),"<h3>");
					strcat((char*)(_bufferRx),"Network IP");
					strcat((char*)(_bufferRx),"<input type=\"text\"name=\"ipValue\"value=\"");
					_webserverPrintIp(&_ipAdrress[0], &_bufferRx[0]);
					strcat((char*)(_bufferRx),"\"style=\"width:90%;padding:12px;margin:8px;border-radius:8px;\">");

					// ip
					// submask
					strcat((char*)(_bufferRx),"Network Submask");
					strcat((char*)(_bufferRx),"<input type=\"text\"name=\"submaskValue\"value=\"");
					_webserverPrintIp(&_subMask[0],&_bufferRx[0]);
					strcat((char*)(_bufferRx),"\"style=\"width:90%;padding:12px;margin:8px;border-radius:8px;\">");
					// submask
					// gateway
//					strcat((char*)(_bufferRx),"Network Gateway");
//					strcat((char*)(_bufferRx),"<input type=\"text\"name=\"gtwValue\"value=\"");
//					_webserverPrintIp(&_gtwAddress[0],&_bufferRx[0]);
//					strcat((char*)(_bufferRx),"\"style=\"width:90%;padding:12px;margin:8px;border-radius:4px;\">");
					// gateway					
					// nombre edificio
					strcat((char*)(_bufferRx),"Building Name");
					strcat((char*)(_bufferRx),"<input type=\"text\"name=\"edificioNombre\"value=\"");
					strcat((char*)(_bufferRx),(char*)(&_edificioString[0]));
					strcat((char*)(_bufferRx),"\"style=\"width:90%;padding:12px;margin:8px;border-radius:8px;\">");
					// nombre edificio							
					// nombre linea
					strcat((char*)(_bufferRx),"Line Name");
					strcat((char*)(_bufferRx),"<input type=\"text\"name=\"lineaNombre\"value=\"");
					strcat((char*)(_bufferRx),(char*)(&_lineaString[0]));	
					strcat((char*)(_bufferRx),"\"style=\"width:90%;padding:12px;margin:8px;border-radius:8px;\">");
					// nombre linea							
					// nombre linea
					strcat((char*)(_bufferRx),"Password");
					strcat((char*)(_bufferRx),"<input type=\"password\"name=\"adminPass\"value=\"");
					//strcat((char*)(_buffer),(char*)(_adminPassString));	
					strcat((char*)(_bufferRx),"\"style=\"width:90%;padding:12px;margin:8px;border-radius:8px;\">");

					
					#ifdef WEBSERVER_SEARCH
						strcat((char*)(_buffer),"<input type=\"submit\" name=\"SearchBtn\" value=\"Search\">");	
					#endif
					
					strcat((char*)(_bufferRx),"<input type=\"submit\" name=\"ApplyBtn\" value=\"Apply\">");	
					strcat((char*)(_bufferRx),"<input type=\"submit\" name=\"RebootBtn\" value=\"Reboot\">");	
					strcat((char*)(_bufferRx),"<input type=\"submit\" name=\"MaxBtn\" value=\"Max\">");	
					strcat((char*)(_bufferRx),"<input type=\"submit\" name=\"OffBtn\" value=\"Off\"></div>");	
					
					if (_maxBtn==0x00 & _offBtn==0x00 & _rebootBtn==0x00 & _applyBtn==0x00){
						strcat((char*)(_bufferRx),"<br><br><br>");
					}
						
					if (_postidx >= 0) 
					{
						if (_applyBtn==0x01)
						{
							strcat((char*)(_bufferRx),"<small>");
							if (strcmp((char*)(_adminPassString),(char*)(_gwadminPassString))==0x00)
							{
			
								strcat((char*)(_bufferRx),"<h2>You must reboot this gateway to apply these changes.</h2>");
							}
							else
							{
							strcat((char*)(_bufferRx),"<h2>Wrong password.</h2>");
							_applyBtn=0x00;
							}
							strcat((char*)(_bufferRx),"</small>");
						}						
						else if (_rebootBtn==0x01)
						{
							strcat((char*)(_bufferRx),"<small>");
							if (strcmp((char*)(_adminPassString),(char*)(_gwadminPassString))==0x00)
							{
							strcat((char*)(_bufferRx),"<h2>Reboot request received.</h2>");
							}
							else
							{
							strcat((char*)(_bufferRx),"<h2>Wrong password.</h2>");
							_rebootBtn=0x00;
							}
							strcat((char*)(_bufferRx),"</small>");
						}					
						else if (_maxBtn==0x01) 
						{
							strcat((char*)(_bufferRx),"<div>Done!</div>");					
						}						
						else if (_offBtn==0x01)
						{
							strcat((char*)(_bufferRx),"<div>Done!</div>");
						}	
											
					}	
					
				
					strcat((char*)(_bufferRx),"<p><small><small> MAC: ");
					webserverPrintMac(&_bufferRx[0]);
					strcat((char*)(_bufferRx),"</small></small></p>");

				
					strcat((char*)(_bufferRx),"<p><small><small>");
					strcat((char*)(_bufferRx),WEBSERVER_TEXT);
					strcat((char*)(_bufferRx),"</small></small></p>");
					if (!((_normaLinkMasterIp[0]==0) && (_normaLinkMasterIp[1]==0) && (_normaLinkMasterIp[2]==0) && (_normaLinkMasterIp[3]==0)))
					{
						strcat((char*)(_bufferRx),"<p><small><small>");
						strcat((char*)(_bufferRx),"IDNG-MG: ");
						_webserverPrintIp(&_normaLinkMasterIp[0],&_bufferRx[0]);
						strcat((char*)(_bufferRx),"</small></small></p>");
					}
					else
					{
						strcat((char*)(_bufferRx),"<p><small><small>");
						strcat((char*)(_bufferRx),"-");
						strcat((char*)(_bufferRx),"</small></small></p>");
					}	
					
					send(1,_bufferRx,strlen((char *)_bufferRx));	
					
					for (n=0;n<1000;n++)
					{
						_bufferRx[n]=0x00;
						_bufferTx[n]=0x00;
					}		
					
					sysTickDelay(100);
					disconnect(1);									
					if (_applyBtn==0x01)
					{					
						configIpAddressSet(&_ipAdrress[0], 0x00);
						configSubmaskAddressSet(&_subMask[0],0x00);
						//configGatewayAddressSet(&_gtwAddress[0],0x00);
						configNombreEdificioSet(&_edificioString[0],0x00);
						configNombreLineaSet(&_lineaString[0], 0x00);
						configStoreInFlash();
					}									
					if (_rebootBtn==0x01)
					{
						wdtReboot();
						configStoreInFlash();
						sysTickDelay(000);
						NVIC_SystemReset();
					}					
					if (_maxBtn==0x01) 
					{
							daliStop();	
							daliSend(DALI_WEBSERVER, 0xFFFF, 0x01, 0xFF, &apply[0], 0x2B, 0x05);
							
					}									
					if (_offBtn==0x01)
					{
							daliStop();
							daliSend(DALI_WEBSERVER, 0xFFFF, 0x01, 0xFF, &apply[0], 0x2B, 0x00);							
					}					
				}					
				else
				{
					sysTickDelay(1);
				}			
				break;
			}					
			case SOCK_FIN_WAIT:
      case SOCK_CLOSING:
      case SOCK_TIME_WAIT:
      case SOCK_CLOSE_WAIT:
      case SOCK_LAST_ACK:
			{
				close(1);
				break;
			}			
		}	
	}	
}
