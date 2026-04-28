#include "stm32h743xx.h"
#include "arm_math.h"
#include "ADC_DMA_TIM.h"
#include "FFT_Calculate.h"
#include "hannig_window.h"
#define ADC_MID_VALUE 32768.0f

__attribute__((aligned(32))) float fft_input_buffer[ADC_BUFFER_SIZE];
__attribute__((aligned(32))) float fft_output_buffer[ADC_BUFFER_SIZE];
__attribute__((aligned(32))) float fft_magnitude[ADC_BUFFER_SIZE/2+1];
static const float window_amplitude_correction = 2.0f;
static arm_rfft_fast_instance_f32 rfft_inst;
Peak_t top_peaks[peaks_count];


void fft_init(void)
{
    arm_rfft_fast_init_f32(&rfft_inst, ADC_BUFFER_SIZE);
}

void calculate_fft(void)
{
    for(uint16_t i=0; i<ADC_BUFFER_SIZE; i++)
    {
        fft_input_buffer[i] = ((float)adc_buffer[i] - ADC_MID_VALUE) * window_buffer[i];
    }

    // 实数 FFT。注意:CMSIS 不允许 in/out 同一缓冲区
    arm_rfft_fast_f32(&rfft_inst, fft_input_buffer, fft_output_buffer, 0);
/*
     arm_rfft_fast_f32 输出打包格式(forward,长度 N):
       out[0]       = X[0].real    (DC,纯实数)
       out[1]       = X[N/2].real  (Nyquist,纯实数,被塞到了 [1])
       out[2k]      = X[k].real    k = 1..N/2-1
       out[2k+1]    = X[k].imag    k = 1..N/2-1
*/
    // 拆包计算幅值
    fft_magnitude[0] = fabsf(fft_output_buffer[0]);   // DC
    fft_magnitude[ADC_BUFFER_SIZE/2] = fabsf(fft_output_buffer[1]);   // Nyquist
    arm_cmplx_mag_f32(&fft_output_buffer[2], &fft_magnitude[1], ADC_BUFFER_SIZE/2 - 1);
    WIN_power_correction(fft_magnitude);
}

void WIN_power_correction(float *fft_magnitude) //修正因子修正
{
    fft_magnitude[ADC_BUFFER_SIZE/2] = fft_magnitude[ADC_BUFFER_SIZE/2] / ADC_BUFFER_SIZE * window_amplitude_correction;
    for(uint16_t i=0; i<ADC_BUFFER_SIZE/2; i++)
    {
        if(i == 0)
        {
            fft_magnitude[i] = fft_magnitude[i] / ADC_BUFFER_SIZE * window_amplitude_correction;
        }
        else
        {
            fft_magnitude[i] = fft_magnitude[i] * 2.0f / ADC_BUFFER_SIZE * window_amplitude_correction;
        }
    }
}


void find_top_peaks_arm(float *fft_magnitude, uint8_t topN, Peak_t peaks[])
{
    if (topN == 0) return;

    // 初始化 topN 榜单 (按 magnitude 降序排列,空位用 -1 占位)
    for (uint8_t i = 0; i < topN; i++)
    {
        peaks[i].magnitude = -1.0f;
        peaks[i].index     = 0;
    }

    // 单次遍历,只考察局部极大值,自动跳过 DC (bin 0) 和 Nyquist (bin N/2)
    const uint16_t last = ADC_BUFFER_SIZE / 2;   // Nyquist 索引,循环不含
    for (uint16_t i = 1; i < last; i++)
    {
        float v = fft_magnitude[i];

        // 局部极大值检查: 比左右两邻都大
        if (v <= fft_magnitude[i - 1]) continue;
        if (v <= fft_magnitude[i + 1]) continue;

        // 比榜单末位还小,直接跳过
        if (v <= peaks[topN - 1].magnitude) continue;

        // 插入排序: 找到插入点,后续元素整体后移一格
        uint8_t j = topN - 1;
        while (j > 0 && v > peaks[j - 1].magnitude)
        {
            peaks[j] = peaks[j - 1];
            j--;
        }
        peaks[j].magnitude = v;
        peaks[j].index     = i;
    }
}

