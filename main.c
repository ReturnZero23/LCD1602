
/*********************************************************************
*
* LCD1602显示之高四位相连的方法
*
* 描述:4线数据宽度，操作Lcd1602
* 在LCD1602屏幕上第一行显示　Hello!LCD1602
* 第二行显示　MSP430G2553
* 硬件电路：MSP430g2553
* 硬件连接：
* MSP430与LCD连接信息
* LCD1602，4位接口，即使用D4-D7数据口，D0-D3不接入MCU
* PIN1 --> 地
* PIN2 --> VCC（一定要接+5V）
* PIN3 -->仿真时悬空，实际电路 2K电阻-->地 (一定要接好，否则没有任何显示)
* PIN4 --> RS --> P1.6
* PIN5 --> R/W --> GND
* PIN6 --> EN --> P1.7
* PIN7 --> D0不接
* PIN8 --> D1不接
* PIN9 --> D2不接
* PIN10 --> D3不接
* PIN11 --> D4 --> P2.4
* PIN12 --> D5 --> P2.5
* PIN13 --> D6 --> P2.6
* PIN14 --> D7 --> P2.7
* PIN15 --> VCC
* PIN16 --> 地
* 调试器：MSP430FET全系列JTAG仿真器
* 调试软件： CCS5.1.1 编译
*****************************************************************/
#include <intrinsics.h>
#include <msp430g2253.h>
#include <msp430.h>
/*****************************************************

                                                     端口定义

****************************************************/
#define LCD_EN_PORT P1OUT    //以下2个要设为同一个口
#define LCD_EN_DDR P1DIR
#define LCD_RS_PORT P1OUT    //以下2个要设为同一个口
#define LCD_RS_DDR P1DIR
#define LCD_DATA_PORT P2OUT  //以下3个要设为同一个口
#define LCD_DATA_DDR P2DIR   //一定要用高4位
#define LCD_RS BIT6
#define LCD_EN BIT7
#define LCD_DATA    BIT7|BIT6|BIT5|BIT4   //4位数据线连接模式
/***************************************************

                                                          预定义函数

**************************************************/
void LCD_init(void);
void LCD_init_first(void);
void LCD_en_write1(void);  //上升沿使能
void LCD_en_write2(void);  //下降沿使能
void LCD_write_command(unsigned char command);
void LCD_write_data(unsigned char data);
void LCD_set_xy (unsigned char x, unsigned char y);
void LCD_write_string(unsigned char X,unsigned char Y, unsigned char *s);
void LCD_write_char(unsigned char X,unsigned char Y, unsigned char data);
void delay_1ms(void);
void delay_nus(unsigned int n);
void delay_nms(unsigned int n);
unsigned char LCDBuf1[]={"Hello!LCD1602"};   //第一行要显示的内容
unsigned char LCDBuf2[]={"MSP430G2553"};     //第二行要显示的内容

/********************************************

                                               主函数

*******************************************/

void main()
{
WDTCTL = WDTPW + WDTHOLD;     // 关闭看门狗
LCD_init_first();
LCD_init();
delay_nms(100);
LCD_write_string(0,0,LCDBuf1);
delay_nms(10);
LCD_write_string(0,1,LCDBuf2);
}
/********************************************

            LCD液晶操作函数

*******************************************/

void LCD_init_first(void)         //LCD1602液晶初始化函数（热启动）
{
        delay_nms(500);
     LCD_DATA_DDR|=LCD_DATA;   //数据口方向为输出
     LCD_EN_DDR|=LCD_EN;       //设置EN方向为输出
     LCD_RS_DDR|=LCD_RS;       //设置RS方向为输出

        delay_nms(50);
        LCD_write_command(0x30);
        delay_nms(50);
        LCD_write_command(0x30);
        delay_nms(5);
        LCD_write_command(0x30);
        delay_nms(500);

}

