//
// Created by okhial on 29/09/2023.
//

#pragma once

#include "libs/vec3/vec3.h"
#include "libs/easyppm/easyppm.h"

class Lampe
{
public:
    Lampe(vec3 pos, float intensite, ppmcolor color);

    vec3 _position = {0, 0, 0};
    float _intensite = 1.0;
    ppmcolor _color = {255, 0, 0};
};
