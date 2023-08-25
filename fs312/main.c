#include	"config.h"
#include <intrins.h>
#include <EEPROM.H>

// 定义常量
#define LED_PIN P16
#define SWITCH_PIN P17
#define EEPROM_ADDR 0x00
//#define EEPROM_START_ADDR 0x0400
//#define EEPROM_END_ADDR 0x1234
#define IAP_ADDRESS 0x0400    //根据你的MCU的具体参数设置
// 定义变量
static unsigned char gear = 1; // 当前档位
unsigned char led_state = 0; // LED状态，0为亮，1为灭
unsigned char switch_state = 0; // 开关状态，0为开，1为关
unsigned char switch_locked = 0; // 开关锁定状态，0为未锁定，1为锁定
unsigned char menu_level = 1; // 菜单级别，1为一级菜单，2为二级菜单
unsigned char menu_timeout = 0; // 菜单超时计数器，用于检测无操作超时
//static unsigned char  gear=1;  //档位全局静态变量 初始档位1
unsigned char button_state = 0; // 按钮状态，0为未按下，1为按下
unsigned int button_timer = 0; // 按钮计时器，用于检测长按和短按
static unsigned char lock =0;   //长按短按
static unsigned long  timelock=0;
static unsigned long  timelock1=0;
static unsigned char gear1;
static unsigned char one=0;
// 定义函数
void delay_ms(unsigned int ms); // 延时函数
void switch_on(); // 开机操作
void switch_off(); // 关机操作
void enter_gear_menu(); // 进入档位选择菜单
void select_gear(unsigned char gear); // 选择档位
void save_gear(unsigned char gear); // 保存档位到EEPROM
unsigned char load_gear(); // 从EEPROM中读取档位
void lock_switch(); // 锁定开关
void unlock_switch(); // 解锁开关
void toggle_switch(); // 切换开关状态
void update_led(); // 更新LED状态
void update_menu_timeout(); // 更新菜单超时计数器
void reset_menu_timeout(); // 重置菜单超时计数器
void exit_menu(); // 退出菜单
//void Timer0_Init(void);
unsigned char check_button();
void Timer0_Init(void)		//30毫秒@24.000MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
	TL0 = 0xA0;				//设置定时初始值
	TH0 = 0x15;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0=1;
	EA=1;
	PT0=0;
}




#define eepromAddress 0x0040
// 主函数
void main()
{		
			//P_SW2 |= 0x80;//使能访问 XFR
			//void IapIdle();
	    // 初始化IO口
		        P1M0 = 0xbf; P1M1 = 0x4f; 


	  // P1M0 = 0xbf; P1M1 = 0x00; 

	//	TSCHEN = 0x0000;    //TK0~TK15

        P3M0 = 0x04; P3M1 = 0x00; 

	//  TSCHEN1 = 0x00;        //TK0~TK7    4K64TL  触摸功能设置
  //  TSCHEN2 = 0x00;        //TK8~TK15
		P10=1;
		P11=1;
		P12=1;
		P13=1;
		//P13=1;///touch
		P32=1; ///
		P14=0;
		P15=0;
		P17=0;



		//P17=0;
	//	gear=1;
		delay_ms(2000);
		//unsigned char gear = 1; // 变量用于存储读取的gear数据，默认为1

		if(P16==0)
		{

				EEPROM_read_n(eepromAddress, &gear, sizeof(gear));
				// 恢复之前的档位状态
				
		}
		else if(P16==1)
		{
		gear = 1; // 默认值为1
		}	
		
		select_gear(gear);
if(gear==5)
{
	gear=0;
}
		//P32=~P32;
		P14=~P14;
		delay_ms(3000);
	//	P32=~P32;
		P14=~P14;
		//delay_ms(1000);

		//select_gear(gear);
		//timelock1=0;
		//select_gear(gear++);

	off:	
    // 初始化开关状态
   // switch_locked = 1; // 初始状态下锁定开关
   //P17=0;
	 one=0;
while(1)
{	
	check_button();
	button_timer=0;
	P32=1;
	if(lock==3)
	{
		//switch_on(); // 打开开关 判读！！
	lock=0;
	P14=1;
	P15=1;
	P32=0;
		break;
	}	
}		
	suo:
		
		while(1)
		{	
			P32=0;
		//	one=1;
			check_button();  //判断长短按
			button_timer=0;
				if(lock==2)
				{
						
				//enter_gear_menu();	//档位选择
												//定时器0启动
				lock=0;
			//	while(P16==0);
				Timer0_Init();
				timelock=0;
				while(1)
				{
					one=1;
					P32=1;
					
					if(timelock>700)
					{
						timelock=0;
						timelock=0;
						P32=0;
						lock=0;
						goto suo;
					}
					//while(P16==0);
					check_button();
					button_timer=0;
				/*	if(lock==3)
					{
						lock=0;
						P14=0;
						P15=0;
						P17=0;
						P32=1;
						goto  off;
					}*/
					//while(P16==0);
					//check_button();
					if(lock==2)
					{
						lock=0;
						//P17=~P17;
					}
					if(lock==1)
					{	
						lock=0;	
						P32=1;
						select_gear(++gear);
						gear1=gear;
						if(gear1==5)
						{
							gear=0;
						}
					}
				}			
			}

				//break;
				if(lock==3)
				{
					lock=0;
					P14=0;
					P15=0;
					P32=1;
					//P17=0;
					goto off;
				}

					
					//switch_off();
			
	
	 }
}
			/*if(lock=3)
			{switch_off();}
    while (1)
    {
			Time0;
			if(timelock==1)
			{goto suo;}
			enter_gear_menu();
			check_button();			
					if(lock==2)
					{
						P17=~P17;
					}
					if(lock==1)
					{
					select_gear(gear++);
					}
					if(gear==5)
					{
						gear=1;
					}
				
			
			
			
			
        // 检测开关状态
        if (SWITCH_PIN == 0 && switch_locked == 0) // 开关按下且未锁定
        {
            toggle_switch(); // 切换开关状态
            reset_menu_timeout(); // 重置菜单超时计数器
        }

    }
}*/

