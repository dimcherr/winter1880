#pragma once
#include <cstdint>
#include "list.h"
#include "tunstring.h"

using Byte = char;
using Bytes = List<Byte>;

namespace tun {

String ReadFile(StringView path);
Bytes ReadFileBinary(StringView path);
bool WriteFile(StringView path, StringView content);

}