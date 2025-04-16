#pragma once

#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>

#include "resource.h"

// WinGDI Library 
#pragma comment(lib, "Msimg32.lib")


// GDI Plus
#include <objidl.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;

// 사운드
#include <mmsystem.h>
#include <dsound.h>
#include <dinput.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")


// 자료구조
#include <vector>
#include <list>
#include <map>
#include <algorithm>

// 문자열
#include <string>
using namespace std;

// Engine 자체 헤더
#include "global.h"