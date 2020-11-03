
#include "./bsp_eval.h"

int itoa_s(int value, char *buf)
{
        int index = 0;
        int i = value % 10;
        if (value >= 10) {
                index += itoa_s(value / 10, buf);
        }
        buf[index] = i+0x30;
        index++;
        return index;
}

char *itoa(int value, char *buf)
{
        int len = itoa_s(value, buf);
        buf[len] = '\0';
        return buf;
}

char *ftoa(float value, int decimals, char *buf)
{
        int index = 0;
        if (value < 0) {
                buf[index] = '-';
                index++;
                value = -value;
        }
        float rounding = 0.5;
        for (int d = 0; d < decimals; rounding /= 10.0, d++);
        value += rounding;
        index += itoa_s((int)(value), buf+index);
        buf[index++] = '.';
        value = value - (int)(value);
        int ival = 1;
        for (int d = 0; d < decimals; ival *= 10, d++);
        ival *= value;
        index += itoa_s(ival, buf+index);
        buf[index] = '\0';
        return buf;
}