/*****************************************
*
*             LCD1602液晶初始化函数
*
****************************************/
void LCD_init(void)
{
delay_nms(500);
LCD_DATA_DDR|=LCD_DATA;   //数据口方向为输出
LCD_EN_DDR|=LCD_EN;       //设置EN方向为输出
LCD_RS_DDR|=LCD_RS;       //设置RS方向为输出

delay_nms(500);

LCD_write_command(0x28);  //4位数据接口
delay_nms(50);
LCD_write_command(0x28);  //4位数据接口
delay_nms(50);
    LCD_write_command(0x28);  //4位数据接口
delay_nms(50);
    LCD_en_write2();
    delay_nms(50);
LCD_write_command(0x28); //4位数据接口
delay_nms(500);
LCD_write_command(0x01); //清屏
LCD_write_command(0x0c); //显示开，关光标，不闪烁
    LCD_write_command(0x06); //设定输入方式，增量不移位
delay_nms(50);


}

/*****************************************
*
*             液晶使能上升沿
*
****************************************/

void LCD_en_write1(void)
{
    LCD_EN_PORT&=~LCD_EN;
    delay_nus(10);
    LCD_EN_PORT|=LCD_EN;
}

/*****************************************
*
*             液晶使能下降沿
*
****************************************/
void LCD_en_write2(void)
{
   LCD_EN_PORT|=LCD_EN;
   delay_nus(10);
   LCD_EN_PORT&=~LCD_EN;
}

/*****************************************
*
*               写指令函数
*
****************************************/
void LCD_write_command(unsigned char command)
{
   delay_nus(16);
   P2SEL=0x00;
   LCD_RS_PORT&=~LCD_RS; //RS=0
   LCD_en_write1();
   LCD_DATA_PORT&=0X0f; //清高四位
   LCD_DATA_PORT|=command&0xf0; //写高四位

   delay_nus(16);
   LCD_en_write2();
   command=command<<4; //低四位移到高四位
   LCD_en_write1();
   LCD_DATA_PORT&=0x0f; //清高四位
   LCD_DATA_PORT|=command&0xf0; //写低四位
   LCD_en_write2();
}

/*****************************************
*
*               写数据函数
*
****************************************/
void LCD_write_data(unsigned char data)
{
   delay_nus(16);
   P2SEL=0x00;
   LCD_RS_PORT|=LCD_RS;      //RS=1
   LCD_en_write1();          //E上升沿
   LCD_DATA_PORT&=0X0f;      //清高四位
   LCD_DATA_PORT|=data&0xf0; //写高四位
   delay_nus(16);
   LCD_en_write2();
   data=data<<4;             //低四位移到高四位
   LCD_en_write1();
   LCD_DATA_PORT&=0X0f;      //清高四位
   LCD_DATA_PORT|=data&0xf0; //写低四位
   LCD_en_write2();
}

/*****************************************
*
*               写地址函数
*
****************************************/
void LCD_set_xy( unsigned char x, unsigned char y )
{
   unsigned char address;
   if (y == 0) address = 0x80 + x;
   else address = 0xc0 + x;
   LCD_write_command( address);
}

/*****************************************
*
*LCD在任意位置写字符串，列x=0~15,行y=0,1
*
****************************************/
void LCD_write_string(unsigned char X,unsigned char Y,unsigned char *s)
{
LCD_set_xy( X, Y ); //写地址
    while (*s)          //写显示字符
    {
      LCD_write_data( *s );
      s++;
    }
}

/*****************************************
*
*     LCD在任意位置写字符,列x=0~15,行y=0,1
*
****************************************/

void LCD_write_char(unsigned char X,unsigned char Y,unsigned char data)
{
   LCD_set_xy( X, Y ); //写地址
   LCD_write_data( data);
}

/*****************************************
*
*               1us延时函数
*
****************************************/

void delay_1us(void)
{
   asm("nop");
}

/*****************************************
*
*               N us延时函数
*
****************************************/
void delay_nus(unsigned int n)
{
   unsigned int i;
   for (i=0;i<n;i++)
   delay_1us();
}

/*****************************************
*
*               1ms延时函数
*
****************************************/
void delay_1ms(void)
{
   unsigned int i;
   for (i=0;i<1140;i++);
}

/*****************************************
*
*               N ms延时函数
*
****************************************/
void delay_nms(unsigned int n)
{
   unsigned int i=0;
   for (i=0;i<n;i++)
   delay_1ms();
}
