#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <./Key/key.h>

#define PIN_KEY_LEFT        GET_PIN(C, 0)   // PC0:  KEY0         --> KEY
#define PIN_KEY_DOWN        GET_PIN(C, 1)   // PC1 :  KEY1         --> KEY
#define PIN_KEY_RIGHT       GET_PIN(C, 4)   // PC4 :  KEY2         --> KEY
#define PIN_KEY_UP          GET_PIN(C, 5)   // PC5:  WK_UP        --> KEY

uint16_t sele = 0;

static uint8_t button_read(void)
{
    static uint8_t last_button_state = 0; // 记录上次读取的按键状态
    static uint32_t last_button_time = 0; // 记录上次按键状态改变的时间
    uint8_t current_button_state = 0;
    uint32_t current_time = rt_tick_get(); // 获取当前系统时间，这里假设使用了rt-thread的tick函数

    // 检测每个按键的状态
    if (rt_pin_read(PIN_KEY_LEFT) == PIN_LOW) {
        current_button_state = 1;
    } else if (rt_pin_read(PIN_KEY_RIGHT) == PIN_LOW) {
        current_button_state = 2;
    } else if (rt_pin_read(PIN_KEY_UP) == PIN_LOW) {
        current_button_state = 3;
    } else if (rt_pin_read(PIN_KEY_DOWN) == PIN_LOW) {
        current_button_state = 4;
    }

    // 如果当前按键状态与上次不同，并且距离上次按键状态改变时间超过防抖时间（比如10毫秒），则认为按键有效
    if (current_button_state != last_button_state && (current_time - last_button_time) > 10) {
        last_button_state = current_button_state; // 更新上次按键状态
        last_button_time = current_time; // 更新上次按键状态改变的时间
        sele = current_button_state; // 更新全局变量
        return current_button_state; // 返回按键值
    }

    return 0; // 没有按键按下，返回0
}

static void button_thread_entry(void *parameter)
{
    while (1) {
        uint8_t button_value = button_read();
        if (button_value != 0) {
            rt_kprintf("Button %d pressed.\n", button_value);
            // 可以在这里处理按键被按下的逻辑
        }
        rt_thread_mdelay(20); // 每隔20毫秒检测一次按键状态
    }
}

void Btns_Init(void)
{
    rt_pin_mode(PIN_KEY_LEFT, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(PIN_KEY_RIGHT, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(PIN_KEY_DOWN, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(PIN_KEY_UP, PIN_MODE_INPUT_PULLUP);

    rt_thread_t thread = rt_thread_create("button_thread", button_thread_entry, RT_NULL, 1024, 28, 10);
    if (thread != RT_NULL) {
        rt_thread_startup(thread);
    } else {
        rt_kprintf("Failed to create button thread.\n");
    }
}


