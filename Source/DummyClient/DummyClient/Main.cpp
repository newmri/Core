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
			CORE_CONSOLE_LOG.Log("/quit: quit DummyClient");
			CORE_CONSOLE_LOG.Log("/logincount: ShowConnectedLoginClientCount");
			CORE_CONSOLE_LOG.Log("/gamecount: ShowConnectedGameClientCount");
			break;
		case HashCode("/logincount"):
			DUMMY_CLIENT.ShowConnectedLoginClientCount();
			break;
		case HashCode("/gamecount"):
			DUMMY_CLIENT.ShowConnectedGameClientCount();
			break;
		default:
			CORE_CONSOLE_LOG.Log("Input /help");
			break;
		}
	}
}

void Run(void)
{
	DUMMY_CLIENT.Run();

	Input();

	DUMMY_CLIENT.Stop();
}

int32_t main(void)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	CORE_THREAD_MANAGER.Push(Run);
	CORE_THREAD_MANAGER.Run();

	return 0;
}