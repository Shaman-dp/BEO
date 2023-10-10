#include "ais.h"
#include "AISPar.h"
#include "externs.h"
#include "UDT_system.h"
#include "TLS_COM.H"

unsigned short int BEO_timer;                           //счётчик пульсов для включения БЭО в работу при запуске контроллера
unsigned short int BEO_Pulse;                           //счётчик циклов для выдачи пульса в БЭО
unsigned short int BEO_Pulse_Link;                      //счётчик пульсов для включения БЭО в работу при запуске контроллера

//Счётчики для диагностики связи с АИС по сети
unsigned short int BEO_Delay_UCP1_A1_v, BEO_Delay_UCP1_A1_n;
unsigned short int BEO_Delay_UCP1_A2_v, BEO_Delay_UCP1_A2_n;
unsigned short int BEO_Delay_RVU_v, BEO_Delay_RVU_n;
unsigned short int BEO_Delay_KST_v, BEO_Delay_KST_n;
unsigned short int BEO_Delay_N_v, BEO_Delay_N_n;
unsigned short int BEO_Delay_TG_v, BEO_Delay_TG_n;
unsigned short int BEO_Delay_BG_v, BEO_Delay_BG_n;
unsigned short int BEO_Delay_VA_v, BEO_Delay_VA_n;
unsigned short int BEO_Delay_E1_v, BEO_Delay_E1_n;
unsigned short int BEO_Delay_UE8_v, BEO_Delay_UE8_n;
unsigned short int BEO_Delay_AVOM_v, BEO_Delay_AVOM_n;
unsigned short int BEO_Delay_AVOM2_v, BEO_Delay_AVOM2_n;
unsigned short int BEO_Delay_UPD1_v, BEO_Delay_UPD1_n;
unsigned short int BEO_Delay_MI_v, BEO_Delay_MI_n;
unsigned short int BEO_Delay_NKU01_v, BEO_Delay_NKU01_n;
unsigned short int BEO_Delay_NKU02_v, BEO_Delay_NKU02_n;
unsigned short int BEO_Delay_NKU03_v, BEO_Delay_NKU03_n;
unsigned short int BEO_Delay_NKU06_v, BEO_Delay_NKU06_n;
unsigned short int BEO_Delay_NKU07_v, BEO_Delay_NKU07_n;
unsigned short int BEO_Delay_NKU08_v, BEO_Delay_NKU08_n;
unsigned short int BEO_Delay_NKU10_v, BEO_Delay_NKU10_n;
unsigned short int BEO_Delay_NKU12_v, BEO_Delay_NKU12_n;
unsigned short int BEO_Delay_NKU13_v, BEO_Delay_NKU13_n;
unsigned short int BEO_Delay_NKU16_v, BEO_Delay_NKU16_n;
unsigned short int BEO_Delay_NKU_AV01_v, BEO_Delay_NKU_AV01_n;
unsigned short int BEO_Delay_NKU_AV02_v, BEO_Delay_NKU_AV02_n;
unsigned short int BEO_Delay_NKU415_v, BEO_Delay_NKU415_n;
unsigned short int BEO_Delay_MP_GTU_v, BEO_Delay_MP_GTU_n;

//состояние исправности контроллера
bool UCP1_A1_ctrl_BEO = true;
bool UCP1_A2_ctrl_BEO = true;
bool RVU_ctrl_BEO = true;
bool KST_ctrl_BEO = true;
bool N_ctrl_BEO = true;
bool TG_ctrl_BEO = true;
bool BG_ctrl_BEO = true;
bool VA_ctrl_BEO = true;
bool E1_ctrl_BEO = true;
bool UE8_ctrl_BEO = true;
bool AVOM_ctrl_BEO = true;
bool AVOM2_ctrl_BEO = true;
bool UPD1_ctrl_BEO = true;
bool MI_ctrl_BEO = true;
bool NKU01_ctrl_BEO = true;
bool NKU02_ctrl_BEO = true;
bool NKU03_ctrl_BEO = true;
bool NKU06_ctrl_BEO = true;
bool NKU07_ctrl_BEO = true;
bool NKU08_ctrl_BEO = true;
bool NKU10_ctrl_BEO = true;
bool NKU12_ctrl_BEO = true;
bool NKU13_ctrl_BEO = true;
bool NKU16_ctrl_BEO = true;
bool NKU_AV01_ctrl_BEO = true;
bool NKU_AV02_ctrl_BEO = true;
bool NKU415_ctrl_BEO = true;
bool MP_GTU_ctrl_BEO = true;

