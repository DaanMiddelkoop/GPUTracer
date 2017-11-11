#include "Camera.h"

Camera::Camera()
{
    //ctor
}

Vector3f Camera::getDirection() {
    return direction;
}

/**
 * {@inheritDoc}
 */
void Camera::setDirection(Vector3f direction) {
    if (this->direction.equals(direction)) {
        return;
    }
    this->direction.set(direction);
    refreshViewMatrix = true;
    refreshInverseProjectionViewMatrix = true;
}

void Camera::doRefreshInverseProjectionViewMatrix() {
    invViewProjectionMatrix.set(getProjectionMatrix());
    invViewProjectionMatrix.mul(getViewMatrix());
    invViewProjectionMatrix.invert();
    refreshInverseProjectionViewMatrix = false;
}

Matrix4f Camera::getInverseProjectionViewMatrix() {
    if (refreshInverseProjectionViewMatrix) {
        doRefreshInverseProjectionViewMatrix();
    }
    return invViewProjectionMatrix;
}

void Camera::getEyeRay(float x, float y, Vector3f res) {
    tmp3.set(x, y, 0.0f, 1.0f);
    getInverseProjectionViewMatrix().transform(tmp3);
    tmp3.scale(1.0f / tmp3.w);
    tmp0.set(tmp3.x, tmp3.y, tmp3.z);
    res.set(tmp0);
    res.sub(position);
}

void Camera::setLookAt(Vector3f position, Vector3f lookAt, Vector3f up) {
    setPosition(position);
    tmp0.set(up);
    tmp0.normalize();
    tmp1.set(lookAt);
    tmp1.sub(position);
    tmp1.normalize();
    setDirection(tmp1);
    /* right = direction x up */
    right.cross(tmp1, tmp0);
    right.normalize();
    /* up = right x direction */
    tmp0.cross(right, tmp1);
    setUp(tmp0);
}

void Camera::doRefreshViewMatrix() {
    tmp0.set(direction);
    tmp0.normalize();
    tmp1.set(up);
    tmp1.normalize();
    /* right = direction x up */
    tmp1.cross(tmp0, tmp1);
    /* up = right x direction */
    tmp2.cross(tmp1, tmp0);
    Matrix4f m = viewMatrix;
    m.m00 = tmp1.x;
    m.m01 = tmp1.y;
    m.m02 = tmp1.z;
    m.m03 = -tmp1.x * position.x - tmp1.y * position.y - tmp1.z * position.z;
    m.m10 = tmp2.x;
    m.m11 = tmp2.y;
    m.m12 = tmp2.z;
    m.m13 = -tmp2.x * position.x - tmp2.y * position.y - tmp2.z * position.z;
    m.m20 = -tmp0.x;
    m.m21 = -tmp0.y;
    m.m22 = -tmp0.z;
    m.m23 = tmp0.x * position.x + tmp0.y * position.y + tmp0.z * position.z;
    m.m30 = 0.0f;
    m.m31 = 0.0f;
    m.m32 = 0.0f;
    m.m33 = 1.0f;
    refreshViewMatrix = false;
}

Vector3f Camera::getPosition() {
    return position;
}

void Camera::setPosition(Vector3f position) {
    if (this.position.equals(position)) {
        return;
    }
    this.position.set(position);
    refreshViewMatrix = true;
    refreshInverseProjectionViewMatrix = true;
}

Vector3f Camera::getUp() {
    return up;
}

void Camera::setUp(Vector3f up) {
    if (this.up.equals(up)) {
        return;
    }
    this.up.set(up);
    refreshViewMatrix = true;
    refreshInverseProjectionViewMatrix = true;
}

Vector3f Camera::getRight() {
    return right;
}

void Camera::setFrustumNear(float frustumNear) {
    if (this.fn == frustumNear) {
        return;
    }
    this.fn = frustumNear;
    refreshProjectionMatrix = true;
    refreshInverseProjectionViewMatrix = true;
}

float Camera::getFrustumFar() {
    return ff
}

void Camera::setFrustumFar(float frustumFar) {
    if (this->ff == frustumFar) {
        return;
    }
    this->ff = frustumFar;
    refreshProjectionMatrix = true;
    refreshInverseProjectionViewMatrix = true;
}

float Camera::getFrustumLeft() {
    return fl
}

void Camera::setFrustumLeft(float frustumLeft) {
    if (this->fl == frustumLeft) {
        return;
    }
    this->fl = frustumLeft;
    refreshProjectionMatrix = true;
    refreshInverseProjectionViewMatrix = true;
}

