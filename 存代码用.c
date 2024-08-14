任务一 one~five

(7)
/*延时函数，两个都可用*/
void delay_ms(int time_ms)
{
    for(int j=time_ms;j>0;j--)
        __delay_cycles(1000);
}
void debounce_delay(unsigned int delay) 
{
    volatile unsigned int j;
    for (j = 0; j < delay; j++);//可通过示波器微调
}

#include <msp430g2553.h>
#include "function.h"
void main() {
    WDTCTL = WDTPW + WDTHOLD; // 停用看门狗定时器
    P1DIR |=BIT0;
    while (1)
    {
        P1OUT ^=BIT0;
        delay_ms(200);//0.2s
    }
}
one
#include <msp430g2553.h>
#include "function.h"

#define BUTTON BIT3

void main() {
    WDTCTL = WDTPW + WDTHOLD; // 停用看门狗定时器
    P1DIR |= BIT0; // P1.0设为输出
    P1REN |= BUTTON; // P1.3设为上拉电阻
    P1OUT |= BUTTON; // P1.3设置为高电平

    while (1) {
        if ((P1IN & BUTTON) == 0) { // 如果按键被按下
            debounce_delay(5000); // 延迟一段时间以等待按键稳定
            if ((P1IN & BUTTON) == 0) { // 如果按键仍被按下
                P1OUT ^= BIT0; // 翻转LED灯
                while ((P1IN & BUTTON) == 0); // 等待按键被松开
            }
        }
    }
}
/****************
 * **************/
two
/*闪亮频率切换*/
#include <msp430.h>
#include "function.h"

#define LED1 BIT0
#define BUTTON BIT3

void main() {
    WDTCTL = WDTPW + WDTHOLD; // 停用看门狗定时器
    P1DIR |= LED1; // LED1设为输出
    P1REN |= BUTTON; // P1.3设为上拉电阻
    P1OUT |= BUTTON; // P1.3设置为高电平
    int i = 0;

    while (1) {
        if ((P1IN & BUTTON) == 0) { // 如果按键被按下
            debounce_delay(10000); // 延迟一段时间以等待按键稳定
            if ((P1IN & BUTTON) == 0){ // 如果按键仍被按下
              i++;
            while ((P1IN & BUTTON) == 0); // 等待按键被松开
          }
         }
        P1OUT ^= LED1;
            switch(i){
                case 1:delay_ms(500);break;
                case 2:delay_ms(250);break;
                case 3:delay_ms(166);break;
                case 4:delay_ms(162);break;
                case 5:delay_ms(100);break;
                case 6:i=0;break;
        }
    }
}

three
/*使用按键外部中断的频率闪烁*/
#include <msp430.h>
#include "function.h"
#define LED1 BIT0
#define BUTTON BIT3
int i=0;

// 按键中断处理函数
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR() {
    if ((P1IFG & BUTTON) == BUTTON) { // 如果是 P1.3 的中断
        P1IFG &= ~BUTTON; // 清除中断标志位
        i++;
}
}
void main() {
    WDTCTL = WDTPW + WDTHOLD; // 停用看门狗定时器
    P1DIR |= LED1; // LED1设为输出
    P1REN |= BUTTON; // P1.3设为上拉电阻
    P1OUT |= BUTTON; // P1.3设置为高电平

    P1IE |= BUTTON; // 开启 P1.3 的中断
    P1IES |= BUTTON; // P1.3 中断触发边沿为下降沿

    __enable_interrupt(); // 开启中断

    while (1) {
      
        P1OUT ^= LED1; // 翻转 LED 灯
        switch(i){
         case 1:delay_ms(500);break;
         case 2:delay_ms(250);break;
         case 3:delay_ms(166);break;
         case 4:delay_ms(162);break;
         case 5:delay_ms(100);break;
         case 6:i=0;break;
        }
    }
}


four
/*定时器闪烁*/
#include <msp430g2553.h>
#include "function.h"
#define LED1 BIT0

volatile unsigned int counter = 0; // 计数器

void main() {
    WDTCTL = WDTPW + WDTHOLD; // 停用看门狗定时器
    P1DIR |= LED1; // LED1设为输出

    TA0CCR0 = 5000; // 定时器比较值为 1000，即 1ms
    TA0CTL = TASSEL_2 + ID_3 + MC_1 + TACLR; // 选择 SMCLK 作为时钟源，分频系数为 8，选择上升沿计数模式，清除计数器
    TA0CCTL0 = CCIE; // 开启定时器中断

    __enable_interrupt(); // 开启中断

    while (1) {
        // 循环体留空
    }
}

