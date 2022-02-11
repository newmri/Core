#include "CoreInclude.h"

IMPLEMENT_SINGLETON(CoreDataTypeManager)

void CoreDataTypeManager::Init(void)
{
	getSizeOfTypeMap.insert(ProcessingMap::value_type(std::string(GET_INT_NAME), &CoreDataTypeManager::GetSizeOfInt));
	getSizeOfTypeMap.insert(ProcessingMap::value_type(std::string(GET_LONG_LONG_NAME), &CoreDataTypeManager::GetSizeOfLongLong));
	getSizeOfTypeMap.insert(ProcessingMap::value_type(std::string(GET_SIZE_T_NAME), &CoreDataTypeManager::GetSizeOfSize_T));
	getSizeOfTypeMap.insert(ProcessingMap::value_type(std::string(GET_FLOAT_NAME), &CoreDataTypeManager::GetSizeOfFloat));
	getSizeOfTypeMap.insert(ProcessingMap::value_type(std::string(GET_DOUBLE_NAME), &CoreDataTypeManager::GetSizeOfDouble));
	getSizeOfTypeMap.insert(ProcessingMap::value_type(std::string(GET_STRING_NAME), &CoreDataTypeManager::GetSizeOfString));
}

void CoreDataTypeManager::Release(void)
{
	GetInstance().~CoreDataTypeManager();
}

size_t CoreDataTypeManager::GetSizeOfInt(void) const
{
	return SIZE_OF_INT;
}

size_t CoreDataTypeManager::GetSizeOfLongLong(void) const
{
	return SIZE_OF_LONG_LONG;
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