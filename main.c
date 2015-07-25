
/*********************************************************************
*
* LCD1602��ʾ֮����λ�����ķ���
*
* ����:4�����ݿ�ȣ�����Lcd1602
* ��LCD1602��Ļ�ϵ�һ����ʾ��Hello!LCD1602
* �ڶ�����ʾ��MSP430G2553
* Ӳ����·��MSP430g2553
* Ӳ�����ӣ�
* MSP430��LCD������Ϣ
* LCD1602��4λ�ӿڣ���ʹ��D4-D7���ݿڣ�D0-D3������MCU
* PIN1 --> ��
* PIN2 --> VCC��һ��Ҫ��+5V��
* PIN3 -->����ʱ���գ�ʵ�ʵ�· 2K����-->�� (һ��Ҫ�Ӻã�����û���κ���ʾ)
* PIN4 --> RS --> P1.6
* PIN5 --> R/W --> GND
* PIN6 --> EN --> P1.7
* PIN7 --> D0����
* PIN8 --> D1����
* PIN9 --> D2����
* PIN10 --> D3����
* PIN11 --> D4 --> P2.4
* PIN12 --> D5 --> P2.5
* PIN13 --> D6 --> P2.6
* PIN14 --> D7 --> P2.7
* PIN15 --> VCC
* PIN16 --> ��
* ��������MSP430FETȫϵ��JTAG������
* ��������� CCS5.1.1 ����
*****************************************************************/
#include <intrinsics.h>
#include <msp430g2253.h>
#include <msp430.h>
/*****************************************************

                                                     �˿ڶ���

****************************************************/
#define LCD_EN_PORT P1OUT    //����2��Ҫ��Ϊͬһ����
#define LCD_EN_DDR P1DIR
#define LCD_RS_PORT P1OUT    //����2��Ҫ��Ϊͬһ����
#define LCD_RS_DDR P1DIR
#define LCD_DATA_PORT P2OUT  //����3��Ҫ��Ϊͬһ����
#define LCD_DATA_DDR P2DIR   //һ��Ҫ�ø�4λ
#define LCD_RS BIT6
#define LCD_EN BIT7
#define LCD_DATA    BIT7|BIT6|BIT5|BIT4   //4λ����������ģʽ
/***************************************************

                                                          Ԥ���庯��

**************************************************/
void LCD_init(void);
void LCD_init_first(void);
void LCD_en_write1(void);  //������ʹ��
void LCD_en_write2(void);  //�½���ʹ��
void LCD_write_command(unsigned char command);
void LCD_write_data(unsigned char data);
void LCD_set_xy (unsigned char x, unsigned char y);
void LCD_write_string(unsigned char X,unsigned char Y, unsigned char *s);
void LCD_write_char(unsigned char X,unsigned char Y, unsigned char data);
void delay_1ms(void);
void delay_nus(unsigned int n);
void delay_nms(unsigned int n);
unsigned char LCDBuf1[]={"Hello!LCD1602"};   //��һ��Ҫ��ʾ������
unsigned char LCDBuf2[]={"MSP430G2553"};     //�ڶ���Ҫ��ʾ������

/********************************************

                                               ������

*******************************************/

void main()
{
WDTCTL = WDTPW + WDTHOLD;     // �رտ��Ź�
LCD_init_first();
LCD_init();
delay_nms(100);
LCD_write_string(0,0,LCDBuf1);
delay_nms(10);
LCD_write_string(0,1,LCDBuf2);
}
/********************************************

            LCDҺ����������

*******************************************/

void LCD_init_first(void)         //LCD1602Һ����ʼ����������������
{
        delay_nms(500);
     LCD_DATA_DDR|=LCD_DATA;   //���ݿڷ���Ϊ���
     LCD_EN_DDR|=LCD_EN;       //����EN����Ϊ���
     LCD_RS_DDR|=LCD_RS;       //����RS����Ϊ���

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
*             LCD1602Һ����ʼ������
*
****************************************/
void LCD_init(void)
{
delay_nms(500);
LCD_DATA_DDR|=LCD_DATA;   //���ݿڷ���Ϊ���
LCD_EN_DDR|=LCD_EN;       //����EN����Ϊ���
LCD_RS_DDR|=LCD_RS;       //����RS����Ϊ���

delay_nms(500);

LCD_write_command(0x28);  //4λ���ݽӿ�
delay_nms(50);
LCD_write_command(0x28);  //4λ���ݽӿ�
delay_nms(50);
    LCD_write_command(0x28);  //4λ���ݽӿ�
delay_nms(50);
    LCD_en_write2();
    delay_nms(50);
LCD_write_command(0x28); //4λ���ݽӿ�
delay_nms(500);
LCD_write_command(0x01); //����
LCD_write_command(0x0c); //��ʾ�����ع�꣬����˸
    LCD_write_command(0x06); //�趨���뷽ʽ����������λ
delay_nms(50);


}

/*****************************************
*
*             Һ��ʹ��������
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
*             Һ��ʹ���½���
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
*               дָ���
*
****************************************/
void LCD_write_command(unsigned char command)
{
   delay_nus(16);
   P2SEL=0x00;
   LCD_RS_PORT&=~LCD_RS; //RS=0
   LCD_en_write1();
   LCD_DATA_PORT&=0X0f; //�����λ
   LCD_DATA_PORT|=command&0xf0; //д����λ

   delay_nus(16);
   LCD_en_write2();
   command=command<<4; //����λ�Ƶ�����λ
   LCD_en_write1();
   LCD_DATA_PORT&=0x0f; //�����λ
   LCD_DATA_PORT|=command&0xf0; //д����λ
   LCD_en_write2();
}

/*****************************************
*
*               д���ݺ���
*
****************************************/
void LCD_write_data(unsigned char data)
{
   delay_nus(16);
   P2SEL=0x00;
   LCD_RS_PORT|=LCD_RS;      //RS=1
   LCD_en_write1();          //E������
   LCD_DATA_PORT&=0X0f;      //�����λ
   LCD_DATA_PORT|=data&0xf0; //д����λ
   delay_nus(16);
   LCD_en_write2();
   data=data<<4;             //����λ�Ƶ�����λ
   LCD_en_write1();
   LCD_DATA_PORT&=0X0f;      //�����λ
   LCD_DATA_PORT|=data&0xf0; //д����λ
   LCD_en_write2();
}

/*****************************************
*
*               д��ַ����
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
*LCD������λ��д�ַ�������x=0~15,��y=0,1
*
****************************************/
void LCD_write_string(unsigned char X,unsigned char Y,unsigned char *s)
{
LCD_set_xy( X, Y ); //д��ַ
    while (*s)          //д��ʾ�ַ�
    {
      LCD_write_data( *s );
      s++;
    }
}

/*****************************************
*
*     LCD������λ��д�ַ�,��x=0~15,��y=0,1
*
****************************************/

void LCD_write_char(unsigned char X,unsigned char Y,unsigned char data)
{
   LCD_set_xy( X, Y ); //д��ַ
   LCD_write_data( data);
}

/*****************************************
*
*               1us��ʱ����
*
****************************************/

void delay_1us(void)
{
   asm("nop");
}

/*****************************************
*
*               N us��ʱ����
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
*               1ms��ʱ����
*
****************************************/
void delay_1ms(void)
{
   unsigned int i;
   for (i=0;i<1140;i++);
}

/*****************************************
*
*               N ms��ʱ����
*
****************************************/
void delay_nms(unsigned int n)
{
   unsigned int i=0;
   for (i=0;i<n;i++)
   delay_1ms();
}
