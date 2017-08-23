#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <complex.h>

#define M_PI 3.14159265358979323846

#define DFT_SIZE 128

complex float InputData[DFT_SIZE];
complex float OutputData[DFT_SIZE];
void clear (void);
void plot (complex float* data, size_t size, float scale, int height);
void plotabs (complex float* data, size_t size, float scale, int height);
void dft (complex float* in, complex float* out, int size);
void rdft (complex float* in, complex float* out, int size);

int main (void)
{
    clear();

    for (int x = 0; x < DFT_SIZE; x++)
        InputData[x] = cosf((x-DFT_SIZE/2) * 5/8.);

    plot(InputData, DFT_SIZE, 4., 30);
    rdft(InputData, OutputData, DFT_SIZE);
    plotabs(OutputData, DFT_SIZE, 0.25, 30);

    // for (int x = 0; x < DFT_SIZE; x++)
    //      printf("%f%+fi\r\n", __real__ OutputData[x], __imag__ OutputData[x]);

    return 0;
}

void clear (void)
{
    printf("\e[2J"); // clear screen
    printf("\e[3J"); // clear scrollback buffer
    printf("\e[H"); // reset cursor
}

void plotabs (complex float* data, size_t size, float scale, int height)
{
    /* x-axis */
    printf("\e[s");
    printf("\e[%dB", (int) roundf(height/2.));
    for (int x = 0; x < size; x++)
        printf("-");
    printf("\e[u");

    /* y-axis */
    printf("\e[s");
    printf("\e[%dC", size / 2);
    for (int y = 0; y < height; y++)
        printf("|\e[B\e[D");
    printf("\e[u");

    /* data */
    printf("\e[31;1m");
    for (int x = 0; x < size; x++)
    {
        printf("\e[s");
        int y = roundf(height/2 - cabsf(data[x]) * scale);
        if (y >= 0)
        {
            printf("\e[%dB", y);
            if (x > 0) printf("\e[%dC", x);
            printf("■");
        }
        printf("\e[u");
    }
    printf("\e[0m");

    printf("\e[%dB", height);
    printf("\r\n");
}

void plot (complex float* data, size_t size, float scale, int height)
{
    /* x-axis */
    printf("\e[s");
    printf("\e[%dB", (int) roundf(height/2.));
    for (int x = 0; x < size; x++)
        printf("-");
    printf("\e[u");

    /* y-axis */
    printf("\e[s");
    printf("\e[%dC", size / 2);
    for (int y = 0; y < height; y++)
        printf("|\e[B\e[D");
    printf("\e[u");

    /* data */
    printf("\e[36;1m");
    for (int x = 0; x < size; x++)
    {
        printf("\e[s");
        int y = roundf(height/2 - __real__ data[x] * scale);
        if (y >= 0)
        {
            printf("\e[%dB", y);
            if (x > 0) printf("\e[%dC", x);
            printf("■");
        }
        printf("\e[u");
    }
    printf("\e[32;1m");
    for (int x = 0; x < size; x++)
    {
        printf("\e[s");
        int y = roundf(height/2 - __imag__ data[x] * scale);
        if (y >= 0)
        {
            printf("\e[%dB", y);
            if (x > 0) printf("\e[%dC", x);
            printf("■");
        }
        printf("\e[u");
    }
    printf("\e[0m");

    printf("\e[%dB", height);
    printf("\r\n");
}

void dft (complex float* in, complex float* out, int size)
{
    for (int k = 0; k < size; k++)
    {
        out[k] = 0;
        for (int x = 0; x < size; x++)
            out[k] += in[x] * cexpf(1i * 2 * M_PI * (k-size/2) * (x-size/2) / size);
    }
}

void rdft (complex float* in, complex float* out, int size)
{
    for (int k = size/2; k < size; k++)
    {
        out[k] = 0;
        for (int x = 0; x < size; x++)
            out[k] += in[x] * cexpf(1i * 2 * M_PI * (k-DFT_SIZE/2) * (x-DFT_SIZE/2) / size);
    }

    for (int k = 0; k < size/2; k++)
        out[k] = out[size - k];
}
