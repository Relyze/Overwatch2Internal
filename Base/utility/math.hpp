#pragma once
#include "../globals.h"


// Math Definitions
#define M_E        2.71828182845904523536   // e
#define M_LOG2E    1.44269504088896340736   // log2(e)
#define M_LOG10E   0.434294481903251827651  // log10(e)
#define M_LN2      0.693147180559945309417  // ln(2)
#define M_LN10     2.30258509299404568402   // ln(10)
#define M_PI       3.14159265358979323846   // pi
#define M_PI_2     1.57079632679489661923   // pi/2
#define M_PI_4     0.785398163397448309616  // pi/4
#define M_1_PI     0.318309886183790671538  // 1/pi
#define M_2_PI     0.636619772367581343076  // 2/pi
#define M_2_SQRTPI 1.12837916709551257390   // 2/sqrt(pi)
#define M_SQRT2    1.41421356237309504880   // sqrt(2)
#define M_SQRT1_2  0.707106781186547524401  // 1/sqrt(2)

#include <cmath>
#include <array>
#include <algorithm>

#define RAD2DEG(x) ((x) * 180.0f / static_cast<float>(M_PI))
#define DEG2RAD(x) ((x) * static_cast<float>(M_PI) / 180.0f)

template<typename T>
class ArrayWrapper
{
    T* data;
    uint32_t size;
    uint32_t capacity;
public:
    ArrayWrapper() : data(nullptr), size(0), capacity(0) { }
    void add(uintptr_t value) {
        decrypt::force_decryption(ImageBase + Offsets::Engine::ArrayInsertU64);
        auto fnArrayInsertU64 = reinterpret_cast<void(__fastcall*)(T*, const uint64_t*)>(ImageBase + Offsets::Engine::ArrayInsertU64);
        fnArrayInsertU64(reinterpret_cast<T*>(&data), &value);
    }

    std::vector<T> asVector() const {
        return std::vector<T>(data, data + size);
    }

    uint32_t getCapacity() {
        return capacity;
    }

    void setCapacity(uint32_t newCapacity) {
        capacity = newCapacity;
    }

    size_t getSize() const {
        return size;
    }

    T* getData() const {
        return data;
    }
};

class Vector2 {
public:
    float x = 0, y = 0;

    Vector2() {}
    Vector2(float x, float y) : x(x), y(y) {}

    float Distance(Vector2 v) {
        return std::sqrt(std::pow(v.x - x, 2.0f) + std::pow(v.y - y, 2.0f));
    }

    float Dot(Vector2 v) const {
        return x * v.x + y * v.y;
    }

    Vector2 operator+(Vector2 v) {
        return { x + v.x, y + v.y };
    }

    Vector2 operator-(Vector2 v) {
        return { x - v.x, y - v.y };
    }

    Vector2 operator*(Vector2 v) {
        return { x * v.x, y * v.y };
    }

    Vector2 operator*(float v) {
        return { x * v, y * v };
    }

    Vector2 operator/(float v) {
        return { x / v, y / v };
    }

    Vector2 operator/(Vector2 v) {
        return { x / v.x, y / v.y };
    }

    void operator+=(Vector2 v) {
        x += v.x, y += v.y;
    }

    void operator-=(Vector2 v) {
        x -= v.x, y -= v.y;
    }

    float GetLength() {
        return std::sqrt(std::pow(x, 2.0f) + std::pow(y, 2.0f));
    }

    Vector2 Normalize() {
		float length = GetLength();
		return { x / length, y / length };
    }
};

class Vector3 {
public:
    float x, y, z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    Vector3(Vector2& vec) : x(vec.x), y(vec.y), z(0) {}
    Vector3(DirectX::XMVECTOR vec) : x(DirectX::XMVectorGetX(vec)), y(DirectX::XMVectorGetY(vec)), z(DirectX::XMVectorGetZ(vec)) {}

