//
// Created by EunwooSong on 2020-11-27.
//

#ifndef ZERO_ENGINE_STDAFX_H
#define ZERO_ENGINE_STDAFX_H

#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#pragma comment(lib, "imm32.lib");
#endif

//////////// DEBUG //////////
#if defined(_WIN32) || defined(_WIN64)
#include <crtdbg.h>
#endif

#include <string>
#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <list>
#include <queue>
#include <set>
#include <stdexcept>
#include <vector>
#include <chrono>
#include <mutex>
#include <sstream>
#include <fstream>
#include <locale>

////////// FONT //////////
#include <ft2build.h>
#include FT_FREETYPE_H


////////// OpenGL //////////
#define FREEGLUT_STATIC
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0

#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <gl/glut.h>


////////// GLM //////////
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"


////////// MACRO //////////
#define SAFE_DELETE(p)  \
	{                   \
		if (p) {        \
			delete (p); \
			(p) = nullptr;    \
		}               \
	}

#define SAFE_DELETE_ARRAY(p) \
	{                        \
		if (p) {             \
			delete[](p);     \
			(p) = nullptr;         \
		}                    \
	}


#if !(defined(_WIN32) || defined(_WIN64))
#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
#endif

#endif //ZERO_ENGINE_STDAFX_H
