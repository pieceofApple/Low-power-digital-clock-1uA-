/*消抖示例*/
if ((P1IN & BUTTON) == 0) { // 如果按键被按下
    debounce_delay(5000); // 延迟一段时间以等待按键稳定
    if ((P1IN & BUTTON) == 0) { // 如果按键仍被按下
        P1OUT ^= BIT0; // 翻转LED灯
        while ((P1IN & BUTTON) == 0); // 等待按键被松开
    }
}