//2017年全国大学生电子设计大赛(M题)
//管道内钢珠运动状态检测
//                       by:wowhywhat

#include <Wire.h> 
#include "LCD12864RSPI.h"
#include <Keypad.h>

#define AR_SIZE( a ) sizeof( a ) / sizeof( a[0] )

//步进电机
int EN = 8;//使能
int DIR = 9;//正反转
int PUL = 10;//脉冲

			 //12864显示的字符
unsigned char showstr[] = { 0xB5,0xE7,0xD7,0xD3,0xC9,0xE8,0xBC,0xC6,0xB4,0xF3,0xC8,0xFC };//电子设计大赛
unsigned char shownull[3] = "   ";
unsigned char showone[] = { 0xD0,0xA1,0xC7,0xF2,0xCA,0xFD,0xC4,0xBF };//小球数目
unsigned char showkeypad[] = { 0xBC,0xFC,0xC5,0xCC,0xBC,0xD3,0xBC,0xF5,0xBF,0xD8,0xD6,0xC6 };//键盘加减控制

																							 //keypad
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
					 //define the cymbols on the buttons of the keypads
char key[2];
char keys[ROWS][COLS] = {
	{ 'A','9','8','7' },
	{ 'B','6','5','4' },
	{ 'C','3','2','1' },
	{ 'D','+','0','-' }
};
//vcc 26 28 30 32 34 36 38 40
byte rowPins[ROWS] = { 24, 26, 28, 30 }; //connect to the row pinouts of the keypad
byte colPins[COLS] = { 7, 6, 5, 4 };//connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

//金属探测器 1
int MetalInduction = 2;
bool value = 1;
int num = 0;
char shownum[3];
//金属探测器 2
int MetalInduction2 = 3;
bool value2 = 1;
int num2 = 0;
char shownum2[3];

//电机延迟
int motodelay = 50;

//easytwo
char showtwos[2] = "10";
char showtwos2[2] = "01";
int i = 0;

//easythree
int readtime = 0;
int readtime1 = 0;
int readtime2 = 0;
char showread1[7];
char showread2[7];
int voicecheck = 0;


int tempint = 0;
char showtemp[2];

//diffone
bool readone = 0;
bool readtwo = 0;//设置两个布尔变量进行运动检测
int cycle = 0;
char showcycle[3];

//diffthree
int angle = 0;
char showangle[3];
int checking;
int returncheck;
int difftime1, difftime, difftime2;

//temp_text
long time_1 = 0;
long time_2 = 0;
bool state_0 = false;
bool state_1 = false;
bool change = false;
long time_over = 0;
char showtempdata[10];

//手动上下摆动
bool sensor_state_0 = false;
bool sensor_state_1 = false;
bool sensor_data_1 = false;
bool sensor_data_2 = false;
int count = 0;
char countStr[3];

