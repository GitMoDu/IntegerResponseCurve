// ResponseCurveTemplate.h

#ifndef _RESPONSECURVETEMPLATE_h
#define _RESPONSECURVETEMPLATE_h

#include <stdint.h>

template<typename ValueType, const ValueType MinValue, const ValueType MaxValue>
struct LimitStruct {
	ValueType Upper = MaxValue;
	ValueType Lower = MinValue;
};

#ifndef INT8_MIN
#define INT8_MIN -127
#endif


#ifndef INT8_MAX
#define INT8_MAX 128
#endif

template<typename ValueType, const ValueType MinValue, const ValueType MaxValue>
class AbstractTemplateResponseCurve
{
protected:
	LimitStruct<ValueType, MinValue, MaxValue> Limits;

public:
	AbstractTemplateResponseCurve()
	{}

	void SetLimit(const ValueType minValue, const ValueType maxValue)
	{
		Limits.Lower = minValue;
		Limits.Upper = maxValue;
	}

	virtual const ValueType Process(const ValueType input)
	{
		return input;
	}

	const ValueType Limit(const ValueType input)
	{
		return constrain(input, Limits.Lower, Limits.Upper);
	}
};

class TemplateResponseCurveU8 : public AbstractTemplateResponseCurve<uint8_t, 0, UINT8_MAX>
{
private:
	uint8_t Saturation = 0;

public:
	TemplateResponseCurveU8(const uint8_t saturation = 127)
		: AbstractTemplateResponseCurve<uint8_t, 0, UINT8_MAX>()
	{
		SetSaturation(saturation);
	}

	void SetSaturation(const uint8_t saturation)
	{
		Saturation = saturation;
	}

	uint8_t Saturate(const uint8_t processed, const uint8_t input)
	{
		return ((Saturation * (processed - input)) / UINT8_MAX) + input;
	}

	uint8_t Get(const uint8_t input)
	{
		uint8_t value = Saturate(Process(input), input);

		return constrain(value, Limits.Lower, Limits.Upper);
	}
};

class TemplateResponseCurveU16 : public AbstractTemplateResponseCurve<uint16_t, 0, UINT16_MAX>
{
private:
	uint8_t Saturation = 0;

public:
	TemplateResponseCurveU16(const uint8_t saturation = 127)
		: AbstractTemplateResponseCurve<uint16_t, 0, UINT16_MAX>()
	{
		SetSaturation(saturation);
	}

	void SetSaturation(const uint8_t saturation)
	{
		Saturation = saturation;
	}

	uint16_t Saturate(const uint16_t processed, const uint16_t input)
	{
		return ((Saturation * ((int32_t)processed - input)) / UINT8_MAX) + input;
	}

	uint16_t Get(const uint16_t input)
	{
		uint16_t value = Saturate(Process(input), input);

		return constrain(value, Limits.Lower, Limits.Upper);
	}
};

template<class ResponseCurveU, typename UnsignedType>
class TemplateDoubleCurve : public ResponseCurveU
{
public:
	TemplateDoubleCurve(const uint8_t saturation = 127)
		: ResponseCurveU(saturation)
	{}

	virtual const UnsignedType Process(const UnsignedType input)
	{
		return ResponseCurveU::Process(ResponseCurveU::Process(input));
	}
};

template<class ResponseCurveU16>
class TemplateDoubleCurveU16 : public TemplateDoubleCurve<ResponseCurveU16, uint16_t>
{
public:
	TemplateDoubleCurveU16(const uint8_t saturation = 127)
		: TemplateDoubleCurve<ResponseCurveU16, uint16_t>(saturation)
	{}
};

template<class ResponseCurveU8>
class TemplateDoubleCurveU8 : public TemplateDoubleCurve<ResponseCurveU8, uint8_t>
{
public:
	TemplateDoubleCurveU8(const uint8_t saturation = 127)
		: TemplateDoubleCurve<ResponseCurveU8, uint8_t>(saturation)
	{}
};

