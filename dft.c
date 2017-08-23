#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <complex.h>

#define M_PI 3.14159265358979323846

#define DFT_SIZE 1024
#define DECIMATION (DFT_SIZE/200)

complex float InputData[DFT_SIZE];
complex float OutputData[DFT_SIZE];
complex float DecimatedInputData[DFT_SIZE/DECIMATION];
complex float DecimatedOutputData[DFT_SIZE/DECIMATION];

void clear (void);
void plot (complex float* data, size_t size, float scale, int height);
void plotabs (complex float* data, size_t size, float scale2, int height);
void dft (complex float* in, complex float* out, int size);
void rdft (complex float* in, complex float* out, int size);
void decimate (complex float* in, complex float* out, int size, int decimation);
float maxabs (complex float* data, int size);

int main (void)
{
    clear();

    for (int x = 0; x < DFT_SIZE; x++)
        InputData[x] = cosf((x-DFT_SIZE/2) * 1/16.) + sinf((x-DFT_SIZE/2) * 3/16.);

    decimate(InputData, DecimatedInputData, DFT_SIZE, DECIMATION);
    plot(DecimatedInputData, DFT_SIZE/DECIMATION, 4, 20);
    rdft(InputData, OutputData, DFT_SIZE);
    decimate(OutputData, DecimatedOutputData, DFT_SIZE, DECIMATION);
    plotabs(DecimatedOutputData, DFT_SIZE/DECIMATION, 0.05, 20);

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

float maxabs (complex float* data, int size)
{
    float m = 0;
    for (int x = 0; x < size; x++)
    {
        if (fabs(__real__ data[x]) > m)
            m = fabs(__real__ data[x]);
        if (fabs(__imag__ data[x]) > m)
            m = fabs(__imag__ data[x]);
    }
    return m;
}

void decimate (complex float* in, complex float* out, int size, int decimation)
{
    for (int x = 0; x < size/decimation; x++)
        out[x] = 0;

    for (int x = 0; x < size; x++)
        out[x/decimation] += in[x];

    for (int x = 0; x < size/decimation; x++)
        out[x] /= decimation;
}

void plotabs (complex float* data, size_t size, float scale2, int height)
{
    float scale = height / maxabs(data, size);

    /* x-axis */
    printf("\e[s");
    printf("\e[%dB", height);
    for (int x = 0; x < size/2; x++)
        printf("-");
    printf("\e[u");

    /* y-axis */
    printf("\e[s");
    for (int y = 0; y < height; y++)
        printf("|\e[B\e[D");
    printf("\e[u");

    /* data */
    printf("\e[41;1m");
    for (int x = size/2; x < size; x++)
    {
        printf("\e[s");
        int y = roundf(cabsf(data[x]) * scale);
        if (y >= 0)
        {
            printf("\e[%dB", height);
            if (x > size/2) printf("\e[%dC", x - size/2);
            for (int n = 0; n <= y; n++)
            {
                printf(" ");
                printf("\e[A\e[D");
            }
        }
        printf("\e[u");
    }
    printf("\e[0m");

    printf("\e[%dB", height);
    printf("\r\n\r\n");
}

void plot (complex float* data, size_t size, float scale, int height)
{
    /* x-axis */
    printf("\e[s");
    printf("\e[%dB", height/2);
    for (int x = 0; x < size; x++)
        printf("-");
    printf("\e[u");

    /* y-axis */
    printf("\e[s");
    printf("\e[%dC", size/2);
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
    printf("\r\n\r\n");
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
            out[k] += in[x] * cexpf(- 1i * 2 * M_PI * (k-DFT_SIZE/2) * (x-DFT_SIZE/2) / size);
    }

    for (int k = 0; k < size/2; k++)
        out[k] = out[size - k];
}
