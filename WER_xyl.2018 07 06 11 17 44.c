#include "ASEIO.h"
#define true 1
#define false 0

//===varDecl===//非参变量定义 
int dbgLevel = 0;
int dh[7]={0};
char *subNameP;
char *subNameC;
char *subNameN;
int curSeq;
bool isServInit = false;
//=============//
//===funcDecl===//函数预定义 
void runsub(int sub,bool getName);
void initServ();
void Drive_Init();//1
void Drive_Departure();//2
void Drive_UpStair();//3
void Drive_Ontime();//4
void Drive_UtlStop();//5
void Drive_CrossJunc();//6
void Drive_UtlLeft();//7
void Drive_UtlRight();//8
void Drive_TurnLeft();//9
void Drive_TurnRight();//10
void Drive_OnlyLeft();//11
void Drive_OnlyRight();//12
void Drive_SpeedUp();//13
void Drive_SpeedDown();//14
void Drive_Fwd();//15
void Drive_Back();//16
void Mission_N01();//101
void Mission_N02();//102
void Mission_N03();//103
void Mission_N04();//104
void Mission_N05();//105
void Mission_N06();//106
void Mission_N07();//107
void Mission_N08();//108
void Mission_N09();//109
void Mission_N10();//110
void Mission_N11();//111
void Mission_N12();//112
void Mission_N13();//113
void Mission_N14();//114
void Mission_N15();//115
void Mission_N16();//116
void Act_01();//201
void Act_02();//202
void Act_03();//203
void Act_04();//204
void Act_05();//205
void Act_06();//206
void Act_07();//207
void Act_08();//208
void Act_09();//209
void Act_10();//210
void Act_11();//211
void Act_12();//212
void Act_13();//213
void Act_14();//214
void Act_15();//215
void Act_16();//216
void Act_17();//217
void Act_18();//218
void Act_19();//219
void Act_20();//220
bool dhs(int port);
int rawDh(int port);
void drive(int lspd,int rspd);
void setServ(float angle,int speed);
void dhUpdate();
void ReadEE();
void ReadEE_Fast();
void WriteEE();
void main_match();
void main_test();
void rldServ();
bool isMotorStop(); 
//==============//

//=================参数设置=================//
//--------------Args--------------//
//按钮 
const int buttonPort = 11;//外接确认按钮端口 
const int selEE = 7;
//地灰 
const bool useSafedh = true;				//使用"安全地灰" (避开地面绿色) 
const int safedh = 100;						//地灰值增量 
const bool ReadEEOnStart = true;			//启动时从EEPROM中读取地灰值 
const int dhPort[7] = {2,3,4,5,6,7,8};	//地灰端口 (由左至右) 
const int EEAddr[7] = {0,1,2,3,4,5,6};	//存储地灰值的EEPROM地址 (由左至右) 
//电机 
const int lMotorPort = 0;					//左电机端口
const int rMotorPort = 1;					//右电机端口
const bool invertL = true;					//反转左电机
const bool invertR = false;				//反转右电机
float lcoe = 1;								//左电机速度系数 
float rcoe = 1; 								//右电机速度系数 
const bool isMotoCL = false;				//速度闭环(???) 
//舵机 
const int servoPort = 9;					//舵机端口
const int servoID = 1;						//舵机ID 
//电池 
const int battMin = 7800;					//电池电压最小值mV
const int battMax = 8400;					//电池电压最大值mV
//--------------Drive--------------//
#define hspeed 60
#define mspeed 45
#define lspeed -60
#define tspeed 50
//60 45 55 40
//50 40 40 40 CL
//Departure 2
const int Speed_2 = hspeed;
//UpStair 3
const int lSpeed_3 = lspeed;
const int mSpeed_3 = 65;
const int hSpeed_3 = 80;
const int tSpeed_3 = 50;
const int upSpeed_3 = 100;
const float upDelay_3 = 0.3;
const int dhdet_3 = 200;
//Ontime 4
const int lSpeed_4 = lspeed;
const int mSpeed_4 = mspeed;
const int hSpeed_4 = hspeed;
const float time_4 = 0.1;
//UtlStop 5
const int lSpeed_5 = lspeed;
const int mSpeed_5 = 40;
const int hSpeed_5 = 50;
const int tSpeed_5 = 30;
//CrossJunc 6
const int lSpeed_6 = lspeed;
const int mSpeed_6 = mspeed;
const int hSpeed_6 = hspeed;
const int tSpeed_6 = tspeed;
//Left 7
const int hSpeed_7 = hspeed;
const int mSpeed_7 = mspeed;
const int lSpeed_7 = lspeed;
const int tSpeed_7 = tspeed;
const float btDelay_7 = 0.15;
const float bttDelay_7 = 0.3;
//Right 8
const int hSpeed_8 = hspeed;
const int mSpeed_8 = mspeed;
const int lSpeed_8 = lspeed;
const int tSpeed_8 = tspeed;
const float btDelay_8 = 0.15;
const float bttDelay_8 = 0.3;
//TLeft 9
const int tSpeed_9 = tspeed;
const float bttDelay_9 = 0.3;
//TRight 10
const int tSpeed_10 = tspeed;
const float bttDelay_10 = 0.3;
//OnlyLeft 11
const int lSpeed_11 = lspeed;
const int mSpeed_11 = mspeed;
const int hSpeed_11 = hspeed;
const float time_11 = 0.5;
//OnlyRight 12
const int lSpeed_12 = lspeed;
const int mSpeed_12 = mspeed;
const int hSpeed_12 = hspeed;
const float time_12 = 0.5;
//SpeedUp 13
const float dtSpeed_13 = 0.2;
//SpeedDown 14
const float dtSpeed_14 = 0.2;
//Fwd 15
const int lSpeed_15 = lspeed;
const float delay_15 = 0.1;
//Back 16
const int lSpeed_16 = lspeed;
const float delay_16 = 0.1;
//---------------------------------//
//Debug
int defdbgLevel = 2; 
float dbgSeqDelay = 1;
int defSequence = 0;
float dbgDelay = 0.0;
//---------------------------------//
bool isMatch = false;
//==========================================//

