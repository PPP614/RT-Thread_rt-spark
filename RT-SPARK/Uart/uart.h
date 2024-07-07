/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-06-15     彭竣       the first version
 */
#ifndef UART_UART_H_
#define UART_UART_H_


static rt_err_t uart_rx_ind(rt_device_t dev, rt_size_t size);
static char uart_sample_get_char(void);
void data_parsing(void);
void uart_data_send(char str[]);
void uart_init(void);

#endif /* UART_UART_H_ */
