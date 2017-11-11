#include "View.h"
#include <math.h>

View::View()
{
    //ctor
}

void View::setPosition(Vector3f position) {
    this->position.set(position);
}

void View::setHorizontalDirection(float angle) {
    this->horizontal_direction = angle;
}

float View::getHorizontalDirection() {
    return horizontal_direction;
}

void View::setVerticalDirection(float direction) {
    this->vertical_direction = direction;
}

float View::getVerticalDirection() {
    return vertical_direction;
}

Vector3f View::getDirection() {
    return direction;
}

Vector3f View::getPosition() {
    return position;
}

Vector3f View::getNormalX() {
    return normal_x;
}

Vector3f View::getNormalY() {
    return normal_y;
}

Vector3f View::getEyeRay(float x, float y) {
    Vector3f ray = Vector3f();
    ray.x = direction.x + normal_x.x * x + normal_y.x * y;
    ray.y = direction.y + normal_x.y * x + normal_y.y * y;
    ray.z = direction.z + normal_x.z * x + normal_y.z * y;
    return ray;
}

void View::process() {
    direction.x = cos(vertical_direction) * cos(horizontal_direction);
    direction.y = sin(vertical_direction);
    direction.z = cos(vertical_direction) * sin(horizontal_direction);

    normal_x.x = -sin(horizontal_direction);
    normal_x.y = 0.0f;
    normal_x.z = cos(horizontal_direction);

    normal_y = normal_x.cross(direction);
    normal_x.normalize();
    normal_y.normalize();
}