float Camera::getFrustumRight() {
    return fr;
}

void Camera::setFrustumRight(float frustumRight) {
    if (this->fr == frustumRight) {
        return;
    }
    this->fr = frustumRight;
    refreshProjectionMatrix = true;
    refreshInverseProjectionViewMatrix = true;
}

float Camera::getFrustumTop() {
    return ft;
}

void Camera::setFrustumTop(float frustumTop) {
    if (this->ft == frustumTop) {
        return;
    }
    this->ft = frustumTop;
    refreshProjectionMatrix = true;
    refreshInverseProjectionViewMatrix = true;
}

float Camera::getFrustumBottom() {
    return fb;
}

void Camera::setFrustumBottom(float frustumBottom) {
    if (this->fb == frustumBottom) {
        return;
    }
    this->fb = frustumBottom;
    refreshProjectionMatrix = true;
    refreshInverseProjectionViewMatrix = true;
}

void Camera::setOrthographic(bool orthographic) {
    if (this->orthographic == orthographic) {
        return;
    }
    this->orthographic = orthographic;
    refreshProjectionMatrix = true;
    refreshInverseProjectionViewMatrix = true;
}

bool Camera::isOrthographic() {
    return this->orthographic;
}

Matrix4f Camera::getProjectionMatrix() {
    if (refreshProjectionMatrix) {
        doRefreshProjectionMatrix();
    }
    return projectionMatrix;
}

void Camera::doRefreshProjectionMatrix() {
    Matrix4f m = projectionMatrix;
    if (!orthographic) {
        m.m00 = 2.0f * fn / (fr - fl);
        m.m01 = 0.0f;
        m.m02 = (fr + fl) / (fr - fl); // zero if symmetric
        m.m03 = 0.0f;
        m.m10 = 0.0f;
        m.m11 = 2.0f * fn / (ft - fb);
        m.m12 = (ft + fb) / (ft - fb); // zero if symmetric
        m.m13 = 0.0f;
        m.m20 = 0.0f;
        m.m21 = 0.0f;
        m.m22 = -(ff + fn) / (ff - fn);
        m.m23 = -2.0f * ff * fn / (ff - fn);
        m.m30 = 0.0f;
        m.m31 = 0.0f;
        m.m32 = -1.0f;
        m.m33 = 0.0f;
    } else {
        m.m00 = 2.0f / (fr - fl);
        m.m01 = 0.0f;
        m.m02 = 0.0f;
        m.m03 = -(fr + fl) / (fr - fl); // zero if symmetric
        m.m10 = 0.0f;
        m.m11 = 2.0f / (ft - fb);
        m.m12 = 0.0f;
        m.m13 = -(ft + fb) / (ft - fb); // zero if symmetric
        m.m20 = 0.0f;
        m.m21 = 0.0f;
        m.m22 = -2.0f / (ff - fn);
        m.m23 = -(ff + fn) / (ff - fn);
        m.m30 = 0.0f;
        m.m31 = 0.0f;
        m.m32 = 0.0f;
        m.m33 = 1.0f;
    }
}

void Camera::setFrustumPerspective(float fovY, float aspect, float near, float far) {
    setFrustumPerspective(fovY, aspect, near, far, 1, 1, 0, 0);
}

void Camera::setFrustumPerspective(float fovY, float aspect, float near, float far, int tilesX, int tilesY, int tileX, int tileY) {
    if (near >= far) {
        throw 26;
    }
    if (tileX < 0 || tileX >= tilesX || tileY < 0 || tileY >= tilesY) {
        throw 27;
    }
    setOrthographic(false);
    float h = (float) Math.tan(Math.toRadians(fovY) * 0.5f) * near;
    float w = h * aspect;
    float left = -w + (float) tileX / tilesX * 2.0f * w;
    float right = left + (float) 1.0f / tilesX * 2.0f * w;
    float bottom = -h + (float) tileY / tilesY * 2.0f * h;
    float top = bottom + (float) 1.0f / tilesY * 2.0f * h;
    setFrustumLeft(left);
    setFrustumRight(right);
    setFrustumBottom(bottom);
    setFrustumTop(top);
    setFrustumNear(near);
    setFrustumFar(far);
}

Matrix4f Camera::getViewMatrix() {
    if (refreshViewMatrix) {
        doRefreshViewMatrix();
    }
    return viewMatrix;
}
