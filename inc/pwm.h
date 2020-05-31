#pragma once
#include "timer.h"

namespace Time{
	enum Polarity{LOW,HIGH};

	class PwmOutput : public Time::TimeBase{
	private:
		int8_t dutyCyclePerChannel[4];
		void (*setChannelCompare)(TIM_TypeDef*,uint32_t);
		bool isActiveChannel(uint8_t channel);
		void setNVIC(uint16_t priority);
	public:
		PwmOutput(TIM_TypeDef *timer_instance,uint32_t systemFrequency,
				uint32_t frequency,uint32_t counterPeriod);
		void configChannel(uint8_t channel,uint8_t dutyCyclePercent=50,enum Polarity polarity=LOW,
				bool enableInterrupt=false,uint8_t interruptPriority=0);
		void start();
		void stop();
		void setDutyCycle(uint8_t channel,uint8_t newDutyCycle);
		int8_t getDutyCycle(uint8_t channelNumber);
	};
};
