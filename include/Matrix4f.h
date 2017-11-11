#ifndef MATRIX4F_H
#define MATRIX4F_H


class Matrix4f
{
    public:
        Matrix4f();
        float m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33;
        void set(Matrix4f other);
        void mul(Matrix4f other);


    protected:

    private:
};

#endif // MATRIX4F_H