// 定时器中断处理函数
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A_ISR() {
    counter++; // 计数器加一
    if (counter == 20) { // 计数器达到 20 时，翻转 LED 灯
        P1OUT ^= LED1;
        counter = 0; // 重置计数器
    }
}

five
/*按键外部中断亮灭*/
#include <msp430.h>
#include "function.h"
#define LED1 BIT0
#define BUTTON BIT3


// 按键中断处理函数
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR() {
    if ((P1IFG & BUTTON) == BUTTON) { // 如果是 P1.3 的中断
        P1IFG &= ~BUTTON; // 清除中断标志位
        P1OUT ^= LED1; // 翻转 LED 灯
}
}
void main() {
    WDTCTL = WDTPW + WDTHOLD; // 停用看门狗定时器
    P1DIR |= LED1; // LED1设为输出
    P1REN |= BUTTON; // P1.3设为上拉电阻
    P1OUT |= BUTTON; // P1.3设置为高电平

    P1IE |= BUTTON; // 开启 P1.3 的中断
    P1IES |= BUTTON; // P1.3 中断触发边沿为下降沿

    __enable_interrupt(); // 开启中断

    while (1);
}

I LOVE HUBU
/****************
 ****************
 ****************/
#include<msp430g2452.h>
int i,j,m,n;
int a[8]={0x40,0x80,0x20,0x10,0x08,0x04,0x02,0x01};
int b[35]={0x00,0x41,0x7F,0x41,0x00,0x00,0x0E,0x1F,
           0x3F,0x7E,0x3F,0x1F,0x0E,0x00,0x00,0x7F,
           0x08,0x08,0x7F,0x00,0x3F,0x40,0x40,0x3F,
	   0x00,0x7F,0x49,0x49,0x36,0x00,0x3F,0x40,
           0x40,0x3F,0x00};
int c[8]={0};
void aaa(int x){
	for(i=0;i<8;i++,x++){
			c[i] = b[x%35];
		}	
}
void abc(int j){
	for(n=0;n<20;n++){
		for(m=0;m<8;m++){
			P1DIR = 0x00;
			P2DIR = 0x00;
			P1DIR |= c[m];
			P1OUT = 0X00;
			P2DIR |= a[m];
			P2SEL = 0X00;
			P2OUT |= a[m];
			__delay_cycles(1000);
		}
	}
}
void main(){
	WDTCTL = WDTPW + WDTHOLD;
	while(1){
		for(j=0;j<35;j++){
			aaa(j);
			abc(j);
		}
	}
}


两只老虎
#include "MSP430G2553.h"
void TA0_init(int n);     
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   
    BCSCTL1 = CALBC1_1MHZ;             
    DCOCTL = CALDCO_1MHZ;
    int i=0,c;
    int song[]={1,2,3,1,0,1,2,3,1,0,3,4,5,0,3,4,5,0,6,4,3,1,0,6,4,3,1,0,2,5,1,0,2,5,1};

    while(1){
      for(i=0;i<39;i++)
      {c=song[i];
      switch(c)
      {  case 7 :TA0_init(262);
        __delay_cycles(500000);break;
         case 6 :TA0_init(294);
        __delay_cycles(500000);break;
         case 5 :TA0_init(330);
        __delay_cycles(500000);break;
         case 4 :TA0_init(349);
        __delay_cycles(500000);break;
         case 3 :TA0_init(392);
        __delay_cycles(500000);break;
         case 2 :TA0_init(440);
        __delay_cycles(500000);break;
         case 1 :TA0_init(494);
        __delay_cycles(500000);break;
      default:__delay_cycles(500000);break;
      }}
    }}

