#ifndef _TIMER_H
#define _TIMER_H

/*
 * Simple counter
 * */

#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_rcc.h"


namespace Time{
class TimeBase{
	private:
		void enableTimClock();
	protected:
		TIM_TypeDef* timer;
	public:
		TimeBase(TIM_TypeDef *timer_instance,uint32_t systemFrequency,
				uint32_t frequency,uint32_t counterPeriod);
	};
class TimerInterrupt : public Time::TimeBase{
	private:
		void setNVIC(uint16_t priority);
	public:
		TimerInterrupt(TIM_TypeDef *timer_instance,uint32_t systemFrequency,
				uint32_t frequency,uint32_t counterPeriod,uint16_t priority);//tested
		void start();//tested
		void stop();
	};
};
#endif
