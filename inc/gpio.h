#ifndef _GPIO_H
#define _GPIO_H
#include "stm32f4xx.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"


enum class Port {
	A,
	B,
	C,
	D,
	E};
enum class PullType{
	Up,
	Down,
	Float
};
enum class Periph {none,usart1,usart2,usart6,i2c1,i2c2,spi1,spi2,
		tim1,tim2,tim3,tim4,tim5};
enum class PinType{
	Input,
	Output,
	Analog,
	Periph
};
enum class PinSpeed {
	Low,
	Medium,
	High,
	VeryHigh
};
namespace IO{
	class GPIO{
	private:
		GPIO_TypeDef* gpio;
		uint8_t pinNumber;
		uint32_t pinMask;
		enum PinType pinType;
		bool outputState;
		bool inputState;
		void enablePortClock();
		uint32_t periphTypeToLL(Periph periph);
		void setPeripheralType(uint32_t alternate);
		void setSpeed(enum PinSpeed pinSpeed);
		void setPull(enum PullType pullType);
	public:
		GPIO(Port port,uint8_t pinNumber,PinType pinType=PinType::Output,
				PinSpeed pinSpeed=PinSpeed::Medium,PullType pullType=PullType::Down,
				Periph periphType=Periph::none);
		void write(bool state);//tested
		bool read();//tested
		void toggle();//tested
		void clean();//maybe is unnecessary
	};
};
#endif
