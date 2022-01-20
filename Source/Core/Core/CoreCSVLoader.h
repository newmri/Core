#pragma once

class CoreCSVLoader : public CoreLoader
{
	OVERRIDE_OBJ(CoreCSVLoader)

private:
	virtual void PreLoad(std::string_view filePath, char*& out) override;
	virtual void Load(char*& out) override;

public:
	virtual size_t Load(std::string_view filePath, char*& out) override;
};