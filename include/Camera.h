#ifndef CAMERA_H
#define CAMERA_H

#include <Matrix4f.h>
#include <Vector3f.h>
#include <Vector4f.h>


class Camera
{
    public:
        Camera();
        Vector3f getDirection();
        void setDirection(Vector3f direction);
        void doRefreshInverseProjectionViewMatrix();
        Matrix4f getInverseProjectionViewMatrix();
        void getEyeRay(float x, float y, Vector3f res);
        void setLookAt(Vector3f position, Vector3f lookAt, Vector3f up);
        void doRefreshViewMatrix();
        Vector3f getPosition();
        void setPosition();
        Vector3f getUp();
        void setUp(Vector3f up);
        Vector3f getRight();
        void setFrustumNear(float frustumNear);
        float getFrustumFar();
        void setFrustumFar(float frustumFar);
        float getFrustumLeft();
        void setFrustumLeft(float frustumLeft);
        float getFrustumRight();
        void setFrustumRight(float frustumRight);
        float getFrustumTop();
        void setFrustumTop(float frustumTop);
        float getFrustumBottom();
        void setFrustumBottom(float frustumBottom);
        void setOrthographic(bool orthographic);
        bool isOrthographic();
        Matrix4f getProjectionMatrix();
        void doRefreshProjectionMatrix();
        void setFrustum(float near, float far, float left, float right, float top, float bttom);

        void setFrustumPerspective(float fovY, float aspect, float near, float far);
        void setFrustumPerspective(float fovY, float aspect, float near, float far, int tilesX, int tilesY, int tileX, int tileY);
        Matrix4f getViewMatrix();


    protected:

    private:

	float fn = 0.001f;
	float ff = 10.0f;
    float fl = -0.05f;
    float fr = 0.05f;
    float ft = 0.05f;
    float fb = -0.05f;
	/**
	 * Whether view matrix needs to be recalculated when requested through
	 * {@link #getViewMatrix()}.
	 */
	bool refreshViewMatrix = true;
	/**
	 * Whether projection matrix needs to be recalculated when requested through
	 * {@link #getProjectionMatrix()}.
	 */
	bool refreshProjectionMatrix = true;
	bool refreshInverseProjectionViewMatrix = true;
	Vector3f position = Vector3f(0.0f, 0.0f, 0.0f);
	Vector3f direction = Vector3f(0.0f, 0.0f, -1.0f);
	Vector3f up = Vector3f(0.0f, 1.0f, 0.0f);
    Vector3f right = Vector3f(1.0f, 0.0f, 0.0f);
	Matrix4f projectionMatrix = Matrix4f();
	Matrix4f viewMatrix = Matrix4f();
	Matrix4f invViewProjectionMatrix = Matrix4f();

	/**
	 * Orthographic projection is inherently different from perspective
	 * projection.
	 */
    bool orthographic;

	/* Cached objects reused for computations */
	Vector3f tmp0 = Vector3f();
	Vector3f tmp1 = Vector3f();
	Vector3f tmp2 = Vector3f();
	Vector4f tmp3 = Vector4f();



};

#endif // CAMERA_H
