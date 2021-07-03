#include "periphery.h"

/*! -------------------------------------------------------------------------
\brief Timer for buzzer control
\details   Initialize timer to count with 0.1 milliseconds
Bus frequency equal to 8 MHz. Divide frequency to 10 kHz (one tick = 0.1 ms) by brescaler. 
TIMx->ARR register set number of milliseconds. With 0.1 ms tick precision of delay is higher. 
*/
void Periphery::InitTim16Buzzer()
{	
	RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;	
	TIM16->CR1 |= TIM_CR1_ARPE;										// Auto reload enabled
	TIM16->CR1 |= TIM_CR1_URS;										// Reload without interrupts
	TIM16->SR = 0; 													// Clear flags  
	TIM16->PSC = 799;							  					// Prescaler 
	TIM16->ARR = 9;													// AutoReload Register 
	TIM16->CNT = 0;
	TIM16->DIER |= TIM_DIER_UIE;									// Allow interrupt update	

	//TIM16->CR1 |= TIM_CR1_CEN;						    		// Start the timer counter 	
}

/*! -------------------------------------------------------------------------
\brief Set new timer value
*/
void Periphery::UpdateTIM16(uint16_t reload_value)
{
	TIM16->CR1 &= ~TIM_CR1_CEN;
	TIM16->ARR = reload_value;
	TIM16->CCR1 = 0;
	TIM16->CNT = 0;
	TIM16->EGR |= TIM_EGR_UG;										// Reinitialize the counter and generates an update of the registers
//	TIM16->CR1 |= TIM_CR1_URS;										// Reload without interrupts
	TIM16->CR1 |= TIM_CR1_CEN;
}

/*! -------------------------------------------------------------------------
\brief Stop timer
*/
void Periphery::StopTIM16()
{
	TIM16->CR1 &= ~TIM_CR1_CEN;						    			// Start the timer counter 	
}
