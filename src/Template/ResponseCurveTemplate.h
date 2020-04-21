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

class TemplateResponseCurveU32 : public AbstractTemplateResponseCurve<uint32_t, 0, UINT32_MAX>
{
private:
	uint8_t Saturation = 0;

public:
	TemplateResponseCurveU32(const uint8_t saturation = 127)
		: AbstractTemplateResponseCurve<uint32_t, 0, UINT32_MAX>()
	{
		SetSaturation(saturation);
	}

	void SetSaturation(const uint8_t saturation)
	{
		Saturation = saturation;
	}

	uint32_t Saturate(const uint32_t processed, const uint32_t input)
	{
		return ((Saturation * ((int64_t)processed - input)) / UINT8_MAX) + input;
	}

	uint32_t Get(const uint32_t input)
	{
		uint32_t value = Saturate(Process(input), input);

		return constrain(value, Limits.Lower, Limits.Upper);
	}
};

template<class ResponseCurveU, typename UnsignedType>
class TemplateDoubleChainedCurve : public ResponseCurveU
{
public:
	TemplateDoubleChainedCurve(const uint8_t saturation = 127)
		: ResponseCurveU(saturation)
	{}

	virtual const UnsignedType Process(const UnsignedType input)
	{
		return ResponseCurveU::Process(ResponseCurveU::Process(input));
	}
};

template<class ResponseCurveU8>
class TemplateDoubleChainedCurveU8 : public TemplateDoubleChainedCurve<ResponseCurveU8, uint8_t>
{
public:
	TemplateDoubleChainedCurveU8(const uint8_t saturation = 127)
		: TemplateDoubleChainedCurve<ResponseCurveU8, uint8_t>(saturation)
	{}
};

template<class ResponseCurveU16>
class TemplateDoubleChainedCurveU16 : public TemplateDoubleChainedCurve<ResponseCurveU16, uint16_t>
{
public:
	TemplateDoubleChainedCurveU16(const uint8_t saturation = 127)
		: TemplateDoubleChainedCurve<ResponseCurveU16, uint16_t>(saturation)
	{}
};

template<class ResponseCurveU32>
class TemplateDoubleChainedCurveU32 : public TemplateDoubleChainedCurve<ResponseCurveU32, uint32_t>
{
public:
	TemplateDoubleChainedCurveU32(const uint8_t saturation = 127)
		: TemplateDoubleChainedCurve<ResponseCurveU32, uint32_t>(saturation)
	{}
};

template<typename ValueType, const ValueType MinValue, const ValueType MaxValue, typename UnsignedType, class ResponseCurveU>
class TemplateResponseCurveS : AbstractTemplateResponseCurve<ValueType, MinValue, MaxValue>
{
private:
	ResponseCurveU UnsignedResponse;

protected:
	using AbstractTemplateResponseCurve<ValueType, MinValue, MaxValue>::Limits;

public:
	TemplateResponseCurveS(const uint8_t saturation = 127)
		: AbstractTemplateResponseCurve<ValueType, MinValue, MaxValue>()
		, UnsignedResponse(saturation)
	{
	}

	void SetSaturation(const uint8_t saturation)
	{
		UnsignedResponse.SetSaturation(saturation);
	}

	ValueType const Process(const ValueType input)
	{
		return 0;
		/*if (input > 0)
		{
			UnsignedType inputU = ((UnsignedType)input * 2) + 1;

			return (ValueType)(UnsignedResponse.Process(inputU) / 2);
		}
		else if (input < 0)
		{
			UnsignedType inputU = (UnsignedType)(-input * 2);

			return (ValueType)(-(UnsignedResponse.Process(inputU) / 2));
		}
		else
		{
			return 0;
		}*/
	}

	ValueType Saturate(const ValueType processed, const ValueType input)
	{
		if (input > 0)
		{
			UnsignedType inputU = (UnsignedType)(input * 2) + 1;
			UnsignedType processedU = (UnsignedType)(processed * 2) + 1;

			return (ValueType)(UnsignedResponse.Saturate(processedU, inputU) / 2);
		}
		else if (input < 0)
		{
			UnsignedType inputU = (UnsignedType)(-input * 2);
			UnsignedType processedU = (UnsignedType)(-processed * 2);

			return (ValueType)(-(UnsignedResponse.Saturate(processedU, inputU) / 2));
		}
		else
		{
			return 0;
		}
	}

	ValueType Get(const ValueType input)
	{
		ValueType value;
		UnsignedType inputU;
		UnsignedType processedU;

		if (input > 0)
		{
			inputU = (UnsignedType)(input * 2) + 1;
			processedU = UnsignedResponse.Process(inputU);
			processedU = UnsignedResponse.Saturate(processedU, inputU);

			value = (ValueType)(processedU / 2);

			return constrain(value, Limits.Lower, Limits.Upper);
		}
		else if (input < 0)
		{
			inputU = (UnsignedType)(-input * 2);
			processedU = UnsignedResponse.Process(inputU);
			processedU = UnsignedResponse.Saturate(processedU, inputU);

			value = (ValueType)(-(processedU / 2));

			return constrain(value, Limits.Lower, Limits.Upper);
		}
		else
		{
			return 0;
		}
	}
};

template<class ResponseCurveU8>
class TemplateResponseCurveS8 : public TemplateResponseCurveS<int8_t, INT8_MIN, INT8_MAX, uint8_t, ResponseCurveU8>
{
public:
	TemplateResponseCurveS8(const uint8_t saturation = 127)
		: TemplateResponseCurveS<int8_t, INT8_MIN, INT8_MAX, uint8_t, ResponseCurveU8>(saturation)
	{}
};

template<class ResponseCurveU16>
class TemplateResponseCurveS16 : public TemplateResponseCurveS<int16_t, INT16_MIN, INT16_MAX, uint16_t, ResponseCurveU16>
{
public:
	TemplateResponseCurveS16(const uint8_t saturation = 127)
		: TemplateResponseCurveS<int16_t, INT16_MIN, INT16_MAX, uint16_t, ResponseCurveU16>(saturation)
	{}
};

template<class ResponseCurveU32>
class TemplateResponseCurveS32 : public TemplateResponseCurveS<int32_t, INT32_MIN, INT32_MAX, uint32_t, ResponseCurveU32>
{
public:
	TemplateResponseCurveS32(const uint8_t saturation = 127)
		: TemplateResponseCurveS<int32_t, INT32_MIN, INT32_MAX, uint32_t, ResponseCurveU32>(saturation)
	{}
};
#endif