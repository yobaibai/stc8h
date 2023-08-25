#include	"config.h"
#include <intrins.h>
#include <EEPROM.H>

// ���峣��
#define LED_PIN P16
#define SWITCH_PIN P17
#define EEPROM_ADDR 0x00
//#define EEPROM_START_ADDR 0x0400
//#define EEPROM_END_ADDR 0x1234
#define IAP_ADDRESS 0x0400    //�������MCU�ľ����������
// �������
static unsigned char gear = 1; // ��ǰ��λ
unsigned char led_state = 0; // LED״̬��0Ϊ����1Ϊ��
unsigned char switch_state = 0; // ����״̬��0Ϊ����1Ϊ��
unsigned char switch_locked = 0; // ��������״̬��0Ϊδ������1Ϊ����
unsigned char menu_level = 1; // �˵�����1Ϊһ���˵���2Ϊ�����˵�
unsigned char menu_timeout = 0; // �˵���ʱ�����������ڼ���޲�����ʱ
//static unsigned char  gear=1;  //��λȫ�־�̬���� ��ʼ��λ1
unsigned char button_state = 0; // ��ť״̬��0Ϊδ���£�1Ϊ����
unsigned int button_timer = 0; // ��ť��ʱ�������ڼ�ⳤ���Ͷ̰�
static unsigned char lock =0;   //�����̰�
static unsigned long  timelock=0;
static unsigned long  timelock1=0;
static unsigned char gear1;
static unsigned char one=0;
// ���庯��
void delay_ms(unsigned int ms); // ��ʱ����
void switch_on(); // ��������
void switch_off(); // �ػ�����
void enter_gear_menu(); // ���뵵λѡ��˵�
void select_gear(unsigned char gear); // ѡ��λ
void save_gear(unsigned char gear); // ���浵λ��EEPROM
unsigned char load_gear(); // ��EEPROM�ж�ȡ��λ
void lock_switch(); // ��������
void unlock_switch(); // ��������
void toggle_switch(); // �л�����״̬
void update_led(); // ����LED״̬
void update_menu_timeout(); // ���²˵���ʱ������
void reset_menu_timeout(); // ���ò˵���ʱ������
void exit_menu(); // �˳��˵�
//void Timer0_Init(void);
unsigned char check_button();
void Timer0_Init(void)		//30����@24.000MHz
{
	AUXR &= 0x7F;			//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TMOD |= 0x01;			//���ö�ʱ��ģʽ
	TL0 = 0xA0;				//���ö�ʱ��ʼֵ
	TH0 = 0x15;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
	ET0=1;
	EA=1;
	PT0=0;
}