//check角度与sin函数对应表
/*double checking[71][2] = {
{ 10 , 0.173648178 } ,{ 11 , 0.190808995 } ,{ 12 , 0.207911691 } ,{ 13 , 0.224951054 } ,{ 14 , 0.241921896 } ,
{ 15 , 0.258819045 } ,{ 16 , 0.275637356 } ,{ 17 , 0.292371705 } ,{ 18 , 0.309016994 } ,{ 19 , 0.325568154 } ,
{ 20 , 0.342020143 } ,{ 21 , 0.35836795 } ,{ 22 , 0.374606593 } ,{ 23 , 0.390731128 } ,{ 24 , 0.406736643 } ,
{ 25 , 0.422618262 } ,{ 26 , 0.438371147 } ,{ 27 , 0.4539905 } ,{ 28 , 0.469471563 } ,{ 29 , 0.48480962 } ,
{ 30 , 0.5 } ,{ 31 , 0.515038075 } ,{ 32 , 0.529919264 } ,{ 33 , 0.544639035 } ,{ 34 , 0.559192903 } ,
{ 35 , 0.573576436 } ,{ 36 , 0.587785252 } ,{ 37 , 0.601815023 } ,{ 38 , 0.615661475 } ,{ 39 , 0.629320391 } ,
{ 40 , 0.64278761 } ,{ 41 , 0.656059029 } ,{ 42 , 0.669130606 } ,{ 43 , 0.68199836 } ,{ 44 , 0.69465837 } ,
{ 45 , 0.707106781 } ,{ 46 , 0.7193398 } ,{ 47 , 0.731353702 } ,{ 48 , 0.743144825 } ,{ 49 , 0.75470958 } ,
{ 50 , 0.766044443 } ,{ 51 , 0.777145961 } ,{ 52 , 0.788010754 } ,{ 53 , 0.79863551 } ,{ 54 , 0.809016994 } ,
{ 55 , 0.819152044 } ,{ 56 , 0.829037573 } ,{ 57 , 0.838670568 } ,{ 58 , 0.848048096 } ,{ 59 , 0.857167301 } ,
{ 60 , 0.866025404 } ,{ 61 , 0.874619707 } ,{ 62 , 0.882947593 } ,{ 63 , 0.891006524 } ,{ 64 , 0.898794046 } ,
{ 65 , 0.906307787 } ,{ 66 , 0.913545458 } ,{ 67 , 0.920504853 } ,{ 68 , 0.927183855 } ,{ 69 , 0.933580426 } ,
{ 70 , 0.939692621 } ,{ 71 , 0.945518576 } ,{ 72 , 0.951056516 } ,{ 73 , 0.956304756 } ,{ 74 , 0.961261696 } ,
{ 75 , 0.965925826 } ,{ 76 , 0.970295726 } ,{ 77 , 0.974370065 } ,{ 78 , 0.978147601 } ,{ 79 , 0.981627183 } ,
{ 80 , 0.984807753 }
};*/

//checktime毫秒级表
int checktime[14][2] = {
	{ 10,93 },{ 12,84 },{ 14,77 },{ 16,71 },{ 18,62 },
	{ 20,58 },{ 22,56 },{ 24,52 },{ 26,51 },{ 28,48 },
	{ 30,47 },{ 32,45 },{ 34,44 },{ 36,43 }
};

//最新的数据表微秒级
long checktimes[31][2] = {
	{ 10, 146000 },{ 12, 132200 },{ 14, 122800 },{ 16, 114200 },{ 18, 108350 },
	{ 20, 104000 },{ 22, 99500 },{ 24, 95000 },{ 26, 92000 },{ 28, 87800 },
	{ 30, 83500 },{ 32, 80450 },{ 34, 77000 },{ 36, 75000 },{ 38, 73000 },
	{ 40, 70300 },{ 42, 68600 },{ 44, 66700 },{ 46, 65700 },{ 48, 64570 },
	{ 50, 63000 },{ 52, 61900 },{ 54, 60500 },{ 56, 59700 },{ 58, 59000 },
	{ 60, 58000 },{ 62, 57000 },{ 65, 54720 },{ 70, 54450 },{ 75, 53950 },
	{ 80, 51760 }
};

//小数转字符串
void dtostr(double val, signed char width, unsigned char prec, char* s)
{
	/*
	char* dtostrf(double _val,signed char _width, unsigned char prec, char* _s)
	_val:要转换的float或者double值。
	_width:转换后整数部分长度。
	_prec：转换后小数部分长度。
	_s:保存到该char数组中。
	*/
	dtostrf(val, width, prec, s);
}

//easyone()
void easyone()
{
	num = 0;
	LCDA.DisplayString(0, 0, showone, AR_SIZE(showone));//第一行第一格显示当前小球
	while (1)
	{
		char key = keypad.getKey();
		if (key == 'A')
		{
			break;
		}
		MetalInductions();
		itoa(num, shownum, 10);
		LCDA.DisplayString(1, 0, shownum, AR_SIZE(shownum));
	}
}

