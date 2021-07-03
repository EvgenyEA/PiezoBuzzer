#include "periphery.h" 

/*! -------------------------------------------------------------------------
\brief Sets tim in PWM mode
*/
void Periphery::InitTim14Pwm()
{
	RCC->APB1RSTR |= RCC_APB1RSTR_TIM14RST;									// reset timer
	RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM14RST;
	RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;									// enable clock
    
	TIM14->PSC = 1;         
	TIM14->ARR =  2024;          											// timer reload 
	TIM14->CNT = 0;	
	TIM14->CR1 &= ~TIM_CR1_ARPE;
	TIM14->CCER |= TIM_CCER_CC1P;											// Channel output 1: OC1 active low
	
	// Channel 1 settings
	TIM14->CCMR1 &= ~TIM_CCMR1_OC1M_0;
	TIM14->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);					// OC1M[2:0]: 110 - PWM mode 1 - in up counting, channel is active as long as TIMx_CNT<TIMx_CCR1 else inactive
	TIM14->CCMR1 |= TIM_CCMR1_OC1PE;										// OC1PE: output compare 2 preload enable
	TIM14->CCMR1 &= ~TIM_CCMR1_CC1S;										// CC1S: Capture/Compare 2 selection | 00: CC3 channel is configured as output
	TIM14->CCR1 = 1012; 													// To check PWM work
	
	TIM14->CCER |= TIM_CCER_CC1E;											// Capture/Compare 1 output enable
}

/*! -------------------------------------------------------------------------
\brief Set new timer value
*/
void Periphery::UpdateTimPWM(uint16_t reload_value, uint16_t count_value)
{
	TIM14->CR1 &= ~TIM_CR1_CEN;
	TIM14->ARR = reload_value;
	TIM14->CCR1 = count_value;
	TIM14->CNT = 0;
	TIM14->CCER |= TIM_CCER_CC1E;
	TIM14->CR1 |= TIM_CR1_CEN;
}

/*! -------------------------------------------------------------------------
\brief Stop timer
*/
void Periphery::StopTimPWM()
{
	TIM14->CCER &= ~TIM_CCER_CC1E;
	TIM14->CR1 &= ~TIM_CR1_CEN;
}