    Vector3 Normalize() const {
        float length = std::sqrt(std::pow(x, 2.0f) + std::pow(y, 2.0f) + std::pow(z, 2.0f));
		return { x / length, y / length, z / length };
    }

    float Dot(const Vector3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    Vector3 Cross(const Vector3& v) {
        return { y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x };
    }

    float Angle(const Vector3& v) {
        return RAD2DEG(std::sqrt(std::pow(v.x - x, 2.0f) + std::pow(v.y - y, 2.0f) + std::pow(v.z - z, 2.0f)));
    }

    float Distance(const Vector3& v) {
        return std::sqrt(std::pow(v.x - x, 2.0f) + std::pow(v.y - y, 2.0f) + std::pow(v.z - z, 2.0f));
    }

    bool operator==(const Vector3& v)
    {
        return x == v.x && y == v.y && z == v.z;
    }

    bool operator!=(const Vector3& v)
    {
        return x != v.x && y != v.y && z != v.z;
    }

    Vector3 operator+(const Vector3& v)
    {
        return { x + v.x, y + v.y, z + v.z };
    }

    Vector3 operator-(const Vector3& v)
    {
        return { x - v.x, y - v.y, z - v.z };
    }

    Vector3 operator*(const Vector3& v)
    {
        return { x * v.x, y * v.y, z * v.z };
    }

    Vector3 operator*(float f)
    {
        return { x * f, y * f, z * f };
    }

    Vector3 operator/(float f)
    {
        return { x / f, y / f, z / f };
    }
    void operator+=(const Vector3& v)
    {
        x += v.x, y += v.y, z += v.z;
    }

    void operator-=(const Vector3& v)
    {
        x -= v.x, y -= v.y, z -= v.z;
    }
    void operator+=(float f)
    {
        this->x = this->x + f;
        this->y = this->y + f;
        this->z = this->z + f;
        return;
    }
    void operator*=(float f)
    {
        this->x = this->x * f;
        this->y = this->y * f;
        this->z = this->z * f;
        return;
    }

    void operator/=(float f)
    {
        this->x = this->x / f;
        this->y = this->y / f;
        this->z = this->z / f;
        return;
    }

    float Length() {
        return std::sqrt(std::pow(x, 2.0f) + std::pow(y, 2.0f) + std::pow(z, 2.0f));
    }

    float Length2D() {
        return std::sqrt(std::pow(x, 2.0f) + std::pow(y, 2.0f));
    }

    float _Length2D() {
        return std::sqrt(std::pow(x, 2.0f) + std::pow(z, 2.0f));
    }

    DirectX::XMVECTOR ConvertToXMVECTOR() const {
        return DirectX::XMVectorSet(x, y, z, 0.0f);
    }
};

class Quarternion {
public:
    float x, y, z, w;
    
    Quarternion() : x(0), y(0), z(0), w(0) {}

    Quarternion(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

    Quarternion operator*(const Quarternion& q) const {
        return {
			w * q.x + x * q.w + y * q.z - z * q.y,
			w * q.y + y * q.w + z * q.x - x * q.z,
			w * q.z + z * q.w + x * q.y - y * q.x,
			w * q.w - x * q.x - y * q.y - z * q.z
		};
	}

    Quarternion operator*(const float f) const {
		return { x * f, y * f, z * f, w * f };
	}

    Quarternion operator+(const Quarternion& q) const {
		return { x + q.x, y + q.y, z + q.z, w + q.w };
	}

    Quarternion operator-(const Quarternion& q) const {
		return { x - q.x, y - q.y, z - q.z, w - q.w };
	}

    Quarternion operator/(const float f) const {
		return { x / f, y / f, z / f, w / f };
	}

    void operator+=(const Quarternion& q) {
		x += q.x, y += q.y, z += q.z, w += q.w;
	}

    void operator-=(const Quarternion& q) {
		x -= q.x, y -= q.y, z -= q.z, w -= q.w;
	}

    void operator*=(const float f) {
		x *= f, y *= f, z *= f, w *= f;
	}

    void operator/=(const float f) {
		x /= f, y /= f, z /= f, w /= f;
	}

    float Length() const {
		return std::sqrt(x * x + y * y + z * z + w * w);
	}

    Quarternion Normalize() const {
		float length = Length();
		return { x / length, y / length, z / length, w / length };
	}
};


struct DecomposedMatrix
{
    float rotation[4];
    float scale[4];
    float translation[4];

