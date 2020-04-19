/**
* Curve Validator
*
*
*/


#define DEBUG_LOG


#define SERIAL_BAUD_RATE 115200

#include <Arduino.h>
#include <ResponseCurveTemplate.h>
#include <PowerCurve.h>
#include <RootCurve.h>


TemplateResponseCurveU8 LinearU8(255);
TemplateResponseCurveS8<TemplateResponseCurveU8> LinearS8(255);
TemplateResponseCurveU16 LinearU16(255);
TemplateResponseCurveS16<TemplateResponseCurveU16> LinearS16(255);

Power2CurveU8 Power2U8(255);
Power2CurveU16 PowerSquare16(255);
Power3CurveU8 Power3U8(255);
Power3CurveU16 Power3U16(255);
Root2CurveU8 RootU8(255);
Root2CurveU16 RootU16(255);


const uint16_t LinearTestSize = 256;
const uint16_t ResponseDurationTestCount = 500;
const uint32_t ToleranceNanos = 200;

void loop()
{
}

void setup()
{
	Serial.begin(SERIAL_BAUD_RATE);
	while (!Serial)
		;
	delay(1000);

	Serial.println(F("Curve Validator"));
	Serial.println();


	PlotLinearOutput();
	Serial.println();
	MeasureDurations();
	Serial.println();
}

void PlotLinearOutput()
{
	Serial.println(F("Linear Response"));

	Serial.println(F("Input\tSquare8\tCube8\tRoot8"));
	for (uint16_t i = 0; i <= LinearTestSize; i++)
	{
		uint8_t Input = map(i, 0, LinearTestSize, 0, UINT8_MAX);
		Serial.print(Input);
		Serial.print('\t');
		Serial.print(Power2U8.Get(Input));
		Serial.print('\t');
		Serial.print(Power3U8.Get(Input));
		Serial.print('\t');
		Serial.print(RootU8.Get(Input));
		Serial.println();
	}
	Serial.println();
	Serial.println();


	Serial.println(F("Input\tSquar16\tCube16\tRoot16"));
	for (uint16_t i = 0; i <= LinearTestSize; i++)
	{
		const uint16_t Input = map(i, 0, LinearTestSize, 0, UINT16_MAX);
		//const uint16_t Input = map(i, 0, LinearTestSize, UINT16_MAX- LinearTestSize, UINT16_MAX);
		//const uint16_t Input = map(i, 0, LinearTestSize, 0, LinearTestSize);
		Serial.print(Input);
		Serial.print('\t');
		Serial.print(PowerSquare16.Get(Input));
		Serial.print('\t');
		Serial.print(Power3U16.Get(Input));
		Serial.print('\t');
		Serial.print(RootU16.Get(Input));
		Serial.println();
	}
}