//电机上升
void motoup()
{
	for (int i = 0; i < 1000; i++)//3200
	{
		digitalWrite(DIR, HIGH);
		digitalWrite(PUL, HIGH);
		delayMicroseconds(motodelay);
		digitalWrite(PUL, LOW);
		delayMicroseconds(motodelay);
	}
}
//电机下降
void motodowm()
{
	for (int i = 0; i < 1000; i++)//3200
	{
		digitalWrite(DIR, LOW);
		digitalWrite(PUL, HIGH);
		delayMicroseconds(motodelay);
		digitalWrite(PUL, LOW);
		delayMicroseconds(motodelay);
	}
}
//differone motogo
void diffmotoDown()
{
	digitalWrite(DIR, LOW);

	digitalWrite(PUL, HIGH);
	delayMicroseconds(motodelay);
	digitalWrite(PUL, LOW);
	delayMicroseconds(motodelay);
}

void diffmotoUp()
{
	digitalWrite(DIR, HIGH);

	digitalWrite(PUL, HIGH);
	delayMicroseconds(motodelay);
	digitalWrite(PUL, LOW);
	delayMicroseconds(motodelay);
}

//电机+、-控制上升下降
void moto()
{
	LCDA.DisplayString(0, 0, showkeypad, AR_SIZE(showkeypad));
	while (1)
	{
		char key = keypad.getKey();
		if (key == '+')//上升
		{
			motoup();
		}
		if (key == '-')//下降
		{
			motodowm();
		}
		if (key == 'D')//确认退出
		{
			LCDA.CLEAR();//清屏
			break;
		}
	}
}

//easytwo
void easytwo()
{
	moto();
	num = num2 = 0;
	while (1)
	{
		char key = keypad.getKey();
		if (key == 'A')
		{
			break;
		}
		//LCDA.DisplayString(1, 0, showstr, AR_SIZE(showstr));

		/*if (num == num2)
		{
		MetalInductions();//读取第一个金属探测值
		MetalInductions2();//读取第二个金属探测值
		}
		else
		{
		while (1)
		{
		char key = keypad.getKey();
		if (key == 'A')
		{
		break;
		}
		if (num == 1)
		{
		int i = 01;
		itoa(i, showtwos, 10);
		LCDA.DisplayString(1, 0, showtwos, AR_SIZE(showtwos));
		break;
		}
		else
		{
		int i = 10;
		itoa(i, showtwos, 10);
		LCDA.DisplayString(1, 0, showtwos, AR_SIZE(showtwos));
		break;
		}
		}
		}*/

		easytwo1();//检测第一个传感器时候为低电平，如果是打印
		easytwo2();//检测第二个传感器时候为低电平，如果是打印

		/*if (key == 'B')
		{
		num = num2 = 0;//按下B键执行清零
		}*/
	}
}
/*
//查表方式一
int checkdouble(double checkin)
{
double check = checkin;
int val = 0;//角度
int i = 0;
for (i = 0; i < 71; i++)
{
double checksmall = checking[i][1];
double checkbig = checking[i + 1][1];
if (check >= checksmall&&check < checkbig) {
return checking[i][0];
}
}
return 1;
}
*/

//easythree测试
void ceshieasythree()
{

	moto();//让其选择角度
		   /*	readtime = millis();
		   easythrees();*/
	temp_text();
	voicecheck = tempint / 10;
	voicecheck = data_found4(voicecheck);
	Serial.print(voicecheck);
	Serial.print("      ");
	Serial.println(voicecheck);
	itoa(voicecheck, showtemp, 10);
	LCDA.DisplayString(0, 0, showtemp, AR_SIZE(showtemp));
}


//easythree
void easythree()
{

	moto();//让其选择角度
		   /*	readtime = millis();
		   easythrees();*/
	temp_text();//微秒级计算
	/*	tempint = data_found4(time_over);
	Serial.println(time_over);
	Serial.print("      ");
	Serial.print(tempint);
	itoa(voicecheck, showtemp, 10);
	LCDA.DisplayString(0, 0, showtemp, AR_SIZE(showtemp));*/
}

//计算角度的第二种方法
void easythreedouble()
{

	moto();//让其选择角度
		   /*	readtime = millis();
		   easythrees();*/
	temp_text2();
	/*	tempint = data_found4(time_over);
	Serial.println(time_over);
	Serial.print("      ");
	Serial.print(tempint);
	itoa(voicecheck, showtemp, 10);
	LCDA.DisplayString(0, 0, showtemp, AR_SIZE(showtemp));*/
}

