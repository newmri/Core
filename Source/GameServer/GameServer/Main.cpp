#include "Include.h"

void Input(void)
{
	bool IsRunning = true;
	std::string command;
	command.reserve(CORE_SMALL_SIZE);

	while (IsRunning)
	{
		std::cout <<"Input Command: ";
		std::cin >> command;

		switch (HashCode(command.c_str()))
		{
		case HashCode("/quit"):
			IsRunning = false;
			break;
		case HashCode("/help"):
			CORE_CONSOLE_LOG.Log("/quit: quit server");
			break;
		default:
			CORE_CONSOLE_LOG.Log("Input /help");
			break;
		}
	}
}

void Run(void)
{
	GAME_SERVER.Run();

	Input();

	GAME_SERVER.Stop();
}

struct MapData
{
	MapData() CORE_DEFAULT;
	~MapData()
	{
		SAFE_DELETE_2_ARRAY(path, count.y);
		SAFE_DELETE_2_ARRAY(objects, count.y);
	}

	NativeInfo::Vec2Int min;
	NativeInfo::Vec2Int max;
	NativeInfo::Vec2Int count;
	int32_t** path = nullptr;
	int64_t** objects = nullptr;
};

int main(void)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	CORE_THREAD_MANAGER.Push(Run);
	CORE_THREAD_MANAGER.Run();

	//MapData data;
	//CSV_MAP_LOAD_AND_TO_STRUCTURE("Data/Map_001.csv", data);

	return 0;
}