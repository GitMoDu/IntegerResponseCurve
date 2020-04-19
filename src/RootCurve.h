// RootCurve.h

// Based on snippets found on: https://stackoverflow.com/questions/1100090/looking-for-an-efficient-integer-square-root-algorithm-for-arm-thumb2

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
public:
	Root2CurveU8(const uint8_t saturation = 127) : TemplateResponseCurveU8(saturation)
	{}

#if defined(__arm__) 
	virtual const uint8_t Process(const uint8_t input)
	{
		return FastIntSqrt((uint32_t)input * UINT8_MAX);
	}
private:
	const uint16_t FastIntSqrt(const uint32_t input)
	{
		uint32_t Root = 1 << ((32 - __CLZ(input)) >> 1);

		Root = (Root + (input / Root)) >> 1;
		Root = (Root + (input / Root)) >> 1;
		Root = (Root + (input / Root)) >> 1;

		return Root;
	}
#else
	virtual const uint8_t Process(const uint8_t input)
	{
		return int_sqrt16((uint16_t)input * UINT8_MAX);
	}

private:
	const uint8_t int_sqrt16(const uint16_t x)
	{
		uint8_t res = 0;
		uint8_t add = 0x80;
		
		for (uint8_t i = 0; i < 8; i++)
		{
			uint8_t temp = res | add;
			uint16_t g2 = (uint16_t)temp * temp;
			if (x >= g2)
			{
				res = temp;
			}
			add >>= 1;
		}

		return res;
	}
#endif
};

class Root2CurveU16 : public TemplateResponseCurveU16
{
public:
	Root2CurveU16(const uint8_t saturation = 127) : TemplateResponseCurveU16(saturation)
	{}

#if defined(__arm__) 
	virtual const uint16_t Process(const uint16_t input)
	{
		return FastIntSqrt((uint32_t)input * UINT16_MAX);
	}

private:
	/*
	FastIntSqrt is based on Wikipedia article:
	https://en.wikipedia.org/wiki/Methods_of_computing_square_roots
	Which involves Newton's method which gives the following iterative formula:

	X(n+1) = (X(n) + S/X(n))/2

	Thanks to ARM CLZ instruction (which counts how many bits in a number are
	zeros starting from the most significant one) we can very successfully
	choose the starting value, so just three iterations are enough to achieve
	maximum possible error of 1. The algorithm uses division, but fortunately
	it is fast enough here, so square root computation takes only about 50-55
	cycles with maximum compiler optimization.
	*/
	const uint16_t FastIntSqrt(const uint32_t input)
	{
		uint32_t Root = 1 << ((32 - __CLZ(input)) >> 1);

		Root = (Root + (input / Root)) >> 1;
		Root = (Root + (input / Root)) >> 1;
		Root = (Root + (input / Root)) >> 1;

		return Root;
	}

#else
public:
	virtual const uint16_t Process(const uint16_t input)
	{
		return int_sqrt32((uint32_t)input * UINT16_MAX);
	}

private:
	/*
	I find that most algorithms are based on simple ideas,
	but are implemented in a way more complicated manner than necessary.
	I've taken the idea from here:
	http://ww1.microchip.com/downloads/en/AppNotes/91040a.pdf (by Ross M. Fosler)
	and made it into a very short C-function:
	https://stackoverflow.com/questions/1100090/looking-for-an-efficient-integer-square-root-algorithm-for-arm-thumb2
	*/
	const uint16_t int_sqrt32(const uint32_t x)
	{
		uint16_t res = 0;
		uint16_t add = 0x8000;

		for (uint8_t i = 0; i < 16; i++)
		{
			uint16_t temp = res | add;
			uint32_t g2 = (uint32_t)temp * temp;
			if (x >= g2)
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