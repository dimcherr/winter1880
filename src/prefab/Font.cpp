#include "Prefab.h"
#include "comp/Font.h"

Entity prefab::Font(StringView path, float fontSize) {
    return hub::Create()
        .Add<comp::Font>().path(path, fontSize).Next()
        .GetEntity();
}
