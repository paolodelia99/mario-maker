#include "../include/Menu.h"

int main(int argc, char** argv) {

    const char *mapFilepath = nullptr;

    if (argc == 2) {
        mapFilepath = argv[1];
    }

    Menu menu(mapFilepath, 704, 416);

    menu.menuLoop();

    return 0;
}
