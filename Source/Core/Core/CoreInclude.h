#pragma once

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <cstdlib>
#endif // _DEBUG

#include <iostream>
#include <fstream>
#include <memory>
#include <shared_mutex>
#include <chrono>
#include <string>
#include <string_view>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <direct.h>
#include <type_traits>
#include <map> // 추후 제작한걸로 대체
#include <functional>

#include "CoreMacro.h"
#include "CoreDataType.h"
#include "CoreObj.h"
#include "CoreLogInclude.h"
#include "CoreMemoryPool.h"
#include "CoreMemoryPoolManager.h"
#include "CoreDummyManager.h"
#include "CoreTimeManager.h"
#include "CoreContainer.h"
#include "CoreArray.h"
#include "CoreVector.h"
#include "CoreList.h"
#include "CoreQueue.h"
#include "CorePriorityQueue.h"
#include "CoreAlgorithm.h"
#include "CoreRandomManager.h"
#include "CoreLoader.h"
#include "CoreCSVLoader.h"
#include "CoreLoadManager.h"
#include "CoreDataTypeManager.h"
#include "CoreStringManager.h"
#include "CoreTestManager.h"