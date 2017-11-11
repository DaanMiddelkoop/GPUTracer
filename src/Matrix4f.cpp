#include "Matrix4f.h"

Matrix4f::Matrix4f()
{
    //ctor
}

Matrix4f::set(Matrix4f other) {
    this->m00 = other.m00;
    this->m01 = other.m01;
    this->m02 = other.m02;
    this->m03 = other.m03;
    this->m10 = other.m10;
    this->m11 = other.m11;
    this->m12 = other.m12;
    this->m13 = other.m13;
    this->m20 = other.m20;
    this->m21 = other.m21;
    this->m22 = other.m22;
    this->m23 = other.m23;
    this->m30 = other.m30;
    this->m31 = other.m31;
    this->m32 = other.m32;
    this->m33 = other.m33;
}

Matrix4f::mul(Matrix4f other) {
    this->m00 = this->m00 * other->m00 + this->m01
}
