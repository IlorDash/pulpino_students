
#define __riscv__
#define LED_DELAY 100000000
#define ACCEL_DATA_STR_LEN 32

// #include <spi.h>
#include <gpio.h>
#include <uart.h>
#include <stdint.h>
#include <string_lib.h>
// #include <utils.h>
#include <pulpino.h>
#include "spi_accel.h"

int intToString(int n, char *buffer)
{
  int i = 0;

  int isNeg = (n < 0) ? 1 : 0;

  unsigned int n1 = isNeg ? -n : n;

  while (n1 != 0)
  {
    buffer[i++] = n1 % 10 + '0';
    n1 = n1 / 10;
  }

  if (isNeg)
  {
    buffer[i++] = '-';
  }

  buffer[i] = '\0';

  for (int t = 0; t < i / 2; t++)
  {
    buffer[t] ^= buffer[i - t - 1];
    buffer[i - t - 1] ^= buffer[t];
    buffer[t] ^= buffer[i - t - 1];
  }

  if (n == 0)
  {
    buffer[0] = '0';
    buffer[1] = '\0';
    i = 1;
  }
  return i;
}

char int_to_hex(int num)
{
  switch (num)
  {
  case 10:
    return 'A';
    break;
  case 11:
    return 'B';
    break;
  case 12:
    return 'C';
    break;
  case 13:
    return 'D';
    break;
  case 14:
    return 'E';
    break;
  case 15:
    return 'F';
    break;

  default:
    return ' ';
    break;
  }
}

void insert_char(int *data, char *str, const int str_cntr)
{
  int hex_digit = *data & 0x0000000f;
  if (hex_digit > 9)
  {
    str[str_cntr] = int_to_hex(hex_digit);
  }
  else
  {
    str[str_cntr] = hex_digit + '0';
  }
  *data = *data >> 4;
}

void reverse_str(char *str)
{
  int i;
  char tmp;
  for (i = 0; i < (DATA_HEX_NUM / 2); i++)
  {
    tmp = str[i];
    str[i] = str[DATA_HEX_NUM - i - 1];
    str[DATA_HEX_NUM - i - 1] = tmp;
  }
}

int accel_data_to_string(struct spi_accel_regs accel, char *str)
{
  int i;
  int str_least, str_cntr = 0;
  int hex_digit;

  str[str_cntr] = 'X';
  str_cntr++;
  str[str_cntr] = ':';
  str_cntr++;
  str[str_cntr] = ' ';
  str_cntr++;
  str[str_cntr] = '0';
  str_cntr++;
  str[str_cntr] = 'x';
  str_cntr++;

  // extract data X
  for (i = 0; i < DATA_HEX_NUM; i++)
  {
    insert_char(&accel.data_x, str, str_cntr);
    str_cntr++;
  }
  reverse_str(&str[str_cntr - DATA_HEX_NUM]);
  str[str_cntr] = ' ';
  str_cntr++;

  str[str_cntr] = 'Y';
  str_cntr++;
  str[str_cntr] = ':';
  str_cntr++;
  str[str_cntr] = ' ';
  str_cntr++;
  str[str_cntr] = '0';
  str_cntr++;
  str[str_cntr] = 'x';
  str_cntr++;

  // extract data Y
  for (i = 0; i < DATA_HEX_NUM; i++)
  {
    insert_char(&accel.data_y, str, str_cntr);
    str_cntr++;
  }
  reverse_str(&str[str_cntr - DATA_HEX_NUM]);
  str[str_cntr] = ' ';
  str_cntr++;

  str[str_cntr] = 'Z';
  str_cntr++;
  str[str_cntr] = ':';
  str_cntr++;
  str[str_cntr] = ' ';
  str_cntr++;
  str[str_cntr] = '0';
  str_cntr++;
  str[str_cntr] = 'x';
  str_cntr++;

  // extract data Z
  for (i = 0; i < DATA_HEX_NUM; i++)
  {
    insert_char(&accel.data_z, str, str_cntr);
    str_cntr++;
  }
  reverse_str(&str[str_cntr - DATA_HEX_NUM]);
  str[str_cntr] = ' ';

  str_cntr++;
  if (str_cntr > ACCEL_DATA_STR_LEN)
  {
    return -1;
  }

  str[str_cntr] = '\n';
  str[str_cntr + 1] = '\0';
  return 0;
}

int main()
{
  struct spi_accel_regs accel_data_regs;
  char accel_data_uart[ACCEL_DATA_STR_LEN] = {0};
  int ret;
  uint16_t raw_accel_data;
  struct spi_accel_real_data accel_data;
  uint8_t data_uart_pos;

  uart_set_cfg(0, 325); // 9600 baud UART, no parity (50MHz CPU)

  uart_send("Hello world!\n", 13); // 13 is a number of chars sent: 12 + "\n"
  uart_wait_tx_done();

  set_pin_function(31, FUNC_GPIO);
  set_gpio_pin_direction(31, DIR_OUT);

  set_gpio_pin_value(31, 0);

  spi_accel_init();

  int i = 0;

  while (1)
  {
    set_gpio_pin_value(31, 0);
    for (int j = 0; j < LED_DELAY; j++)
    {
// wait some time
#ifdef __riscv__
      asm volatile("nop");
#else
      asm volatile("l.nop");
#endif
    }
    set_gpio_pin_value(31, 1);

    ret = spi_accel_get_data(&accel_data_regs);
    if (ret != 0)
    {
      uart_send("Failed get accelerometer data!\n", 31);
      continue;
    }

    if (i % 100)
    {
      /* Send Accelerometer RAW data in HEX to UART */
      // ret = accel_data_to_string(accel_data_regs, accel_data_uart);
      // if (ret == 0)
      // {
      //   uart_send(accel_data_uart, ACCEL_DATA_STR_LEN);
      // }
      // else
      // {
      //   uart_send("Failed convert accelerometer data to string!\n", 45);
      // }

      /* Send Accelerometer recalculated data to UART */
      raw_accel_data = (int16_t)(accel_data_regs.data_x & 0xffff);
      accel_data.x = raw_accel_data / (1000 / (RANGE / 2));

      raw_accel_data = (int16_t)(accel_data_regs.data_y & 0xffff);
      accel_data.y = raw_accel_data / (1000 / (RANGE / 2));

      raw_accel_data = (int16_t)(accel_data_regs.data_z & 0xffff);
      accel_data.z = raw_accel_data / (1000 / (RANGE / 2));

      strcpy(accel_data_uart, "X: ");
      data_uart_pos = 3;
      data_uart_pos += intToString(accel_data.x, &accel_data_uart[data_uart_pos]);
      strcpy(&accel_data_uart[data_uart_pos], " Y: ");
      data_uart_pos += 4;
      data_uart_pos += intToString(accel_data.y, &accel_data_uart[data_uart_pos]);
      strcpy(&accel_data_uart[data_uart_pos], " Z: ");
      data_uart_pos += 4;
      intToString(accel_data.z, &accel_data_uart[data_uart_pos]);
      uart_send(accel_data_uart, ACCEL_DATA_STR_LEN);
    }

    i++;
  }
}
