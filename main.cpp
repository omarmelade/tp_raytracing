#include "Sphere.h"
#include "Rayon.h"

#include "libs/easyppm/easyppm.h"
#include "Lampe.h"
#include "Camera.h"
#include "color.h"

#include <optional>
#include <list>

bool hit(Sphere &s, Rayon& r, std::optional<float> &t)
{
    vec3 A = r.origin();
    vec3 B = r.direction();
    vec3 C = s._center;
    float R = s._radius;

    float a = dot(B, B);
    float b = 2 * dot(A - C, B);
    float c = dot(A - C, A - C) - (R * R);
    float d = sqrt((b * b) - (4 * a * c));
    if (d >= 0) {
        float t_min = (-b - d) / (2 * a);
        float t_max = (-b + d) / (2 * a);
        t = t_min > 0 ? t_min : t_max;
        return true;
    }
    return false;
}

float distance(vec3 v1, vec3 v2)
{
    return sqrt(pow(v2.x() - v1.x(), 2) + pow(v2.y() - v1.y(), 2) + pow(v2.z() - v1.z(), 2));
}

color rayColor(const Rayon& r) {
    vec3 unitDirection = unit_vector(r.direction());
    auto a = 0.5*(unitDirection.y() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

unsigned char getColorFromDistance(float distance) {
    return 255 - distance;
}

int main() {

    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int imageWidth = 800;

    // Calculate height
    int imageHeight = static_cast<int>(imageWidth / aspect_ratio);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;

    // Camera
    float focalLength = 1.0;
    float viewport_height = 2.0;
    float viewport_width = viewport_height * (static_cast<float>(imageWidth)/imageHeight);
    auto camera_center = point3(0, 0, 0);
    Camera camera(focalLength, viewport_width, viewport_height, camera_center);

    PPM ppm = easyppm_create(imageWidth, imageHeight, IMAGETYPE_PPM);
    Sphere s = {vec3(400, 300, 400), 100.0};
    std::list<Sphere> spheres = {s};
    Lampe lampe(vec3(300, 500, 400), 1.0, {255,255,0});

    for (int j = 0; j < imageHeight; j++) {
        std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
        for (int i = 0; i < imageWidth; i++) {
            Rayon r = {vec3(i, j, 0), vec3(0, 0, 1)};
            easyppm_set(&ppm, i, j, {0, 0, 0});

            for (auto sphere : spheres) {
                std::optional<float> t;
                if (hit(sphere, r, t) && t.has_value()) {
                    vec3 hitPoint = r.origin() + t.value() * r.direction();
                    vec3 dirLampe = lampe.position() - hitPoint;
                    float distLampe = dirLampe.length();

                    dirLampe.make_unit_vector();

                    Rayon rayVisibility = {hitPoint + 0.0001 * dirLampe, dirLampe};
                    
                    std::optional<float> t2;
                    if (hit(sphere, rayVisibility, t2) && t2.has_value()) {
                        vec3 pointIntersection = rayVisibility.origin() + t2.value() * rayVisibility.direction();
                        if (t2.value() < distLampe) {
                            vec3 normal = unit_vector(pointIntersection - sphere._center);
                            float cosAngle = dot(normal, dirLampe) / (normal.length() * dirLampe.length());
                            unsigned char c = getColorFromDistance(t2.value());
                            easyppm_set(&ppm, i, j, {c, c, 0});
                        } else {
                            easyppm_set(&ppm, i, j, {0, 0, 0});
                        }
                    }
                }
            }
        }
    }
    easyppm_write(&ppm, "test.ppm");
    std::clog << "\rDone.                 \n";
}