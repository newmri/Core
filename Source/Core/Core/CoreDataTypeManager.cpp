#include "CoreInclude.h"

IMPLEMENT_SINGLETON(CoreDataTypeManager)

void CoreDataTypeManager::Init(void)
{
	getSizeOfTypeMap.insert(ProcessingMap::value_type(BOOL_NAME, &CoreDataTypeManager::GetSizeOfBool));
	getSizeOfTypeMap.insert(ProcessingMap::value_type(UINT8_NAME, &CoreDataTypeManager::GetSizeOfUInt8));
	getSizeOfTypeMap.insert(ProcessingMap::value_type(UINT16_NAME, &CoreDataTypeManager::GetSizeOfUInt16));
	getSizeOfTypeMap.insert(ProcessingMap::value_type(UINT32_NAME, &CoreDataTypeManager::GetSizeOfUInt32));
	getSizeOfTypeMap.insert(ProcessingMap::value_type(UINT64_NAME, &CoreDataTypeManager::GetSizeOfUInt64));
	getSizeOfTypeMap.insert(ProcessingMap::value_type(INT8_NAME, &CoreDataTypeManager::GetSizeOfInt8));
	getSizeOfTypeMap.insert(ProcessingMap::value_type(INT16_NAME, &CoreDataTypeManager::GetSizeOfInt16));
	getSizeOfTypeMap.insert(ProcessingMap::value_type(INT32_NAME, &CoreDataTypeManager::GetSizeOfInt32));
	getSizeOfTypeMap.insert(ProcessingMap::value_type(INT64_NAME, &CoreDataTypeManager::GetSizeOfInt64));
	getSizeOfTypeMap.insert(ProcessingMap::value_type(SIZE_T_NAME, &CoreDataTypeManager::GetSizeOfSize_T));
	getSizeOfTypeMap.insert(ProcessingMap::value_type(FLOAT_NAME, &CoreDataTypeManager::GetSizeOfFloat));
	getSizeOfTypeMap.insert(ProcessingMap::value_type(DOUBLE_NAME, &CoreDataTypeManager::GetSizeOfDouble));
	getSizeOfTypeMap.insert(ProcessingMap::value_type(STRING_NAME, &CoreDataTypeManager::GetSizeOfString));
}

void CoreDataTypeManager::Release(void)
{
	GetInstance().~CoreDataTypeManager();
}

size_t CoreDataTypeManager::GetSizeOfBool(void) const
{
	return SIZE_OF_BOOL;
}

size_t CoreDataTypeManager::GetSizeOfUInt8(void) const
{
	return SIZE_OF_UINT8;
}

size_t CoreDataTypeManager::GetSizeOfUInt16(void) const
{
	return SIZE_OF_UINT16;
}

size_t CoreDataTypeManager::GetSizeOfUInt32(void) const
{
	return SIZE_OF_UINT32;
}

size_t CoreDataTypeManager::GetSizeOfUInt64(void) const
{
	return SIZE_OF_UINT64;
}

size_t CoreDataTypeManager::GetSizeOfInt8(void) const
{
	return SIZE_OF_INT8;
}

size_t CoreDataTypeManager::GetSizeOfInt16(void) const
{
	return SIZE_OF_INT16;
}

size_t CoreDataTypeManager::GetSizeOfInt32(void) const
{
	return SIZE_OF_INT32;
}

size_t CoreDataTypeManager::GetSizeOfInt64(void) const
{
	return SIZE_OF_INT64;
}

size_t CoreDataTypeManager::GetSizeOfSize_T(void) const
{
	return SIZE_OF_SIZE_T;
}

size_t CoreDataTypeManager::GetSizeOfFloat(void) const
{
	return SIZE_OF_FLOAT;
}

size_t CoreDataTypeManager::GetSizeOfDouble(void) const
{
	return SIZE_OF_DOUBLE;
}

size_t CoreDataTypeManager::GetSizeOfString(void) const
{
	return SIZE_OF_STRING;
}

size_t CoreDataTypeManager::GetSizeOfType(std::string_view dataType) const
{
	auto iter = getSizeOfTypeMap.find(dataType.data());

	if (iter != getSizeOfTypeMap.end())
		return (this->*((*iter).second))();

	return 0;
}