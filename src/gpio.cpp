#include "gpio.h"


IO::GPIO::GPIO(enum Port::port port,uint8_t pinNumber,enum PinType pinType,
		enum PinSpeed pinSpeed,enum Pull::PullType pullType){
	this->pinNumber=pinNumber;
	if(port==Port::A){ gpio=GPIOA; }
	else if(port==Port::B){ gpio=GPIOB; }
	else if(port==Port::C){ gpio=GPIOC; }
	else if(port==Port::D){ gpio=GPIOD; }
	else if(port==Port::E){ gpio=GPIOE; }
	else {  }
	this->pinType=pinType;
	outputState=false;
	inputState=false;
	enablePortClock();
	pinMask=1U<<(this->pinNumber);
	if(this->pinType == OUTPUT){
		LL_GPIO_SetPinMode(gpio,pinMask,LL_GPIO_MODE_OUTPUT);
		LL_GPIO_SetPinOutputType(gpio,pinMask,LL_GPIO_OUTPUT_PUSHPULL);
	}else if(this->pinType == INPUT){
		LL_GPIO_SetPinMode(gpio,pinMask,LL_GPIO_MODE_INPUT);
		setPull(pullType);
	}else if(this->pinType == ANALOG){
		LL_GPIO_SetPinMode(gpio,pinMask,LL_GPIO_MODE_ANALOG);
	}else if(this->pinType == PERIPH){
		//as alternate
	}
	setSpeed(pinSpeed);
}
void IO::GPIO::enablePortClock(){
	if(gpio==GPIOA){ LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA); }
	else if(gpio==GPIOB) {  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB); }
	else if(gpio==GPIOC) {  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC); }
	else if(gpio==GPIOD) {  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD); }
	else if(gpio==GPIOE) {  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE); }
}
void IO::GPIO::setSpeed(enum PinSpeed pinSpeed){
	uint32_t llPinSpeed=0;
	if(pinSpeed==LOW){ llPinSpeed=LL_GPIO_SPEED_FREQ_LOW; }
	else if(pinSpeed==MEDIUM) { llPinSpeed=LL_GPIO_SPEED_FREQ_MEDIUM; }
	else if(pinSpeed==HIGH) { llPinSpeed=LL_GPIO_SPEED_FREQ_HIGH; }
	else if(pinSpeed==VERY_HIGH) { llPinSpeed=LL_GPIO_SPEED_FREQ_VERY_HIGH; }
	else { }
	LL_GPIO_SetPinSpeed(gpio,pinMask,llPinSpeed);
}
void IO::GPIO::write(bool state){
	if(pinType!=OUTPUT)
		return;
	state ? LL_GPIO_SetOutputPin(gpio,pinMask) : LL_GPIO_ResetOutputPin(gpio,pinMask);
}
void IO::GPIO::toggle(){
	if(pinType!=OUTPUT)
		return;
	LL_GPIO_TogglePin(gpio,pinMask);
}
void IO::GPIO::setPull(enum Pull::PullType pullType){
	uint32_t llPullType;
	if(pullType==Pull::DOWN){ llPullType=LL_GPIO_PULL_DOWN; }
	else if(pullType==Pull::UP) { llPullType=LL_GPIO_PULL_UP; }
	else { llPullType==LL_GPIO_PULL_NO; }
	LL_GPIO_SetPinPull(gpio,pinMask,llPullType);
}
bool IO::GPIO::read(){
	if(pinType!=INPUT)
		return false;
	uint16_t inputReg=gpio->IDR;
	inputReg&=pinMask;
	if(inputReg>0){
		return true;
	}else{
		return false;
	}
}
void IO::GPIO::setPeripheralType(uint32_t alternate){
	if(pinNumber<=7){
	LL_GPIO_SetAFPin_0_7(gpio,pinMask,alternate);
	}else{
		LL_GPIO_SetAFPin_8_15(gpio,pinMask,alternate);
	}
}
void IO::GPIO::clean(){
}

