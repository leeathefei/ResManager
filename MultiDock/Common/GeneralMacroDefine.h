#pragma once

#ifdef _COMMON_DLL
#define COMMON_DLLEXPORT __declspec(dllexport)
#else					
#define COMMON_DLLEXPORT
#endif
