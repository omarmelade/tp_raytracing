//
// Created by okhial on 29/09/2023.
//

#pragma once

#include "libs/vec3/vec3.h"
#include "libs/easyppm/easyppm.h"

class Lampe
{
public:
    Lampe(point3 pos, float intensite, ppmcolor color) : _position(pos) , _intensite(intensite), _color(color) {}

    inline const point3& position() { return _position; }
    inline const float intensite() { return _intensite; }

private:
    point3 _position;
    float _intensite;
    ppmcolor _color = {255, 0, 0};
};
