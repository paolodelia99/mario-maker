//
// Created by paolo on 10/01/21.
//

#include "MapCamera.h"

MapCamera::MapCamera(Vector2 position, float camWidth, float camHeight, float mapWidth, float mapHeight)
: position_(position), mapWidth_(mapWidth), mapHeight_(mapHeight), camHeight_(camHeight), camWidth_(camWidth)
{
    offset_ = Vector2{camWidth - position.x, camHeight - position.y};
}

MapCamera MapCamera::operator+(Vector2 offset) const
{
    float new_x = position_.x + offset.x;
    float new_y = position_.y + offset.y;
    return MapCamera(Vector2{new_x, new_y}, camWidth_, camHeight_, mapWidth_, mapHeight_, offset_);
}

MapCamera::MapCamera(Vector2 position, float camWidth, float camHeight, float mapWidth, float mapHeight,
                     Vector2 offset)
                     : position_(position), mapWidth_(mapWidth), mapHeight_(mapHeight), camHeight_(camHeight),
                     camWidth_(camWidth), offset_(offset)
{

}

const Vector2 &MapCamera::getPosition() const {
    return position_;
}

