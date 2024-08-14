#include <msp430.h>
void init_timer()
{
    // 配置定时器
    TA0CTL |= TASSEL_1 + MC_1; // 设置定时器时钟为ACLK，连续计数模式
    TA0CCTL0 = CCIE; // 使能比较中断
    TA0CCR0 = 1000; // 设置计数值为10000
}
void init_LPM_IO()
{
    P1DIR = 0x00; // 配置P1口为输入
    P1OUT = 0xFF; // 设置P1口上拉电阻
    P1REN = 0xFF-BIT6;

    P2DIR &=~ BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;// 配置P2口为输入
    P2OUT |= BIT0+BIT1+BIT2+BIT3+BIT4+BIT5; // 设置P2口上拉电阻
    P2REN |= BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;
    //DHT11 DATA数据引脚，电压1.5V，手动调零；
    P2DIR |= BIT5;
    P2OUT &=~ BIT5;

    P3DIR = 0x00; // 配置P3口为输入
    P3OUT = 0xFF; // 设置P3口上拉电阻
    P3REN = 0xFF;
}
void init_clock()
{
    BCSCTL1 &= ~XTS; // LFXTCLK 0:Low Freq
    BCSCTL2 |= SELM_3; // 选择 MCLK 时钟源为 LFXT1
    BCSCTL3 |= LFXT1S_0; // 选择 LFXT1 时钟源为 32768外部晶振
    BCSCTL1 |= DIVA_0; // 将 ACLK 分频为 1
}
void main(void)
{
    // 关闭所有外设和中断
    WDTCTL = WDTPW + WDTHOLD; // 关闭看门狗定时器

    init_clock();
    init_LPM_IO();
    init_timer();
        //P1DIR |=BIT7;
    // 进入LPM3模式
    __bis_SR_register(LPM3_bits+GIE);

    //while(1);
}

// 定时器中断服务函数
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void)
{
   // P1OUT ^=BIT7;
    // 在此处添加您的计数代码
}
