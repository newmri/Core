#pragma once

#define CORE_DATA_TYPE_MANAGER GET_INSTANCE(CoreDataTypeManager)

#define GET_SHORT_NAME "short"
#define GET_INT_NAME "int"
#define GET_LONG_LONG_NAME "long long"
#define GET_SIZE_T_NAME "size_t"
#define GET_FLOAT_NAME "float"
#define GET_DOUBLE_NAME "double"
#define GET_STRING_NAME "string"
#define GET_CHAR_NAME "char"

#define SIZE_OF_INT sizeof(int)
#define SIZE_OF_LONG_LONG sizeof(long long)
#define SIZE_OF_SIZE_T sizeof(size_t)
#define SIZE_OF_FLOAT sizeof(float)
#define SIZE_OF_DOUBLE sizeof(double)
#define SIZE_OF_STRING sizeof(std::string)

#define CHAR_TO_INT_REF *(int*)
#define CHAR_TO_LONG_LONG_REF *(long long*)
#define CHAR_TO_SIZE_T_REF *(size_t*)
#define CHAR_TO_FLOAT_REF *(float*)
#define CHAR_TO_DOUBLE_REF *(double*)
#define CHAR_TO_STRING_REF *(std::string*)

class CoreDataTypeManager
{
	DECLARE_SINGLETON(CoreDataTypeManager)

private:
	size_t GetSizeOfInt(void) const;
	size_t GetSizeOfLongLong(void) const;
	size_t GetSizeOfSize_T(void) const;
	size_t GetSizeOfFloat(void) const;
	size_t GetSizeOfDouble(void) const;
	size_t GetSizeOfString(void) const;

public:
	size_t GetSizeOfType(std::string_view dataType) const;
	SQLUSMALLINT GetSQLType(const char* const dataType) const;

private:
	typedef size_t(CoreDataTypeManager::* FUNC)() const;
	typedef std::map<std::string, FUNC> ProcessingMap;
	ProcessingMap getSizeOfTypeMap;
};