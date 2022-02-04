#include "CoreInclude.h"

CoreLoader::CoreLoader()
{
	Init();
}

CoreLoader::~CoreLoader()
{

}

void CoreLoader::Init(void)
{
	this->logStart.reserve(CORE_BIG_SIZE);
	this->logEnd.reserve(CORE_BIG_SIZE);
}

void CoreLoader::PreLoad(std::string_view filePath, char*& out)
{
	this->filePath = filePath;
	this->dataTypes.reset();
	this->rows = 0;
	this->columns = 0;
	this->rowSize = 0;

	Open();

	LogLoadingStart();
}

void CoreLoader::Open()
{
	this->fileStream.open(std::string(this->filePath).c_str(), std::ios::in);

	if (!this->fileStream.is_open())
	{
		throw std::invalid_argument(CORE_LOG.MakeLog(LogType::LOG_ERROR, filePath, __FILE__, __FUNCTION__, __LINE__));
	}
}

void CoreLoader::Rewind(void)
{
	this->fileStream.seekg(0, std::ios::beg);
}

void CoreLoader::Close(void)
{
	this->fileStream.close();
}

void CoreLoader::LogLoadingStart(void)
{
	this->logStart = "[";
	this->logStart.append(this->filePath);
	this->logStart.append(" is loading...]");

	CORE_LOG.Log(LogType::LOG_INFO, this->logStart);
}

void CoreLoader::LogLoadingEnd(void)
{
	this->logEnd = "[";

	this->logEnd.append(this->filePath);
	this->logEnd.append(" is loaded...]");

	CORE_LOG.Log(LogType::LOG_INFO, this->logEnd);
}

void CoreLoader::Parse(std::string& in, char* out)
{
	for (size_t i = 0; i < this->columns; ++i)
	{
		Parse(in, i, out);
		out += CORE_DATA_TYPE_MANAGER.GetSizeOfType(this->dataTypes[i]);
	}
}

void CoreLoader::Parse(std::string& in, const size_t dataTypeIndex, char* out)
{
#if _DEBUG
	unsigned int hashCode = HashCode(this->dataTypes[dataTypeIndex].c_str());

	if (hashCode == HashCode(GET_INT_NAME))
	{
		Parse(in, CHAR_TO_INT_REF out);
	}

	else if (hashCode == HashCode(GET_LONG_LONG_NAME))
	{
		Parse(in, CHAR_TO_LONG_LONG_REF out);
	}

	else if (hashCode == HashCode(GET_FLOAT_NAME))
	{
		Parse(in, CHAR_TO_FLOAT_REF out);
	}

	else if (hashCode == HashCode(GET_DOUBLE_NAME))
	{
		Parse(in, CHAR_TO_DOUBLE_REF out);
	}

	else if (hashCode == HashCode(GET_SIZE_T_NAME))
	{
		Parse(in, CHAR_TO_SIZE_T_REF out);
	}

	else if (hashCode == HashCode(GET_STRING_NAME))
	{
		Parse(in, CHAR_TO_STRING_REF out);
	}
#else
	switch (HashCode(this->dataTypes[dataTypeIndex].c_str()))
	{
	case HashCode(GET_INT_NAME):
		Parse(in, CHAR_TO_INT_REF out);
		break;
	case HashCode(GET_LONG_LONG_NAME):
		Parse(in, CHAR_TO_LONG_LONG_REF out);
		break;
	case HashCode(GET_SIZE_T_NAME):
		Parse(in, CHAR_TO_SIZE_T_REF out);
		break;
	case HashCode(GET_FLOAT_NAME):
		Parse(in, CHAR_TO_FLOAT_REF out);
		break;
	case HashCode(GET_DOUBLE_NAME):
		Parse(in, CHAR_TO_DOUBLE_REF out);
		break;
	case HashCode(GET_STRING_NAME):
		Parse(in, CHAR_TO_STRING_REF out);
		break;
	}
#endif // _DEBUG
}

std::string CoreLoader::Parse(std::string& in)
{
	std::string str;
	str.reserve(CORE_BIG_SIZE);
	std::string::size_type findPos = 0;

	findPos = in.find(this->delimiter);

	for (std::string::size_type pos = 0; pos < findPos; ++pos)
	{
		str.push_back(in[pos]);
	}

	in.erase(0, findPos + 1);

	return str;
}

void CoreLoader::Parse(std::string& in, int& out)
{
	out = stoi(Parse(in));
}

void CoreLoader::Parse(std::string& in, long& out)
{
	out = stol(Parse(in));
}

void CoreLoader::Parse(std::string& in, long long& out)
{
	out = stoll(Parse(in));
}

void CoreLoader::Parse(std::string& in, unsigned long& out)
{
	out = stoul(Parse(in));
}

void CoreLoader::Parse(std::string& in, unsigned long long& out)
{
	out = stoull(Parse(in));
}

void CoreLoader::Parse(std::string& in, float& out)
{
	out = stof(Parse(in));
}

void CoreLoader::Parse(std::string& in, double& out)
{
	out = stod(Parse(in));
}

void CoreLoader::Parse(std::string& in, long double& out)
{
	out = stold(Parse(in));
}

void CoreLoader::ParseDataTypesAndCalRowSize(std::string& strForParse)
{
	this->dataTypes = std::make_unique<std::string[]>(this->columns);

	for (size_t i = 0; i < this->columns; ++i)
	{
		this->dataTypes[i] = Parse(strForParse);
	}

	size_t lastIndex = this->columns - 1;

	this->dataTypes[lastIndex] = STRING_MANAGER.ReplaceAll(this->dataTypes[lastIndex], "\n", "");

	this->rowSize = 0;

	for (size_t i = 0; i < this->columns; ++i)
	{
		this->rowSize += CORE_DATA_TYPE_MANAGER.GetSizeOfType(this->dataTypes[i]);
	}
}

void CoreLoader::Parse(std::string& in, std::string& out)
{
	out = Parse(in);
}

