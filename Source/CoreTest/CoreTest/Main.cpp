#include "../../Core/Core/CoreInclude.h"

//class Test
//{
//	DECLARE_MEMORY_POOL(Test);
//
//public:
//	int i = 0;
//	int k = 0;
//};
//
//IMPLEMENAT_MEMORY_POOL(Test);

int main(void)
{
	CoreParallelArray<size_t> arr;
	std::cout << arr.GetSize() << std::endl;

	//Test::InitMemoryPool();

	GET_INSTANCE(CoreTimeManager);

	//CORE_TIME_MANAGER.End();

	return 0;
}