// 判断按钮状态，返回值为，1表示短按，2表示长按
unsigned char check_button()
{
		
    if (P16 == 1) // 按钮按下
    {
				while(P16==1)
				{
						//button_state = 1; // 更新按钮状态
						button_timer++; // 计时器加1
						delay_ms(20);
					if(button_timer ==100)
					{
						P32=~P32;
					}
					if(button_timer ==250 && one==0)
					{
						P14=~P14;
						P15=~P15;
					}
				}
				
						if(button_timer>250)
						{
								button_state = 0; // 更新按钮状态
								button_timer = 0; // 重置计时器								
								//P32=~P32;						//灭灯提示
								//P14=~P14;
								lock=3;						// 返回关机状态
						}
						if(button_timer>=100 && button_timer<=199)
						{
								button_state = 0; // 更新按钮状态
								button_timer = 0; // 重置计时器								
							//	P32=1;						//灭灯提示
								lock=2;						// 返回长按状态
						}
						if(button_timer>=5 && button_timer<=85)
						{
								button_state = 0; // 更新按钮状态
								button_timer = 0; // 重置计时器
								lock=1;						// 返回短按状态
						}
		
		}
}

/*void Timer0_Init(void)		//1毫秒@24.000MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x30;				//设置定时初始值
	TH0 = 0xF8;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0=1;
	EA=1;
	PT0=0;

}*/

// 定时器0中断服务函数
void Timer0_Routine() interrupt 1
{

//	static unsigned char timelock1;
	TL0 = 0x40;		//设置定时初始值
	TH0 = 0xA2;		//设置定时初始值
	timelock++;
	//timelock1=timelock;
	/*if(timelock==350)
	{
		P32=0;
	//	goto suo;
		
		//timelock1=1;
	}*/
}
  /*  // 更新菜单超时计数器
    update_menu_timeout();
		if(timelock==1)
		{
		timelock=1;
		}

*/
// 延时函数
void delay_ms(unsigned int ms)
{
	unsigned char data i, j;
	while(ms)
	{
		i = 32;
		j = 40;
		do
		{
			while (--j);
		} while (--i);
		ms--;
	}
	}

// 开机操作
void switch_on()
{
	
	P14=1;
	P15=1;
	P32=0;
	//select_gear(gear);
    // TODO: 实现开机操作
}

// 关机操作
void switch_off()
{
	P14=0;
	P15=0;
	P32=1;
    // TODO: 实现关机操作
}

// 进入档位选择菜单
void enter_gear_menu()
{
			
			check_button();
			if(lock==2)
			{	 lock=0;
				 P32=1;


			}
    // TODO: 实现进入档位选择菜单操作
}

