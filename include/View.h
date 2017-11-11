#ifndef VIEW_H
#define VIEW_H

#include <Vector3f.h>


class View
{
    public:
        View();

        void process();
        void setHorizontalDirection(float horizontal_direction);
        void setVerticalDirection(float vertical_direction);
        void setPosition(Vector3f position);
        float getHorizontalDirection();
        float getVerticalDirection();
        Vector3f getDirection();
        Vector3f getPosition();
        Vector3f getNormalX();
        Vector3f getNormalY();
        Vector3f getEyeRay(float x, float y);



    protected:

    private:
        Vector3f normal_x;
        Vector3f normal_y;
        Vector3f direction;
        Vector3f position;
        float horizontal_direction;
        float vertical_direction;
};

#endif // VIEW_H
