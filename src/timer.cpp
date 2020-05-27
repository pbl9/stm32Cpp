#include "timer.h"

Time::TimeBase::TimeBase(TIM_TypeDef *timer_instance,uint32_t systemFrequency,
		uint32_t frequency,uint32_t counterPeriod){
	timer=timer_instance;
	enableTimClock();
	uint32_t prescalerValue=(systemFrequency/frequency)/counterPeriod;
	LL_TIM_SetPrescaler(timer, prescalerValue-1);
	LL_TIM_SetAutoReload(timer, counterPeriod-1);
}
void Time::TimeBase::enableTimClock(){
	/*
	 * add info about number of alternate channels and bitness
	 * 32 bit - ARR
	 * */
	if(timer==TIM1){ LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1); }
	else if(timer==TIM2) { LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2); }
	else if(timer==TIM3) { LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3); }
	else if(timer==TIM4) { LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4); }
	else if(timer==TIM5) { LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM5); }
	else if(timer==TIM9) { LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM9); }
	else if(timer==TIM10) { LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM10); }
	else if(timer==TIM11) { LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM11); }
}

Time::TimerInterrupt::TimerInterrupt(TIM_TypeDef *timer_instance,uint32_t systemFrequency,
		uint32_t frequency,uint32_t counterPeriod,
		uint16_t priority) : TimeBase(timer_instance,systemFrequency,frequency,counterPeriod) {
	setNVIC(priority);
	LL_TIM_EnableIT_UPDATE(timer);
	LL_TIM_GenerateEvent_UPDATE(timer);
}
void Time::TimerInterrupt::setNVIC(uint16_t priority){
	IRQn_Type irq;
	if(timer==TIM1){ irq=TIM1_UP_TIM10_IRQn; }//TIM1 Update Interrupt and TIM10 global interrupt
	else if(timer==TIM2) { irq=TIM2_IRQn; }
	else if(timer==TIM3) { irq=TIM3_IRQn; }
	else if(timer==TIM4) { irq=TIM4_IRQn; }
	else if(timer==TIM5) { irq=TIM5_IRQn; }
	else if(timer==TIM9) { irq=TIM1_BRK_TIM9_IRQn; }//tim9 share interrupt with tim1 break
	else if(timer==TIM10) { irq=TIM1_UP_TIM10_IRQn; }

	NVIC_SetPriority(irq,priority);
	NVIC_EnableIRQ(irq);
}
void Time::TimerInterrupt::start(){
	LL_TIM_EnableCounter(timer);
}
void Time::TimerInterrupt::stop(){
	LL_TIM_DisableCounter(timer);
}
