#include "ASEIO.h"
#define true 1
#define false 0
//=================Settings=================//
//Button
const int buttonPort = 10;
//dh
const bool useSafedh = true;
const int safedh = 100;
const bool ReadEEOnStart = true;
const int dhPort[7] = {2,3,4,5,6,7,8};
const int EEAddr[7] = {0,1,2,3,4,5,6};
//Motor
const int lMotorPort = 0;
const int rMotorPort = 1;
const bool invertL = true;
const bool invertR = false;
//Servo
const int servoPort = 9;
const int servoID = 1;
//Battery
const int battMin = 7800;
const int battMax = 8500;
//--------------Drive--------------//
//Departure 2
const int Speed_2 = 50;
//UpStair 3
const int lSpeed_3 = -40;
const int mSpeed_3 = 35;
const int hSpeed_3 = 70;
const int tSpeed_3 = 40;
const int upSpeed_3 = 100;
const float upDelay_3 = 0.3;
const int dhdet_3 = 200;
//Back 4
const int mSpeed_4 = 40;
const float backDelay_4 = 0.75;
//UtlStop 5
const int lSpeed_5 = -40;
const int mSpeed_5 = 30;
const int hSpeed_5 = 50;
const int tSpeed_5 = 30;
//CrossJunc 6
const int lSpeed_6 = -40;
const int mSpeed_6 = 35;
const int hSpeed_6 = 60;
const int tSpeed_6 = 40;
//Left 7
const int hSpeed_7 = 60;
const int mSpeed_7 = 35;
const int lSpeed_7 = -40;
const int tSpeed_7 = 40;
const float btDelay_7 = 0.2;
const float bttDelay_7 = 0.5;
const float btttDelay_7 = 0.05;
//Right 8
const int hSpeed_8 = 60;
const int mSpeed_8 = 35;
const int lSpeed_8 = -40;
const int tSpeed_8 = 40;
const float btDelay_8 = 0.2;
const float bttDelay_8 = 0.5;
const float btttDelay_8 = 0.05;
//TLeft 9
const int tSpeed_9 = 40;
const float btDelay_9 = 0.15;
const float bttDelay_9 = 0.2;
//TRight 10
const int tSpeed_10 = 40;
const float btDelay_10 = 0.15;
const float bttDelay_10 = 0.2;
//DownStair 11
const int lSpeed_11 = -20;
const int mSpeed_11 = 20;
const int hSpeed_11 = 40;
const int tSpeed_11 = 20;
//---------------------------------//
//Debug
int defdbgLevel = 2; 
float dbgSeqDelay = 1;
int defSequence = 0;
float dbgDelay = 0.0;
//---------------------------------//
bool isMatch = false;
//==========================================//

//===varDecl===//
int dbgLevel = 0;
int dh[7]={0};
char *subNameP;
char *subNameC;
char *subNameN;
int curSeq;
bool isServInit = false;
//=============//

//===funcDecl===//
void runsub(int sub,bool getName);
void initServ();
void Drive_Init();//1
void Drive_Departure();//2
void Drive_UpStair();//3
void Drive_Back();//4
void Drive_UtlStop();//5
void Drive_CrossJunc();//6
void Drive_UtlLeft();//7
void Drive_UtlRight();//8
void Drive_TurnLeft();//9
void Drive_TurnRight();//10
void Drive_DownStair();//11
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
//==============//

//=================SEQUENCE=================//
int Sequence[8][100] = {
						{201,2,7,7,219,206,9,6,7,6,8,208,9,6,6,8,6,205,9,9,8,7,6,8,6},//一楼套 
						{201,2,7,6,7,5,204,9,6,8,6},//电话 
						{201,2,8,7,213,6,6,207},   //下载
						{218,8,7,8,101,10,7,8,7,6},  //bd
						{2,210}, //
						{201,1,2,6,202,9,3,0},  //上楼
						{6,218,7,8,6,7,5,214,9,7,8,6},  //环 
						{201,2,8,7,8,5,204,9,7,201,8,7,6},}; 
