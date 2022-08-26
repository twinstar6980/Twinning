#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#pragma once

#include <optional>
#include <vector>
#include <string>
#include <regex>
#include <filesystem>
#include <Windows.h>
#include <shellapi.h>
#include <wrl/module.h>
#include <wrl/implements.h>
#include <wrl/client.h>
#include <wil/resource.h>
#include <ShObjIdl_core.h>
#include <shellapi.h>

using Microsoft::WRL::RuntimeClass;
using Microsoft::WRL::RuntimeClassFlags;
using Microsoft::WRL::ClassicCom;
using Microsoft::WRL::ComPtr;
using Microsoft::WRL::Make;
using Microsoft::WRL::Module;
using Microsoft::WRL::InProc;

#define thiz (*this)

template <typename T>
using X = T;
