#include "prefab/PhysicsSceneLib.h"
#include "comp/Model.h"
#include "comp/BrickWallKey.h"
#include "Tags.h"

void prefab::Appliance(Entity entity, Entity modelAsset, int floor, StringView applianceName) {
    String floorName = "";
    if (floor == 0) {
        floorName = "Basement";
    } else if (floor == 1) {
        floorName = "First floor";
    } else if (floor == 2) {
        floorName = "Second floor";
    }
    prefab::StaticBody(entity, modelAsset);
    hub::Modify(entity)
        .Add<comp::BrickWall>().progress(100).name(tun::formatToString("{}. {}", floorName, applianceName)).Next()
        .Tag<tag::Hacker>();
}