//查时间表方式三采用相等的方法
/*
unsigned char data_found3(int data)
{
	while (1)
	{
		for (unsigned char i = 0; i < 28; i++)
		{
			if (checktimes[i][1] == data)
			{
				return (checktimes[i][0]);
			}
		}
	}
}*/
//查时间表方式四
unsigned char data_found4(long data)
{
	while (1)
	{
		for (long i = 0; i < 31; i++)
		{
			if (checktimes[i][1] <= data)
			{
				return (checktime[i][0]);
			}
		}

		data = data + 1;//防止查询出现错误时的保守措施，但查询无数据时使查询值不断+1使其有数据产生
	}
}
//查时间表方式四补充
unsigned char data_found44(long data)
{
	while (1)
	{
		for (long i = 0; i < 32; i++)
		{
			if (checktimes[i][1] <= data)
			{
				return (checktime[i][0]);
			}
			if (i = 31)
			{
				return 80;
			}
		}

		data = data + 1;
	}
}
//查时间表方式五
unsigned char data_found5(int data)
{
	while (1)
	{
		for (long i = 0; i < 28; i++)
		{
			if (checktimes[i][1] <= data)
			{
				return (checktime[i][0]);
			}
		}

		data = data + 1;
	}
}

//easythree second已弃用
/*void easythrees()
{
 readtime1 = angle = 0;
 if (readtime < 0)
 {
 readtime = readtime * (-1);
 }
 while (1)
 {
 char key = keypad.getKey();
 if (key == 'A')
 {
 break;
 }
 easythree1();
 easythree2();
 itoa(readtime1, showread1, 10);
 LCDA.CLEAR();//清屏
 LCDA.DisplayString(1, 0, showread1, AR_SIZE(showread1));
 }
}*/

//读取金属探测器1
void MetalInductions()
{
	value = digitalRead(MetalInduction);
	if (value == LOW)
	{

		while (1)
		{
			value = digitalRead(MetalInduction);
			if (value == HIGH)
			{
				break;
			}
		}
		num++;
		Serial.println(num);
	}
	/*value = digitalRead(MetalInduction);
	if (value == LOW)
	{
	num++;
	Serial.println(num);
	delay(50);
	}*/
}

//读取金属探测器2
void MetalInductions2()  
{
	value2 = digitalRead(MetalInduction2);
	if (value2 == LOW)
	{

		while (1)
		{
			value2 = digitalRead(MetalInduction2);
			if (value2 == HIGH)
			{
				break;
			}
		}
		num2 = num2 + 1;
		Serial.println(num2);
	}
	/*value = digitalRead(MetalInduction);
	if (value == LOW)
	{
	num++;
	Serial.println(num);
	delay(50);
	}*/
}

//普通金属探测器读取已弃用
void delayMetalInductions()
{
	/*value = digitalRead(MetalInduction);
	if (value == LOW)
	{

	while (1)
	{
	value = digitalRead(MetalInduction);
	if (value == HIGH)
	{
	break;
	}
	}
	num++;
	Serial.println(num);
	}*/
	value = digitalRead(MetalInduction);
	if (value == LOW)
	{
		num++;
		Serial.println(num);
		delay(50);
	}
}
//读取金属探测器2已弃用
void delayMetalInductions2()  //读取 金属探测器的电平
{
	/*	value2 = digitalRead(MetalInduction2);
	if (value2 == LOW)
	{

	while (1)
	{
	value2 = digitalRead(MetalInduction2);
	if (value2 == HIGH)
	{
	break;
	}
	}
	num2 = num2 + 1;
	Serial.println(num2);
	}*/
	value = digitalRead(MetalInduction);
	if (value == LOW)
	{
		num++;
		Serial.println(num);
		delay(50);
	}
}

//easythree2
void easythree2()  //读取 金属探测器的电平
{
	value2 = digitalRead(MetalInduction2);
	if (value2 == LOW)
	{
		readtime = millis();
	}
}
//easythree1
void easythree1()
{
	value = digitalRead(MetalInduction);
	if (value == LOW)
	{
		readtime1 = millis();
		readtime1 = readtime1 - readtime;
	}
}


