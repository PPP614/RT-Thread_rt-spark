#include <rtthread.h>
#include <rtdevice.h>
#include <string.h>  // for strlen, strncpy, strchr
#include <stdlib.h>  // for atof

#define SAMPLE_UART_NAME                 "uart2"
#define DATA_CMD_END                     '\r'       /* 结束位设置为 \r，即回车符 */
#define ONE_DATA_MAXLEN                  50         /* 不定长数据的最大长度 */

/* 全局变量用于存储解析后的数据 */
char uart_flag[5]; // 假设单词最大长度为 5，加上 null 结尾符
float probability;

/* 用于接收消息的信号量 */
static struct rt_semaphore rx_sem;
static rt_device_t serial;
static rt_thread_t serial_tid;
static rt_mq_t uart2_rx_mq;
char buf[64];

struct mq_msg {
    rt_device_t dev;
    rt_size_t size;
};

/* 接收数据回调函数 */
static rt_err_t uart_rx_ind(rt_device_t dev, rt_size_t size)
{
    struct mq_msg msg;
    msg.dev = dev;
    msg.size = size;
    rt_mq_send(uart2_rx_mq, &msg, sizeof(msg));
    return RT_EOK;
}

/* 数据解析线程 */
void serial_thread_entry(void *parameter)
{
    struct mq_msg msg;
    static char received_data[128];
    static int data_index = 0;
    rt_size_t rx_len;

    while (1) {
        rt_memset(&msg, 0, sizeof(msg));
        rt_memset(buf, 0, sizeof(buf));

        if (rt_mq_recv(uart2_rx_mq, &msg, sizeof(msg), RT_WAITING_FOREVER) == RT_EOK) {
            rx_len = rt_device_read(msg.dev, 0, buf, sizeof(buf));
            rt_device_write(serial, 0, buf, rx_len);
            rt_kprintf("rx_len = %d, content: %s\n", rx_len, buf);

            // 将接收到的数据添加到 received_data 中
            strncpy(received_data + data_index, buf, rx_len);
            data_index += rx_len;

            // 检查 received_data 中是否包含完整的消息
            char *start = strchr(received_data, '<');
            char *end = strchr(received_data, '>');

            if (start && end && (start < end)) {
                // 提取 < 和 * 之间的字符串
                char *uart_flag_start = start + 1;
                char *uart_flag_end = strchr(uart_flag_start, '*');
                if (uart_flag_end) {
                    int uart_flag_len = uart_flag_end - uart_flag_start;
                    strncpy(uart_flag, uart_flag_start, uart_flag_len);
                    uart_flag[uart_flag_len] = '\0';
                    rt_kprintf("uart_flag = %s\n", uart_flag);
                }

                // 提取 * 和 > 之间的浮点数
                char *prob_start = uart_flag_end + 1;
                char *prob_end = strchr(prob_start, '%');
                if (prob_end) {
                    char prob_str[20]; // 假设概率字符串不超过 20 个字符
                    int prob_len = prob_end - prob_start;
                    strncpy(prob_str, prob_start, prob_len);
                    prob_str[prob_len] = '\0';
                    probability = atof(prob_str) / 100.0; // 转换成浮点数
                    rt_kprintf("probability = %f\n", probability);
                }

                // 重置 received_data 和 data_index，准备接收下一条消息
                rt_memset(received_data, 0, sizeof(received_data));
                data_index = 0;
            }
        }
    }
}

/* 初始化串口并发送数据 */
void uart_init(void)
{
    rt_err_t ret = RT_EOK;
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;

    // 查找系统中的串口设备
    serial = rt_device_find(SAMPLE_UART_NAME);
    if (!serial) {
        rt_kprintf("找不到 %s 设备!\n", SAMPLE_UART_NAME);
        return;
    }

    // 配置串口参数
    config.baud_rate = 115200;
    config.data_bits = DATA_BITS_8;
    config.stop_bits = STOP_BITS_1;
    config.bufsz     = 80;
    config.parity    = PARITY_NONE;

    // 应用配置到串口设备
    rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);

    // 初始化用于同步的信号量
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);

    // 以中断接收模式打开串口设备
    rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);

    // 设置接收回调函数
    rt_device_set_rx_indicate(serial, uart_rx_ind);

    // 创建消息队列用于串口接收数据
    uart2_rx_mq = rt_mq_create("uart_mq", 128, 8, RT_IPC_FLAG_FIFO);

    // 创建线程处理串口数据
    serial_tid = rt_thread_create("serial", serial_thread_entry, RT_NULL, 1024, 20, 10);

    // 启动串口数据处理线程
    rt_thread_startup(serial_tid);
}

void uart_data_send(const char *str)
{
   //  检查串口设备是否已初始化
    if (!serial) {
        rt_kprintf("UART设备未初始化!\n");
        return;
    }

    // 通过串口发送字符串
    rt_device_write(serial, 0, str, strlen(str));
}
