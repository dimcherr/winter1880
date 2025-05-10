#include "prefab/UIPrefabLib.h"
#include "comp/TransformComp.h"
#include "comp/Text3DComp.h"

Entity prefab::Text3D(LangString* text, Color color, Vec translation, Quat rotation, Vec scale) {
    Entity entity = hub::Create().GetEntity();

    auto& transform = hub::AddComp<TransformComp>(entity);
    transform.translation = translation;
    transform.rotation = rotation;
    transform.scale = scale;
    transform.Update();

    auto& textComp = hub::AddComp<Text3DComp>(entity);
    textComp.langString = text;
    textComp.text = textComp.langString->Get();
    textComp.font = State::Get().regularFont;
    textComp.color = color;

    return entity;
}
