#pragma once

#include <stdint.h>
#include <stddef.h>

class ByteField
{
	size_t*  fieldSizes; // Maybe its type can be smaller
	size_t   fieldCnt;

	uint8_t* data;
	size_t*  fieldOffsets;

public:
	ByteField(size_t* fieldSizes, size_t fieldCnt);
	~ByteField();

	void WriteField(uint32_t field, void* data);
	void ReadField(uint32_t field, void* data);
};
