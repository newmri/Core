#pragma once

#define CSV_LOAD GET_INSTANCE(CoreLoadManager<CoreCSVLoader>)
#define MAP_LOAD GET_INSTANCE(CoreLoadManager<CoreCSVMapLoader>)

template<typename T>
class CoreLoadManager
{
	DECLARE_SINGLETON(CoreLoadManager<T>)

public:
	size_t Load(std::string_view filePath, char*& out);

private:
	std::unique_ptr<CoreLoader> loader;
};

IMPLEMENT_TEMPLATE_SINGLETON(CoreLoadManager)

template<typename T>
void CoreLoadManager<T>::Init(void)
{
	loader = std::make_unique<T>();
}

template<typename T>
void CoreLoadManager<T>::Release(void)
{
	GetInstance().~CoreLoadManager();
}

template<typename T>
size_t CoreLoadManager<T>::Load(std::string_view filePath, char*& out)
{
	return loader->Load(filePath, out);
}