template<class ResponseCurveU8>
class TemplateResponseCurveS8 : AbstractTemplateResponseCurve<int8_t, INT8_MIN, INT8_MAX>
{
private:
	ResponseCurveU8 UnsignedResponse;
	int8_t ValueHelper = 0;

public:
	TemplateResponseCurveS8(const uint8_t saturation = 127)
		: AbstractTemplateResponseCurve<int8_t, INT8_MIN, INT8_MAX >()
		, UnsignedResponse(saturation)
	{
	}

	void SetSaturation(const uint8_t saturation)
	{
		UnsignedResponse.SetSaturation(saturation);
	}

	int8_t Get(const int8_t input)
	{
		if (input > 0)
		{
			ValueHelper = (int8_t)(UnsignedResponse.Saturate(UnsignedResponse.Process((((uint8_t)input) * 2) + 1), input) / 2);

			return constrain(ValueHelper, Limits.Lower, Limits.Upper);
		}
		else if (input < 0)
		{
			ValueHelper = (int8_t)(-((UnsignedResponse.Saturate(UnsignedResponse.Process((((uint8_t)(-input) - 1) * 2)), input) / 2) + 1));

			return constrain(ValueHelper, Limits.Lower, Limits.Upper);
		}
		else
		{
			return 0;
		}
	}

	int8_t const Process(const int8_t input)
	{
		if (input > 0)
		{
			return (int8_t)((UnsignedResponse.Process((((uint8_t)input) * 2) + 1), input) / 2);
		}
		else if (input < 0)
		{
			return (int8_t)(-((UnsignedResponse.Saturate(UnsignedResponse.Process((((uint8_t)(-input) - 1) * 2)), input) / 2) + 1));
		}
		else
		{
			return 0;
		}
	}

	int8_t Saturate(const int8_t processed, const int8_t input)
	{
		if (input > 0)
		{
			return (int8_t)(UnsignedResponse.Saturate((((uint8_t)input) * 2) + 1) / 2);
		}
		else if (input < 0)
		{
			return (int8_t)(-(((UnsignedResponse.Saturate((((uint8_t)(-input - 1)) * 2)), input) / 2) + 1));
		}
		else
		{
			return 0;
		}
	}
};

template<class ResponseCurveU16>
class TemplateResponseCurveS16 : AbstractTemplateResponseCurve<int16_t, INT16_MIN, INT16_MAX>
{
private:
	ResponseCurveU16 UnsignedResponse;

	int16_t ValueHelper = 0;

public:
	TemplateResponseCurveS16(const uint8_t saturation = 127)
		: AbstractTemplateResponseCurve<int16_t, INT16_MIN, INT16_MAX>()
		, UnsignedResponse(saturation)
	{
	}

	void SetSaturation(const uint8_t saturation)
	{
		UnsignedResponse.SetSaturation(saturation);
	}

	int16_t const Process(const uint16_t input)
	{
		if (input > 0)
		{
			return (int16_t)(UnsignedResponse.Process((((uint16_t)input) * 2) + 1) / 2);
		}
		else if (input < 0)
		{
			return (int16_t)(-(((UnsignedResponse.Process((((uint16_t)(-input - 1)) * 2)), input) / 2) + 1));
		}
		else
		{
			return 0;
		}
	}

	int16_t Saturate(const int16_t processed, const int16_t input)
	{
		if (input > 0)
		{
			return (int16_t)(UnsignedResponse.Saturate((((uint16_t)input) * 2) + 1) / 2);
		}
		else if (input < 0)
		{
			return (int16_t)(-(((UnsignedResponse.Saturate((((uint16_t)(-input - 1)) * 2)), input) / 2) + 1));
		}
		else
		{
			return 0;
		}
	}

	int16_t Get(const int16_t input)
	{
		int16_t value = 0;

		if (input > 0)
		{
			value = (int16_t)(UnsignedResponse.Saturate(UnsignedResponse.Process((((uint16_t)input) * 2) + 1), input) / 2);

			return constrain(value, Limits.Lower, Limits.Upper);
		}
		else if (input < 0)
		{
			value = (int16_t)(-((UnsignedResponse.Saturate(UnsignedResponse.Process((((uint16_t)(-input - 1)) * 2)), input) / 2) + 1));

			return constrain(value, Limits.Lower, Limits.Upper);
		}
		else
		{
			return value;
		}
	}
};
#endif