//easytwo1
void easytwo1()
{
	value = digitalRead(MetalInduction);
	if (value == LOW)
	{
		LCDA.CLEAR();//清屏
					 //	i = 01;
					 //	itoa(i, showtwos, 10);
		LCDA.DisplayString(1, 0, showtwos, AR_SIZE(showtwos));
		//delay(50);
		while (1)
		{
			char key = keypad.getKey();
			if (key == 'A')
			{
				break;
			}
		}
	}
	/*value = digitalRead(MetalInduction);
	if (value == LOW)
	{
	num++;
	Serial.println(num);
	delay(50);
	}*/
}
//easytwo2
void easytwo2()  //读取 金属探测器的电平
{
	value2 = digitalRead(MetalInduction2);
	if (value2 == LOW)
	{
		LCDA.CLEAR();//清屏
					 //	i = 10;
					 //	itoa(i, showtwos, 10);
		LCDA.DisplayString(1, 0, showtwos2, AR_SIZE(showtwos2));
		while (1)
		{
			char key = keypad.getKey();
			if (key == 'A')
			{
				break;
			}
		}
	}
	/*value = digitalRead(MetalInduction);
	if (value == LOW)
	{
	num++;
	Serial.println(num);
	delay(50);
	}*/
}

//上下摆动管道
void diffmotogo()
{

	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 1600; i++)
		{
			diffmotoUp();
		}
	}
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 1600; i++)
		{
			diffmotoDown();
		}
	}
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 1600; i++)
		{
			diffmotoDown();
		}
	}
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 1600; i++)
		{
			diffmotoUp();
		}
	}
}

//diffonecheck
void diffonecheck1()
{
	value = digitalRead(MetalInduction);
	if (value == LOW)
	{
		readone++;
		//	delay(50);
	}
}

//diffonecheck2
void diffonecheck2()
{
	value2 = digitalRead(MetalInduction2);
	if (value2 == LOW)
	{
		readtwo++;
		//delay(50);
	}
}

//发挥一
void diffone()
{
	motodelay = 18;
	cycle = 0 - 1;
	while (1)
	{
		diffmotogo();
		char key = keypad.getKey();
		if (key == 'A')
		{
			break;
		}
		diffonecheck1();
		diffonecheck2();
		if (readone == readtwo)
		{
			cycle++;
			itoa(cycle, showcycle, 10);
			LCDA.DisplayString(1, 0, showcycle, AR_SIZE(showcycle));
		}
	}
}

//发挥二
void difftwo()
{
	num = 0;
	LCDA.DisplayString(0, 0, showone, AR_SIZE(showone));//第一行第一格显示当前小球
	while (1)
	{
		char key = keypad.getKey();
		if (key == 'A')
		{
			break;
		}
		MetalInductions();
		itoa(num, shownum, 10);
		LCDA.DisplayString(1, 0, shownum, AR_SIZE(shownum));
	}
}

//查时间表已弃用
int diffchecktime(int diffcheck)
{
	int check = diffcheck;
	int val = 0;//角度
	int i = 0;
	for (i = 0; i < 15; i++)
	{
		int checksmall = checktime[i][1];
		int checkbig = checktime[i + 1][1];
		if (check >= checksmall&&check < checkbig) {
			return checktime[i][0];
		}
	}
	return 1;
}
//查时间表2
unsigned char data_found(int data)
{
	while (1)
	{
		for (unsigned char i = 0; i < 14; i++)
		{
			if (checktime[i][1] == data)
			{
				return (checktime[i][0]);
			}
		}

		data = data + 1;
	}
}


//发挥三
void diffthree()
{
	readtime1 = angle = 0;
	moto();
	while (1)
	{
		char key = keypad.getKey();
		if (key == 'A')
		{
			break;
		}
		easythree2();
		easythree1();
		//angle = diffchecktime(readtime1);
		angle = data_found(readtime1);
		LCDA.CLEAR();//清屏
		itoa(angle, showangle, 10);
		LCDA.DisplayString(0, 0, showangle, AR_SIZE(showangle));

		Serial.print(readtime1);
		Serial.print("     ");
		Serial.println(angle);

		itoa(readtime1, showread1, 10);
		LCDA.DisplayString(1, 0, showread1, AR_SIZE(showread1));
	}

}

