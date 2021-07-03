#include "cstr.h"

char uintToStringOutput[128];

const char* to_string(uint64_t value) {
	
	uint8_t size;
	uint64_t sizeTest = value;

	while (sizeTest / 10 > 0) {
		sizeTest /= 10;
		size++;
	}

	uint8_t index = 0;

	while (value / 10 > 0) {

		uint8_t remainder = value % 10;
		value /= 10;

		uintToStringOutput[size - index] = remainder + '0';
		index++;

	}

	uint8_t remainder = value % 10;
	uintToStringOutput[size - index] = remainder + '0';	
	uintToStringOutput[size + 1] = 0;

	return uintToStringOutput;

}

char hexToStringOutput[128];

const char* to_hstring(uint64_t value) {

	uint64_t* valPtr = &value;
	uint8_t* ptr;
	uint8_t temp;
	uint8_t size = 8 * 2 - 1;

	for (uint8_t i = 0; i < size; i++) {

		ptr = ((uint8_t*)valPtr + i);
		temp = ((*ptr & 0xF0) >> 4);
		hexToStringOutput[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : '0');
		temp = ((*ptr & 0x0F));
		hexToStringOutput[size - (i * 2)] = temp + (temp > 9 ? 55 : '0');

	}

	hexToStringOutput[size + 1] = 0;
	return hexToStringOutput;

}

char hexToStringOutput32[128];

const char* to_hstring(uint32_t value) {

	uint32_t* valPtr = &value;
	uint8_t* ptr;
	uint8_t temp;
	uint8_t size = 4 * 2 - 1;

	for (uint8_t i = 0; i < size; i++) {

		ptr = ((uint8_t*)valPtr + i);
		temp = ((*ptr & 0xF0) >> 4);
		hexToStringOutput32[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : '0');
		temp = ((*ptr & 0x0F));
		hexToStringOutput32[size - (i * 2)] = temp + (temp > 9 ? 55 : '0');

	}

	hexToStringOutput32[size + 1] = 0;
	return hexToStringOutput32;

}

char hexToStringOutput16[128];

const char* to_hstring(uint16_t value) {

	uint16_t* valPtr = &value;
	uint8_t* ptr;
	uint8_t temp;
	uint8_t size = 2 * 2 - 1;

	for (uint8_t i = 0; i < size; i++) {

		ptr = ((uint8_t*)valPtr + i);
		temp = ((*ptr & 0xF0) >> 4);
		hexToStringOutput16[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : '0');
		temp = ((*ptr & 0x0F));
		hexToStringOutput16[size - (i * 2)] = temp + (temp > 9 ? 55 : '0');

	}

	hexToStringOutput16[size + 1] = 0;
	return hexToStringOutput16;

}

char hexToStringOutput8[128];

const char* to_hstring(uint8_t value) {

	uint8_t* valPtr = &value;
	uint8_t* ptr;
	uint8_t temp;
	uint8_t size = 1 * 2 - 1;

	for (uint8_t i = 0; i < size; i++) {

		ptr = ((uint8_t*)valPtr + i);
		temp = ((*ptr & 0xF0) >> 4);
		hexToStringOutput8[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : '0');
		temp = ((*ptr & 0x0F));
		hexToStringOutput8[size - (i * 2)] = temp + (temp > 9 ? 55 : '0');

	}

	hexToStringOutput8[size + 1] = 0;
	return hexToStringOutput8;

}

char intToStringOutput[128];

const char* to_string(int64_t value) {
	
	uint8_t isNegative = 0;

	if (value < 0) {
		isNegative = 1;
		value *= -1;
		intToStringOutput[0] = '-';
	}

	uint8_t size;
	uint64_t sizeTest = value;

	while (sizeTest / 10 > 0) {
		sizeTest /= 10;
		size++;
	}

	uint8_t index = 0;

	while (value / 10 > 0) {

		uint8_t remainder = value % 10;
		value /= 10;

		intToStringOutput[isNegative + size - index] = remainder + '0';
		index++;

	}

	uint8_t remainder = value % 10;
	intToStringOutput[isNegative + size - index] = remainder + '0';	
	intToStringOutput[isNegative + size + 1] = 0;

	return intToStringOutput;

}

char doubleToStringOutput[128];

const char* to_string(double value, uint8_t decimalPlaces) {
	
	if (decimalPlaces > 20) decimalPlaces = 20;

	char* intPtr = (char*) to_string((int64_t)value);
	char* doublePointer = doubleToStringOutput;

	if (value < 0) {
		value *= -1;
	}

	while (*intPtr != 0) {
		*doublePointer = *intPtr;
		doublePointer++;
		intPtr++;
	}

	*doublePointer = '.';
	doublePointer++;

	double newValue = value - (int)value;

	for (uint8_t i = 0; i < decimalPlaces; i++) {
		newValue *= 10;
		*doublePointer = (int)newValue + '0';
		newValue -= (int)newValue;
		doublePointer++;
	} 

	*doublePointer = 0;

	return doubleToStringOutput;

}

const char* to_string(double value) {
	return to_string(value, 2);
}