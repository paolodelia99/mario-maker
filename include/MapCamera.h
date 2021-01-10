//
// Created by paolo on 10/01/21.
//

#ifndef MARIO_MAKER_MAPCAMERA_H
#define MARIO_MAKER_MAPCAMERA_H
#include <raylib.h>

class MapCamera {
public:
    MapCamera(Vector2 position, float camWidth, float camHeight, float mapWidth, float mapHeight);

    MapCamera(Vector2 position, float camWidth, float camHeight, float mapWidth, float mapHeight, Vector2 offset);

    MapCamera operator+ (Vector2 offset) const;

private:
    Vector2 position_;
public:
    const Vector2 &getPosition() const;

private:
    float camWidth_;
    float camHeight_;
    float mapWidth_;
    float mapHeight_;
    Vector2 offset_;
};


#endif //MARIO_MAKER_MAPCAMERA_H