    // Constructor
    DecomposedMatrix() {
        // Initialize the matrix as an identity matrix
        for (int i = 0; i < 4; i++) {
            rotation[i] = 0.0f;
            scale[i] = 0.0f;
            translation[i] = 0.0f;
        }
    }

    void printMatrix() {
        printf("Rotation:");
        for (int i = 0; i < 4; i++)
            printf(" %f", rotation[i]);
        printf("\n");

        printf("Scale:");
        for (int i = 0; i < 4; i++)
            printf(" %f", scale[i]);
        printf("\n");

        printf("Translation:");
        for (int i = 0; i < 4; i++)
            printf(" %f", translation[i]);
        printf("\n\n");
    }

    DecomposedMatrix operator*(const DecomposedMatrix& other) const {
        DecomposedMatrix result;
        for (int i = 0; i < 4; i++) {
            result.rotation[i] = rotation[i] * other.rotation[i];
            result.scale[i] = scale[i] * other.scale[i];
            result.translation[i] = translation[i] * other.translation[i];
        }
        return result;
    }
};

struct Matrix
{
    float m[16];
    // Constructor
    Matrix() {
        // Initialize the matrix as an identity matrix
        for (int i = 0; i < 16; i++) {
            m[i] = (i % 5 == 0) ? 1.0f : 0.0f;
        }
    }

    // Allow for direct initialization of the matrix
    Matrix(const float* values) {
        for (int i = 0; i < 16; i++) {
			m[i] = values[i];
		}
	}

    void printMatrix() {
        for (int i = 0; i < 16; i += 4) {
            printf(enc("%.4f %.4f %.4f %.4f\n").c_str(), m[i], m[i + 1], m[i + 2], m[i + 3]);
        }
        printf("\n");
    }

    Vector3 GetTranslation() const {
		DirectX::XMMATRIX invView = Inverse().ConvertToXMMATRIX();
        return Vector3(DirectX::XMVectorGetX(invView.r[3]) / DirectX::XMVectorGetW(invView.r[3]), DirectX::XMVectorGetY(invView.r[3]) / DirectX::XMVectorGetW(invView.r[3]), DirectX::XMVectorGetZ(invView.r[3]) / DirectX::XMVectorGetW(invView.r[3]));
	}

    Vector3 GetScale() const {
        return Vector3(m[0], m[5], m[10]);
    }

    Matrix Inverse() const {
        float det = m[0] * (m[5] * m[10] - m[9] * m[6]) - m[4] * (m[1] * m[10] - m[9] * m[2]) + m[8] * (m[1] * m[6] - m[5] * m[2]);
		if (det == 0) 
            return Matrix(this->m);

		float invdet = 1 / det;

        float matrix[16]{
			m[5] * m[10] - m[9] * m[6],
			m[8] * m[6] - m[4] * m[10],
			m[4] * m[9] - m[8] * m[5],
			0,

			m[9] * m[2] - m[1] * m[10],
			m[0] * m[10] - m[8] * m[2],
			m[8] * m[1] - m[0] * m[9],
			0,

			m[1] * m[6] - m[5] * m[2],
			m[4] * m[2] - m[0] * m[6],
			m[0] * m[5] - m[4] * m[1],
			0,

			-(m[0] * m[12] + m[4] * m[13] + m[8] * m[14]),
			-(m[1] * m[12] + m[5] * m[13] + m[9] * m[14]),
			-(m[2] * m[12] + m[6] * m[13] + m[10] * m[14]),
			1
		};

        Matrix mat(matrix);
        mat *= invdet;

        return mat;
    }

