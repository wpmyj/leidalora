/*
     Device  808 .h
*/
#ifndef  _DEVICE808
#define   _DEVICE808

#include <rtthread.h>
#include <rthw.h>
#include "stm32f2xx.h"

#include  <stdlib.h>
#include  <stdio.h>
#include  <string.h>
#include "App_moduleConfig.h"




//--------  Protocol IO define -------------
//----- in pins  -------
#define  ACC_IO_Group          GPIOE               // ACC 管脚设置
#define  ACC_Group_NUM         GPIO_Pin_9

#define  WARN_IO_Group         GPIOE               // 紧急报警 管脚设置
#define  WARN_Group_NUM        GPIO_Pin_8


//---- 报警状态  ------------------
/*
  -------------------------------------------------------------
           F4  行车记录仪 TW705   管脚定义
  -------------------------------------------------------------
  遵循  GB10956 (2012)  Page26  表A.12  规定
 -------------------------------------------------------------
 | Bit  |      Note       |  必备|   MCUpin  |   PCB pin  |   Colour | ADC
 ------------------------------------------------------------
     D7      刹车           *            PE11             9                棕
     D6      左转灯     *             PE10            10               红
     D5      右转灯     *             PC2              8                白
     D4      远光灯     *             PC0              4                黑
     D3      近光灯     *             PC1              5                黄
     D2      雾灯          add          PC3              7                绿      *
     D1      车门          add          PA1              6                灰      *
     D0      预留
*/

#define BREAK_IO_Group                GPIOE                 //  刹车灯
#define BREAK_Group_NUM             GPIO_Pin_11

#define LEFTLIGHT_IO_Group         GPIOE                // 左转灯
#define LEFTLIGHT_Group_NUM       GPIO_Pin_10

#define RIGHTLIGHT_IO_Group       GPIOC               // 右转灯
#define RIGHTLIGHT_Group_NUM      GPIO_Pin_2

#define FARLIGHT_IO_Group         GPIOC              // 远光灯
#define FARLIGHT_Group_NUM        GPIO_Pin_0

#define NEARLIGHT_IO_Group        GPIOA             // 近光灯
#define NEARLIGHT_Group_NUM       GPIO_Pin_6

#define FOGLIGHT_IO_Group          GPIOA          //  雾灯
#define FOGLIGHT_Group_NUM         GPIO_Pin_7

#define DOORLIGHT_IO_Group        GPIOA             // 车门灯   预留
#define DOORLIGHT_Group_NUM       GPIO_Pin_1


//------  out pins ---
#define RELAY_IO_Group           GPIOB               //继电器
#define RELAY_Group_NUM          GPIO_Pin_1

#define Buzzer_IO_Group          GPIOB               //蜂鸣器 
#define Buzzer_Group_Num         GPIO_Pin_6


typedef  struct  _AD_POWER
{
    u16   ADC_ConvertedValue; //电池电压AD数值
    u16   AD_Volte;      // 采集到的实际电压数值
    u16   Classify_Door;   //  区分大车小车类型，  >16V  大型车17V 欠压            <16V 小型车   10V 欠压
    u16   LowWarn_Limit_Value;  //  欠压报警门限值
    u16   LowPowerCounter;
    u16   CutPowerCounter;
    u16   PowerOK_Counter;
    u8    Battery_Flag;

} AD_POWER;



//-----  WachDog related----
extern u8    wdg_reset_flag;    //  Task Idle Hook 相关

//----------AD  电压采集-----
extern AD_POWER  Power_AD;
extern  u16   ADC_ConValue[3];   //   3  个通道ID
extern  u16   AD_2through[2]; //  另外2 路AD 的数值

extern u32  IC2Value;   //
extern u32  DutyCycle;



/*
     -----------------------------
    1.    输入管脚状态监测
     -----------------------------
*/
extern void  WatchDog_Feed(void);
extern void  WatchDogInit(void);
extern void  APP_IOpinInit(void) ;
//  INPUT
extern u8  HardWareGet(void);

//   OUTPUT
extern void  Enable_Relay(void);
extern void  Disable_Relay(void);


/*
     -----------------------------
    2.  应用相关
     -----------------------------
*/
extern   void Init_ADC(void);
extern void TIM3_Config( void );
extern void TIM3_IRQHandler(void);
/*
     -----------------------------
    3.  RT 驱动相关
     -----------------------------
*/

/*
        ----------------------------
       蜂鸣器驱动相关
        ----------------------------
*/
extern	void GPIO_Config_PWM(void);
extern  void TIM_Config_PWM(void);


/*
       -----------------------------
       新驱动应用
      -----------------------------
*/
//  1 .  循环存储
extern   u8       Api_cycle_write(u8 *buffer, u16 len);
extern   u8       Api_cycle_read(u8 *buffer, u16 len);
// 2. Config
extern   u8    Api_Config_write(u8 *name, u16 ID, u8 *buffer, u16 wr_len);

//  3.  其他

extern   void   Api_MediaIndex_Init(void);
extern   u32  Api_DFdirectory_Query(u8 *name, u8  returnType);
extern   u8   Api_DFdirectory_Write(u8 *name, u8 *buffer, u16 len);
extern   u8    Api_DFdirectory_Read(u8 *name, u8 *buffer, u16 len, u8  style , u16 numPacket); // style  1. old-->new   0 : new-->old
extern   u8   Api_DFdirectory_Delete(u8 *name);
extern   u8   API_List_Directories(void );
extern  void  Api_WriteInit_var_rd_wr(void);    //   写初始化话各类型读写记录地址
extern  void  Api_Read_var_rd_wr(void);    //   读初始化话各类型读写记录地址



extern  u8      Api_Config_read(u8 *name, u16 ID, u8 *buffer, u16 Rd_len);
extern   u8     Api_RecordNum_Write( u8 *name, u8 Rec_Num, u8 *buffer, u16 len);
extern    u8    Api_RecordNum_Read( u8 *name, u8 Rec_Num, u8 *buffer, u16 len);

extern u8	DF_Write_RecordAdd(u32 Wr_Address, u32 Rd_Address, u8 Type);
extern u8	DF_Read_RecordAdd(u32 Wr_Address, u32 Rd_Address, u8 Type);


//------------  AD    电压相关  --------------------
extern void  AD_PowerInit(void);
extern void  Voltage_Checking(void);

extern  void  loralight_control(u8 invalue);


#endif
