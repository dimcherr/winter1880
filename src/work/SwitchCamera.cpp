#include "Work.h"
#include "tun/math.h"
#include "tun/log.h"
#include "tun/builder.h"
#include "comp/Camera.h"
#include "Tags.h"

void work::SwitchCamera() {
    using comp::Camera;
    using tag::Fly;
    using tag::FirstPerson;
    using tag::Current;

    Entity fly = hub::Reg().view<Fly, Camera>().back();
    Entity firstPerson = hub::Reg().view<FirstPerson, Camera>().back();

    if (hub::Reg().any_of<Current>(fly)) {
        hub::Reg().remove<Current>(fly);
        hub::Reg().emplace<Current>(firstPerson);
        State::Get().firstPerson = true;
        State::Get().flyMode = false;
    } else if (hub::Reg().any_of<Current>(firstPerson)) {
        hub::Reg().remove<Current>(firstPerson);
        hub::Reg().emplace<Current>(fly);
        State::Get().firstPerson = false;
        State::Get().flyMode = true;
    }
}