//==========================================//
//Upstair: {201,1,2,6,202,9,3,0}
//int Sequence[8][100] = {
//						{201,2,8,8,5,203,9,7,7,6},
//						{201,1,2,6,202,9,3,0}, 
//						{201,2,8,7,213,6,6,207},
//						{201,2,7,8,7,8,7,7,7,8,5,206,10,8,8,215,8,7,8,7,6,8,6},
//						{201,2,8,7,8,5,204,9,7,201,8,7,6},
//						{6,218,7,8,6,7,5,214,9,7,8,6},
//						{6,209},
//						{101}}; 

//						{201,2,7,8,7,8,7,7,7,8,5,206,10,8,8,7,7,7,5,205,10,8,8,7,8,7,8,7,6,6,8,5,203,9,7,7,6},//一楼套 
//						{201,2,8,7,8,5,204,9,7,201,8,7,6},//电话 
//						{201,2,8,7,213,6,6,207},   //下载
//						{218,2,8,7,6,8,101,10,7,8,7,6},  //bd 
//						{201,1,2,6,202,9,3,0},  //上楼
//						{6,218,7,8,6,7,5,214,9,7,8,6},  //环 
//						{201,2,8,7,8,5,204,9,7,201,8,7,6},
//						{101}}; 
void main(){
	if(isMatch) main_match();
	else main_test();
	return;
}