void TA0_init(int n)
{
    P2DIR |= BIT0;                              
    P2SEL |= BIT0;
    P2SEL2&=~BIT0;
    TA1CCR0 = n;                          
    TA1CCTL0 = OUTMOD_4;               
    TA1CCR1 = n/2;                             
    TA1CTL = TASSEL_2 + MC_1;  
}
汇总
{
#include "MSP430G2553.h"
void TA0_init(int n)
{
    P2DIR |= BIT0;                              
    P2SEL |= BIT0;
    P2SEL2&=~BIT0;
    TA1CCR0 = n;                          
    TA1CCTL0 = OUTMOD_4;               
    TA1CCR1 = n/2;                             
    TA1CTL = TASSEL_2 + MC_1;  
}

int i,j,m,n,k,cnt=0;
int a[8]={0x40,0x80,0x20,0x10,0x08,0x04,0x02,0x01};
int b[35]={0x00,0x41,0x7F,0x41,0x00,0x00,0x0E,0x1F,
           0x3F,0x7E,0x3F,0x1F,0x0E,0x00,0x00,0x7F,
           0x08,0x08,0x7F,0x00,0x3F,0x40,0x40,0x3F,
	   0x00,0x7F,0x49,0x49,0x36,0x00,0x3F,0x40,
           0x40,0x3F,0x00};
int c[8]={0};
void aaa(int x){
	for(i=0;i<8;i++,x++){
			c[i] = b[x%35];
		}	
}
void abc(int j){
	for(n=0;n<20;n++){
		for(m=0;m<8;m++){
			P1DIR = 0x00;
			P2DIR = 0x00;
			P1DIR |= c[m];
			P1OUT = 0X00;
			P2DIR |= a[m];
			P2SEL = 0X00;
			P2OUT |= a[m];
			__delay_cycles(1000);
		}
	}
}
void main(){
    WDTCTL = WDTPW + WDTHOLD;
    TA0CCR0 = 50000; // 定时器比较值为 1000，即 1ms
    TA0CTL = TASSEL_2 + ID_3 + MC_1 + TACLR; // 选择 SMCLK 作为时钟源，分频系数为 8，选择上升沿计数模式，清除计数器
    TA0CCTL0 = CCIE; // 开启定时器中断

    __enable_interrupt(); // 开启中断
    int i=0;
    int song[]={1,2,3,1,0,1,2,3,1,0,3,4,5,0,3,4,5,0,6,4,3,1,0,6,4,3,1,0,2,5,1,0,2,5,1};

    while(1){
      if(k==40) k=0;
      if(cnt==400) cnt=0;
      if(cnt%5==0)
      switch(song[k])
      {  case 7 :TA0_init(262);k++;
        break;
         case 6 :TA0_init(294);k++;
        break;
         case 5 :TA0_init(330);k++;
        break;
         case 4 :TA0_init(349);k++;
        break;
         case 3 :TA0_init(392);k++;
        break;
         case 2 :TA0_init(440);k++;
        break;
         case 1 :TA0_init(494);k++;
        break;
      default:break;
      }
      }
	for(j=0;j<35;j++)
        {
		aaa(j);
		abc(j);
	}
}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A_ISR() {
cnt++;
}
}


