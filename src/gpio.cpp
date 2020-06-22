#include "gpio.h"


IO::GPIO::GPIO(Port port,uint8_t pinNumber,PinType pinType,
		PinSpeed pinSpeed,PullType pullType,Periph periphType){
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
	if(this->pinType == PinType::Output){
		LL_GPIO_SetPinMode(gpio,pinMask,LL_GPIO_MODE_OUTPUT);
		LL_GPIO_SetPinOutputType(gpio,pinMask,LL_GPIO_OUTPUT_PUSHPULL);
	}else if(this->pinType == PinType::Input){
		LL_GPIO_SetPinMode(gpio,pinMask,LL_GPIO_MODE_INPUT);
	}else if(this->pinType == PinType::Analog){
		LL_GPIO_SetPinMode(gpio,pinMask,LL_GPIO_MODE_ANALOG);
	}else if(this->pinType == PinType::Periph){
		LL_GPIO_SetPinMode(gpio,pinMask,LL_GPIO_MODE_ALTERNATE);
		if(periphType==Periph::i2c1 || periphType==Periph::i2c2){
			LL_GPIO_SetPinOutputType(gpio,pinMask,LL_GPIO_OUTPUT_OPENDRAIN);
		}
		setPeripheralType(periphTypeToLL(periphType));
	}
	setPull(pullType);
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
	if(pinSpeed==PinSpeed::Low){ llPinSpeed=LL_GPIO_SPEED_FREQ_LOW; }
	else if(pinSpeed==PinSpeed::Medium) { llPinSpeed=LL_GPIO_SPEED_FREQ_MEDIUM; }
	else if(pinSpeed==PinSpeed::High) { llPinSpeed=LL_GPIO_SPEED_FREQ_HIGH; }
	else if(pinSpeed==PinSpeed::VeryHigh) { llPinSpeed=LL_GPIO_SPEED_FREQ_VERY_HIGH; }
	LL_GPIO_SetPinSpeed(gpio,pinMask,llPinSpeed);
}
void IO::GPIO::write(bool state){
	if(pinType!=PinType::Output)
		return;
	state ? LL_GPIO_SetOutputPin(gpio,pinMask) : LL_GPIO_ResetOutputPin(gpio,pinMask);
}
void IO::GPIO::toggle(){
	if(pinType!=PinType::Output)
		return;
	LL_GPIO_TogglePin(gpio,pinMask);
}
void IO::GPIO::setPull(PullType pullType){
	uint32_t llPullType;
	if(pullType==PullType::Down){ llPullType=LL_GPIO_PULL_DOWN; }
	else if(pullType==PullType::Up) { llPullType=LL_GPIO_PULL_UP; }
	else { llPullType=LL_GPIO_PULL_NO; }
	LL_GPIO_SetPinPull(gpio,pinMask,llPullType);
}
bool IO::GPIO::read(){
	if(pinType!=PinType::Input)
		return false;
	uint16_t inputReg=gpio->IDR;
	inputReg&=pinMask;
	if(inputReg>0){
		return true;
	}else{
		return false;
	}
}
uint32_t IO::GPIO::periphTypeToLL(Periph periph){
	uint32_t llPeriphPinType=0;
	if(periph==Periph::usart1){ llPeriphPinType=LL_GPIO_AF_7; }
	else if(periph==Periph::usart2) { llPeriphPinType=LL_GPIO_AF_7; }
	else if(periph==Periph::usart6) { llPeriphPinType=LL_GPIO_AF_8; }
	else if(periph==Periph::i2c1) { llPeriphPinType=LL_GPIO_AF_4; }
	else if(periph==Periph::i2c2) { llPeriphPinType=LL_GPIO_AF_4; }
	else if(periph==Periph::spi1) { llPeriphPinType=LL_GPIO_AF_5; }
	else if(periph==Periph::spi2) { llPeriphPinType=LL_GPIO_AF_6; }
	else if(periph==Periph::tim1 || periph==Periph::tim2) { llPeriphPinType=LL_GPIO_AF_1; }
	else if(periph==Periph::tim3 || periph==Periph::tim4 ||
			periph==Periph::tim5) { llPeriphPinType=LL_GPIO_AF_2; }
	return llPeriphPinType;
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