void main_match(){
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
	runFlag=false;
	int cnt = 0;
		while(true){
			while(true){
			bool cbatt=false;
			int batt;
			batt = Get_Battery();
			if(batt<battMin||batt>battMax)cbatt=true;
				switch(curSeq){
					case 0:
						printf("        MATCH\nChoose Sequence:\n     >0<      1\n      2       3\n      4       5\n      6       7\nBatt:%d%s\nPrev     Sel     Next",batt,cbatt?"<-ATTENTION":" ");
						break;
					case 1:
						printf("        MATCH\nChoose Sequence:\n      0      >1<\n      2       3\n      4       5\n      6       7\nBatt:%d%s\nPrev     Sel     Next",batt,cbatt?"<-ATTENTION":" ");
						break;
					case 2:
						printf("        MATCH\nChoose Sequence:\n      0       1\n     >2<      3\n      4       5\n      6       7\nBatt:%d%s\nPrev     Sel     Next",batt,cbatt?"<-ATTENTION":" ");
						break;
					case 3:
						printf("        MATCH\nChoose Sequence:\n      0       1\n      2      >3<\n      4       5\n      6       7\nBatt:%d%s\nPrev     Sel     Next",batt,cbatt?"<-ATTENTION":" ");
						break;
					case 4:
						printf("        MATCH\nChoose Sequence:\n      0       1\n      2       3\n     >4<      5\n      6       7\nBatt:%d%s\nPrev     Sel     Next",batt,cbatt?"<-ATTENTION":" ");
						break;
					case 5:
						printf("        MATCH\nChoose Sequence:\n      0       1\n      2       3\n      4      >5<\n      6       7\nBatt:%d%s\nPrev     Sel     Next",batt,cbatt?"<-ATTENTION":" ");
						break;
					case 6:
						printf("        MATCH\nChoose Sequence:\n      0       1\n      2       3\n      4       5\n     >6<      7\nBatt:%d%s\nPrev     Sel     Next",batt,cbatt?"<-ATTENTION":" ");
						break;
					case 7:
						printf("        MATCH\nChoose Sequence:\n      0       1\n      2       3\n      4       5\n      6      >7<\nBatt:%d%s\nPrev     Sel     Next",batt,cbatt?"<-ATTENTION":" ");
						break;
				}
				if(Get_Button()||AI(buttonPort))break;
				if(Get_BtnLeft()){
					BEEP(1000,0.15);
					if(--curSeq<0)curSeq=7; 
					break;
				}
				if(Get_BtnRight()){
					BEEP(1000,0.15);
					if(++curSeq>7)curSeq=0;
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
					switch(curSeq){
					case 0:
						printf("        MATCH\nChoose Sequence:\n     >0<      1\n      2       3\n      4       5\n      6       7\n     RUNNING SEQ\nPrev     Sel     Next");
						break;
					case 1:
						printf("        MATCH\nChoose Sequence:\n      0      >1<\n      2       3\n      4       5\n      6       7\n     RUNNING SEQ\nPrev     Sel     Next");
						break;
					case 2:
						printf("        MATCH\nChoose Sequence:\n      0       1\n     >2<      3\n      4       5\n      6       7\n     RUNNING SEQ\nPrev     Sel     Next");
						break;
					case 3:
						printf("        MATCH\nChoose Sequence:\n      0       1\n      2      >3<\n      4       5\n      6       7\n     RUNNING SEQ\nPrev     Sel     Next");
						break;
					case 4:
						printf("        MATCH\nChoose Sequence:\n      0       1\n      2       3\n     >4<      5\n      6       7\n     RUNNING SEQ\nPrev     Sel     Next");
						break;
					case 5:
						printf("        MATCH\nChoose Sequence:\n      0       1\n      2       3\n      4      >5<\n      6       7\n     RUNNING SEQ\nPrev     Sel     Next");
						break;
					case 6:
						printf("        MATCH\nChoose Sequence:\n      0       1\n      2       3\n      4       5\n     >6<      7\n     RUNNING SEQ\nPrev     Sel     Next");
						break;
					case 7:
						printf("        MATCH\nChoose Sequence:\n      0       1\n      2       3\n      4       5\n      6      >7<\n    RUNNING SEQ\nPrev     Sel     Next");
						break;
				}
	while(true){
		runsub(curStep,0);
		BEEP(1000,0.15);
		if(++curStep>Steps){
			BEEP(1000,0.15);
			break;
		}
	}
	if(++curSeq>7)curSeq=7;
	drive(0,0);
	setServ(45,512);
	goto A;
}

void main_test(){
	BEEP(700,0.25);
	if(ReadEEOnStart)ReadEE_Fast();
	curSeq = defSequence;
	int curStep = 0;
	int i=0,Steps=0;
	bool runFlag=false;
	A:
	i=0;
	Steps=0;
	curStep = 0;
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
				if(!batt<battMin&&!batt>battMax)
				cnt=0;
			}
				switch(curSeq){
					case 0:
						printf("        Debug\nChoose Sequence:\n     >0<      1\n      2       3\n      4       5\n      6       7\nBatt:%d%s\nPrev     Sel     Next",batt,cbatt?"<-ATTENTION":" ");
						break;
					case 1:
						printf("        Debug\nChoose Sequence:\n      0      >1<\n      2       3\n      4       5\n      6       7\nBatt:%d%s\nPrev     Sel     Next",batt,cbatt?"<-ATTENTION":" ");
						break;
					case 2:
						printf("        Debug\nChoose Sequence:\n      0       1\n     >2<      3\n      4       5\n      6       7\nBatt:%d%s\nPrev     Sel     Next",batt,cbatt?"<-ATTENTION":" ");
						break;
					case 3:
						printf("        Debug\nChoose Sequence:\n      0       1\n      2      >3<\n      4       5\n      6       7\nBatt:%d%s\nPrev     Sel     Next",batt,cbatt?"<-ATTENTION":" ");
						break;
					case 4:
						printf("        Debug\nChoose Sequence:\n      0       1\n      2       3\n     >4<      5\n      6       7\nBatt:%d%s\nPrev     Sel     Next",batt,cbatt?"<-ATTENTION":" ");
						break;
					case 5:
						printf("        Debug\nChoose Sequence:\n      0       1\n      2       3\n      4      >5<\n      6       7\nBatt:%d%s\nPrev     Sel     Next",batt,cbatt?"<-ATTENTION":" ");
						break;
					case 6:
						printf("        Debug\nChoose Sequence:\n      0       1\n      2       3\n      4       5\n     >6<      7\nBatt:%d%s\nPrev     Sel     Next",batt,cbatt?"<-ATTENTION":" ");
						break;
					case 7:
						printf("        Debug\nChoose Sequence:\n      0       1\n      2       3\n      4       5\n      6      >7<\nBatt:%d%s\nPrev     Sel     Next",batt,cbatt?"<-ATTENTION":" ");
						break;
				}
				if(cnt>29&&cbatt)BEEP(2000,0.25);
				else wait(0.05);
				if(Get_Button()||AI(buttonPort))break;
				if(Get_BtnLeft()){
					BEEP(1000,0.15);
					if(--curSeq<0)curSeq=7; 
					break;
				}
				if(Get_BtnRight()){
					BEEP(1000,0.15);
					if(++curSeq>7)curSeq=0;
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
				if(cnt>29&&cbatt)BEEP(2000,0.25);
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
		goto A;
	}
	Steps--;
	while(true){
		if(dbgLevel==2){
			runsub(curStep,1);
			if(curStep==0){
				printf("        Debug\n\n\n Cur==>%2d-%s\n Next: %2d-%s\n\n\nPrev     Run     Next",curStep,subNameC,curStep+1,subNameN);
			}else if(curStep==Steps){
				printf("        Debug\n\n Prev: %2d-%s\n Cur==>%2d-%s\n\n\n\nPrev     Run     Next",curStep-1,subNameP,curStep,subNameC);
			}else{
				printf("        Debug\n\n Prev: %2d-%s\n Cur==>%2d-%s\n Next: %2d-%s\n\n\nPrev     Run     Next",curStep-1,subNameP,curStep,subNameC,curStep+1,subNameN);
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
						if(buf>150){
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
								goto A;
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
						if(buf>150){
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
					printf("        Debug\n\n  Seq %2d Completed\n\n\n\n\n      Continue",curSeq);
					drive(0,0);
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
	goto A;
}

void runsub(int sub,bool getName){
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
				Drive_Back();
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
				Drive_DownStair();
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
					subName="D_Back";
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
					subName="D_DStair";
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

void initServ(){
	if(!isServInit){
		ServoPort_Init(servoPort);
		isServInit = true;
	}
}

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
		}else if(dhs(1)){
			drive(lSpeed_3,hSpeed_3);
		}else if(dhs(5)){
			drive(hSpeed_3,lSpeed_3);
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

void Drive_Back(){//4
	drive(-mSpeed_4,-mSpeed_4);
	wait(backDelay_4);
}

void Drive_UtlStop(){//5
	resettime();
	while(true){
		if(!GetPrevSpeed(0)&&!GetPrevSpeed(1)&&seconds()>1){
			drive(0,0);
			return;
		}else if(dhs(1)){
			wait(0.05);
			drive(lSpeed_5,hSpeed_5);
		}else if(dhs(5)){
		   wait(0.05);
			drive(hSpeed_5,lSpeed_5);
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
		if(!GetPrevSpeed(0)&&!GetPrevSpeed(1)){
			resettime();
			while(!GetPrevSpeed(0)&&!GetPrevSpeed(1)&&seconds()<2){;}
			if(seconds()>2)return;
		}
		if(dhs(0)||dhs(6)){
			drive(hSpeed_6,hSpeed_6);
			while((dhs(0)||dhs(6))){;}
			wait(0.1);
			return;
		}else if(dhs(1)){
			drive(lSpeed_6,hSpeed_6);
		}else if(dhs(5)){
			drive(hSpeed_6,lSpeed_6);
		}else if(dhs(2)){
			drive(mSpeed_6,hSpeed_6);
		}else if(dhs(4)){
			drive(hSpeed_6,mSpeed_6);
		}else if(!dhs(3)){
			drive(tSpeed_6,-tSpeed_6);
		}else{
			drive(hSpeed_6,hSpeed_6);
		}
	}
}

void Drive_UtlLeft(){//7
	while(true){
		if(!GetPrevSpeed(0)&&!GetPrevSpeed(1)){
			resettime();
			while(!GetPrevSpeed(0)&&!GetPrevSpeed(1)&&seconds()<2){;}
			if(seconds()>2)return;
		}
		if(dhs(0)){
			drive(tSpeed_7,tSpeed_7);
			wait(btDelay_7);
			drive(-tSpeed_7,tSpeed_7);
			wait(bttDelay_7);
			while(!dhs(3)){;}
			drive(hSpeed_7,hSpeed_7);
			wait(btttDelay_7);
			return;
		}else if(dhs(1)){
			drive(lSpeed_7,hSpeed_7);
		}else if(dhs(5)){
			drive(hSpeed_7,lSpeed_7);
		}else if(dhs(2)){
			drive(mSpeed_7,hSpeed_7);
		}else if(dhs(4)){
			drive(hSpeed_7,mSpeed_7);
		}else if(!dhs(3)){
			drive(tSpeed_7,-tSpeed_7);
		}else{
			drive(hSpeed_7,hSpeed_7);
		}
	}
}

void Drive_UtlRight(){//8
	while(true){
		if(!GetPrevSpeed(0)&&!GetPrevSpeed(1)){
			resettime();
			while(!GetPrevSpeed(0)&&!GetPrevSpeed(1)&&seconds()<2){;}
			if(seconds()>2)return;
		}
		if(dhs(6)){
			drive(tSpeed_8,tSpeed_8);
			wait(btDelay_8);
			drive(tSpeed_8,-tSpeed_8);
			wait(bttDelay_8);
			while(!dhs(3)){;}
			drive(hSpeed_8,hSpeed_8);
			wait(btttDelay_8);
			return;
		}else if(dhs(1)){
			drive(lSpeed_8,hSpeed_8);
		}else if(dhs(5)){
			drive(hSpeed_8,lSpeed_8);
		}else if(dhs(2)){
			drive(mSpeed_8,hSpeed_8);
		}else if(dhs(4)){
			drive(hSpeed_8,mSpeed_8);
		}else if(!dhs(3)){
			drive(tSpeed_8,-tSpeed_8);
		}else{
			drive(hSpeed_8,hSpeed_8);
		}
	}
}

void Drive_TurnLeft(){//9
	drive(tSpeed_9,tSpeed_9);
	wait(btDelay_9);
	drive(-tSpeed_9,tSpeed_9);
	wait(bttDelay_9);
	while(!dhs(3)){;}
	return;
}

void Drive_TurnRight(){//10
	drive(tSpeed_10,tSpeed_10);
	wait(btDelay_10);
	drive(tSpeed_10,-tSpeed_10);
	wait(bttDelay_10);
	while(!dhs(3)){;}
	return;
}

void Drive_DownStair(){//11
	resettime();
	while(true){
		if((dhs(0)||dhs(6))&&seconds()>4){
			drive(hSpeed_11,hSpeed_11);
			while((dhs(0)||dhs(6))){;}
			return;
		}else if(dhs(1)){
			drive(lSpeed_11,hSpeed_11);
		}else if(dhs(5)){
			drive(hSpeed_11,lSpeed_11);
		}else if(dhs(2)){
			drive(mSpeed_11,hSpeed_11);
		}else if(dhs(4)){
			drive(hSpeed_11,mSpeed_11);
		}else if(!dhs(3)){
			drive(tSpeed_11,-tSpeed_11);
		}else{
			drive(hSpeed_11,hSpeed_11);
		}
	}
}

void Mission_N01(){//101 bd
	setServ(60,512);
	Act_19();
	drive(0,0);
	wait(1);
	setServ(100,256);
	drive(-30,-30);
	wait(0.25);
	drive(-45,-45);
	wait(0.4);
	drive(0,0);
}
void Mission_N02(){//102
}
void Mission_N03(){//103
}
void Mission_N04(){//104
}
void Mission_N05(){//105
}
void Mission_N06(){//106
}
void Mission_N07(){//107
}
void Mission_N08(){//108
}
void Mission_N09(){//109
}
void Mission_N10(){//110
}
void Mission_N11(){//111
}
void Mission_N12(){//112
}
void Mission_N13(){//113
}
void Mission_N14(){//114
}
void Mission_N15(){//115
}
void Mission_N16(){//116
}

void Act_01(){//201  初始 
	setServ(-90,512); 
}
void Act_02(){//202  开门上楼 
	drive(40,40);
	wait(0.25);
	drive(55,-15);
	wait(0.45);
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
void Act_03(){//203  拔框 
	drive(-45,-45);
	wait(0.2);//0.2
	drive(0,0);
	setServ(85,512);
	wait(0.7);
	drive(-45,70);
	wait(0.5);
	drive(0,0);
	setServ(-90,512);
	wait(0.5);
}
void Act_04(){//204  电话 
	setServ(5,512);
	drive(-45,-45);
	wait(0.5);
	drive(45,45);
	wait(0.6);
	setServ(70,512);
	drive(-45,-45);
	wait(0.5);
	drive(45,45);
	wait(0.6);
	drive(-45,-45);
	wait(0.55);
	setServ(-90,1023);
}
void Act_05(){//205  电脑
	drive(30,30);
	while(GetPrevSpeed(0)||GetPrevSpeed(1)){;}
	setServ(85,1023);
	drive(-30,-30);
	wait(0.5); 
	drive(0,0);
	wait(0.15);
	drive(30,30);
	wait(0.3);
	setServ(105,1023);
	wait(0.05);
	drive(0,0);
	wait(0.1);
	setServ(75,256);
	wait(0.25);
	drive(-45,-45);
	wait(0.5);
	setServ(-90,512);
}
void Act_06(){//206  导航
	drive(-30,-30);
	wait(0.1);
	setServ(45,1024); 
	wait(0.65);
	setServ(-90,1023); 
}
void Act_07(){//207  下载
	int i=0;
	for(;i<6;i++){
		setServ(38,512);
		drive(45,45);
		wait(0.4);
		drive(-45,-45);
		wait(0.15);
		drive(45,45);
		wait(0.15);
		drive(0,0);
		setServ(65,512);
		wait(0.3);
		drive(40,40);
		wait(0.25);
		drive(0,0);
		setServ(45,512);
		wait(0.2);
		drive(-45,-45); 
		wait(0.6);
	}
	setServ(-90,512);
	drive(-45,-55);
	wait(3);
}
void Act_08(){//208  进制
	setServ(0,512);
	Drive_UtlStop();
	drive(0,0);
	setServ(45,512);
	wait(0.1);
	drive(0,0);
	setServ(65,512);
	wait(1);
	drive(-45,-45);
	setServ(90,128);
	wait(0.75);
	drive(0,0);
	setServ(-90,512);
}
void Act_09(){//209  存贮
	setServ(45,512);
	Drive_CrossJunc();
	drive(45,15);
	wait(0.5);
	drive(15,45);
	wait(0.5);
	drive(30,30);
	while(GetPrevSpeed(0)||GetPrevSpeed(1)){;}
	drive(-40,-40);
	wait(0.1);
	drive(0,0);
	setServ(110,256);
	wait(0.25);
	drive(-40,-40);
	wait(1);
	setServ(45,512);
}
void Act_10(){//210  交换
	setServ(45,512);
	Drive_CrossJunc();
	drive(-10,-10);
	wait(0.1);
	drive(0,0);
	setServ(110,128);
	wait(3);
	drive(-45,-45);
	setServ(45,512);
	wait(2);
}
void Act_11(){//211  推车
	drive(45,45);
	while((!dhs(0)||!dhs(6))){;}
	while((dhs(0)||dhs(6))){;}
	wait(0.15);
	drive(57,23);
	wait(1.1);
	drive(15,15);
	wait(0.75);
	drive(15,20);
	while((!dhs(0)||!dhs(6))){;}
	while((dhs(0)||dhs(6))){;}
	wait(0.1);
	drive(-45,-45);
	wait(0.5);
	while((!dhs(0)&&!dhs(6))){;}
	drive(45,45);
	wait(0.25);
}
void Act_12(){//212  退 
	drive(-45,-45);
	wait(0.25);
}
void Act_13(){//213  下载准备 
	setServ(38,512);
}
void Act_14(){//214  勾环 
	drive(0,0);
	setServ(132,512);
	wait(1);
	drive(-45,-45);
	wait(1);
	drive(0,0);
	setServ(0,512);
}
void Act_15(){//215  右走1
	resettime();
	while(true){
		if(seconds()>0.5)return;
		if(dhs(0)||dhs(6)){
			drive(hSpeed_6,hSpeed_6);
			while((dhs(0)||dhs(6))){;}
		}else if(dhs(5)){
			drive(hSpeed_6,lSpeed_6);
		}else if(dhs(2)){
			drive(mSpeed_6,hSpeed_6);
		}else if(dhs(4)){
			drive(hSpeed_6,mSpeed_6);
		}else if(!dhs(3)){
			drive(tSpeed_6,-tSpeed_6);
		}else{
			drive(hSpeed_6,hSpeed_6);
		}
	}
}
void Act_16(){//216  左走1
	resettime();
	while(true){
		if(seconds()>0.5)return;
		if(dhs(0)||dhs(6)){
			drive(hSpeed_6,hSpeed_6);
			while((dhs(0)||dhs(6))){;}
		}else if(dhs(1)){
			drive(lSpeed_6,hSpeed_6);
		}else if(dhs(2)){
			drive(mSpeed_6,hSpeed_6);
		}else if(dhs(4)){
			drive(hSpeed_6,mSpeed_6);
		}else if(!dhs(3)){
			drive(tSpeed_6,-tSpeed_6);
		}else{
			drive(hSpeed_6,hSpeed_6);
		}
	}
}
void Act_17(){//217  瓶子 
	setServ(0,512);
	Drive_UtlStop();
	drive(0,0);
	wait(0.5); 
	drive(-30,-30);
	wait(1);
	drive(0,0);
	setServ(125,512);
	wait(0.5);
	Act_19();
	drive(0,0);
	wait(0.5);
	drive(-40,0);
	wait(0.3);
	drive(0,0);
	wait(0.25);
	drive(40,0);
	wait(0.35);
	drive(0,0);
	wait(0.25);
	drive(0,-50);
	wait(0.3);
	drive(0,0);
	wait(0.25);
	drive(0,40);
	wait(0.35);
}
void Act_18(){//218  钩子初始 
		setServ(0,512);
}
void Act_19(){//219  慢速utlstop 
	resettime();
	int ldt=10,rdt=10;
	bool flag=true;
	while(true){
		if(!GetPrevSpeed(0)&&!GetPrevSpeed(1)&&seconds()>1){
			drive(0,0);
			return;
		}else if(dhs(1)){
			wait(0.05);
			drive(lSpeed_5-ldt,hSpeed_5-rdt);
		}else if(dhs(5)){
		   wait(0.05);
			drive(hSpeed_5-ldt,lSpeed_5-rdt);
		}else if(dhs(2)){
			drive(mSpeed_5-ldt,hSpeed_5-rdt);
		}else if(dhs(4)){
			drive(hSpeed_5-ldt,mSpeed_5-rdt);
		}else{
			drive(hSpeed_5-ldt,hSpeed_5-rdt);
		}
	}
}
void Act_20(){//220  进 
	drive(45,45);
	wait(0.25);
}


bool dhs(int port){
	if(useSafedh)
		return rawDh(port)>dh[port]+safedh;
	else
		return rawDh(port)>dh[port];
}

int rawDh(int port){
	return AI(dhPort[port]);
}

void drive(int lspd,int rspd){
	SetMoto(lMotorPort,(invertL?-1:1)*lspd);
	SetMoto(rMotorPort,(invertR?-1:1)*rspd);
}

void setServ(float angle,int speed){
	initServ();
	int intAngle = (angle + 150) / 300 * 1023;
	PO16_Move(servoID,speed,intAngle);
}

void dhUpdate(){
	int i=0,j=0;
	int maxDh[7]={0,0,0,0,0,0,0};
	int minDh[7]={0,0,0,0,0,0,0};
	BEEP(700,0.25);
	ReadEE();
	printf("      dhUpdate\nEEData:\n 01   02   03   04\n%4d %4d %4d %4d\n\n 05   06   07\n%4d %4d %4d\n    >Press Enter<",dh[0],dh[1],dh[2],dh[3],dh[4],dh[5],dh[6]);
	while(!Get_Button()||AI(buttonPort)){;}
	BEEP(1000,0.15);
	wait(0.5);
	drive(50,-50);
	BEEP(1000,0.15);
	printf("      dhUpdate\n\n Collecting Data...\n\n         0/100");
	for(i=0;i<7;i++){
		maxDh[i]=rawDh(i);
		minDh[i]=maxDh[i];
	}
	for(i=0;i<100;i++){
		for(j=0;j<7;j++){
			int tmpDh=0;
			tmpDh=rawDh(j);
			if(tmpDh>maxDh[j])maxDh[j]=tmpDh;
			if(tmpDh<minDh[j])minDh[j]=tmpDh;
		}
        printf("      dhUpdate\n\n Collecting Data...\n\n       %3d/100",i+1);
	wait(0.1);
	}
    drive(0,0);
    wait(2);
	for(i=0;i<7;i++){
		dh[i]=(maxDh[i]+minDh[i])/2;
	}
	printf("      dhUpdate\nCurrentData:\n 01   02   03   04\n%4d %4d %4d %4d\n\n 05   06   07\n%4d %4d %4d\n    >Press Enter<",dh[0],dh[1],dh[2],dh[3],dh[4],dh[5],dh[6]);
	while(!Get_Button()||AI(buttonPort)){;}
	BEEP(1000,0.15);
	WriteEE();
	wait(0.5);
	return;
}

void ReadEE(){
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

void ReadEE_Fast(){
	int i=0;
	for(i=0;i<7;i++){
		dh[i]=ReadEEPROM(EEAddr[i]);
	}
}

void WriteEE(){
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
