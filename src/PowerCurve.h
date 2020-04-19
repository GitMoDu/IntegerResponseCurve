// PowerCurve.h

#ifndef _POWER_CURVE_h
#define _POWER_CURVE_h

#include "Template\ResponseCurveTemplate.h"

class Power2CurveU8 : public TemplateResponseCurveU8
{
public:
	Power2CurveU8(const uint8_t saturation = 127) : TemplateResponseCurveU8(saturation)
	{}

	virtual const uint8_t Process(const uint8_t input)
	{
		// Optimized version of min(pow(input, 2) / (UINT8_MAX - 1), UINT8_MAX).
		return (((uint16_t)input * input) + (input)) >> 8;
	}
};

class Power2CurveU16 : public TemplateResponseCurveU16
{
public:
	Power2CurveU16(const uint8_t saturation = 127) : TemplateResponseCurveU16(saturation)
	{}

	virtual const uint16_t Process(const uint16_t input)
	{
		// Optimized version of pow(input, 2) / UINT16_MAX.
		return (((uint32_t)input * input) + input) >> 16;
	}
};

class Power3CurveU8 : public TemplateResponseCurveU8
{
public:
	Power3CurveU8(const uint8_t saturation = 127) : TemplateResponseCurveU8(saturation)
	{}

	virtual const uint8_t Process(const uint8_t input)
	{
		// Optimized version of  min(pow(input, 3) / (UINT16_MAX - (UINT8_MAX * 3)), UINT8_MAX).
		return  (((uint16_t)input * input * (uint32_t)(input + 3))) >> 16;
	}
};

class Power3CurveU16 : public TemplateResponseCurveU16
{
public:
	Power3CurveU16(const uint8_t saturation = 127) : TemplateResponseCurveU16(saturation)
	{}

	virtual const uint16_t Process(const uint16_t input)
	{
		// Optimized version of pow(input, 3) / (UINT32_MAX - ((uint32_t)UINT16_MAX * 3)).
		uint32_t Power2Shifted = (uint32_t)input * input >> 16;
		return (((Power2Shifted)*input) + (Power2Shifted + ((uint32_t)input + 1))) >> 16;
	}
};
#endif