#define eepromAddress 0x0040
// ������
void main()
{		
			//P_SW2 |= 0x80;//ʹ�ܷ��� XFR
			//void IapIdle();
	    // ��ʼ��IO��
		        P1M0 = 0xbf; P1M1 = 0x4f; 


	  // P1M0 = 0xbf; P1M1 = 0x00; 

	//	TSCHEN = 0x0000;    //TK0~TK15

        P3M0 = 0x04; P3M1 = 0x00; 

	//  TSCHEN1 = 0x00;        //TK0~TK7    4K64TL  ������������
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
		//unsigned char gear = 1; // �������ڴ洢��ȡ��gear���ݣ�Ĭ��Ϊ1

		if(P16==0)
		{

				EEPROM_read_n(eepromAddress, &gear, sizeof(gear));
				// �ָ�֮ǰ�ĵ�λ״̬
				
		}
		else if(P16==1)
		{
		gear = 1; // Ĭ��ֵΪ1
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
    // ��ʼ������״̬
   // switch_locked = 1; // ��ʼ״̬����������
   //P17=0;
	 one=0;
while(1)
{	
	check_button();
	button_timer=0;
	P32=1;
	if(lock==3)
	{
		//switch_on(); // �򿪿��� �ж�����
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
			check_button();  //�жϳ��̰�
			button_timer=0;
				if(lock==2)
				{
						
				//enter_gear_menu();	//��λѡ��
												//��ʱ��0����
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
				
			
			
			
			
        // ��⿪��״̬
        if (SWITCH_PIN == 0 && switch_locked == 0) // ���ذ�����δ����
        {
            toggle_switch(); // �л�����״̬
            reset_menu_timeout(); // ���ò˵���ʱ������
        }

    }
}*/

// �жϰ�ť״̬������ֵΪ��1��ʾ�̰���2��ʾ����
unsigned char check_button()
{
		
    if (P16 == 1) // ��ť����
    {
				while(P16==1)
				{
						//button_state = 1; // ���°�ť״̬
						button_timer++; // ��ʱ����1
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
								button_state = 0; // ���°�ť״̬
								button_timer = 0; // ���ü�ʱ��								
								//P32=~P32;						//�����ʾ
								//P14=~P14;
								lock=3;						// ���عػ�״̬
						}
						if(button_timer>=100 && button_timer<=199)
						{
								button_state = 0; // ���°�ť״̬
								button_timer = 0; // ���ü�ʱ��								
							//	P32=1;						//�����ʾ
								lock=2;						// ���س���״̬
						}
						if(button_timer>=5 && button_timer<=85)
						{
								button_state = 0; // ���°�ť״̬
								button_timer = 0; // ���ü�ʱ��
								lock=1;						// ���ض̰�״̬
						}
		
		}
}

/*void Timer0_Init(void)		//1����@24.000MHz
{
	AUXR &= 0x7F;			//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TL0 = 0x30;				//���ö�ʱ��ʼֵ
	TH0 = 0xF8;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
	ET0=1;
	EA=1;
	PT0=0;

}*/

// ��ʱ��0�жϷ�����
void Timer0_Routine() interrupt 1
{

//	static unsigned char timelock1;
	TL0 = 0x40;		//���ö�ʱ��ʼֵ
	TH0 = 0xA2;		//���ö�ʱ��ʼֵ
	timelock++;
	//timelock1=timelock;
	/*if(timelock==350)
	{
		P32=0;
	//	goto suo;
		
		//timelock1=1;
	}*/
}
  /*  // ���²˵���ʱ������
    update_menu_timeout();
		if(timelock==1)
		{
		timelock=1;
		}

*/
// ��ʱ����
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

// ��������
void switch_on()
{
	
	P14=1;
	P15=1;
	P32=0;
	//select_gear(gear);
    // TODO: ʵ�ֿ�������
}

// �ػ�����
void switch_off()
{
	P14=0;
	P15=0;
	P32=1;
    // TODO: ʵ�ֹػ�����
}

// ���뵵λѡ��˵�
void enter_gear_menu()
{
			
			check_button();
			if(lock==2)
			{	 lock=0;
				 P32=1;


			}
    // TODO: ʵ�ֽ��뵵λѡ��˵�����
}

// ѡ��λ
void select_gear(unsigned char gear)
{
					unsigned char gearValue;

					switch(gear)       //��λѡ����
					{
						case 1:        //1�� ���е������� 5v led��˸һ��
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
            // ��gearֵд��EEPROM
						gearValue = (unsigned char)gear;
						EEPROM_SectorErase(eepromAddress); // �Ȳ���EEPROM����
						EEPROM_write_n(eepromAddress, &gearValue, sizeof(gearValue)); // д��gearֵ

						break;
						case 2:        //2�� 51k�������ͣ��������� 9v led��˸����
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
            // ��gearֵд��EEPROM
						gearValue = (unsigned char)gear;
						EEPROM_SectorErase(eepromAddress); // �Ȳ���EEPROM����
						EEPROM_write_n(eepromAddress, &gearValue, sizeof(gearValue)); // д��gearֵ
						break;
						case 3:        //3�� 100k�������ͣ��������� 12v led��˸����
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
            // ��gearֵд��EEPROM
						gearValue = (unsigned char)gear;
						EEPROM_SectorErase(eepromAddress); // �Ȳ���EEPROM����
						EEPROM_write_n(eepromAddress, &gearValue, sizeof(gearValue)); // д��gearֵ

						break;
						case 4:        //4�� 140k�������ͣ��������� 15v led��˸�Ĵ�
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
            // ��gearֵд��EEPROM
						gearValue = (unsigned char)gear;
						EEPROM_SectorErase(eepromAddress); // �Ȳ���EEPROM����
						EEPROM_write_n(eepromAddress, &gearValue, sizeof(gearValue)); // д��gearֵ

						break;
						case 5:       //�嵵 180k�������ͣ��������� 20v led��˸���
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
            // ��gearֵд��EEPROM
						gearValue = (unsigned char)gear;
						EEPROM_SectorErase(eepromAddress); // �Ȳ���EEPROM����
						EEPROM_write_n(eepromAddress, &gearValue, sizeof(gearValue)); // д��gearֵ
						
					//	gear=1;
						/*if(gear==6)
						{
							gear=1;
						}
						reset_menu_timeout();*/
						
						break;
					}
	
    // TODO: ʵ��ѡ��λ����
}

// ���浵λ��EEPROM
void save_gear(unsigned char gear)
{
    // TODO: ʵ�ֱ��浵λ��EEPROM����
}

// ��EEPROM�ж�ȡ��λ
unsigned char load_gear()
{
    // TODO: ʵ�ִ�EEPROM�ж�ȡ��λ����
}

// ��������
void lock_switch()
{
    switch_locked = 1;
}

// ��������
void unlock_switch()
{
    switch_locked = 0;
}

// �л�����״̬
void toggle_switch()
{
    if (switch_state == 0) // ���ش�
    {
        switch_off(); // �ػ�
        lock_switch(); // ��������
    }
    else // ���عر�
    {
        switch_on(); // ����
        unlock_switch(); // ��������
    }
}

/*// ����LED״̬
void update_led()
{
    if (menu_level == 2) // �ڵ�λѡ��˵���
    {
        if (led_state == 0) // LED��
        {
            LED_PIN = 0; // ����LED
            delay_ms(gear * 100); // ��ʱ
            LED_PIN = 1; // Ϩ��LED
            led_state = 1; // ����LED״̬
        }
        else // LED��
        {
            LED_PIN = 1; // Ϩ��LED
            delay_ms((6 - gear) * 100); // ��ʱ
            LED_PIN = 0; // ����LED
            led_state = 0; // ����LED״̬
        }
    }
    else // ���ڵ�λѡ��˵���
    {
        // TODO: ʵ������LED���Ʋ���
    }
}*/

// ���²˵���ʱ������
void update_menu_timeout()
{
    if (menu_level == 2) // �ڵ�λѡ��˵���
    {
        menu_timeout++;				// ��ʱ��������1
				if(menu_timeout=10000)
				{
				timelock=0;
				}
    }
    else // ���ڵ�λѡ��˵���
    {
        menu_timeout = 0; // ���ó�ʱ������
    }
}

// ���ò˵���ʱ������
void reset_menu_timeout()
{
    timelock = 0;
}

// �˳��˵�
void exit_menu()
{
    menu_level = 1; // ����һ���˵�
    // TODO: ʵ���˳��˵�����
}
