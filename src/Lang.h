#pragma once
#include "tun/tunstring.h"

enum class Lang : int {
    eng = 0,
    rus = 1,
};

struct LangString {
    String variants[2] {};
    StringView Get() const;
    operator StringView() const;
};

namespace LangStrings {
    static LangString play {"I am ready", "Я готов"};
    static LangString replay {"Again", "Ещё раз"};
    static LangString currentLang {"English", "Русский"};
    static LangString tooltip {"Tooltip", "Подсказка"};
    static LangString mouseSense {"Mouse sensitivity", "Чувствительность мыши"};
    static LangString soundVolume {"Sound Volume", "Громкость звуков"};
    static LangString musicVolume {"Music Volume", "Громкость музыки"};

    static LangString putDown {"Drop", "Положить"};
    static LangString open {"Open", "Открыть"};
    static LangString close {"Close", "Закрыть"};
    static LangString pull {"Pull", "Потянуть"};
    static LangString take {"Take", "Взять"};
    static LangString placeInto {"Put", "Поставить"};
    static LangString wrongPiece {"Wrong part", "Не подходит"};
    static LangString needAPart {"Need a part", "Нужна деталь"};

    static LangString subtitle0 {"I need to keep going forward", "Надо продолжать идти вперёд"};
    static LangString subtitle1 {"It's so damn cold", "Так чертовски холодно"};
    static LangString subtitle2 {"This is the cruelest winter in my life", "Это самая свирепая зима в моей жизни"};
    static LangString subtitle3 {"I think there is a light up ahead", "Кажется, впереди есть свет"};
    static LangString subtitle4 {"This is a big old brick factory", "Это большой старый кирпичный завод"};
    static LangString subtitle5 {"I need to push forward, or I'm going to freeze to death", "Нужно идти вперёд, иначе я окоченею"};

    static LangString subtitle6 {"The doors opened by themselves", "Двери открылись сами собой"};
    static LangString subtitle7 {"Inside is nearly as cold, but there's power", "Внутри почти так же холодно, но есть электричество"};
    static LangString subtitle8 {"There are instructions on the wall", "На стене есть инструкция"};
    static LangString subtitle9 {"I hope the working generator will warm me up", "Надеюсь, запущенный генератор сможет меня согреть"};

    static LangString subtitleGear {"This is where the gear goes", "Здесь должна быть шестерёнка"}; // first look at the "need a part"
    static LangString subtitleGear1 {"I have to fix this machine", "Мне нужно починить механизм"};

    static LangString subtitleGearSet {"The gear is set, but it's not getting any warmer", "Шестерёнка на месте, но теплее не стало"}; // first gear is set
    static LangString subtitleGearSet1 {"I need to somehow power up this machine", "Я должен как-то запустить механизм"};

    static LangString subtitleGateOpen {"The machine is working! The gate just opened", "Механизм заработал! Открылся люк"}; // gate opened
    static LangString subtitleGateOpen1 {"This factory is enormous inside", "Этот завод внутри просто огромный"};

    static LangString subtitleStairwell {"The stairwell looks like it came from a fever dream", "Лестница выглядит как порождение лихорадочного сна"}; // distance to furnace is smaller than 20
    static LangString subtitleStairwell1 {"The cold starts to get in my bones", "Холод пробирает до самых костей"}; // 

    static LangString subtitleFurnaceDoorOpen {"There she is! The Furnace!", "Вот она! Печка!"}; // furnace door opened
    static LangString subtitleFurnaceDoorOpen1 {"Now I'll finally stop shivering", "Наконец-то я перестану дрожать от холода"};

    static LangString subtitleOutro {"What the hell is this on the floor?", "Чёрт, что это вокруг на полу?"}; // outro
    static LangString subtitleOutro1 {"I'm next. And that... was orchestrated perfectly", "Я следующий. Система работает безупречно."};

    //static LangString subtitle16 {"There has to be a lever here", "Рядом должен быть рычаг"};

    static LangString instruction0 {"Instructions", "Инструкции"};
    static LangString instruction1 {"Fix the machine", "Почините машину"};
    static LangString instruction2 {"Power up the generator", "Запустите генератор"};
};
