#include "Sphere.h"
#include "Rayon.h"

#include "libs/easyppm/easyppm.h"
#include "Lampe.h"

#include <optional>
#include <list>

bool hit(Sphere &s, Rayon& r, std::optional<float> &t)
{
    vec3 A = r._origin;
    vec3 B = r._direction;
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

unsigned char getColorFromDistance(float distance)
{
    return 255 - distance;
}

int main() {
    PPM ppm = easyppm_create(800, 600, IMAGETYPE_PPM);

    Sphere s = {vec3(400, 300, 400), 100.0};
    Sphere s2 = {vec3(500, 250, 200), 50.0};
    std::list<Sphere> spheres = {s, s2};
    Lampe lampe(vec3(300, 500, 400), 1.0, {255,255,0});

    for (int x = 0; x < 800; x++) {
        for (int y = 0; y < 600; y++) {
            Rayon r = {vec3(x, y, 0), vec3(0, 0, 1)};
            easyppm_set(&ppm, x, y, {0, 0, 0});

            for (auto sphere : spheres) {
                std::optional<float> t;
                if (hit(sphere, r, t) && t.has_value()) {
                    vec3 hitPoint = r._origin + t.value() * r._direction;
                    vec3 dirLampe = lampe._position - hitPoint;
                    float distLampe = dirLampe.length();

                    dirLampe.make_unit_vector();

                    Rayon rayVisibility = {hitPoint + 0.0001 * dirLampe, dirLampe};
                    
                    std::optional<float> t2;
                    if (hit(sphere, rayVisibility, t2) && t2.has_value()) {
                        vec3 pointIntersection = rayVisibility._origin + t2.value() * rayVisibility._direction;
                        if (t2.value() < distLampe) {
                            vec3 normal = unit_vector(pointIntersection - sphere._center);
                            float cosAngle = dot(normal, dirLampe) / (normal.length() * dirLampe.length());
                            unsigned char c = getColorFromDistance(t2.value());
                            easyppm_set(&ppm, x, y, {c, c, 0});
                        } else {
                            easyppm_set(&ppm, x, y, {0, 0, 0});
                        }
                    }
                }
            }
        }
    }
    easyppm_write(&ppm, "test.ppm");
}