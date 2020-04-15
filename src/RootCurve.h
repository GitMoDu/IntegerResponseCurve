// RootCurve.h

#ifndef _ROOT_CURVE_h
#define _ROOT_CURVE_h

#include "Template\ResponseCurveTemplate.h"

#if defined(__arm__) 
#ifndef __CLZ
#define __CLZ             (uint8_t)__builtin_clz
#endif
#endif

class Root2CurveU8 : public TemplateResponseCurveU8
{
private:
	uint8_t res = 0;
	uint8_t temp = 0;
	uint16_t g2 = 0;
	uint16_t add = 0;

	uint32_t Helper = 0;
	uint16_t RootHelper = 0;


public:
	Root2CurveU8(const uint8_t saturation = 127) : TemplateResponseCurveU8(saturation)
	{}

#if defined(__arm__) 
	virtual const uint8_t Process(const uint8_t input)
	{
		if (!input)
			return 0;

		Helper = (input * UINT8_MAX);

		RootHelper = 1 << ((32 - __CLZ(Helper)) / 2);
		RootHelper = (RootHelper + Helper / RootHelper) / 2;
		RootHelper = (RootHelper + Helper / RootHelper) / 2;
		RootHelper = (RootHelper + Helper / RootHelper) / 2;

		return RootHelper;
	}
#else
	virtual const uint8_t Process(const uint8_t input)
	{
		if (!input)
			return 0;

		Helper = input * UINT8_MAX;

		res = 0;
		temp = 0;
		g2 = 0;

		add = 1 << 8;

		for (uint8_t i = 0; i < 8; i++)
		{
			temp = res | add;
			g2 = temp * temp;
			if (Helper >= g2)
			{
				res = temp;
			}
			add >>= 1;
		}

		return res;
	}
#endif
};


//https://stackoverflow.com/questions/1100090/looking-for-an-efficient-integer-square-root-algorithm-for-arm-thumb2
class Root2CurveU16 : public TemplateResponseCurveU16
{
private:
	uint32_t RootHelper = 0;
	uint32_t Helper = 0;


	uint16_t res = 0;
	uint16_t temp = 0;
	uint32_t g2 = 0;
	uint16_t add = 0;


public:
	Root2CurveU16(const uint8_t saturation = 127) : TemplateResponseCurveU16(saturation)
	{}


#if defined(__arm__) 
	 /* FastIntSqrt is based on Wikipedia article:
	 https://en.wikipedia.org/wiki/Methods_of_computing_square_roots
	 Which involves Newton's method which gives the following iterative formula:
	
	 X(n+1) = (X(n) + S/X(n))/2
	
	 Thanks to ARM CLZ instruction (which counts how many bits in a number are
	 zeros starting from the most significant one) we can very successfully
	 choose the starting value, so just three iterations are enough to achieve
	 maximum possible error of 1. The algorithm uses division, but fortunately
	 it is fast enough here, so square root computation takes only about 50-55
	 cycles with maximum compiler optimization.
	const uint32_t FastIntSqrt(const uint32_t input)
	{
		if (!input)
			return 0;

		RootHelper = 1 << ((32 - __CLZ(input)) / 2);
		RootHelper = (RootHelper + input / RootHelper) / 2;
		RootHelper = (RootHelper + input / RootHelper) / 2;
		RootHelper = (RootHelper + input / RootHelper) / 2;

		return RootHelper;
	}*/

	virtual const uint16_t Process(const uint16_t input)
	{
		if (!input)
			return 0;

		Helper = input * UINT16_MAX;

		RootHelper = 1 << ((32 - __CLZ(Helper)) / 2);
		RootHelper = (RootHelper + Helper / RootHelper) / 2;
		RootHelper = (RootHelper + Helper / RootHelper) / 2;
		RootHelper = (RootHelper + Helper / RootHelper) / 2;

		return RootHelper;
	}
#else
	/* I find that most algorithms are based on simple ideas,
	but are implemented in a way more complicated manner than necessary.
	I've taken the idea from here:
	http://ww1.microchip.com/downloads/en/AppNotes/91040a.pdf
	(by Ross M. Fosler) and made it into a very short C-function
	uint16_t int_sqrt32(const uint32_t x)
	{
		uint16_t res = 0;
		uint16_t temp = 0;
		uint32_t g2 = 0;

		uint16_t add = 0x8000;

		for (uint8_t i = 0; i < 16; i++)
		{
			temp = res | add;
			g2 = temp * temp;
			if (x >= g2)
			{
				res = temp;
			}
			add >>= 1;
		}

		return res;
	}*/

	virtual const uint16_t Process(const uint16_t input)
	{
		if (!input)
			return 0;

		Helper = input * UINT16_MAX;

		res = 0;
		temp = 0;
		g2 = 0;

		add = 1 << 16;

		for (uint8_t i = 0; i < 16; i++)
		{
			temp = res | add;
			g2 = temp * temp;
			if (Helper >= g2)
			{
				res = temp;
			}
			add >>= 1;
		}

		return res;
	}
#endif
};
#endif