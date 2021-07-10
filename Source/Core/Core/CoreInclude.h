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

#include "CoreMacro.h"
#include "CoreDataType.h"
#include "CoreObj.h"
#include "CoreLogInclude.h"
#include "CoreMemoryPool.h"
#include "CoreMemoryPoolManager.h"
#include "CoreDummyManager.h"
#include "CoreTimeManager.h"
#include "CoreRandomManager.h"
#include "CoreParallelSTL.h"
#include "CoreParallelArray.h"