#include<reg52.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>


sbit DU=P2^0;
sbit WE=P2^1;
sbit noisy=P2^3;

uchar table[]={
0x3f,0x06,0x5b,0x4f,    //0~3
0x66,0x6d,0x7d,0x07,    //4~7
0x7f,0x6f,0x77,0x7c,    //8~b
0x39,0x5e,0x79,0x71    //c~f
};

uchar LEDIndex[]={
	0xff,0x7f,0xbf,0xdf,
	0xef,0xf7,0xfb,0xfd,
	0xfe
};

uchar monthDay[]={
	31,28,31,30,31,30,
	31,31,30,31,30,31
};


uchar second0=0;
uchar second1=0;
uchar min0=0;
uchar min1=0;
uchar hour0=2;
uchar hour1=1;
uchar weekday=0;
uchar count=0;

uchar *y;
uchar *m;
uchar *d;
uint year;
uchar month;
uchar day;


uchar temp[5];

/* ???? */
//uchar alarmMin0=-1;
//uchar alarmMin1=-1;
//uchar alarmHour0=-1;
//uchar alarmHour1=-1;

uchar TimeStr[]=__TIME__;
uchar DateStr[]=__DATE__;

void knockTime(){
	noisy=0;
}

// checkAlarmTime(){
// 	if(second0==0&&second1==0&&min0==alarmMin0&&min1==alarmMin1&&hour0==alarmHour0&&hour1==alarmHour1){
// 		return 1;
// 	}
// 	return 0;
// }

// alarm(){
// 	return 0;
// }


// ????
updateTime(){
	second0++;
	if(!noisy){
		noisy=1;
	}
	if(second0==10){
		second0=0;
		second1++;
		if(second1==6){
			second1=0;
			min0++;
			//if(checkAlarmTime()){
			//	alarm();
			//}
			if(min0==10){
				min0=0;
				min1++;
				if(min1==6){
					min1=0;
					hour0++;
					knockTime();
					if(hour1<2&&hour0==10){
						hour0=0;
						hour1++;
					}else if(hour1==2&&hour0==4){
						hour0=0;
						hour1=0;
						weekday++;
						if(weekday==7){
							weekday=0;
						}
					}
				}
			}
		}
	}
	return 0;
}

void delay10ms()   //?? 0us
{
    unsigned char a,b,c;
    for(c=1;c>0;c--)
        for(b=38;b>0;b--)
            for(a=130;a>0;a--);
}

void delay1ms()   //?? 0us
{
    unsigned char a,b,c;
    for(c=1;c>0;c--)
        for(b=142;b>0;b--)
            for(a=2;a>0;a--);
}

uchar getMonthDay(uint year,uchar month){
	if(month==2){
		return (((year%4 == 0) && (year%100 != 0) || (year%400 == 0))?29:28);
	}
	return monthDay[month-1];
}

uchar calcWeekDay(uint year,uchar month,uchar day){
	uint nday=0;
	uint i;
	for(i=1900;i<year;i++){
		nday+=((i%4==0&&i%100!=0)||(i%400==0)?366:365);
	}
	for(i=1;i<month;i++){
		nday+=getMonthDay(year,i);
	}
	nday+=day;
	return nday%7;
}

uchar getWeekDay(){
	m=strtok(DateStr," ");
	d=strtok(NULL," ");
	day=atoi(d);
	y=strtok(NULL," ");
	year=atoi(y);
	if(!strcmp(m,"Jan")){
		month=1;
	}else if(!strcmp(m,"Feb")){
		month=2;
	}else if(!strcmp(m,"Mar")){
		month=3;
	}else if(!strcmp(m,"Apr")){
		month=4;
	}else if(!strcmp(m,"May")){
		month=5;
	}else if(!strcmp(m,"Jun")){
		month=6;
	}else if(!strcmp(m,"Jul")){
		month=7;
	}else if(!strcmp(m,"Aug")){
		month=8;
	}else if(!strcmp(m,"Sep")){
		month=9;
	}else if(!strcmp(m,"Oct")){
		month=10;
	}else if(!strcmp(m,"Dec")){
		month=11;
	}else if(!strcmp(m,"Nov")){
		month=12;
	}
	
	return calcWeekDay(year,month,day);
}

//?????,???????
void initTime(){
	hour1=TimeStr[0]-'0';
	hour0=TimeStr[1]-'0';
	min1=TimeStr[3]-'0';
	min0=TimeStr[4]-'0';
	second1=TimeStr[6]-'0';
	second0=TimeStr[7]-'0';
	weekday=getWeekDay();
}

void lightLED(index,number){
	DU=1;
	P0=table[number];
	DU=0;
	P0=0xff;
	WE=1;
	P0=LEDIndex[index];
	WE=0;
}

// ????,?????
void showTime(){
	lightLED(8,hour1);
	delay1ms();
	lightLED(7,hour0);
	delay1ms();
	lightLED(6,min1);
	delay1ms();
	lightLED(5,min0);
	delay1ms();
	lightLED(4,second1);
	delay1ms();
	lightLED(3,second0);
	delay1ms();
	lightLED(1,weekday);
	delay1ms();
}



void init(){
	//EX0=1;
	EX1=1;
	IT0=1;
	IT1=1;
	ET0=1;
	ET1=1;
	TMOD=0x61;
	TL0=0x00;
	TH0=0x4c;
	TH1=0xf7;
	TL1=0xf7;
	TR0=1;
	TR1=1;
	EA=1;
	T1=0;
}

// ???
timer0() interrupt 1{
	//count++;
	//if(count==18){
		//updateTime();
		//count=0;
	//}
	T1=~T1;
	TH0=0x3c;
	TL0=0xb0;
}

//???
timer1() interrupt 3{
	updateTime();
}

// ???? ????
int0() interrupt 0{
	TR0=~TR0;
}


void main(){
	init();
	initTime();
	while(1){
		showTime();
	}
}