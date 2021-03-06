/*
     GB19056.h
*/
#ifndef  GB_19056
#define  GB_19056
#include <rtthread.h>
#include <rthw.h>
#include "stm32f2xx.h"
#include "usart.h"
#include "board.h"
#include <serial.h>

#include  <stdlib.h>//数字转换成字符串
#include  <stdio.h>
#include  <string.h>
#include "App_moduleConfig.h"
#include "spi_sd.h"
#include "Usbh_conf.h"
//#include "usbh_usr.h"

//     DRV  CMD
//  采集命令
#define  GB_SAMPLE_00H    0x00
#define  GB_SAMPLE_01H    0x01
#define  GB_SAMPLE_02H    0x02
#define  GB_SAMPLE_03H    0x03
#define  GB_SAMPLE_04H    0x04
#define  GB_SAMPLE_05H    0x05
#define  GB_SAMPLE_06H    0x06
#define  GB_SAMPLE_07H    0x07
#define  GB_SAMPLE_08H    0x08
#define  GB_SAMPLE_09H    0x09
#define  GB_SAMPLE_10H    0x10
#define  GB_SAMPLE_11H    0x11
#define  GB_SAMPLE_12H    0x12
#define  GB_SAMPLE_13H    0x13
#define  GB_SAMPLE_14H    0x14
#define  GB_SAMPLE_15H    0x15
//   设置命令
#define  GB_SET_82H      0x82
#define  GB_SET_83H      0x83
#define  GB_SET_84H      0x84
#define  GB_SET_82H      0x82
#define  GB_SET_C2H      0x82
#define  GB_SET_C3H      0x83
#define  GB_SET_C4H      0x84







//    国标安全警示功能
/*
    5  分钟一组， 每组提示3 次
    a.超时驾驶b.未登录 c . 超时报警 d. 记录仪状态异常
*/
typedef struct _GB_WARN
{
    u8   Warn_state_Enable;
    u8   group_playTimes;      //  每一组播放次数
    u32  FiveMin_sec_counter; //  5  分钟定时器
} GB_WARN;






typedef  struct  _GB_STRKT
{
    u8  workstate;  // 记录仪串口工作模式  1:enable  2:disable
    u8  RX_CMD;   //  接收命令字
    u8  TX_CMD;   //  发送命令字
    u8  RX_FCS;   //   接收校验
    u8  TX_FCS;   //   发送校验
    u8  rx_buf[128]; //接收字符
    u16  rx_infoLen; //信息块长度
    u8  rx_wr; //
    u8  rx_flag;
    u32 u1_rx_timer;

    //---------usb -----------
    u8  usb_exacute_output;  // 使能输出     0 : disable     1:  enable    2:  usb output   working
    u8 Usbfilename[256];
    u8 usb_write_step;  //  写入操作步骤
    u8 usb_xor_fcs;

    u8   DB9_7pin;  //    0: instate    1:   outsate
    u16  DeltaPlus_out_Duration;    //  在指定脉冲系数前提下，输入脉冲的间隔
    u8   Deltaplus_outEnble; //  RTC output    1         signal    output     2      3  output ack   idle=0；

    u32  Plus_tick_counter;

    u32  DoubtData3_counter; //  事故疑点3  计数器
    u8   DoubtData3_triggerFlag; //  事故疑点3    触发 标志位
    u32  Delta_lati;
    u32  Delta_longi;

    // 速度日志相关
    u32  speed_larger40km_counter;  //  速度大于40 km/h   couter      5 分钟内 速度差值，速度差值偏差大于11%(差值除以GPS速度) 认为异常
    // 在范围内认为是正常，每个日历天内至少要判断速度状态1次，同存储速度状态
    u8   speedlog_Day_save_times; //  当前日历天内存储的次数(最大255次) ， 跨天状态清 0
    u32  gps_largeThan40_counter;
    u32  delataSpd_largerThan11_counter; //
    u8   start_record_speedlog_flag;  // 开始记录速度日志状态  每天只能记录1 次

    // 起始时间结束时间
    u32  Query_Start_Time;
    u32  Query_End_Time;
    u16  Max_dataBlocks;
    u16  Total_dataBlocks;
    u16  Send_add;


    //  安全警示
    //   a.超时驾驶b.未登录 c . 超速度报警 d. 记录仪状态异常
    GB_WARN   SPK_DriveExceed_Warn;  	// 超时报警
    GB_WARN	 SPK_UnloginWarn;   // 未登录报警
    GB_WARN	 SPK_Speed_Warn;    //   超速报警
    GB_WARN   SPK_SpeedStatus_Abnormal;      // 速度异常报警
    GB_WARN   SPK_PreTired;    //   疲劳驾驶预报警

    // 检定状态
    u8   Checking_status;   // 进入检定状态标志位

} GB_STRKT;


extern GB_STRKT GB19056;
extern u8  Warn_Play_controlBit;
/*
									使能播报状态控制BIT
								  BIT	0:	  驾驶员未登录状态提示(建议默认取消)
								  BIT	1:	  超时预警
								  BIT	2:	  超时报警
								  BIT	3:	  速度异常报警(建议默认取消)
								  BIT	4:	  超速报警
								  BIT	5:
						   */

extern struct rt_semaphore GB_RX_sem;  //  gb  接收AA 75
extern void GB_Drv_app_init(void);
extern void GB_doubt_Data3_Trigger(u32  lati_old, u32 longi_old, u32  lati_new, u32 longi_new);
extern void  GB_SpeedSatus_Judge(void);
extern void  GB_Warn_Running(void);
extern u8  GB_fcs(u8 *instr, u16 infolen, u8 fcs_value);
extern u32 Time_sec_u32(u8 *instr, u8 len);
extern u8  BCD2HEX(u8 BCDbyte);
extern void  gb_usb_out(void);
extern void GB_out_E1_ACK(void);
extern u16  GB_00_07_info_only(u8 *dest, u8 ID);
extern u16  GB_557A_protocol_00_07_stuff(u8 *dest, u8 ID);
//extern void  output_spd(u16 speed);



extern void  LoRa_Write_log(u8 *info, u16 inf_len);
extern void  LoRa_Read_log(u8 Type);
extern void  Lora_WriteLOG(u8* instr);
extern void  Lora_Tx(u8* txStr);

extern void lora_L_addr(u32 address);
extern void lora_L_ch(u8 channel);
extern void lora_set_baud(u32 baud);
extern void lora_set_desc(u8* desc);
extern void lora_info(void);




#endif