    Vector2 WorldToScreen(Vector3 worldPos) const {
        float screenX = (m[0] * worldPos.x) + (m[4] * worldPos.y) + (m[8] * worldPos.z) + m[12];
        float screenY = (m[1] * worldPos.x) + (m[5] * worldPos.y) + (m[9] * worldPos.z) + m[13];
        float screenW = (m[3] * worldPos.x) + (m[7] * worldPos.y) + (m[11] * worldPos.z) + m[15];

        float camX = render::width * 0.5f;
        float camY = render::height * 0.5f;

        float x = camX + (camX * screenX / screenW);
        float y = camY - (camY * screenY / screenW);

        return { x, y };
    }

    bool WorldToScreen(Vector3 worldPos, Vector2* OutPos) const {
        float screenX = (m[0] * worldPos.x) + (m[4] * worldPos.y) + (m[8] * worldPos.z) + m[12];
        float screenY = (m[1] * worldPos.x) + (m[5] * worldPos.y) + (m[9] * worldPos.z) + m[13];
        float screenW = (m[3] * worldPos.x) + (m[7] * worldPos.y) + (m[11] * worldPos.z) + m[15];

        float camX = render::width * 0.5f;
        float camY = render::height * 0.5f;

        float x = camX + (camX * screenX / screenW);
        float y = camY - (camY * screenY / screenW);

        OutPos->x = x;
        OutPos->y = y;

        return (screenW >= 0.001f);
    }

    void RotateY(float angle) {
		float c = std::cos(angle);
		float s = std::sin(angle);

		Matrix rotation;
		rotation.m[0] = c;
		rotation.m[2] = -s;
		rotation.m[8] = s;
		rotation.m[10] = c;

		*this = rotation * *this;
	}

    // implement equivalent to XMVector3Transform
    Vector3 TransformVector(Vector3 v) const {
        return Vector3(
            v.x * m[0] + v.y * m[4] + v.z * m[8] + m[12],
            v.x * m[1] + v.y * m[5] + v.z * m[9] + m[13],
            v.x * m[2] + v.y * m[6] + v.z * m[10] + m[14]
        );
    }

    Vector3 GetCameraVec() const
    {
        // Calculate the determinant of the upper-left 3x3 matrix
        float detA = m[5] * m[10] - m[9] * m[6];
        float detB = m[9] * m[2] - m[1] * m[10];
        float detC = m[1] * m[6] - m[5] * m[2];

        // Calculate the determinant of the entire 4x4 matrix
        float detZ = m[0] * detA + m[4] * detB + m[8] * detC;

        // Check if the determinant is close to zero to avoid division by zero
        if (std::abs(detZ) < 0.0001)
            return Vector3(); // Return a default vector if the determinant is too small

        // Calculate the elements needed for the camera position
        float detD = m[8] * m[6] - m[4] * m[10];
        float detE = m[0] * m[10] - m[8] * m[2];
        float detF = m[4] * m[2] - m[0] * m[6];
        float detG = m[4] * m[9] - m[8] * m[5];
        float detH = m[8] * m[1] - m[0] * m[9];
        float detK = m[0] * m[5] - m[4] * m[1];

        // Calculate and return the camera position vector
        return Vector3(-(detA * m[12] + detD * m[13] + detG * m[14]) / detZ,
            -(detB * m[12] + detE * m[13] + detH * m[14]) / detZ,
            -(detC * m[12] + detF * m[13] + detK * m[14]) / detZ);
    }


