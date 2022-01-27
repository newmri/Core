#pragma once

class CoreLoader : public CoreObj
{
	OVERRIDE_OBJ(CoreLoader)

protected:
	virtual void PreLoad(std::string_view filePath, char*& out);

private:
	virtual void Load(char*& out) = 0;

public:
	virtual size_t Load(std::string_view filePath, char*& out) = 0;

protected:
	void Open(void);
	void Rewind(void);
	void Close(void);
	void LogLoadingStart(void);
	void LogLoadingEnd(void);
	void Parse(std::string& in, char* out);
	void Parse(std::string& in, const size_t dataTypeIndex, char* out);

private:
	std::string Parse(std::string& in);
	void Parse(std::string& in,int& out);
	void Parse(std::string& in,long& out);
	void Parse(std::string& in,long long& out);
	void Parse(std::string& in,unsigned long& out);
	void Parse(std::string& in,unsigned long long& out);
	void Parse(std::string& in,float& out);
	void Parse(std::string& in,double& out);
	void Parse(std::string& in,long double& out);

protected:
	void ParseDataTypesAndCalRowSize(std::string& strForParse);
	void Parse(std::string& in, std::string& out);

protected:
	std::string delimiter;
	std::string_view filePath;
	std::fstream fileStream;
	size_t rows = 0, columns = 0;
	size_t rowSize = 0;
	std::unique_ptr<std::string[]> dataTypes;
private:
	std::string logStart, logEnd;
};