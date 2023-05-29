
#define __riscv__
#define LED_DELAY 1000000
#define ACCEL_DATA_STR_LEN 32

// #include <spi.h>
#include <gpio.h>
#include <uart.h>
// #include <utils.h>
#include <pulpino.h>
#include "spi_accel.h"

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

int accel_data_to_string(struct spi_accel_data accel, char *str)
{
  int i;
  int str_least, str_cntr = 0;
  int hex_digit;
  // extract data X
  for (i = 0; i < DATA_HEX_NUM; i++)
  {
    insert_char(&accel.data_x, str, str_cntr);
    str_cntr++;
    if (str_cntr > ACCEL_DATA_STR_LEN)
    {
      return -1;
    }
  }
  reverse_str(&str[str_cntr - DATA_HEX_NUM]);
  str[str_cntr] = ' ';

  str_cntr++;
  if (str_cntr > ACCEL_DATA_STR_LEN)
  {
    return -1;
  }
  // extract data Y
  for (i = 0; i < DATA_HEX_NUM; i++)
  {
    insert_char(&accel.data_y, str, str_cntr);
    str_cntr++;
    if (str_cntr > ACCEL_DATA_STR_LEN)
    {
      return -1;
    }
  }
  reverse_str(&str[str_cntr - DATA_HEX_NUM]);
  str[str_cntr] = ' ';

  str_cntr++;
  if (str_cntr > ACCEL_DATA_STR_LEN)
  {
    return -1;
  }
  // extract data Z
  for (i = 0; i < DATA_HEX_NUM; i++)
  {
    insert_char(&accel.data_z, str, str_cntr);
    str_cntr++;
    if (str_cntr > ACCEL_DATA_STR_LEN)
    {
      return -1;
    }
  }
  reverse_str(&str[str_cntr - DATA_HEX_NUM]);
  str[str_cntr] = ' ';

  str_cntr++;
  if (str_cntr > ACCEL_DATA_STR_LEN)
  {
    return -1;
  }

  str_least = ACCEL_DATA_STR_LEN - str_cntr - 2; // for '\n' and '\0'
  for (i = 0; i < str_least; i++)
  {
    str[str_cntr] = 't';
    str_cntr++;
    if (str_cntr > ACCEL_DATA_STR_LEN)
    {
      return -1;
    }
  }
  str[str_cntr] = '\n';
  str[str_cntr + 1] = '\0';
  return 0;
}

int main()
{
  struct spi_accel_data accel_data;
  char accel_data_uart[ACCEL_DATA_STR_LEN] = {0};
  int ret;

  uart_set_cfg(0, 325); // 9600 baud UART, no parity (50MHz CPU)

  uart_send("Hello world!\n", 13); // 13 is a number of chars sent: 12 + "\n"
  uart_wait_tx_done();

  set_pin_function(31, FUNC_GPIO);
  set_gpio_pin_direction(31, DIR_OUT);

  set_gpio_pin_value(31, 0);

  spi_accel_init();

  while (1)
  {

    for (int i = 0; i < LED_DELAY; i++)
    {
// wait some time
#ifdef __riscv__
      asm volatile("nop");
#else
      asm volatile("l.nop");
#endif
    }

    set_gpio_pin_value(31, 1);

    for (int i = 0; i < LED_DELAY; i++)
    {
// wait some time
#ifdef __riscv__
      asm volatile("nop");
#else
      asm volatile("l.nop");
#endif
    }

    set_gpio_pin_value(31, 0);

    ret = spi_accel_get_data(&accel_data);
    if (ret != 0)
    {
      uart_send("Failed get accelerometer data!\n", 31);
      continue;
    }

    ret = accel_data_to_string(accel_data, accel_data_uart);
    if (ret == 0)
    {
      uart_send(accel_data_uart, ACCEL_DATA_STR_LEN);
    }
    else
    {
      uart_send("Failed convert accelerometer data to string!\n", 45);
    }
  }
}