//=================SEQUENCE=================//
//子程序序列(0-49) 
int Sequence[50][100]= {{2,8,7,7,8,7,7,8,9,7,8,8,7,8,6},//单 航  
						{2,7,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8},//单转换 
						{201,2,8,7,202,6,6,114},   //单下载
						{201,2,8,8,5,111,9,7,7,6},  //
						{2,115}, //交换
						{201,1,2,6,101,9,3,0},  //上楼
						{201,2,8,7,8,113,9,7,8,7,6},
						{201,5,105}, 
						{201,5,104}, 
						{112},
						{6,110,10,6},
						{6,116},
						{107},
						{201,8,7,103},
						{102,10,7,103},
						{207,106}
						}; 
//子程序四行注释(0-49)
char *cmt[50][4] = {{"sao wan yi2hao","","",""},
					{"sao wan yi","zhe shi sao wan yi"," sao de bu xin","666"},
					{"","xia zai","",""},
					{"","ba kuang","",""},
					{"","jiaohuan","",""},
					{"","shanglou","",""},
					{"","jinzhi","",""},
					{"","diannao","",""},
					{"","dianhua","",""},
					{"","caiping","",""},
					{"","tuiche","",""},
					{"","diufangkuai","",""},
					{"","bd","",""},
					{"","wx(up)","",""},
					{"","gouhuan","",""},
					{"","printer","",""}
					};
							 