void MeasureDurations()
{
	Serial.println(F("Reference duration measure"));
	Serial.println();


	uint16_t TestValue = 0;
	uint16_t DummyValue = 0;
	uint32_t Timestamp = 0;

	Timestamp = micros();
	for (uint16_t i = 0; i < ResponseDurationTestCount; i++)
	{
		TestValue = LinearU8.Get(i);
	}
	uint32_t ReferenceDurationU8 = ((micros() - Timestamp) * 1000) / ResponseDurationTestCount;
	DummyValue += TestValue;

	Timestamp = micros();
	for (uint16_t i = 0; i < ResponseDurationTestCount; i++)
	{
		TestValue = LinearS8.Get(i);
	}
	uint32_t ReferenceDurationS8 = ((micros() - Timestamp) * 1000) / ResponseDurationTestCount;
	DummyValue += TestValue;


	Timestamp = micros();
	for (uint16_t i = 0; i < ResponseDurationTestCount; i++)
	{
		TestValue = LinearU16.Get(i);
	}
	uint32_t ReferenceDurationU16 = ((micros() - Timestamp) * 1000) / ResponseDurationTestCount;
	DummyValue += TestValue;


	Timestamp = micros();
	for (uint16_t i = 0; i < ResponseDurationTestCount; i++)
	{
		TestValue = LinearS16.Get(i);
	}
	uint32_t ReferenceDurationS16 = ((micros() - Timestamp) * 1000) / ResponseDurationTestCount;
	DummyValue += TestValue;


	///
	Timestamp = micros();
	for (uint16_t i = 0; i < ResponseDurationTestCount; i++)
	{
		TestValue = Power2U8.Get(i);
	}
	uint32_t DurationPowerSquare8 = ((micros() - Timestamp) * 1000) / ResponseDurationTestCount;
	DummyValue += TestValue;


	Timestamp = micros();
	for (uint16_t i = 0; i < ResponseDurationTestCount; i++)
	{
		TestValue = PowerSquare16.Get(i);
	}
	uint32_t DurationPowerSquare16 = ((micros() - Timestamp) * 1000) / ResponseDurationTestCount;
	DummyValue += TestValue;


	Timestamp = micros();
	for (uint16_t i = 0; i < ResponseDurationTestCount; i++)
	{
		TestValue = Power3U8.Get(i);
	}
	uint32_t DurationPowerCube8 = ((micros() - Timestamp) * 1000) / ResponseDurationTestCount;
	DummyValue += TestValue;


	Timestamp = micros();
	for (uint16_t i = 0; i < ResponseDurationTestCount; i++)
	{
		TestValue = Power3U16.Get(i);
	}
	uint32_t DurationPowerCube16 = ((micros() - Timestamp) * 1000) / ResponseDurationTestCount;
	DummyValue += TestValue;

	Timestamp = micros();
	for (uint16_t i = 0; i < ResponseDurationTestCount; i++)
	{
		TestValue = RootU8.Get(i);
	}
	uint32_t DurationRoot8 = ((micros() - Timestamp) * 1000) / ResponseDurationTestCount;
	DummyValue += TestValue;


	Timestamp = micros();
	for (uint16_t i = 0; i < ResponseDurationTestCount; i++)
	{
		TestValue = RootU16.Get(i);
	}
	uint32_t DurationRoot16 = ((micros() - Timestamp) * 1000) / ResponseDurationTestCount;
	DummyValue += TestValue;


	DurationPowerSquare8 -= ReferenceDurationU8 - ToleranceNanos;
	DurationPowerCube8 -= ReferenceDurationU8 - ToleranceNanos;
	DurationRoot8 -= ReferenceDurationU8 - ToleranceNanos;
	DurationPowerSquare16 -= ReferenceDurationU16 - ToleranceNanos;
	DurationPowerCube16 -= ReferenceDurationU16 - ToleranceNanos;
	DurationRoot16 -= ReferenceDurationU16 - ToleranceNanos;

	Serial.println(F("Overhead (ns)"));
	Serial.print(F("U8: "));
	Serial.println(ReferenceDurationU8);
	Serial.print(F("U16: "));
	Serial.println(ReferenceDurationU16);
	Serial.print(F("S8: "));
	Serial.println(ReferenceDurationS8);
	Serial.print(F("S16: "));
	Serial.println(ReferenceDurationS16);
	Serial.println();

	Serial.print(F("Response curve processing duration "));
	if (ReferenceDurationU8 > 40000)
	{
		DurationPowerSquare8 /= 1000;
		DurationPowerCube8 /= 1000;
		DurationPowerSquare16 /= 1000;
		DurationPowerCube16 /= 1000;
		Serial.println(F("(us)"));
	}
	else
	{
		Serial.println(F("(ns)"));
	}

	Serial.print(F("Square8: "));
	Serial.println(DurationPowerSquare8);
	Serial.print(F("Cube8: "));
	Serial.println(DurationPowerCube8);
	Serial.print(F("RootU8: "));
	Serial.println(DurationRoot8);
	Serial.print(F("Square16: "));
	Serial.println(DurationPowerSquare16);
	Serial.print(F("Cube16: "));
	Serial.println(DurationPowerCube16);
	Serial.print(F("RootU16: "));
	Serial.println(DurationRoot16);
	Serial.println();
	Serial.println(DummyValue);
}