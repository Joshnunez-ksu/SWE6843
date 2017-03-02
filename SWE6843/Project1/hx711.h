#ifndef HX711_H_
#define HX711_H_

#include <stdint.h>

class Hx711
{
	public:
		Hx711(uint8_t _pin_dout, uint8_t _pin_slk);
		//virtual ~Hx711();
		long getValue();
		long averageValue(uint8_t times = 32);
		void setOffset(long offset);
		void setScale(float scale = 742.f);
		float getGram();

	private:
		const uint8_t pin_dout;
		const uint8_t pin_slk;
		long offset;
		float scale;
};

#endif