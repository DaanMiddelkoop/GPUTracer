#include "Vector3f.h"
#include <math.h>

Vector3f::Vector3f(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

bool Vector3f::equals(Vector3f other) {
    return other.x == x && other.y == y && other.z == z;
}

void Vector3f::set(Vector3f other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
}

Vector3f Vector3f::cross(Vector3f other) {
     return Vector3f(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}

float Vector3f::length() {
    return sqrt(x * x + y * y + z * z);
}

void Vector3f::normalize() {
    x = x / length();
    y = y / length();
    z = z / length();
}
