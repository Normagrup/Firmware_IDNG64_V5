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

void rtccInit (void);
void rtccTimeSet (Time time);
void rtccDateSet (Date date);
Time rtccTimeGet (void);
Date rtccDateGet (void);
void rtccSecondsTick (void);
void rtccMinutesTick (void);
void rtccTick (Date date, Time time);








