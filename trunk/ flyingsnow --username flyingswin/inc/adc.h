#ifndef __ADC_H
#define __ADC_H



/**
  * @brief ADC analog channel selection
  */
typedef enum {
  ADC_CHANNEL_0  = (UCHAR)0x00, /* Analog channel 0 */
  ADC_CHANNEL_1  = (UCHAR)0x01, /* Analog channel 1 */
  ADC_CHANNEL_2  = (UCHAR)0x02, /* Analog channel 2 */
  ADC_CHANNEL_3  = (UCHAR)0x03, /* Analog channel 3 */
  ADC_CHANNEL_4  = (UCHAR)0x04, /* Analog channel 4 */
  ADC_CHANNEL_5  = (UCHAR)0x05  /* Analog channel 5 */
} ADC_Channel_TypeDef;


VOID InitAdc(VOID);

UINT AdcReadChannel(ADC_Channel_TypeDef ADC_Channel);


#endif 