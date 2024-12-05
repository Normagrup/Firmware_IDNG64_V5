#include "polling.h"
#include "rtcc.h"
#include "temp.h"
#include "test.h"
#include "normalink.h"
#include "ethTransceiver.h"

#ifndef TIME_S
	typedef struct time_s
	{
			unsigned char hours;
			unsigned char minutes;
			unsigned char seconds;
	} Time;
#define	TIME_S
#endif

#ifndef DATE_S
	typedef struct date_s
	{
		unsigned char day;
		unsigned char weekday;
		unsigned char month;
		unsigned char year;
	} Date;
#define	DATE_S
#endif

Date _RTCC_date;
Time _RTCC_time;
	
void rtccInit (void)
{
	// TODO
}


unsigned char dec2hex (unsigned char dec)
{
	unsigned char digit[2];	
	digit[1]=(dec/10);
	digit[0]=(dec-(10*digit[1]));	
	return (16*digit[1]+digit[0]);	
}

unsigned char hex2dec (unsigned char hex)
{
	unsigned char digit[2];	
	digit[1]=(hex/16);
	digit[0]=(hex-(16*digit[1]));	
	return (10*digit[1]+digit[0]);	
}

void rtccTimeSet (Time time)
{
	time.hours=hex2dec(time.hours);
	time.minutes=hex2dec(time.minutes);
	time.seconds=hex2dec(time.seconds);	
	_RTCC_time=time;	
	boardTimeSet(time);	
}

void rtccDateSet (Date date)
{
	date.day=hex2dec(date.day);
	date.month=hex2dec(date.month);
	date.weekday=hex2dec(date.weekday);
	date.year=hex2dec(date.year);
	_RTCC_date=date;	
	boardDateSet(date);	
}

Time rtccTimeGet (void)
{
	Time time;
	time=_RTCC_time;
	time.hours=dec2hex(time.hours);
	time.minutes=dec2hex(time.minutes);
	time.seconds=dec2hex(time.seconds);	
	return time;
}

Date rtccDateGet (void)
{
	Date date;
	date=_RTCC_date;
	date.day=dec2hex(date.day);
	date.weekday=dec2hex(date.weekday);
	date.month=dec2hex(date.month);
	date.year=dec2hex(date.year);	
	return date;
}

void rtccSecondsTick (void)
{	
	sensoresRtcTick();
	polling_Tick();
	daliLoopRtccTick();
	wdtTick();
	tempTick();
}

void rtccMinutesTick (void)
{
	testRTCC_Tick();
	tempCheck();
	normaLinkTick();
	ethTransceiverRTCC_Tick();

}

void rtccTick (Date date, Time time)
{
	_RTCC_date=date;
	_RTCC_time=time;	
	if (time.seconds==0)
	{
		rtccMinutesTick();
	}
	rtccSecondsTick();	
}