// 选择档位
void select_gear(unsigned char gear)
{
					unsigned char gearValue;

					switch(gear)       //档位选择函数
					{
						case 1:        //1档 所有电阻拉高 5v led闪烁一次
						reset_menu_timeout();
						P10=1;
						P11=1;
						P12=1;
						P13=1;
						P32=0;
						delay_ms(250);
						P32=1;
						delay_ms(250);
						reset_menu_timeout();
            // 将gear值写入EEPROM
						gearValue = (unsigned char)gear;
						EEPROM_SectorErase(eepromAddress); // 先擦除EEPROM扇区
						EEPROM_write_n(eepromAddress, &gearValue, sizeof(gearValue)); // 写入gear值

						break;
						case 2:        //2档 51k电阻拉低，其余拉高 9v led闪烁二次
						reset_menu_timeout();
						P10=0;
						P11=1;
						P12=1;
						P13=1;
						P32=0;
						delay_ms(250);
						P32=1;
						delay_ms(250);
						P32=0;
						delay_ms(250);
						P32=1;
						delay_ms(250);
						reset_menu_timeout();
            // 将gear值写入EEPROM
						gearValue = (unsigned char)gear;
						EEPROM_SectorErase(eepromAddress); // 先擦除EEPROM扇区
						EEPROM_write_n(eepromAddress, &gearValue, sizeof(gearValue)); // 写入gear值
						break;
						case 3:        //3档 100k电阻拉低，其余拉高 12v led闪烁三次
						reset_menu_timeout();
						P10=1;
						P11=0;
						P12=1;
						P13=1;
						P32=0;
						delay_ms(250);
						P32=1;
						delay_ms(250);
						P32=0;
						delay_ms(250);
						P32=1;
						delay_ms(250);
					  P32=0;
						delay_ms(250);
						P32=1;
						delay_ms(250);
						reset_menu_timeout();
            // 将gear值写入EEPROM
						gearValue = (unsigned char)gear;
						EEPROM_SectorErase(eepromAddress); // 先擦除EEPROM扇区
						EEPROM_write_n(eepromAddress, &gearValue, sizeof(gearValue)); // 写入gear值

						break;
						case 4:        //4档 140k电阻拉低，其余拉高 15v led闪烁四次
						reset_menu_timeout();
						P10=1;
						P11=1;
						P12=0;
						P13=1;
						P32=0;
						delay_ms(250);
						P32=1;
						delay_ms(250);				
						P32=0;
						delay_ms(250);
						P32=1;
						delay_ms(250);			
						P32=0;
						delay_ms(250);
						P32=1;
						delay_ms(250);			
						P32=0;
						delay_ms(250);
						P32=1;
						delay_ms(250);
						reset_menu_timeout();
            // 将gear值写入EEPROM
						gearValue = (unsigned char)gear;
						EEPROM_SectorErase(eepromAddress); // 先擦除EEPROM扇区
						EEPROM_write_n(eepromAddress, &gearValue, sizeof(gearValue)); // 写入gear值

						break;
						case 5:       //五档 180k电阻拉低，其余拉高 20v led闪烁五次
						reset_menu_timeout();
						P10=1;
						P11=1;
						P12=1;
						P13=0;
						P32=0;
						delay_ms(250);
						P32=1;
						delay_ms(250);				
						P32=0;
						delay_ms(250);
						P32=1;
						delay_ms(250);			
						P32=0;
						delay_ms(250);
						P32=1;
						delay_ms(250);				
						P32=0;
						delay_ms(250);
						P32=1;
						delay_ms(250);						
						P32=0;
						delay_ms(250);
						P32=1;
						delay_ms(250);
						reset_menu_timeout();
            // 将gear值写入EEPROM
						gearValue = (unsigned char)gear;
						EEPROM_SectorErase(eepromAddress); // 先擦除EEPROM扇区
						EEPROM_write_n(eepromAddress, &gearValue, sizeof(gearValue)); // 写入gear值
						
					//	gear=1;
						/*if(gear==6)
						{
							gear=1;
						}
						reset_menu_timeout();*/
						
						break;
					}
	
    // TODO: 实现选择档位操作
}

// 保存档位到EEPROM
void save_gear(unsigned char gear)
{
    // TODO: 实现保存档位到EEPROM操作
}

// 从EEPROM中读取档位
unsigned char load_gear()
{
    // TODO: 实现从EEPROM中读取档位操作
}

// 锁定开关
void lock_switch()
{
    switch_locked = 1;
}

// 解锁开关
void unlock_switch()
{
    switch_locked = 0;
}

// 切换开关状态
void toggle_switch()
{
    if (switch_state == 0) // 开关打开
    {
        switch_off(); // 关机
        lock_switch(); // 锁定开关
    }
    else // 开关关闭
    {
        switch_on(); // 开机
        unlock_switch(); // 解锁开关
    }
}

/*// 更新LED状态
void update_led()
{
    if (menu_level == 2) // 在档位选择菜单中
    {
        if (led_state == 0) // LED亮
        {
            LED_PIN = 0; // 点亮LED
            delay_ms(gear * 100); // 延时
            LED_PIN = 1; // 熄灭LED
            led_state = 1; // 更新LED状态
        }
        else // LED灭
        {
            LED_PIN = 1; // 熄灭LED
            delay_ms((6 - gear) * 100); // 延时
            LED_PIN = 0; // 点亮LED
            led_state = 0; // 更新LED状态
        }
    }
    else // 不在档位选择菜单中
    {
        // TODO: 实现其他LED控制操作
    }
}*/

// 更新菜单超时计数器
void update_menu_timeout()
{
    if (menu_level == 2) // 在档位选择菜单中
    {
        menu_timeout++;				// 超时计数器加1
				if(menu_timeout=10000)
				{
				timelock=0;
				}
    }
    else // 不在档位选择菜单中
    {
        menu_timeout = 0; // 重置超时计数器
    }
}

// 重置菜单超时计数器
void reset_menu_timeout()
{
    timelock = 0;
}

// 退出菜单
void exit_menu()
{
    menu_level = 1; // 返回一级菜单
    // TODO: 实现退出菜单操作
}
