#include "pwm.h"

Time::PwmOutput::PwmOutput(TIM_TypeDef *timer_instance,uint32_t systemFrequency,
				uint32_t frequency,uint32_t counterPeriod)
				: TimeBase(timer_instance,systemFrequency,frequency,counterPeriod){
	for(uint8_t i=0;i<4;i++){
		dutyCyclePerChannel[i]=-1;
	}
	setChannelCompare=nullptr;
}
void Time::PwmOutput::configChannel(uint8_t channel,uint8_t dutyCyclePercent,enum Time::Polarity polarity,
				bool enableInterrupt,uint8_t interruptPriority){
	uint32_t llChannel;
	if(channel==1){
		llChannel=LL_TIM_CHANNEL_CH1;
		setChannelCompare=LL_TIM_OC_SetCompareCH1;
	}
	else if(channel==2) {
		llChannel=LL_TIM_CHANNEL_CH2;
		setChannelCompare=LL_TIM_OC_SetCompareCH2;
	}
	else if(channel==3) {
		llChannel=LL_TIM_CHANNEL_CH3;
		setChannelCompare=LL_TIM_OC_SetCompareCH3;
	}
	else if(channel==4) {
		llChannel=LL_TIM_CHANNEL_CH4;
		setChannelCompare=LL_TIM_OC_SetCompareCH4;
	}
	LL_TIM_OC_SetMode(timer,llChannel,LL_TIM_OCMODE_PWM1);
	if(polarity == Polarity::LOW) {
		LL_TIM_OC_SetPolarity(timer,llChannel,LL_TIM_OCPOLARITY_LOW);
	}else{
		LL_TIM_OC_SetPolarity(timer,llChannel,LL_TIM_OCPOLARITY_HIGH);
	}
	dutyCyclePerChannel[channel-1]=(int8_t)dutyCyclePercent;
	uint32_t channelCompare=dutyCyclePercent*period/100;
	setChannelCompare(timer,channelCompare);
	LL_TIM_OC_EnablePreload(timer,llChannel);
	if(enableInterrupt){
		//Todo setNVIC
		//enableIT CC
	}
	LL_TIM_CC_EnableChannel(timer,llChannel);
}

bool Time::PwmOutput::isActiveChannel(uint8_t channel){
	bool isActive=false;
	if(channel>=1 && channel<=4){
		if(dutyCyclePerChannel[channel-1]!=-1)
			isActive=true;
	}
	return isActive;
}
void Time::PwmOutput::start(){
	LL_TIM_EnableCounter(timer);
	LL_TIM_GenerateEvent_UPDATE(timer);
}
void Time::PwmOutput::stop(){
	LL_TIM_DisableCounter(timer);
}
void Time::PwmOutput::setDutyCycle(uint8_t channel,uint8_t newDutyCycle){
	uint32_t newChannelCompare=newDutyCycle<=100 ? newDutyCycle*period/100 : period/2;
	if(channel==1 && isActiveChannel(channel)){
		setChannelCompare=LL_TIM_OC_SetCompareCH1;
	}else if(channel==2 && isActiveChannel(channel)){
		setChannelCompare=LL_TIM_OC_SetCompareCH2;
	}else if(channel==3 && isActiveChannel(channel)){
		setChannelCompare=LL_TIM_OC_SetCompareCH3;
	}else if(channel==4 && isActiveChannel(channel)){
		setChannelCompare=LL_TIM_OC_SetCompareCH4;
	}
	setChannelCompare(timer,newChannelCompare);
}
int8_t Time::PwmOutput::getDutyCycle(uint8_t channelNumber){
	if(isActiveChannel(channelNumber)){
		return dutyCyclePerChannel[channelNumber-1];
	}else{
		return -1;
	}
}
