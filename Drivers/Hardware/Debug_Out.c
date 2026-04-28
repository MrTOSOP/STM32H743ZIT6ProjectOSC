#include "stm32h743xx.h"
#include "stdio.h"
#include "ADC_DMA_TIM.h"
#include "FFT_Calculate.h"
#include "InputCaptrue.h"
#include "lcd_spi_154.h"
#include "arm_math.h"
#include "PIN_EXTI_Slove.h"
static const float fft_fuscalu = 976.5625f;
//static const float fft_magnitude_to_v = 0.025781643396658274f;
void Print_ADC_Value(void)//未经处理的原始数据
{
    for(uint16_t i=0;i<ADC_BUFFER_SIZE;i=i+2)//每2个点输出一个点为了减少printf带来的卡顿
    {
        printf("%f\n",adc_buffer[i]*3.3f/65535);
    }
}


void Print_out_FFT_fft_magnitude(void)
{
//    for(uint8_t i =0;i<128;i++)
//    {
//        printf("0\n");
//    }
    for(uint16_t i=0;i<=ADC_BUFFER_SIZE/2;i++)
    {
        printf("%f\n",fft_magnitude[i]*3.3f/65535);
    }
}

void Print_InputCaptrue_Freq_Duty(void)
{
    for(uint8_t i=1;i<128;i++)
    {
        printf("%f,%d",IC_GetFreq(),IC_GetDuty());
    }
}


void LCD_Show_Basic(void)
{
    LCD_DisplayString(0,0,"Freq =");
    LCD_DisplayString(00,25,"Duty =");
    LCD_DisplayString(0,50,"Magnitude");
    LCD_DisplayString(130,50,"FFT_Freq");
    LCD_DisplayString(0,180,"Flags :");
    LCD_DisplayString(0,200,"PF6:");
    LCD_DisplayString(160,200,"PF7:");
    
}


void LCD_Show_Freq_Duty(void)
{
    LCD_DisplayDecimals( 60,0, IC_GetFreq(), 9,1);
    LCD_DisplayNumber( 60,25, IC_GetDuty(), 3);
}

void LCD_Show_FFT_Freq_magnitude(void)
{
//fs = 240Mhz/(psc+1)/(arr+1)
//频率 = peaks[n].index × (fs / ADC_BUFFER_SIZE)
//实际电压振幅 (V_peak) ≈ fft_magnitude[i] * (V_ref / 65535) * (ADC_BUFFER_SIZE / window_amplitude_correction)
    for(uint8_t i=0;i<peaks_count;i++)
    {
        LCD_DisplayDecimals(0,(75+25*i),top_peaks[i].magnitude/65535,3,1);
        LCD_DisplayDecimals(140,(75+25*i),top_peaks[i].index*fft_fuscalu,9,1);
    }
}



void LCD_Show_Flags(void)
{
    LCD_DisplayNumber( 40,200, Flag_GPIOF_PIN_6, 2);
    LCD_DisplayNumber( 200,200, Flag_GPIOF_PIN_7, 2);
}


void LCD_Show_Numbers_test(void)
{
    uint16_t i=0;
    for(i=0;i<128;i++)
    {
        LCD_DisplayDecimals(70,100,i,3,1);
    }
    i=0;
}


/**
  * @brief  在LCD上简单绘制ADC波形（连线图）
  * @param  adc_buffer: 指向ADC数据数组的指针
  * @param  size: 数组的大小（元素个数），建议小于或等于屏幕宽度以获得最佳效果
  * @param  plot_color: 波形的颜色（24位RGB888格式，例如0x00FF00为绿色）
  * @param  vref: 参考电压，用于将ADC值映射到屏幕高度（例如3.3f）
  * @param  adc_bits: ADC的位数（例如12，对应最大值4095）
  * @retval 无
  */
void Plot_WaveformSimple(uint16_t* adc_buffer, uint32_t size, uint32_t plot_color, float vref, uint8_t adc_bits)
{
    uint32_t i;
    uint16_t x1, y1, x2, y2;
    uint16_t adc_max_value = (1 << adc_bits) - 1;
    uint16_t screen_width = LCD_Width;
    uint16_t screen_height = LCD_Height;
    uint16_t plot_margin = 5;
    LCD_Clear();
    LCD_SetColor(plot_color);

    for(i = 0; i < size; i++) {
        x1 = plot_margin + (uint16_t)((float)i * (screen_width - 2*plot_margin) / (size - 1));
        y1 = screen_height - plot_margin - 1 - (uint16_t)((float)adc_buffer[i] * (screen_height - 2*plot_margin) / adc_max_value);

        if (x1 < plot_margin) x1 = plot_margin;
        if (x1 >= screen_width - plot_margin) x1 = screen_width - plot_margin - 1;
        if (y1 < plot_margin) y1 = plot_margin;
        if (y1 >= screen_height - plot_margin) y1 = screen_height - plot_margin - 1;

        if(i == 0) {
            x2 = x1;
            y2 = y1;
            LCD_DrawPoint(x1, y1, plot_color);
        } else {
            LCD_DrawLine(x2, y2, x1, y1);
            x2 = x1;
            y2 = y1;
        }
    }
}





