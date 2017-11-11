#ifndef VECTOR3F_H
#define VECTOR3F_H


class Vector3f
{
    public:
        Vector3f() {};
        Vector3f(float x, float y, float z);

        bool equals(Vector3f other);
        void set(Vector3f other);
        Vector3f cross(Vector3f other);
        void normalize();
        float length();

        float x, y, z;
        float len;
        bool refreshLength;

    protected:

    private:
};

#endif // VECTOR3F_H