//点阵蜂鸣器
#include <msp430g2553.h>
int i,j,x,cnt1,cnt,xlong;
int timer0_count1,timer0_count2;
int timer0_count3,timer1_count2,flag;
int LED[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
int LED_P2_high[7]={0x80,0x40,0x20,0x10,0x08,0x04,0x02};
int LED_model[39]={0x00,0x81,0xFF,0x81,0x00,0x00,0x1E,0x3F,0x7F,0xFE,0xFE,0x7F,0x3F,0x1E,0x00,0x00,
0xFF,0x08,0x08,0xFF,0x00,0x00,0x7F,0x80,0x80,0x7F,0x00,0x00,0xFF,0x99,0x99,0x66,
0x00,0x00,0x7F,0x80,0x80,0x7F,0x00,};

/*int LED_model[39]={0x00,0x41,0x7F,0x41,0x00,0x00,0x0E,0x1F,
           0x3F,0x7E,0x3F,0x1F,0x0E,0x00,0x00,0x7F,
           0x08,0x08,0x7F,0x00,0x3F,0x40,0x40,0x3F,
            0x00,0x7F,0x49,0x49,0x36,0x00,0x3F,0x40,
           0x40,0x3F,0x00,0x00,0x00,0x00,0x00};*/
int LED_P1_low[8]={0};      
unsigned int music_tone[7]={1912,1703,1517,1432,1275,1136,1012,};//do re mi fa so la si 
unsigned int music_num[42]={
1,1,5,5,6,6,5,4,4,3,3,2,2,1,
5,5,4,4,3,3,2,5,5,4,4,3,3,2,
1,1,5,5,6,6,5,4,4,3,3,2,2,1
};
unsigned int music_long[42]={
1,1,1,1,1,1,2,1,1,1,1,1,1,2,
1,1,1,1,1,1,2,1,1,1,1,1,1,2,
1,1,1,1,1,1,2,1,1,1,1,1,1,2
};
void CLK_clock(void){
   BCSCTL1 = CALBC1_1MHZ;     // 设置 DCO 时钟为 1MHz
   DCOCTL = CALDCO_1MHZ;
}
void LED_init(void){
  P2DIR |= 0XFE;
  P1DIR |= 0XFF;
  P2SEL &= ~0XFE;
}
void LED_check(void){
  for(i=0;i<8;i++){
    for(j=0;j<8;j++){
      P1DIR |= LED[i];
      P1OUT &= ~LED[i];
      P2SEL = 0x00;
      P2DIR |= LED[j];
      P2OUT |= LED[j];
      __delay_cycles(10000);
      P2OUT=0x00;
      P1DIR=0x00;
      P2DIR=0x00;
    }
  }
}
void TA0_1_led_init(void){
  TA0CTL = TASSEL_2 + ID_0 + MC_1;  // 选择 SMCLK 作为计时器时钟，不分频，连续计数模式
  TA0CCR0 = 5000 - 1;               // 定时器计数值为 50000，对应 50ms 的中断间隔
  TA0CCR1 = 1000 - 1;               // 定时器计数值为 1000，对应 1ms 的中断间隔
  TA0CCTL1 = CCIE; 
}
void BEEP_init(void){
  P2DIR |= BIT0;
  P2SEL |= BIT0;  // 将P2.0管脚设置为第二功能
  P2DIR |= BIT0;  // 将P2.0管脚设置为输出  
}
void TA1_beep_init(void){
  TA1CTL |= TASSEL_2 + MC_1;  // 选择SMCLK作为时钟源，选择增计数模式
  TA1CCTL0 = OUTMOD_4;        // PWM output 
  TA1CCR0 = 0;                // 预设CCR0寄存器值为0
}
void BEEP_check(void){
  for(i=0;i<7;i++){
    P2DIR |= BIT0;
    TA1CCR0 = music_tone[i]; 
    TA1CCR1 = music_tone[i]/2;
    __delay_cycles(10000);
    P2DIR &= ~BIT0;
    __delay_cycles(1000);
  }
}

void TA0_0_beep_init(void){
  TA0CTL = TASSEL_2 + ID_0 + MC_1;  // 选择 SMCLK 作为计时器时钟，不分频，连续计数模式
  TA0CCR0 = 5000 - 1; 
  TA0CCTL0 = CCIE; 
}

void beep_work(int x){
  BEEP_init();
  //TA1_beep_init();
  TA1CCR0 = music_tone[music_num[x]]; 
  TA1CCR1 = music_tone[music_num[x]]/2;
}

void main(void){
  WDTCTL = WDTPW + WDTHOLD;
  CLK_clock();
  LED_init();
  LED_check();//led点阵的自检
  BEEP_init();
  TA1_beep_init();
  BEEP_check();//蜂鸣器的自检
  //开始进行同时显示
  LED_init();
  TA0_1_led_init();
  BEEP_init();
  TA1_beep_init();
  TA0_0_beep_init();
  __enable_interrupt();  // 开启全局中断
  while(1){
    for(i=0;i<8;i++){
    P1DIR = 0x00;
    P2DIR &= ~(BIT2+BIT1+BIT3+BIT4+BIT5+BIT6+BIT7);
    P1DIR |= LED_P1_low[i];
    P1OUT = 0X00;
    P2DIR |= LED_P2_high[i];
    P2SEL = 0X01;
    P2OUT |= LED_P2_high[i];
   }
  }
}
#pragma vector = TIMER0_A0_VECTOR//蜂鸣器
__interrupt void Timer0(void) {
  timer0_count3++;
  if (music_long[cnt] == 1){
    xlong = 100;
    if (flag == 1){
      xlong = 10;
    }
  }else if (music_long[cnt] == 2){
    xlong = 160;
    if (flag == 1){
      xlong = 10;
    }
  }
  if(flag == 0){
    beep_work(cnt);
    if(timer0_count3 >= xlong){
        flag = 1;
        cnt++;
        if (cnt == 43){
          cnt = 0;
        }
        timer0_count3 = 0;
    }
  }else if (flag == 1){
    P2DIR &= ~BIT0;
    if (timer0_count3 >= xlong){
       flag = 0;
       timer0_count3 = 0;
    }
  }
}
#pragma vector = TIMER0_A1_VECTOR//点阵
__interrupt void Timer_A(void) {
  timer0_count2++;
  if(timer0_count1 == 39){
    timer0_count1 = 0;
  }
  if(timer0_count2 == 40){//当同一个画面刷新60次，更换下一个
    timer0_count2 = 0;
    x = timer0_count1;
    for(i=0;i<8;i++,x++){
      LED_P1_low[i] = LED_model[x%39];
    }
    timer0_count1++;
  }
  
   TA0CCTL1 &= ~CCIFG;
}