#pragma once

#define CORE_DATA_TYPE_MANAGER GET_INSTANCE(CoreDataTypeManager)

#define BOOL_NAME "bool"
#define UINT8_NAME "uint8_t"
#define UINT16_NAME "uint16_t"
#define UINT32_NAME "uint32_t"
#define UINT64_NAME "uint64_t"
#define INT8_NAME "int8_t"
#define INT16_NAME "int16_t"
#define INT32_NAME "int32_t"
#define INT64_NAME "int64_t"
#define SIZE_T_NAME "size_t"
#define FLOAT_NAME "float_t"
#define DOUBLE_NAME "double_t"
#define STRING_NAME "string"
#define CHAR_NAME "char"

#define SIZE_OF_BOOL sizeof(bool)
#define SIZE_OF_UINT8 sizeof(uint8_t)
#define SIZE_OF_UINT16 sizeof(uint16_t)
#define SIZE_OF_UINT32 sizeof(uint32_t)
#define SIZE_OF_UINT64 sizeof(uint64_t)
#define SIZE_OF_INT8 sizeof(int8_t)
#define SIZE_OF_INT16 sizeof(int16_t)
#define SIZE_OF_INT32 sizeof(int32_t)
#define SIZE_OF_INT64 sizeof(int64_t)
#define SIZE_OF_SIZE_T sizeof(size_t)
#define SIZE_OF_FLOAT sizeof(float_t)
#define SIZE_OF_DOUBLE sizeof(double_t)
#define SIZE_OF_STRING sizeof(std::string)

#define CHAR_TO_BOOL_REF *(bool*)
#define CHAR_TO_UINT8_REF *(uint8_t*)
#define CHAR_TO_UINT16_REF *(uint16_t*)
#define CHAR_TO_UINT32_REF *(uint32_t*)
#define CHAR_TO_UINT64_REF *(uint64_t*)
#define CHAR_TO_INT8_REF *(int8_t*)
#define CHAR_TO_INT16_REF *(int16_t*)
#define CHAR_TO_INT32_REF *(int32_t*)
#define CHAR_TO_INT64_REF *(int64_t*)
#define CHAR_TO_SIZE_T_REF *(size_t*)
#define CHAR_TO_FLOAT_REF *(float_t*)
#define CHAR_TO_DOUBLE_REF *(double_t*)
#define CHAR_TO_STRING_REF *(std::string*)

class CoreDataTypeManager
{
	DECLARE_SINGLETON(CoreDataTypeManager)

private:
	size_t GetSizeOfBool(void) const;
	size_t GetSizeOfUInt8(void) const;
	size_t GetSizeOfUInt16(void) const;
	size_t GetSizeOfUInt32(void) const;
	size_t GetSizeOfUInt64(void) const;
	size_t GetSizeOfInt8(void) const;
	size_t GetSizeOfInt16(void) const;
	size_t GetSizeOfInt32(void) const;
	size_t GetSizeOfInt64(void) const;
	size_t GetSizeOfSize_T(void) const;
	size_t GetSizeOfFloat(void) const;
	size_t GetSizeOfDouble(void) const;
	size_t GetSizeOfString(void) const;

public:
	size_t GetSizeOfType(std::string_view dataType) const;

private:
	typedef size_t(CoreDataTypeManager::* FUNC)() const;
	typedef std::map<std::string, FUNC> ProcessingMap;
	ProcessingMap getSizeOfTypeMap;
};