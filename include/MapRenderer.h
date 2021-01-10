//
// Created by paolo on 10/01/21.
//

#ifndef MARIO_MAKER_MAPRENDERER_H
#define MARIO_MAKER_MAPRENDERER_H
#include <raylib.h>
#include "Map.h"
#include "MapCamera.h"


class MapRenderer {
public:

    MapRenderer(Map *map);

    void render();

    virtual ~MapRenderer();

private:

    void drawGraphicsLayer(unsigned int** mapToRender);

private:
    Map map_;
};


#endif //MARIO_MAKER_MAPRENDERER_H
