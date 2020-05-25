#ifndef _GPIO_H
#define _GPIO_H
#include "stm32f4xx.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"

namespace Port{
	enum port {A,B,C,D,E};
};
namespace Pull{
	enum PullType {UP,DOWN,FLOATING};
};

namespace IO{

	enum PinType {INPUT,OUTPUT,ANALOG,PERIPH};
	enum PinSpeed {LOW,MEDIUM,HIGH,VERY_HIGH};

	class GPIO{
	private:
		GPIO_TypeDef* gpio;
		uint8_t pinNumber;
		uint32_t pinMask;
		enum PinType pinType;
		bool outputState;
		bool inputState;
		void enablePortClock();
		void setSpeed(enum PinSpeed pinSpeed);
		void setPull(enum Pull::PullType pullType);
	public:
		GPIO(enum Port::port port,uint8_t pinNumber,enum PinType pinType=OUTPUT,
				enum PinSpeed pinSpeed=MEDIUM,enum Pull::PullType pullType=Pull::DOWN);
		void setPeripheralType(uint32_t alternate);//better will be make enum with alternate types
		void write(bool state);//tested
		bool read();//tested
		void toggle();//tested
		void clean();//maybe is unnecessary
	};
};
#endif
