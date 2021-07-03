#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

const uint8_t BUTTON_PRESS_WAIT = 0;
const uint8_t BUTTON_PRESS_VALID = 1;
const uint8_t BUTTON_PRESS_NOT_VALID = 2;

/*! -------------------------------------------------------------------------
\brief Class for button 1
*/

class Button1_class
{
	public:
		void Pressed();
		uint8_t Check();
		
	private:
		uint32_t start_time;
		uint32_t stop_time;
		static const uint32_t FILTER_TIME = 50;
};

extern Button1_class Button1;


/*! -------------------------------------------------------------------------
\brief Class for button 2
*/
class Button2_class
{
	public:
		void Pressed();
		uint8_t Check();
		
	private:
		uint32_t start_time;
		uint32_t stop_time;
		static const uint32_t FILTER_TIME = 50;
};

extern Button2_class Button2;

#endif
