#include "stm32f4xx.h"                  // Device header

int main()
{
	GPIO_InitTypeDef         GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef         NVIC_InitStructure; // interruptlari düzenleyen conf. ayarlari
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_TimeBaseStructure.TIM_Prescaler          =8399; //+1den baslar isleme
	TIM_TimeBaseStructure.TIM_CounterMode        = TIM_CounterMode_Up;// counterin aldigi input sayisini sayar
	TIM_TimeBaseStructure.TIM_Period             =9999;
	TIM_TimeBaseStructure.TIM_ClockDivision      =TIM_CKD_DIV4; //Cloccku kaça bölmesi gerektigini söyler.
	TIM_TimeBaseStructure.TIM_RepetitionCounter  =50;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	TIM_Cmd(TIM2,ENABLE);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel                     = TIM2_IRQn; // Hangi interrupt kaynagi conf.
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority   = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority          = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd                  = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	/*    
    TIM4 is connected to APB1 bus, which has on F407 device 42MHz clock                 
    But, timer has internal PLL, which double this frequency for timer, up to 84MHz     
    Remember: Not each timer is connected to APB1, there are also timers connected     
    on APB2, which works at 84MHz by default, and internal PLL increase                 
    this to up to 168MHz                                                             
    
    Set timer prescaller 
    Timer count frequency is set with 
    
    timer_tick_frequency = Timer_default_frequency / (prescaller_set + 1)    10k  
    
    In our case, we want a max frequency for timer, so we set prescaller to 0         
    And our timer will have tick frequency        
   
      (Period) * (Prescaler)
sn   = ----------------------
      (Clock Speed)
    timer_tick_frequency = 84000000 / (0 + 1) = 84000000 
*/ 
/*
    Set timer period when it have reset
    First you have to know max value for timer
    In our case it is 16bit = 65535
    To get your frequency for PWM, equation is simple
    
    PWM_frequency = timer_tick_frequency / (TIM_Period + 1)
    
    If you know your PWM frequency you want to have timer period set correct
    
    TIM_Period = timer_tick_frequency / PWM_frequency - 1
    
    In our case, for 10Khz PWM_frequency, set Period to
    
    TIM_Period = 84000000 / 10000 - 1 = 8399
    
    If you get TIM_Period larger than max timer value (in our case 65535),
    you have to choose larger prescaler and slow down timer tick frequency
*/
 while(1)
 {
 
 } 
	
}

void TIM2_IRQHandler()
{
  if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	{
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	GPIO_ToggleBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		
	}
}

