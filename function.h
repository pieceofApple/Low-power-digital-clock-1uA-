/*延时函数 1ms*/
void delay_ms(int time_ms)
{
    for(int j=time_ms;j>0;j--)
        __delay_cycles(1000);
}
void debounce_delay(unsigned int delay) 
{
    volatile unsigned int j;
    for (j = 0; j < delay; j++);
}
/*the other way*/
/*
void delay_ms(int time_ms)
{
    int i=time_ms*1000; //自行调整，可以通过示波器测量
    while(i--);
}
*/