//微秒级计算
void temp_text()
{
	attachInterrupt(0, Interrupt_0, FALLING);//当int.0下降沿时,触发中断函数Interrupt0
	attachInterrupt(1, Interrupt_1, FALLING);//当int.1上升沿时,触发中断函数Interrupt1

	while (1)
	{
		char key = keypad.getKey();
		if (key == 'A')
		{
			detachInterrupt(0);
			detachInterrupt(1);
			break;
		}

		if (change == true)
		{
			time_over = 0;
			time_over = time_1 - time_2;
			Serial.println(time_over);
			tempint = data_found4(time_over);//查表赋值
			/*Serial.print(time_over);
			Serial.print("      ");
			Serial.println(tempint);*/
			itoa(tempint, showtemp, 10);
			LCDA.DisplayString(0, 0, showtemp, AR_SIZE(showtemp));
			/*
			LCDA.CLEAR();//清屏
			itoa(time_over, showtempdata, 10);
			LCDA.DisplayString(0, 0, showtempdata, AR_SIZE(showtempdata));*/

			change = false;
		}
	}
}

void temp_text2()
{
	attachInterrupt(0, Interrupt_0, FALLING);//当int.0下降沿时,触发中断函数Interrupt0
	attachInterrupt(1, Interrupt_1, FALLING);//当int.1上升沿时,触发中断函数Interrupt1

	while (1)
	{
		char key = keypad.getKey();
		if (key == 'A')
		{
			detachInterrupt(0);
			detachInterrupt(1);
			break;
		}

		if (change == true)
		{
			time_over = 0;
			time_over = time_1 - time_2;
			Serial.println(time_over);
			tempint = data_found44(time_over);
			Serial.print(time_over);
			Serial.print("      ");
			Serial.println(tempint);
			itoa(tempint, showtemp, 10);
			LCDA.DisplayString(0, 0, showtemp, AR_SIZE(showtemp));
			/*
			LCDA.CLEAR();//清屏
			itoa(time_over, showtempdata, 10);
			LCDA.DisplayString(0, 0, showtempdata, AR_SIZE(showtempdata));*/

			change = false;
		}
	}
}

void Interrupt_0()//中断函数
{
	state_0 = 1 - state_0;

	if (state_0 == true) //这一段为刚进来的时候
	{
		attachInterrupt(0, Interrupt_0, RISING);	//设定为上升沿，出去时触发
	}

	if (state_0 == false) //这一段是刚出去的时候
	{
		attachInterrupt(0, Interrupt_0, FALLING);	//重新设定为下降升沿，另一个新进入时触发
		time_1 = micros();
		change = true;
	}

}

void Interrupt_1()//中断函数
{
	state_1 = 1 - state_1;

	if (state_1 == true)	//这一段为刚进来的时候
	{
		attachInterrupt(1, Interrupt_1, RISING);	//设定为上升沿，出去时触发
		time_2 = micros();
	}

	if (state_1 == false)  //这一段是刚出去的时候
	{
		attachInterrupt(1, Interrupt_1, FALLING);	//重新设定为下降升沿，另一个新进入时触发
	}

}

//摆动测试
void up_to_down()
{
	attachInterrupt(0, sensor_Interrupt_0, FALLING);//当int.0下降沿时,触发中断函数Interrupt0
	attachInterrupt(1, sensor_Interrupt_1, FALLING);//当int.1上升沿时,触发中断函数Interrupt1

	count = 0;

	while (1)
	{
		char key = keypad.getKey();
		if (key == 'A')
		{
			detachInterrupt(0);
			detachInterrupt(1);
			break;
		}

		LCDA.CLEAR();//清屏
		itoa((count / 2), countStr, 10);
		LCDA.DisplayString(0, 0, countStr, AR_SIZE(countStr));

		up_down();



	}
}