    Vector3 GetCameraRotation() const {
        // Extract the rotation matrix (upper-left 3x3 matrix)
        float r00 = m[0], r01 = m[1], r02 = m[2];
        float r10 = m[4], r11 = m[5], r12 = m[6];
        float r20 = m[8], r21 = m[9], r22 = m[10];

        // Calculate the Euler angles (yaw, pitch, roll)
        float yaw = std::atan2(r21, r22);
        float pitch = std::atan2(-r20, std::sqrt(r21 * r21 + r22 * r22));
        float roll = std::atan2(r10, r00);

        return Vector3(yaw, pitch, roll);
    }

    float Determinant() const {
        return
            m[12] * m[9] * m[6] * m[3] -
            m[8] * m[13] * m[6] * m[3] -
            m[12] * m[5] * m[10] * m[3] +
            m[4] * m[13] * m[10] * m[3] +

            m[8] * m[5] * m[14] * m[3] -
            m[4] * m[9] * m[14] * m[3] -
            m[12] * m[9] * m[2] * m[7] +
            m[8] * m[13] * m[2] * m[7] +

            m[12] * m[1] * m[10] * m[7] -
            m[0] * m[13] * m[10] * m[7] -
            m[8] * m[1] * m[14] * m[7] +
            m[0] * m[9] * m[14] * m[7] +

            m[12] * m[5] * m[2] * m[11] -
            m[4] * m[13] * m[2] * m[11] -
            m[12] * m[1] * m[6] * m[11] +
            m[0] * m[13] * m[6] * m[11] +

            m[4] * m[1] * m[14] * m[11] -
            m[0] * m[5] * m[14] * m[11] -
            m[8] * m[5] * m[2] * m[15] +
            m[4] * m[9] * m[2] * m[15] +

            m[8] * m[1] * m[6] * m[15] -
            m[0] * m[9] * m[6] * m[15] -
            m[4] * m[1] * m[10] * m[15] +
            m[0] * m[5] * m[10] * m[15];
    }

    // Operator overrides
    Matrix operator*(const Matrix& other) const {
        Matrix result;
        for (int i = 0; i < 16; i++) {
            result.m[i] = 0.0f;
            for (int j = 0; j < 4; j++) {
                result.m[i] += m[j + (i / 4) * 4] * other.m[i % 4 + j * 4];
            }
        }
        return result;
    }

    Matrix operator*=(const float value) const {
        Matrix result;
        for (int i = 0; i < 16; i++) {
            result.m[i] *= value;
        }
        return result;
    }

    Matrix operator+=(const Matrix& other) const {
        Matrix result;
        for (int i = 0; i < 16; i++) {
            result.m[i] += other.m[i];
        }
        return result;
    }

    // Create rotation matrix for a quaternion onto the matrix
    Matrix& operator=(const DecomposedMatrix& dm) {
        // Apply scale
        m[0] = dm.scale[0];
        m[5] = dm.scale[1];
        m[10] = dm.scale[2];

        // Apply rotation
        float x = dm.rotation[0];
        float y = dm.rotation[1];
        float z = dm.rotation[2];
        float w = dm.rotation[3];

        m[0] *= 1 - 2 * (y * y + z * z);
        m[1] = 2 * (x * y - z * w);
        m[2] = -2 * (x * z + y * w);
        m[4] = 2 * (x * y + z * w);
        m[5] *= 1 - 2 * (x * x + z * z);
        m[6] = 2 * (y * z - x * w);
        m[8] = -2 * (x * z - y * w);
        m[9] = 2 * (y * z + x * w);
        m[10] *= 1 - 2 * (x * x + y * y);

        // Apply translation
        m[12] = dm.translation[0];
        m[13] = dm.translation[1];
        m[14] = dm.translation[2];

        return *this;
    }

    DirectX::XMMATRIX ConvertToXMMATRIX() const {
        return DirectX::XMMATRIX(
            m[0], m[1], m[2], m[3],
            m[4], m[5], m[6], m[7],
            m[8], m[9], m[10], m[11],
            m[12], m[13], m[14], m[15]
        );
    }
};