//========================================//
//=================================================主函数=================================================// 
void main(){//主程序 

	if(isMatch) main_match();
	else main_test();
	return;
}
void main_match(){//比赛用主程序 
	BEEP(700,0.25);
	printf("        Debug\n\n     Init Servo\n (on port %2d ID %3d)",servoPort,servoID);
	initServ();
	Drive_Init();
	if(ReadEEOnStart)ReadEE_Fast();
	curSeq = defSequence;
	int curStep = 0;
	int i=0,Steps=0;
	bool runFlag=false;
	A:
	i=0;
	Steps=0;
	curStep = 0;
	lcoe = 1.0;
	rcoe = 1.0;
	runFlag=false;
		while(true){
			while(true){
			bool cbatt=false;
			int batt;
			batt = Get_Battery(); 
			if(batt<battMin||batt>battMax)cbatt=true;
				printf("        MATCH\nChoose Sequence: %2d\n%s\n%s\n%s\n%s\nBatt:%d%s\nPrev     Sel     Next",curSeq,cmt[curSeq][0],cmt[curSeq][1],cmt[curSeq][2],cmt[curSeq][3],batt,cbatt?"<-ATTENTION":" ");
				if(Get_Button()||AI(buttonPort))break;
				if(Get_BtnLeft()){
					BEEP(1000,0.15);
					if(--curSeq<0)curSeq=49; 
					break;
				}
				if(Get_BtnRight()){
					BEEP(1000,0.15);
					if(++curSeq>49)curSeq=0;
					break;
				}
				wait(0.05);
			}
			if(Get_Button()||AI(buttonPort)){
				BEEP(1000,0.15);
				break;
			}
		}
	dbgLevel = 0;
	wait(0.5);
	for(i=0;true;i++){
		if(Sequence[curSeq][i])Steps++;
		else break;
	}
	if(!Steps){
		printf("        Debug\n\n Invalid Sequence %2d\n\n\n\n\n      Continue",curSeq);
		while(!Get_Button()||AI(buttonPort)){;}
		BEEP(1000,0.15);
		curStep = 0;
		i=0;
		Steps=0;
		goto A;
	}
	Steps--;
	printf("        MATCH\nChoose Sequence:  %2d\n\n   RUNNING SEQ %2d\n\n\n\n",curSeq,curSeq);
	while(true){
		runsub(curStep,0);
		if(++curStep>Steps){
			BEEP(1000,0.15);
			break;
		}
	}
	if(++curSeq>49)curSeq=49;
	drive(0,0);
	setServ(45,512);
	goto A;
}
void main_test(){//调试用主程序 
	BEEP(700,0.25);
	if(ReadEEOnStart)ReadEE_Fast();
	curSeq = defSequence;
	curSeq = ReadEEPROM(selEE);
	if(curSeq>49||curSeq<0)curSeq=defSequence;
	int curStep = 0;
	int i=0,Steps=0;
	bool runFlag=false;
	B:
	i=0;
	Steps=0;
	curStep = 0;
	lcoe = 1.0;
	rcoe = 1.0;
	runFlag=false;
	dbgLevel=defdbgLevel;
	int cnt = 0;
	if(dbgLevel == 2){
		while(true){
			while(true){
			bool cbatt;
			int batt;
			batt = Get_Battery();
			if(++cnt>15&&(batt<battMin||batt>battMax)){
				cbatt = true;
				if(cnt>30)cnt=0;
			}else{
				cbatt = false;
				if(batt>battMin&&batt<battMax)
				cnt=0;
			}
			WriteEEPROM(selEE,curSeq);
			printf("        Debug\nChoose Sequence: %2d\n%s\n%s\n%s\n%s\nBatt:%d%s\nPrev     Sel     Next",curSeq,cmt[curSeq][0],cmt[curSeq][1],cmt[curSeq][2],cmt[curSeq][3],batt,cbatt?"<-ATTENTION":" ");
				if(Get_Button()||AI(buttonPort))break;
				if(cnt>29&&cbatt)BEEP(2000,0.25);
				else wait(0.05);
				if(Get_Button()||AI(buttonPort))break;
				if(Get_BtnLeft()){
					BEEP(1000,0.15);
					if(--curSeq<0)curSeq=49; 
					break;
				}
				if(Get_BtnRight()){
					BEEP(1000,0.15);
					if(++curSeq>49)curSeq=0;
					break;
				}
			}
			if(Get_Button()||AI(buttonPort)){
				BEEP(1000,0.15);
				break;
			}
		}
		while(true){
			while(true){
				bool cbatt;
				int batt;
				batt = Get_Battery();
				if(++cnt>15&&(batt<battMin||batt>battMax)){
					cbatt = true;
					if(cnt>30)cnt=0;
				}else{
					cbatt = false;
					if(!batt<battMin&&!batt>battMax)
					cnt=0;
				}
				switch(dbgLevel){
				case 0:
					printf("        Debug\nChoose dbgLevel:\n\n  >0<     1      2\n\n\nBatt:%d%s\nPrev     Sel     Next",batt,cbatt?"<-ATTENTION":" ");
					break;
				case 1:
					printf("        Debug\nChoose dbgLevel:\n\n   0     >1<     2\n\n\nBatt:%d%s\nPrev     Sel     Next",batt,cbatt?"<-ATTENTION":" ");
					break;
				case 2:
					printf("        Debug\nChoose dbgLevel:\n\n   0      1     >2<\n\n\nBatt:%d%s\nPrev     Sel     Next",batt,cbatt?"<-ATTENTION":" ");
					break;
				}
				if(cnt>29&&cbatt){
					BEEP(2000,0.25);
					cnt=0;
				}
				else wait(0.05);
				if(Get_Button()||AI(buttonPort))break;
				if(Get_BtnLeft()){
					BEEP(1000,0.15);
					if(--dbgLevel<0)dbgLevel=2; 
					break;
				}
				if(Get_BtnRight()){
					BEEP(1000,0.15);
					if(++dbgLevel>2)dbgLevel=0;
					break;
				}
			}
			if(Get_Button()||AI(buttonPort)){
				BEEP(1000,0.15);
				break;
			}
		}
	}
	if(dbgLevel){
		printf("        Debug\n\n     Init Servo\n (on port %2d ID %3d)",servoPort,servoID);
		wait(0.5);
	}
	initServ();
	if(dbgLevel){
		printf("        Debug\n\n Loading Sequence %2d",curSeq);
		wait(0.5);
	}
	for(i=0;true;i++){
		if(Sequence[curSeq][i])Steps++;
		else break;
	}
	if(!Steps){
		printf("        Debug\n\n Invalid Sequence %2d\n\n\n\n\n      Continue",curSeq);
		while(!Get_Button()||AI(buttonPort)){;}
		BEEP(1000,0.15);
		curStep = 0;
		i=0;
		Steps=0;
		goto B;
	}
	Steps--;
	while(true){
		if(dbgLevel==2){
			runsub(curStep,1);
			if(curStep==0){
				printf("        Dbg%2d\n\n\n Cur==>%2d-%s\n Next: %2d-%s\n\n\nPrev     Run     Next",curSeq,curStep,subNameC,curStep+1,subNameN);
			}else if(curStep==Steps){
				printf("        Dbg%2d\n\n Prev: %2d-%s\n Cur==>%2d-%s\n\n\n\nPrev     Run     Next",curSeq,curStep-1,subNameP,curStep,subNameC);
			}else{
				printf("        Dbg%2d\n\n Prev: %2d-%s\n Cur==>%2d-%s\n Next: %2d-%s\n\n\nPrev     Run     Next",curSeq,curStep-1,subNameP,curStep,subNameC,curStep+1,subNameN);
			}
			while(true){
				if(Get_Button()||AI(buttonPort)){
					break;
				}
				if(Get_BtnLeft()){
					int buf=0;
					while(Get_BtnLeft()){
						buf++;
						wait(0.01);
						if(buf>50){
							BEEP(1000,0.15);
							printf("        Debug\n\n  Return to SeqSel?");
							wait(1);
							printf("        Debug\n\n  Return to SeqSel?\n\n\n\n\nNo                Yes");
							while(true){
								if(Get_BtnLeft()||Get_BtnRight()){
									break;
								}
							}
							if(Get_BtnLeft()){
								break;
							}
							if(Get_BtnRight()){
								BEEP(1000,0.15);
								goto B;
								break;
							}
						}
					}
					if(Get_BtnLeft()){
						BEEP(1000,0.15);
						break;
					}
					BEEP(1000,0.15);
					if(--curStep<0)curStep=Steps; 
					break;
				}
				if(Get_BtnRight()){
					int buf=0;
					bool bufFlag=true;
					while(Get_BtnRight()){
						buf++;
						wait(0.01);
						if(buf>50){
							bufFlag=false;
							BEEP(1000,0.15);
							printf("        Debug\n\n    Run dhUpdate?");
							wait(1);
							if(useSafedh)
								printf("        Debug\n\n    Run dhUpdate?\n\n     ***NOTE***\n   Safe dh Enabled\n\nNo                Yes");
							else
								printf("        Debug\n\n    Run dhUpdate?\n\n\n\n\nNo                Yes");
							while(true){
								if(Get_BtnLeft()||Get_BtnRight()){
									break;
								}
							}
							if(Get_BtnLeft()){
								while(Get_BtnLeft()){;}
							}
							if(Get_BtnRight()){
								BEEP(1000,0.15);
								dhUpdate();
								break;
							}
						}
					}
					if(bufFlag){
						BEEP(1000,0.15);
						if(++curStep>Steps)curStep=0;
						break;
					}else break;
				}
				if(Get_Button()||AI(buttonPort)){
					BEEP(1000,0.15);
					printf("        Debug\n\nRunning:\n    Seq %2d Step %2d",curSeq,curStep);
					runFlag=true;
					break;
				}
			}
			
		}
		if(runFlag||dbgLevel!=2){
			runsub(curStep,0);
			if(dbgLevel){
				drive(0,0);
				BEEP(700,0.25);
				wait(dbgDelay);
			}
			if(runFlag)runFlag=false;
			if(++curStep>Steps){
				if(dbgLevel!=2){
					printf("        Debug\n\n  Seq %2d Completed\n\n\n  *Resetting Servo*\n\n",curSeq);
					drive(0,0);
					rldServ();
					printf("        Debug\n\n  Seq %2d Completed\n\n\n\n\n      Continue",curSeq);
					BEEP(700,0.25);
					while(!Get_Button()&&!AI(buttonPort)){;}
					BEEP(1000,0.15);
					break;
				}else{
					curStep--;
				}
			}
		}
	}
	goto B;
}
//===============================================寻线类函数===============================================//
void Drive_Init(){//1 
	int i=0;
	for(i=0;i<7;i++){
		rawDh(i);
	}
}
void Drive_Departure(){//2
	drive(Speed_2,Speed_2);
	while(!(dhs(0)||dhs(6))){;}
	while(dhs(0)||dhs(6)){;}
	return;
}
void Drive_UpStair(){//3
	setServ(-90,512);
	int i=0;
	for(i=0;i<7;i++){
		dh[i]+=dhdet_3;
	}
	resettime();
	while(true){
		if(dhs(6)&&seconds()>4){
			drive(upSpeed_3,upSpeed_3);
			wait(upDelay_3);
			drive(mSpeed_3,mSpeed_3);
			break;
		}else if(dhs(1)&&!dhs(0)){
			drive(lSpeed_3,hSpeed_3);
			while(!dhs(2)){;}
		}else if(dhs(5)&&!dhs(6)){
			drive(hSpeed_3,lSpeed_3);
			while(!dhs(4)){;}
		}else if(dhs(2)){
			drive(mSpeed_3,hSpeed_3);
		}else if(dhs(4)){
			drive(hSpeed_3,mSpeed_3);
		}else if(!dhs(3)){
			drive(tSpeed_3,-tSpeed_3);
		}else{
			drive(hSpeed_3,hSpeed_3);
		}
	}
	for(i=0;i<7;i++){
		dh[i]-=dhdet_3;
	}
	drive(mSpeed_3,mSpeed_3);
	wait(0.5);
}
void Drive_Ontime(){//4
	resettime();
	while(true){
		if(seconds()>time_4){
			return;
		}
		if(dhs(1)&&!dhs(0)){
			drive(lSpeed_4,hSpeed_4);
			while(!dhs(2)){;}
		}else if(dhs(5)&&!dhs(6)){
			drive(hSpeed_4,lSpeed_4);
			while(!dhs(4)){;}
		}else if(dhs(2)){
			drive(mSpeed_4,hSpeed_4);
		}else if(dhs(4)){
			drive(hSpeed_4,mSpeed_4);
		}else{
			drive(mSpeed_4,mSpeed_4);
		}
	}
}
void Drive_UtlStop(){//5
	resettime();
	while(true){
		if(isMotorStop()&&seconds()>0.5){
			drive(0,0);
			return;
		}else if(dhs(1)&&!dhs(0)){
			drive(lSpeed_5,hSpeed_5);
			while(!dhs(2)){;}
		}else if(dhs(5)&&!dhs(6)){
			drive(hSpeed_5,lSpeed_5);
			while(!dhs(4)){;}
		}else if(dhs(2)){
			drive(mSpeed_5,hSpeed_5);
		}else if(dhs(4)){
			drive(hSpeed_5,mSpeed_5);
		}else{
			drive(mSpeed_5,mSpeed_5);
		}
	}
}
void Drive_CrossJunc(){//6
	while(true){
		if(dhs(0)||dhs(6)){
			drive(hSpeed_6,hSpeed_6);
			while((dhs(0)||dhs(6))){;}
			return;
		}else if(dhs(1)&&!dhs(0)){
			drive(lSpeed_6,hSpeed_6);
			while(!dhs(2)){;}
		}else if(dhs(5)&&!dhs(6)){
			drive(hSpeed_6,lSpeed_6);
			while(!dhs(4)){;}
		}else if(dhs(2)){
			drive(mSpeed_6,hSpeed_6);
		}else if(dhs(4)){
			drive(hSpeed_6,mSpeed_6);
		}else{
			drive(hSpeed_6,hSpeed_6);
		}
	}
}
void Drive_UtlLeft(){//7	*********************
	while(true){
		if(dhs(0)&&dhs(1)&&dhs(2)){
			drive(tSpeed_7,tSpeed_7);
			wait(btDelay_7);
			drive(0,0);
			drive(-tSpeed_7,tSpeed_7);
			wait(bttDelay_7);
			while(true){
//				if(!dhs(0)&&!dhs(5)&&!dhs(6)&&(dhs(1)||dhs(2)||dhs(3)||dhs(4)))
				if(!dhs(0)&&!dhs(3)&&!dhs(4)&&!dhs(5)&&!dhs(6)&&(dhs(1)||dhs(2)))
				return;
			}
		}else if(dhs(1)&&!dhs(0)){
			drive(lSpeed_7,hSpeed_7);
			while(!dhs(2)){;}
		}else if(dhs(5)&&!dhs(6)){
			drive(hSpeed_7,lSpeed_7);
			while(!dhs(4)){;}
		}else if(dhs(2)){
			drive(mSpeed_7,hSpeed_7);
		}else if(dhs(4)){
			drive(hSpeed_7,mSpeed_7);
		}else{
			drive(hSpeed_7,hSpeed_7);
		}
	}
}
void Drive_UtlRight(){//8	*********************
	while(true){
		if(dhs(6)&&dhs(5)&&dhs(4)){
			drive(tSpeed_8,tSpeed_8);
			wait(btDelay_8);
			drive(0,0);
			drive(tSpeed_8,-tSpeed_8);
			wait(bttDelay_8);
			while(true){
				if(!dhs(0)&&!dhs(1)&&!dhs(2)&&!dhs(3)&&!dhs(6)&&(dhs(4)||dhs(5)))
				return;
			}
		}else if(dhs(1)&&!dhs(0)){
			drive(lSpeed_8,hSpeed_8);
			while(!dhs(2)){;}
		}else if(dhs(5)&&!dhs(6)){
			drive(hSpeed_8,lSpeed_8);
			while(!dhs(4)){;}
		}else if(dhs(2)){
			drive(mSpeed_8,hSpeed_8);
		}else if(dhs(4)){
			drive(hSpeed_8,mSpeed_8);
		}else{
			drive(hSpeed_8,hSpeed_8);
		}
	}
}
void Drive_TurnLeft(){//9
	drive(-tSpeed_9,tSpeed_9);
	wait(bttDelay_9);
	while(true){
//		if(!dhs(0)&&!dhs(1)&&!dhs(5)&&!dhs(6)&&(dhs(2)||dhs(3)||dhs(4)))
//		if(!dhs(0)&&!dhs(5)&&!dhs(6)&&(dhs(1)||dhs(2)||dhs(3)||dhs(4)))
		if(!dhs(0)&&!dhs(3)&&!dhs(4)&&!dhs(5)&&!dhs(6)&&(dhs(1)||dhs(2)))
		return;
	}
}
void Drive_TurnRight(){//10
	drive(tSpeed_10,-tSpeed_10);
	wait(bttDelay_10);
	while(true){
//		if(!dhs(0)&&!dhs(1)&&!dhs(5)&&!dhs(6)&&(dhs(2)||dhs(3)||dhs(4)))
//		if(!dhs(0)&&!dhs(1)&&!dhs(6)&&(dhs(2)||dhs(3)||dhs(4)||dhs(5)))
		if(!dhs(0)&&!dhs(1)&&!dhs(2)&&!dhs(3)&&!dhs(6)&&(dhs(4)||dhs(5)))
		return;
	}
}
void Drive_OnlyLeft(){//11
	resettime();
	while(true){
		if(seconds()>time_11){
			return;
		}
		if(dhs(1)&&!dhs(0)){
			drive(lSpeed_7,hSpeed_7);
		}else if(dhs(2)){
			drive(mSpeed_7,hSpeed_7);
		}else if(dhs(4)){
			drive(hSpeed_7,mSpeed_7);
		}
	}
}
void Drive_OnlyRight(){//12
	resettime();
	while(true){
		if(seconds()>time_12){
			return;
		}
		if(dhs(5)&&!dhs(6)){
			drive(hSpeed_7,lSpeed_7);
		}else if(dhs(2)){
			drive(mSpeed_7,hSpeed_7);
		}else if(dhs(4)){
			drive(hSpeed_7,mSpeed_7);
		}
	}
}
void Drive_SpeedUp(){//13
	lcoe+=dtSpeed_13;
	rcoe+=dtSpeed_13;
}
void Drive_SpeedDown(){//14
	lcoe-=dtSpeed_14;
	rcoe-=dtSpeed_14;
}
void Drive_Fwd(){//15
	drive(lSpeed_15,lSpeed_15);
	wait(delay_15);
	return;
}
void Drive_Back(){//16
	drive(-lSpeed_16,-lSpeed_16);
	wait(delay_16);
	return;
}
//===============================================任务类函数===============================================// 
void Mission_N01(){//101  开门上楼 crossjunc后	*cp
	drive(40,40);
	wait(0.45);
	drive(55,-15);
	wait(0.55);
	drive(-20,-20);
	setServ(110,1023); 
	wait(0.3);
	drive(10,10);
	wait(0.3);
	drive(-10,-10);
	wait(0.25);
	drive(45,45);
	setServ(110,512); 
	wait(0.25);
	drive(-45,-45);
	setServ(0,512);
	wait(0.35);
	drive(30,30);
	setServ(110,512); 
	wait(0.75);
	drive(-45,-45);
	wait(0.45);
	while(!dhs(0)&&!dhs(6)){;}
	drive(30,30);
	wait(0.6);
}
void Mission_N02(){//102  勾环 
	setServ(100,512);
	Drive_UtlStop();
	drive(10,10);
	setServ(130,512);
	wait(0.2);
	drive(-45,-45);
	wait(0.5);
	drive(0,0);
	setServ(0,512);
}
void Mission_N03(){//103  卫星  Ses1 			*hcp
	setServ(-10,512);
	Drive_UtlStop();
	drive(-25,-25);
	wait(0.6);
	drive(0,0);
	setServ(125,256);
	wait(1);//
	drive(25,25);
	wait(0.7);
	drive(5,5);
	setServ(95,256);
	wait(0.5);
}
void Mission_N04(){//104  电话 无stop 			*hcp
	setServ(6,512);
	drive(-55,-55);
	wait(0.35);
	drive(55,55);
	wait(0.4);
	drive(0,0);
	wait(0.1);
	setServ(0,512);
	wait(0.1);
	drive(-55,-55);
	wait(0.4);
	setServ(70,512);
	drive(0,0);
	wait(0.15);
	drive(55,55);
	wait(0.4);
	drive(0,0);
	wait(0.2);
	drive(-55,-55);
	wait(0.4);
	setServ(-90,1023);
}
void Mission_N05(){//105  电脑 					*hcp
	drive(30,30);
	while(GetPrevSpeed(lMotorPort)||GetPrevSpeed(rMotorPort)){;}
	setServ(90,1023);
	drive(-45,-45);
	wait(0.65); 
	drive(0,0);
	wait(0.15);
	drive(45,45);
	wait(0.65);
	setServ(110,1023);//105
	wait(0.05);
	drive(0,0);
	wait(0.1);
	setServ(75,256);
	wait(0.25);
	drive(-65,-65);
	wait(0.2);
	setServ(-90,512);
}
void Mission_N06(){//106  打印  
	Act_06();
	drive(-15,-15);
	wait(0.25);
	//////
	drive(-15,20);
	wait(0.3);
	drive(20,-15);
	wait(0.3);
	drive(-15,20);
	wait(0.3);
	drive(20,-15);
	wait(0.3);
	drive(-10,20);
	wait(0.3);
	drive(20,-10);
	wait(0.3);
	//////
	drive(15,15);
	wait(0.2);
	int i=20;
	for(;i<101;i++){
		SetMoto(2,i);
		wait(0.05);
	}
	wait(10);
	SetMoto(2,0);
}
void Mission_N07(){//107  病毒 带stop 需改		*hcp 
	setServ(125,256);
	Drive_UtlStop();
	Act_05();
	drive(20,20);
	wait(1);
	setServ(45,64);
	wait(2);
	setServ(-45,1023);
	wait(3); 
	drive(-20,-20);
	wait(1);
	
}
void Mission_N08(){//108
}
void Mission_N09(){//109  导航 无stop 带归位 	*t *hcp
	drive(-30,-30);
	wait(0.1);
	setServ(45,1024); 
	wait(0.65);
	setServ(-90,1023);  
}
void Mission_N10(){//110  推车  				*hcp **args
	int pt=1,dest=2300;//dt=r-l
	bool invert=false;
	if(!invert){
		setServ(85,512);
		drive(35,35);
		wait(0.2);
		drive(50,25);
		wait(1.5);

		resettime();
		while(dest<AI(pt)||seconds()<0.5){
			if(seconds()<0.5)
			drive(45,40);
			else
			drive(35,30);
			if(isMotorStop())break;
		}
		drive(0,0);
		wait(0.5);
		setServ(0,512); 
		wait(0.25);
		drive(-50,-50);
		wait(1);
		while(!dhs(0)&&!dhs(6)){;}
		drive(35,35);
		wait(0.25);
	}else{
		setServ(0,512);
		drive(35,35);
		wait(0.2);
		drive(45,20	);
		wait(1.5);
		resettime();
		while(dest<AI(pt)||seconds()<0.5){
			if(seconds()<0.5)
			drive(45,40);
			else
			drive(35,30);
			if(isMotorStop())break;
		}
		drive(0,0);
		wait(0.25);
		setServ(85,512); 
		wait(0.5);
		while(!dhs(0)){
			if(seconds()<0.5)
			drive(-40,-45);
			else
			drive(-30,-40);
		}
		while(dhs(0)){;}
		wait(0.2); 
		drive(0,0);
		wait(0.5);
		setServ(0,512);
		wait(0.25);
		drive(-50,-50);
		while(!dhs(0)&&!dhs(6)){;}
		drive(35,35);
		wait(0.25);
	}
}
void Mission_N11(){//111  拔框 无stop 带归位 	*cp
	drive(-45,-45);
	wait(0.2);//0.2
	drive(0,0);
	setServ(90,512);
	wait(0.7);
	drive(-45,70);
	wait(0.5);
	drive(0,0);
	setServ(-90,512);
	wait(0.5);
}
void Mission_N12(){//112  瓶子 					*hcp 
	setServ(90,512);
	Drive_UtlStop();
	wait(1);
	SetMoto(2,40);
	wait(0.5);
	SetMoto(2,-40);
	wait(1);
	SetMoto(2,40);
	wait(0.5);
	SetMoto(2,0);
	drive(-45,-45);
	wait(1);
}
void Mission_N13(){//113  进制 带stop 带初始	*h 
	setServ(0,512);
	Drive_UtlStop();
	drive(0,0);
	setServ(45,512);
	wait(0.1);
	drive(0,0);
	setServ(65,512);
	wait(0.4);
	drive(-45,-45);
	setServ(90,128);
	wait(0.5);
	drive(0,0);
	setServ(-90,512);
}
void Mission_N14(){//114  下载 直回				*hcp
	int i=0;
//	Drive_SpeedUp();
//	for(;i<6;i++){
//		setServ(38,512);
//		drive(45,45);
//		wait(0.5);//0.4
//		drive(-45,-45);
//		wait(0.15);
//		drive(45,45);
//		wait(0.15);
//		drive(0,0);
//		setServ(65,512);
//		wait(0.45);//0.3
//		drive(40,40);
//		wait(0.25);
//		drive(0,0);
//		setServ(45,512);
//		wait(0.35);
//		drive(-45,-45); 
//		wait(0.8);
//	}
//	setServ(38,512);
//	drive(45,45);
//	wait(0.6);
//	Drive_SpeedDown();
//	drive(-50,-55);
//	wait(0.5);
//	setServ(-90,512);
//	wait(2);
	drive(45,45);
	wait(0.2);
	setServ(55,1023);
	wait(0.4);
	for(;i<4;i++){
		drive(-45,-45);
		wait(0.5);
		drive(45,45);
		wait(0.55);
	}
	setServ(90,512);
	drive(-51,-59);
	wait(1);
	setServ(-90,512);
	wait(2);
}
void Mission_N15(){//115  交换 带过线 			*hcp
	setServ(45,512);
	Drive_CrossJunc();
	drive(30,30);
	wait(0.3);
	drive(0,0);
	setServ(110,128);
	wait(3);
	drive(-45,-45);
	setServ(45,512);
	wait(2);
}
void Mission_N16(){//116  丢方块 需改			*hcp	**args
	bool invert=true;
	if(!invert){
		setServ(45,128);
		Drive_CrossJunc();
		drive(45,20);
		wait(0.45);
		drive(20,45);
		wait(0.45);
		drive(30,30);
		while(!isMotorStop()){;}
		wait(0.1);
		drive(-40,-40);
		wait(0.1);
		drive(0,0);
		setServ(110,256);
		wait(1);
		drive(-40,-40);
		wait(1);
		setServ(45,512);
	}else{
		setServ(-15,128);
		Drive_CrossJunc();
		drive(45,20);
		wait(0.35);
		drive(20,45);
		wait(0.35);
		drive(30,30);
		while(!isMotorStop()){;}
		wait(0.1);
		drive(-40,-40);
		wait(0.1);
		drive(0,0);
		setServ(110,128);
		wait(2);
		setServ(-15,512);
		drive(-40,-40);
		wait(1);
	} 
} 
//===============================================其他操作函数===============================================// 
void Act_01(){//201  通用舵机初始
	setServ(-90,512); 
}
void Act_02(){//202  下载舵机初始 
	setServ(65,512);
}
void Act_03(){//203  钩子舵机初始 
	setServ(0,512);
}
void Act_04(){//204
}
void Act_05(){//205  抖右
	int i=3;
	for(;i>0;i--){
		drive(-20,20);
		wait(0.15);
		drive(20,-20);
		wait(0.15);
	} 
} 
void Act_06(){//206  慢速utlstop 
	resettime();
	while(true){
		if(isMotorStop()&&seconds()>0.5){
			drive(0,0);
			return;
		}else if(dhs(1)&&!dhs(0)){
			drive(-25,25);
			while(!dhs(2)){;}
		}else if(dhs(5)&&!dhs(6)){
			drive(25,-25);
			while(!dhs(4)){;}
		}else if(dhs(2)){
			drive(15,25);
		}else if(dhs(4)){
			drive(25,15);
		}else{
			drive(25,25);
		}
	}
}
void Act_07(){//207
	while(true){
		if(dhs(0)&&dhs(1)&&dhs(2)){
			drive(25,25);
			wait(0.7);
			drive(0,0);
			drive(-25,25);
			wait(0.2);
			while(true){
//				if(!dhs(0)&&!dhs(5)&&!dhs(6)&&(dhs(1)||dhs(2)||dhs(3)||dhs(4)))
				if(!dhs(0)&&!dhs(4)&&!dhs(5)&&!dhs(6)&&dhs(3))
				return;
			}
		}else if(dhs(1)&&!dhs(0)){
			drive(-25,25);
			while(!dhs(2)){;}
		}else if(dhs(5)&&!dhs(6)){
			drive(25,-25);
			while(!dhs(4)){;}
		}else if(dhs(2)){
			drive(15,25);
		}else if(dhs(4)){
			drive(25,15);
		}else{
			drive(25,25);
		}
	}
}
void Act_08(){//208
}
void Act_09(){//209
	int a,b;
	int spd=20;
	while(true){
		a=AI(1);
		b=AI(10);
		a-=100;
		drive((a-b)*0.05+spd,(b-a)*0.05+spd);
	} 
}
void Act_10(){//210
}
void Act_11(){//211
}
void Act_12(){//212
} 
void Act_13(){//213
}
void Act_14(){//214
}
void Act_15(){//215
}
void Act_16(){//216
}
void Act_17(){//217
}
void Act_18(){//218
}
void Act_19(){//219
}
void Act_20(){//220
}
//================================================支持函数================================================// 
void runsub(int sub,bool getName){//子函数调用函数 
	if(!getName){
		switch(Sequence[curSeq][sub]){
			case 1:
				Drive_Init();
				break;
			case 2:
				Drive_Departure();
				break;
			case 3:
				Drive_UpStair();
				break;
			case 4:
				Drive_Ontime();
				break;
			case 5:
				Drive_UtlStop();
				break;
			case 6:
				Drive_CrossJunc();
				break;
			case 7:
				Drive_UtlLeft();
				break;
			case 8:
				Drive_UtlRight();
				break;
			case 9:
				Drive_TurnLeft();
				break;
			case 10:
				Drive_TurnRight();
				break;
			case 11:
				Drive_OnlyLeft();
				break;
			case 12:
				Drive_OnlyRight(); 
				break;
			case 13:
				Drive_SpeedUp();
				break;
			case 14:
				Drive_SpeedDown();
				break;
			case 15:
				Drive_Fwd();
				break;
			case 16:
				Drive_Back();
				break;
			case 101:
				Mission_N01();
				break;
			case 102:
				Mission_N02();
				break;
			case 103:
				Mission_N03();
				break;
			case 104:
				Mission_N04();
				break;
			case 105:
				Mission_N05();
				break;
			case 106:
				Mission_N06();
				break;
			case 107:
				Mission_N07();
				break;
			case 108:
				Mission_N08();
				break;
			case 109:
				Mission_N09();
				break;
			case 110:
				Mission_N10();
				break;
			case 111:
				Mission_N11();
				break;
			case 112:
				Mission_N12();
				break;
			case 113:
				Mission_N13();
				break;
			case 114:
				Mission_N14();
				break;
			case 115:
				Mission_N15();
				break;
			case 116:
				Mission_N16();
				break;
			case 201:
				Act_01();
				break;
			case 202:
				Act_02();
				break;
			case 203:
				Act_03();
				break;
			case 204:
				Act_04();
				break;
			case 205:
				Act_05();
				break;
			case 206:
				Act_06();
				break;
			case 207:
				Act_07();
				break;
			case 208:
				Act_08();
				break;
			case 209:
				Act_09();
				break;
			case 210:
				Act_10();
				break;
			case 211:
				Act_11();
				break;
			case 212:
				Act_12();
				break;
			case 213:
				Act_13();
				break;
			case 214:
				Act_14();
				break;
			case 215:
				Act_15();
				break;
			case 216:
				Act_16();
				break;
			case 217:
				Act_17();
				break;
			case 218:
				Act_18();
				break;
			case 219:
				Act_19();
				break;
			case 220:
				Act_20();
				break;
		}
	}else{
		char *subName;
		int i=0;
		for(i=0;i<3;i++){
			switch(Sequence[curSeq][sub-1+i]){
				case 1:
					subName="D_Init";
					break;
				case 2:
					subName="D_Dept";
					break;
				case 3:
					subName="D_UStair";
					break;
				case 4:
					subName="D_Ontime";
					break;
				case 5:
					subName="D_UtStop";
					break;
				case 6:
					subName="D_CrJunc";
					break;
				case 7:
					subName="D_UtLeft";
					break;
				case 8:
					subName="D_UtRght";
					break;
				case 9:
					subName="D_TuLeft";
					break;
				case 10:
					subName="D_TuRght";
					break;
				case 11:
					subName="D_OlLeft"; 
					break;
				case 12:
					subName="D_OlRight"; 
					break;
				case 13:
					subName="D_SpdUp"; 
					break;
				case 14:
					subName="D_SpdDwn"; 
					break;
				case 15: 
					subName="D_Fwd"; 
					break;
				case 16:
					subName="D_Back"; 
					break;
				case 101:
					subName="M_N01";
					break;
				case 102:
					subName="M_N02";
					break;
				case 103:
					subName="M_N03";
					break;
				case 104:
					subName="M_N04";
					break;
				case 105:
					subName="M_N05";
					break;
				case 106:
					subName="M_N06";
					break;
				case 107:
					subName="M_N07";
					break;
				case 108:
					subName="M_N08";
					break;
				case 109:
					subName="M_N09";
					break;
				case 110:
					subName="M_N10";
					break;
				case 111:
					subName="M_N11";
					break;
				case 112:
					subName="M_N12";
					break;
				case 113:
					subName="M_N13";
					break;
				case 114:
					subName="M_N14";
					break;
				case 115:
					subName="M_N15";
					break;
				case 116:
					subName="M_N16";
					break;
				case 201:
					subName="A_01";
					break;
				case 202:
					subName="A_02";
					break;
				case 203:
					subName="A_03";
					break;
				case 204:
					subName="A_04";
					break;
				case 205:
					subName="A_05";
					break;
				case 206:
					subName="A_06";
					break;
				case 207:
					subName="A_07";
					break;
				case 208:
					subName="A_08";
					break;
				case 209:
					subName="A_09";
					break;
				case 210:
					subName="A_10";
					break;
				case 211:
					subName="A_11";
					break;
				case 212:
					subName="A_12";
					break;
				case 213:
					subName="A_13";
					break;
				case 214:
					subName="A_14";
					break;
				case 215:
					subName="A_15";
					break;
				case 216:
					subName="A_16";
					break;
				case 217:
					subName="A_17";
					break;
				case 218:
					subName="A_18";
					break;
				case 219:
					subName="A_19";
					break;
				case 220:
					subName="A_20";
					break;
				default:
					subName="*INVALID*";
			}
			switch(i){
				case 0:
					subNameP=subName;
					break;
				case 1:
					subNameC=subName;
					break;
				case 2:
					subNameN=subName;
			}
		}
		
	}
	return;
}
void initServ(){//舵机初始化 
	if(!isServInit){
		ServoPort_Init(servoPort);
		isServInit = true;
	}
}
void rldServ(){
	 _SetDSO(servoPort,0);
	 isServInit = false;
}
bool dhs(int port){//获取地灰状态 
	if(useSafedh)
		return rawDh(port)>dh[port]+safedh;
	else
		return rawDh(port)>dh[port];
}
int rawDh(int port){//获取地灰值 
	return AI(dhPort[port]);
}
void drive(int lspd,int rspd){//设置速度 
	int oalspd,oarspd;
	oalspd = (invertL?-1:1) * lspd * lcoe;
	oarspd = (invertR?-1:1) * rspd * rcoe;
	if(oalspd>100)oalspd=100;
	if(oalspd<-100)oalspd=-100;
	if(oarspd>100)oarspd=100;
	if(oarspd<-100)oarspd=-100;
	if(isMotoCL){
		SetMotoCL(lMotorPort,oalspd);
		SetMotoCL(rMotorPort,oarspd);
	}else{
		SetMoto(lMotorPort,oalspd);
		SetMoto(rMotorPort,oarspd);
	}
}
void setServ(float angle,int speed){//设置舵机角度 
	initServ();
	int intAngle = (angle + 150) / 300 * 1023;
	PO16_Move(servoID,speed,intAngle);
}
void dhUpdate(){//采集地灰值 
	int i=0,j=0;
	int maxDh[7]={0,0,0,0,0,0,0};
	int minDh[7]={0,0,0,0,0,0,0};
	BEEP(700,0.25);
	ReadEE();
	printf("      dhUpdate\nEEData:\n 01   02   03   04\n%4d %4d %4d %4d\n\n 05   06   07\n%4d %4d %4d\n      Continue",dh[0],dh[1],dh[2],dh[3],dh[4],dh[5],dh[6]);
	while(!Get_Button()||AI(buttonPort)){;}
	BEEP(1000,0.15);
	wait(0.5);
	drive(50,-50);
	BEEP(1000,0.15);
	printf("      dhUpdate\n\n Collecting Data...\n\n         0/50");
	for(i=0;i<7;i++){
		maxDh[i]=rawDh(i);
		minDh[i]=maxDh[i];
	}
	for(i=0;i<50;i++){
		for(j=0;j<7;j++){
			int tmpDh=0;
			tmpDh=rawDh(j);
			if(tmpDh>maxDh[j])maxDh[j]=tmpDh;
			if(tmpDh<minDh[j])minDh[j]=tmpDh;
		}
        printf("      dhUpdate\n\n Collecting Data...\n\n       %3d/50",i+1);
	wait(0.1);
	}
    drive(0,0);
    wait(2);
	for(i=0;i<7;i++){
		dh[i]=(maxDh[i]+minDh[i])/2;
	}
	printf("      dhUpdate\nCurrentData:\n 01   02   03   04\n%4d %4d %4d %4d\n\n 05   06   07\n%4d %4d %4d\n    Write EEPROM",dh[0],dh[1],dh[2],dh[3],dh[4],dh[5],dh[6]);
	while(!Get_Button()||AI(buttonPort)){;}
	BEEP(1000,0.15);
	WriteEE();
	wait(0.5);
	return;
}
void ReadEE(){//读EEPROM 
	int i=0;
	printf("       EEPROM\n\n  Reading EEPROM...\n\n         0/7");
	wait(0.5);
	for(i=0;i<7;i++){
		dh[i]=ReadEEPROM(EEAddr[i]);
		printf("       EEPROM\n\n  Reading EEPROM...\n\n         %d/7",i+1);
		wait(0.2);
	}
	wait(0.2);
	printf("");
}
void ReadEE_Fast(){//速读EEPROM 
	int i=0;
	for(i=0;i<7;i++){
		dh[i]=ReadEEPROM(EEAddr[i]);
	}
}
void WriteEE(){//存EEPROM 
	int i=0;
	printf("       EEPROM\n\n  Writing EEPROM...\n\n         0/7");
	wait(0.5);
	for(i=0;i<7;i++){
		WriteEEPROM(EEAddr[i],dh[i]);
		printf("       EEPROM\n\n  Writing EEPROM...\n\n         %d/7",i+1);
		wait(0.2);
	}
	wait(0.2);
	printf("");
}
bool isMotorStop(){//电机是否停转 
	return !(GetPrevSpeed(lMotorPort)||GetPrevSpeed(rMotorPort));
}