void LinkBEO(void)
{
//уставки
unsigned short int BEO_pulse_t      = (unsigned short int)(5000/cycleTime.base);  //периодичность выдачи пульса в БЭО                                 //раз в 1 сек
unsigned short int BEO_pulse_link_t = (unsigned short int)(220/cycleTime.base);   //периодичность выдачи пульса по сети                               //раз в 220 мс
unsigned short int BEO_noLink_sp    = (unsigned short int)(1220/cycleTime.base);  //Уставка по времени отсутствия связи с другими АИС для запуска ЭО  //нет связи в течение 1,2 сек
unsigned short int BEO_start_delay  = (unsigned short int)(10000/cycleTime.base); //Задержка включения БЭО в работу при запуске контроллера           //10 сек

//Таймер БЭО
BEO_timer++;
if (BEO_timer > BEO_start_delay) BEO_timer = BEO_start_delay;   //Задержка на запуск БЭО
if (BEO_timer >= BEO_start_delay - 1){
dgo.WatchDog_ON=true; //Включить БЭО в работу

BEO_Pulse++;          //сигнал на физический контроллер
BEO_Pulse_Link++;    //сигнал по сети

if (BEO_Pulse_Link >= BEO_pulse_link_t) {	alg.WatchDog_Pulse_UCP1 = !alg.WatchDog_Pulse_UCP1;
											BEO_Pulse_Link = 0;
}
/*///////////////////////////////////////////////////////////////////////
                      Анализ работы АИСов
//////////////////////////////////////////////////////////////////////*/ 

//UCP1_A1
if (UCP1_A1.alg_WatchDog_Pulse_UCP1_A1) {   BEO_Delay_UCP1_A1_v++;
																BEO_Delay_UCP1_A1_n = 0;}
else									{   BEO_Delay_UCP1_A1_n++;
																BEO_Delay_UCP1_A1_v = 0;}

anpar.UCP1_A1_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_UCP1_A1_v), TNANFloat(BEO_Delay_UCP1_A1_n) );
alg.UCP1_A1_brk = ((BEO_Delay_UCP1_A1_v >= BEO_noLink_sp) || (BEO_Delay_UCP1_A1_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.UCP1_A1_brk) UCP1_A1_ctrl_BEO = false;

//UCP1_A2
if (UCP1_A2.alg_WatchDog_Pulse_UCP1_A2) {   BEO_Delay_UCP1_A2_v++;
																BEO_Delay_UCP1_A2_n = 0;}
else									{   BEO_Delay_UCP1_A2_n++;
																BEO_Delay_UCP1_A2_v = 0;}

anpar.UCP1_A2_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_UCP1_A2_v), TNANFloat(BEO_Delay_UCP1_A2_n) );
alg.UCP1_A2_brk = ((BEO_Delay_UCP1_A2_v >= BEO_noLink_sp) || (BEO_Delay_UCP1_A2_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.UCP1_A2_brk) UCP1_A2_ctrl_BEO = false;

//RVU
if (RVU.alg_WatchDog_Pulse_RVU) {   BEO_Delay_RVU_v++;
																BEO_Delay_RVU_n = 0;}
else									{   BEO_Delay_RVU_n++;
																BEO_Delay_RVU_v = 0;}

anpar.RVU_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_RVU_v), TNANFloat(BEO_Delay_RVU_n) );
alg.RVU_brk = ((BEO_Delay_RVU_v >= BEO_noLink_sp) || (BEO_Delay_RVU_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.RVU_brk) RVU_ctrl_BEO = false;

//KST
if (KST.alg_WatchDog_Pulse_KST) {   BEO_Delay_KST_v++;
																BEO_Delay_KST_n = 0;}
else									{   BEO_Delay_KST_n++;
																BEO_Delay_KST_v = 0;}

anpar.KST_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_KST_v), TNANFloat(BEO_Delay_KST_n) );
alg.KST_brk = ((BEO_Delay_KST_v >= BEO_noLink_sp) || (BEO_Delay_KST_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.KST_brk) KST_ctrl_BEO = false;

//N
if (N.alg_WatchDog_Pulse_N) {   BEO_Delay_N_v++;
																BEO_Delay_N_n = 0;}
else									{   BEO_Delay_N_n++;
																BEO_Delay_N_v = 0;}

anpar.N_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_N_v), TNANFloat(BEO_Delay_N_n) );
alg.N_brk = ((BEO_Delay_N_v >= BEO_noLink_sp) || (BEO_Delay_N_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.N_brk) N_ctrl_BEO = false;

//TG
if (TG.alg_WatchDog_Pulse_TG) {   BEO_Delay_TG_v++;
																BEO_Delay_TG_n = 0;}
else									{   BEO_Delay_TG_n++;
																BEO_Delay_TG_v = 0;}

anpar.TG_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_TG_v), TNANFloat(BEO_Delay_TG_n) );
alg.TG_brk = ((BEO_Delay_TG_v >= BEO_noLink_sp) || (BEO_Delay_TG_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.TG_brk) TG_ctrl_BEO = false;

//BG
if (BG.alg_WatchDog_Pulse_BG) {   BEO_Delay_BG_v++;
																BEO_Delay_BG_n = 0;}
else									{   BEO_Delay_BG_n++;
																BEO_Delay_BG_v = 0;}

anpar.BG_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_BG_v), TNANFloat(BEO_Delay_BG_n) );
alg.BG_brk = ((BEO_Delay_BG_v >= BEO_noLink_sp) || (BEO_Delay_BG_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.BG_brk) BG_ctrl_BEO = false;

//VA
if (VA.alg_WatchDog_Pulse_VA) {   BEO_Delay_VA_v++;
																BEO_Delay_VA_n = 0;}
else									{   BEO_Delay_VA_n++;
																BEO_Delay_VA_v = 0;}

anpar.VA_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_VA_v), TNANFloat(BEO_Delay_VA_n) );
alg.VA_brk = ((BEO_Delay_VA_v >= BEO_noLink_sp) || (BEO_Delay_VA_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.VA_brk) VA_ctrl_BEO = false;

//E1
if (E1.alg_WatchDog_Pulse_E1) {   BEO_Delay_E1_v++;
																BEO_Delay_E1_n = 0;}
else									{   BEO_Delay_E1_n++;
																BEO_Delay_E1_v = 0;}

anpar.E1_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_E1_v), TNANFloat(BEO_Delay_E1_n) );
alg.E1_brk = ((BEO_Delay_E1_v >= BEO_noLink_sp) || (BEO_Delay_E1_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.E1_brk) E1_ctrl_BEO = false;

//UE8
if (UE8.alg_WatchDog_Pulse_UE8) {   BEO_Delay_UE8_v++;
																BEO_Delay_UE8_n = 0;}
else									{   BEO_Delay_UE8_n++;
																BEO_Delay_UE8_v = 0;}

anpar.UE8_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_UE8_v), TNANFloat(BEO_Delay_UE8_n) );
alg.UE8_brk = ((BEO_Delay_UE8_v >= BEO_noLink_sp) || (BEO_Delay_UE8_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.UE8_brk) UE8_ctrl_BEO = false;

//AVOM
if (AVOM.alg_WatchDog_Pulse_AVOM) {   BEO_Delay_AVOM_v++;
																BEO_Delay_AVOM_n = 0;}
else									{   BEO_Delay_AVOM_n++;
																BEO_Delay_AVOM_v = 0;}

anpar.AVOM_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_AVOM_v), TNANFloat(BEO_Delay_AVOM_n) );
alg.AVOM_brk = ((BEO_Delay_AVOM_v >= BEO_noLink_sp) || (BEO_Delay_AVOM_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.AVOM_brk) AVOM_ctrl_BEO = false;

//AVOM2
if (AVOM2.alg_WatchDog_Pulse_AVOM2) {   BEO_Delay_AVOM2_v++;
																BEO_Delay_AVOM2_n = 0;}
else									{   BEO_Delay_AVOM2_n++;
																BEO_Delay_AVOM2_v = 0;}

anpar.AVOM2_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_AVOM2_v), TNANFloat(BEO_Delay_AVOM2_n) );
alg.AVOM2_brk = ((BEO_Delay_AVOM2_v >= BEO_noLink_sp) || (BEO_Delay_AVOM2_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.AVOM2_brk) AVOM2_ctrl_BEO = false;

//UPD1
if (UPD1.alg_WatchDog_Pulse_UPD1) {   BEO_Delay_UPD1_v++;
																BEO_Delay_UPD1_n = 0;}
else									{   BEO_Delay_UPD1_n++;
																BEO_Delay_UPD1_v = 0;}

anpar.UPD1_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_UPD1_v), TNANFloat(BEO_Delay_UPD1_n) );
alg.UPD1_brk = ((BEO_Delay_UPD1_v >= BEO_noLink_sp) || (BEO_Delay_UPD1_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.UPD1_brk) UPD1_ctrl_BEO = false;

//MI
if (MI.alg_WatchDog_Pulse_MI) {   BEO_Delay_MI_v++;
																BEO_Delay_MI_n = 0;}
else									{   BEO_Delay_MI_n++;
																BEO_Delay_MI_v = 0;}

anpar.MI_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_MI_v), TNANFloat(BEO_Delay_MI_n) );
alg.MI_brk = ((BEO_Delay_MI_v >= BEO_noLink_sp) || (BEO_Delay_MI_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.MI_brk) MI_ctrl_BEO = false;

//NKU01
if (NKU01.alg_WatchDog_Pulse_NKU01) {   BEO_Delay_NKU01_v++;
																BEO_Delay_NKU01_n = 0;}
else									{   BEO_Delay_NKU01_n++;
																BEO_Delay_NKU01_v = 0;}

anpar.NKU01_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_NKU01_v), TNANFloat(BEO_Delay_NKU01_n) );
alg.NKU01_brk = ((BEO_Delay_NKU01_v >= BEO_noLink_sp) || (BEO_Delay_NKU01_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.NKU01_brk) NKU01_ctrl_BEO = false;

//NKU02
if (NKU02.alg_WatchDog_Pulse_NKU02) {   BEO_Delay_NKU02_v++;
																BEO_Delay_NKU02_n = 0;}
else									{   BEO_Delay_NKU02_n++;
																BEO_Delay_NKU02_v = 0;}

anpar.NKU02_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_NKU02_v), TNANFloat(BEO_Delay_NKU02_n) );
alg.NKU02_brk = ((BEO_Delay_NKU02_v >= BEO_noLink_sp) || (BEO_Delay_NKU02_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.NKU02_brk) NKU02_ctrl_BEO = false;

//NKU03
if (NKU03.alg_WatchDog_Pulse_NKU03) {   BEO_Delay_NKU03_v++;
																BEO_Delay_NKU03_n = 0;}
else									{   BEO_Delay_NKU03_n++;
																BEO_Delay_NKU03_v = 0;}

anpar.NKU03_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_NKU03_v), TNANFloat(BEO_Delay_NKU03_n) );
alg.NKU03_brk = ((BEO_Delay_NKU03_v >= BEO_noLink_sp) || (BEO_Delay_NKU03_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.NKU03_brk) NKU03_ctrl_BEO = false;

//NKU06
if (NKU06.alg_WatchDog_Pulse_NKU06) {   BEO_Delay_NKU06_v++;
																BEO_Delay_NKU06_n = 0;}
else									{   BEO_Delay_NKU06_n++;
																BEO_Delay_NKU06_v = 0;}

anpar.NKU06_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_NKU06_v), TNANFloat(BEO_Delay_NKU06_n) );
alg.NKU06_brk = ((BEO_Delay_NKU06_v >= BEO_noLink_sp) || (BEO_Delay_NKU06_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.NKU06_brk) NKU06_ctrl_BEO = false;

//NKU07
if (NKU07.alg_WatchDog_Pulse_NKU07) {   BEO_Delay_NKU07_v++;
																BEO_Delay_NKU07_n = 0;}
else									{   BEO_Delay_NKU07_n++;
																BEO_Delay_NKU07_v = 0;}

anpar.NKU07_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_NKU07_v), TNANFloat(BEO_Delay_NKU07_n) );
alg.NKU07_brk = ((BEO_Delay_NKU07_v >= BEO_noLink_sp) || (BEO_Delay_NKU07_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.NKU07_brk) NKU07_ctrl_BEO = false;

//NKU08
if (NKU08.alg_WatchDog_Pulse_NKU08) {   BEO_Delay_NKU08_v++;
																BEO_Delay_NKU08_n = 0;}
else									{   BEO_Delay_NKU08_n++;
																BEO_Delay_NKU08_v = 0;}

anpar.NKU08_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_NKU08_v), TNANFloat(BEO_Delay_NKU08_n) );
alg.NKU08_brk = ((BEO_Delay_NKU08_v >= BEO_noLink_sp) || (BEO_Delay_NKU08_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.NKU08_brk) NKU08_ctrl_BEO = false;

//NKU10
if (NKU10.alg_WatchDog_Pulse_NKU10) {   BEO_Delay_NKU10_v++;
																BEO_Delay_NKU10_n = 0;}
else									{   BEO_Delay_NKU10_n++;
																BEO_Delay_NKU10_v = 0;}

anpar.NKU10_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_NKU10_v), TNANFloat(BEO_Delay_NKU10_n) );
alg.NKU10_brk = ((BEO_Delay_NKU10_v >= BEO_noLink_sp) || (BEO_Delay_NKU10_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.NKU10_brk) NKU10_ctrl_BEO = false;

//NKU12
if (NKU12.alg_WatchDog_Pulse_NKU12) {   BEO_Delay_NKU12_v++;
																BEO_Delay_NKU12_n = 0;}
else									{   BEO_Delay_NKU12_n++;
																BEO_Delay_NKU12_v = 0;}

anpar.NKU12_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_NKU12_v), TNANFloat(BEO_Delay_NKU12_n) );
alg.NKU12_brk = ((BEO_Delay_NKU12_v >= BEO_noLink_sp) || (BEO_Delay_NKU12_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.NKU12_brk) NKU12_ctrl_BEO = false;

//NKU13
if (NKU13.alg_WatchDog_Pulse_NKU13) {   BEO_Delay_NKU13_v++;
																BEO_Delay_NKU13_n = 0;}
else									{   BEO_Delay_NKU13_n++;
																BEO_Delay_NKU13_v = 0;}

anpar.NKU13_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_NKU13_v), TNANFloat(BEO_Delay_NKU13_n) );
alg.NKU13_brk = ((BEO_Delay_NKU13_v >= BEO_noLink_sp) || (BEO_Delay_NKU13_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.NKU13_brk) NKU13_ctrl_BEO = false;

//NKU16
if (NKU16.alg_WatchDog_Pulse_NKU16) {   BEO_Delay_NKU16_v++;
																BEO_Delay_NKU16_n = 0;}
else									{   BEO_Delay_NKU16_n++;
																BEO_Delay_NKU16_v = 0;}

anpar.NKU16_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_NKU16_v), TNANFloat(BEO_Delay_NKU16_n) );
alg.NKU16_brk = ((BEO_Delay_NKU16_v >= BEO_noLink_sp) || (BEO_Delay_NKU16_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.NKU16_brk) NKU16_ctrl_BEO = false;

//NKU_AV01
if (NKU_AV01.alg_WatchDog_Pulse_NKU_AV01) {   BEO_Delay_NKU_AV01_v++;
																BEO_Delay_NKU_AV01_n = 0;}
else									{   BEO_Delay_NKU_AV01_n++;
																BEO_Delay_NKU_AV01_v = 0;}

anpar.NKU_AV01_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_NKU_AV01_v), TNANFloat(BEO_Delay_NKU_AV01_n) );
alg.NKU_AV01_brk = ((BEO_Delay_NKU_AV01_v >= BEO_noLink_sp) || (BEO_Delay_NKU_AV01_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.NKU_AV01_brk) NKU_AV01_ctrl_BEO = false;

//NKU_AV02
if (NKU_AV02.alg_WatchDog_Pulse_NKU_AV02) {   BEO_Delay_NKU_AV02_v++;
																BEO_Delay_NKU_AV02_n = 0;}
else									{   BEO_Delay_NKU_AV02_n++;
																BEO_Delay_NKU_AV02_v = 0;}

anpar.NKU_AV02_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_NKU_AV02_v), TNANFloat(BEO_Delay_NKU_AV02_n) );
alg.NKU_AV02_brk = ((BEO_Delay_NKU_AV02_v >= BEO_noLink_sp) || (BEO_Delay_NKU_AV02_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.NKU_AV02_brk) NKU_AV02_ctrl_BEO = false;

//NKU415
if (NKU415.alg_WatchDog_Pulse_NKU415) {   BEO_Delay_NKU415_v++;
																BEO_Delay_NKU415_n = 0;}
else									{   BEO_Delay_NKU415_n++;
																BEO_Delay_NKU415_v = 0;}

anpar.NKU415_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_NKU415_v), TNANFloat(BEO_Delay_NKU415_n) );
alg.NKU415_brk = ((BEO_Delay_NKU415_v >= BEO_noLink_sp) || (BEO_Delay_NKU415_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.NKU415_brk) NKU415_ctrl_BEO = false;

//MP_GTU
if (MP_GTU.alg_WatchDog_Pulse_MP_GTU) {   BEO_Delay_MP_GTU_v++;
																BEO_Delay_MP_GTU_n = 0;}
else									{   BEO_Delay_MP_GTU_n++;
																BEO_Delay_MP_GTU_v = 0;}

anpar.MP_GTU_noLink_time = cycleTime.base*maxValue( TNANFloat(BEO_Delay_MP_GTU_v), TNANFloat(BEO_Delay_MP_GTU_n) );
alg.MP_GTU_brk = ((BEO_Delay_MP_GTU_v >= BEO_noLink_sp) || (BEO_Delay_MP_GTU_n >= BEO_noLink_sp)) && ! tch.imit;
if (!alg.MP_GTU_brk) MP_GTU_ctrl_BEO = false;

alg.AIS_brk_EO = 	//Обобщённый признак обрыва связи с учётом участия АИС в запуске ЭО
					alg.UCP1_A1_brk && alg.UCP1_A1_brk_isEO ||
					alg.UCP1_A2_brk && alg.UCP1_A2_brk_isEO ||
					alg.RVU_brk && alg.RVU_brk_isEO ||
					alg.KST_brk && alg.KST_brk_isEO ||
					alg.N_brk && alg.N_brk_isEO ||
					alg.TG_brk && alg.TG_brk_isEO ||
					alg.BG_brk && alg.BG_brk_isEO ||
					alg.VA_brk && alg.VA_brk_isEO ||
					alg.E1_brk && alg.E1_brk_isEO ||
					alg.UE8_brk && alg.UE8_brk_isEO ||
					alg.AVOM_brk && alg.AVOM_brk_isEO ||
					alg.AVOM2_brk && alg.AVOM2_brk_isEO ||
					alg.UPD1_brk && alg.UPD1_brk_isEO ||
					alg.MI_brk && alg.MI_brk_isEO ||
					alg.NKU01_brk && alg.NKU01_brk_isEO ||
					alg.NKU02_brk && alg.NKU02_brk_isEO ||
					alg.NKU03_brk && alg.NKU03_brk_isEO ||
					alg.NKU06_brk && alg.NKU06_brk_isEO ||
					alg.NKU07_brk && alg.NKU07_brk_isEO ||
					alg.NKU08_brk && alg.NKU08_brk_isEO ||
					alg.NKU10_brk && alg.NKU10_brk_isEO ||
					alg.NKU12_brk && alg.NKU12_brk_isEO ||
					alg.NKU13_brk && alg.NKU13_brk_isEO ||
					alg.NKU16_brk && alg.NKU16_brk_isEO ||
					alg.NKU_AV01_brk && alg.NKU_AV01_brk_isEO ||
					alg.NKU_AV02_brk && alg.NKU_AV02_brk_isEO ||
					alg.NKU415_brk && alg.NKU415_brk_isEO ||
					alg.MP_GTU_brk && alg.MP_GTU_brk_isEO;
//блокировка подачи импульсов в БЭО при неисправности
if (!alg.AIS_brk_EO) {dgo.WatchDog_Pulse = !dgo.WatchDog_Pulse; //Выдаём пульс в БЭО, когда связь с АИС исправна

alg.AIS_brk_AO = 	//Обобщённый признак обрыва связи с учётом участия АИС в запуске АО
					alg.UCP1_A1_brk && alg.UCP1_A1_brk_isAO ||
					alg.UCP1_A2_brk && alg.UCP1_A2_brk_isAO ||
					alg.RVU_brk && alg.RVU_brk_isAO ||
					alg.KST_brk && alg.KST_brk_isAO ||
					alg.N_brk && alg.N_brk_isAO ||
					alg.TG_brk && alg.TG_brk_isAO ||
					alg.BG_brk && alg.BG_brk_isAO ||
					alg.VA_brk && alg.VA_brk_isAO ||
					alg.E1_brk && alg.E1_brk_isAO ||
					alg.UE8_brk && alg.UE8_brk_isAO ||
					alg.AVOM_brk && alg.AVOM_brk_isAO ||
					alg.AVOM2_brk && alg.AVOM2_brk_isAO ||
					alg.UPD1_brk && alg.UPD1_brk_isAO ||
					alg.MI_brk && alg.MI_brk_isAO ||
					alg.NKU01_brk && alg.NKU01_brk_isAO ||
					alg.NKU02_brk && alg.NKU02_brk_isAO ||
					alg.NKU03_brk && alg.NKU03_brk_isAO ||
					alg.NKU06_brk && alg.NKU06_brk_isAO ||
					alg.NKU07_brk && alg.NKU07_brk_isAO ||
					alg.NKU08_brk && alg.NKU08_brk_isAO ||
					alg.NKU10_brk && alg.NKU10_brk_isAO ||
					alg.NKU12_brk && alg.NKU12_brk_isAO ||
					alg.NKU13_brk && alg.NKU13_brk_isAO ||
					alg.NKU16_brk && alg.NKU16_brk_isAO ||
					alg.NKU_AV01_brk && alg.NKU_AV01_brk_isAO ||
					alg.NKU_AV02_brk && alg.NKU_AV02_brk_isAO ||
					alg.NKU415_brk && alg.NKU415_brk_isAO ||
					alg.MP_GTU_brk && alg.MP_GTU_brk_isAO || smd.PZ;
alg.AIS_brk_VO = 	//Обобщённый признак обрыва связи с учётом участия АИС в запуске ВО
					alg.UCP1_A1_brk && alg.UCP1_A1_brk_isVO ||
					alg.UCP1_A2_brk && alg.UCP1_A2_brk_isVO ||
					alg.RVU_brk && alg.RVU_brk_isVO ||
					alg.KST_brk && alg.KST_brk_isVO ||
					alg.N_brk && alg.N_brk_isVO ||
					alg.TG_brk && alg.TG_brk_isVO ||
					alg.BG_brk && alg.BG_brk_isVO ||
					alg.VA_brk && alg.VA_brk_isVO ||
					alg.E1_brk && alg.E1_brk_isVO ||
					alg.UE8_brk && alg.UE8_brk_isVO ||
					alg.AVOM_brk && alg.AVOM_brk_isVO ||
					alg.AVOM2_brk && alg.AVOM2_brk_isVO ||
					alg.UPD1_brk && alg.UPD1_brk_isVO ||
					alg.MI_brk && alg.MI_brk_isVO ||
					alg.NKU01_brk && alg.NKU01_brk_isVO ||
					alg.NKU02_brk && alg.NKU02_brk_isVO ||
					alg.NKU03_brk && alg.NKU03_brk_isVO ||
					alg.NKU06_brk && alg.NKU06_brk_isVO ||
					alg.NKU07_brk && alg.NKU07_brk_isVO ||
					alg.NKU08_brk && alg.NKU08_brk_isVO ||
					alg.NKU10_brk && alg.NKU10_brk_isVO ||
					alg.NKU12_brk && alg.NKU12_brk_isVO ||
					alg.NKU13_brk && alg.NKU13_brk_isVO ||
					alg.NKU16_brk && alg.NKU16_brk_isVO ||
					alg.NKU_AV01_brk && alg.NKU_AV01_brk_isVO ||
					alg.NKU_AV02_brk && alg.NKU_AV02_brk_isVO ||
					alg.NKU415_brk && alg.NKU415_brk_isVO ||
					alg.MP_GTU_brk && alg.MP_GTU_brk_isVO || smd.PZ;
}

//сброс счетчиков
else
{
	BEO_Pulse = BEO_Pulse_Link = 0;
	BEO_Delay_UCP1_A1_v = BEO_Delay_UCP1_A1_n = 0;
	BEO_Delay_UCP1_A2_v = BEO_Delay_UCP1_A2_n = 0;
	BEO_Delay_RVU_v = BEO_Delay_RVU_n = 0;
	BEO_Delay_KST_v = BEO_Delay_KST_n = 0;
	BEO_Delay_N_v = BEO_Delay_N_n = 0;
	BEO_Delay_TG_v = BEO_Delay_TG_n = 0;
	BEO_Delay_BG_v = BEO_Delay_BG_n = 0;
	BEO_Delay_VA_v = BEO_Delay_VA_n = 0;
	BEO_Delay_E1_v = BEO_Delay_E1_n = 0;
	BEO_Delay_UE8_v = BEO_Delay_UE8_n = 0;
	BEO_Delay_AVOM_v = BEO_Delay_AVOM_n = 0;
	BEO_Delay_AVOM2_v = BEO_Delay_AVOM2_n = 0;
	BEO_Delay_UPD1_v = BEO_Delay_UPD1_n = 0;
	BEO_Delay_MI_v = BEO_Delay_MI_n = 0;
	BEO_Delay_NKU01_v = BEO_Delay_NKU01_n = 0;
	BEO_Delay_NKU02_v = BEO_Delay_NKU02_n = 0;
	BEO_Delay_NKU03_v = BEO_Delay_NKU03_n = 0;
	BEO_Delay_NKU06_v = BEO_Delay_NKU06_n = 0;
	BEO_Delay_NKU07_v = BEO_Delay_NKU07_n = 0;
	BEO_Delay_NKU08_v = BEO_Delay_NKU08_n = 0;
	BEO_Delay_NKU10_v = BEO_Delay_NKU10_n = 0;
	BEO_Delay_NKU12_v = BEO_Delay_NKU12_n = 0;
	BEO_Delay_NKU13_v = BEO_Delay_NKU13_n = 0;
	BEO_Delay_NKU16_v = BEO_Delay_NKU16_n = 0;
	BEO_Delay_NKU_AV01_v = BEO_Delay_NKU_AV01_n = 0;
	BEO_Delay_NKU_AV02_v = BEO_Delay_NKU_AV02_n = 0;
	BEO_Delay_NKU415_v = BEO_Delay_NKU415_n = 0;
	BEO_Delay_MP_GTU_v = BEO_Delay_MP_GTU_n = 0;
}
}