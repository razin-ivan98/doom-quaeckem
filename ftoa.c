

// #include "unistd.h"
// #include "stdio.h"
// #include "stdlib.h"
// #include <string.h>

#include "./duke.h"

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

int    insert_zeros(int ival, int decimals, char *buf)
{
        int res;
        int i;

        res = 0;
        while (ival)
        {
                ival /= 10;
                res++;
        }
        res = decimals - res;
                // printf("res\t%d\n\n", res);

        i = 0;
        ft_strcpy(buf, "");
        while (i < res)
        {
                ft_strcat(buf, "0");
                i++;
        }
        return (res);
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

        // printf("ival\t%d\n\n", ival);

        index += insert_zeros(ival, decimals, buf+index);
        index += itoa_s(ival, buf+index);
        buf[index] = '\0';
        return buf;
}


// int main(int ac, char **av)
// {
//         float a = -0.9545609;
//         float b = 0.9545609;
//         float c = -0.01;
//         float d = -0.00765;

//         char lol[60];

//         puts(ftoa(a, atoi(av[1]), lol));
//         puts(ftoa(b, atoi(av[1]), lol));
//         puts(ftoa(c, atoi(av[1]), lol));
//         puts(ftoa(d, atoi(av[1]), lol));

//         return 0;
// }