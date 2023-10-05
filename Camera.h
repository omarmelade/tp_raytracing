//
// Created by omark on 05/10/2023.
//

#ifndef TP_SYNTHESE_RAYTRACING_CAMERA_H
#define TP_SYNTHESE_RAYTRACING_CAMERA_H

#include "libs/vec3/vec3.h"

class Camera {

public:
    Camera() {}
    Camera(float focalLength, float viewportWidth, float viewportHeight, point3& cameraCenter)
        : _focalLength(focalLength), _viewportWidth(viewportWidth), _viewportHeight(viewportHeight), _cameraCenter(cameraCenter) {}

private:
    float _focalLength;
    float _viewportWidth;
    float _viewportHeight;
    point3 _cameraCenter;
};

#endif //TP_SYNTHESE_RAYTRACING_CAMERA_H
