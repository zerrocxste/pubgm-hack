#include <Windows.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <array>
#include <limits>

#include <dwmapi.h>
#pragma comment (lib, "dwmapi.lib")

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <d3d9.h>
#pragma comment (lib, "d3d9")

#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"

#include "../DXWF/DXWF.h"

#ifdef _WIN64
#pragma comment (lib, "DXWF/x64/DXWF.lib")
#else
#pragma comment (lib, "DXWF/x86/DXWF.lib")
#endif

#include "math/martix4x4/matrix4x4.h"
#include "math/vector/vector.h"

#include "../memory_utils/memory_utils.h"

#include "global_vars/global_vars.h"
#include "vars/vars.h"

#include "draw-list/draw-list.h"
#include "utils/utils.h"
#include "overlay/overlay.h"
#include "features/features.h"