#pragma once

#include "libs/vec3/vec3.h"

class Rayon {
public:
    Rayon() {}
    Rayon(const point3& origin, const vec3& direction) : _origin(origin), _direction(direction) {}

    point3 origin() const  { return _origin; }
    vec3 direction() const { return _direction; }

    point3 at(double t) const {
        return _origin + t*_direction;
    }
private:
    point3 _origin;
    vec3 _direction;
};