void up_down()
{
	if (sensor_data_1 == 1)
	{
		if (sensor_data_2 == 0)
		{
			//Serial.println("here");
			while (1)
			{
				if (sensor_data_1 == 1)
				{
					delayMicroseconds(10);
					Serial.println("this");

					if (sensor_data_2 == 1)
					{
						//Serial.println("why");
						count++;
						sensor_data_1 = 0;
						sensor_data_2 = 0;
						Serial.println(count);

						break;
					}
				}

				if (sensor_data_1 == 0)
				{
					Serial.println("this");
					delayMicroseconds(10);

					if (sensor_data_2 == 0)
					{
						count++;
						Serial.println(count);

						break;
					}
				}
			}
		}
	}

	if (sensor_data_1 == 0)
	{
		if (sensor_data_2 == 1)
		{
			while (1)
			{
				if (sensor_data_1 == 1)
				{
					Serial.println("this");
					delayMicroseconds(10);

					if (sensor_data_2 == 1)
					{
						count++;
						Serial.println(count);
						sensor_data_1 = 0;
						sensor_data_2 = 0;

						break;
					}
				}

				if (sensor_data_1 == 0)
				{
					Serial.println("this");
					delayMicroseconds(10);

					if (sensor_data_2 == 0)
					{
						count++;
						Serial.println(count);

						break;
					}
				}
			}
		}

	}
}

void sensor_Interrupt_0()//中断函数
{
	sensor_state_0 = 1 - sensor_state_0;

	if (sensor_state_0 == true) //这一段为刚进来的时候
	{
		attachInterrupt(0, sensor_Interrupt_0, RISING);	//设定为上升沿，出去时触发
	}

	if (sensor_state_0 == false) //这一段是刚出去的时候
	{
		attachInterrupt(0, sensor_Interrupt_0, FALLING);	//重新设定为下降升沿，另一个新进入时触发
		sensor_data_1 = 1 - sensor_data_1;
	}

}

void sensor_Interrupt_1()//中断函数
{
	sensor_state_1 = 1 - sensor_state_1;

	if (sensor_state_1 == true)	//这一段为刚进来的时候
	{
		attachInterrupt(1, sensor_Interrupt_1, RISING);	//设定为上升沿，出去时触发
	}

	if (sensor_state_1 == false)  //这一段是刚出去的时候
	{
		attachInterrupt(1, sensor_Interrupt_1, FALLING);	//重新设定为下降升沿，另一个新进入时触发
		sensor_data_2 = 1 - sensor_data_2;
	}

}

void setup() {
	Serial.begin(115200);
	pinMode(MetalInduction, INPUT);  //初始化 金属探测器
	pinMode(MetalInduction2, INPUT);  //初始化 金属探测器
									  // 步进电机
	pinMode(EN, OUTPUT);//使能
	pinMode(DIR, OUTPUT);//正反转
	pinMode(PUL, OUTPUT);//脉冲
	digitalWrite(EN, LOW);
	digitalWrite(DIR, HIGH);

	//12864
	LCDA.Initialise(); // 屏幕初始化
	delay(100);
	LCDA.CLEAR();//清屏
	LCDA.DisplayString(0, 0, showstr, AR_SIZE(showstr));//第1行第2格开始，显示X
}

void loop() {

	char key = keypad.getKey();
	//基础一
	if (key == '1') {
		LCDA.CLEAR();//清屏
		easyone();
	}
	//基础二
	if (key == '2') {
		LCDA.CLEAR();//清屏
		easytwo();
	}
	//基础三
	if (key == '3') {
		LCDA.CLEAR();//清屏
		easythree();
	}
	//发挥一
	if (key == '4') {
		LCDA.CLEAR();//清屏
		up_to_down();
	}
	//发挥二
	if (key == '5') {
		LCDA.CLEAR();//清屏
		easytwo();
	}
	//发挥三
	if (key == '6') {
		LCDA.CLEAR();//清屏
		easythree();
	}

	if (key == '7') {
		LCDA.CLEAR();//清屏
		temp_text();
	}
	if (key == '8') {
		LCDA.CLEAR();//清屏
		easythreedouble();
	}
}