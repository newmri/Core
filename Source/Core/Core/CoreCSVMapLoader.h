#pragma once

class CoreCSVMapLoader : public CoreCSVLoader
{
	OVERRIDE_OBJ(CoreCSVMapLoader)

private:
	virtual void PreLoad(std::string_view filePath, char*& out) override;
	virtual void Load(char*& out) override;

public:
	virtual size_t Load(std::string_view filePath, char*& out) override;

private:
	size_t offset = 0;
};