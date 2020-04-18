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
		return ((input * input) + UINT8_MAX) >> 8;

		//Aproximation of:
		//return ((input * input)) / UINT8_MAX;
	}
};

class Power2CurveU16 : public TemplateResponseCurveU16
{
public:
	Power2CurveU16(const uint8_t saturation = 127) : TemplateResponseCurveU16(saturation)
	{}

	virtual const uint16_t Process(const uint16_t input)
	{
		return (((uint32_t)input * input) + UINT16_MAX) >> 16;

		//Aproximation of:
		//return ((input * input) + 2) / UINT16_MAX;
	}
};

class Power3CurveU8 : public TemplateResponseCurveU8
{
public:
	Power3CurveU8(const uint8_t saturation = 127) : TemplateResponseCurveU8(saturation)
	{}

	virtual const uint8_t Process(const uint8_t input)
	{
		return ((input * input * input) + UINT16_MAX) >> 16;

		return (((uint32_t)input * input * input) + UINT16_MAX) >> 16;

		//Aproximation of:
		//return ((uint16_t)input * input * input) / UINT16_MAX;
	}
};

class Power3CurveU16 : public TemplateResponseCurveU16
{
public:
	Power3CurveU16(const uint8_t saturation = 127) : TemplateResponseCurveU16(saturation)
	{}

	virtual const uint16_t Process(const uint16_t input)
	{
		return (((uint64_t)input * input * input) + (UINT32_MAX)) >> 32;

		return (((uint64_t)input * input * input) + UINT32_MAX) >> 32;

		//Aproximation of:
		//return (((uint64_t)input * input * input)) / UINT32_MAX;
	}
};
#endif