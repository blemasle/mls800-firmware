#include "MLS800.h"

ISR(INT6_vect)
{
	_inputInterrupted = true;
}