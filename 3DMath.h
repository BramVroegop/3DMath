#pragma once
#include <math.h>

struct Vector2f {
    float x, y;

    static float inner(Vector2f v1, Vector2f v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }

    /*
    Technically the cross product does not exist in 2d
    but it's useful in finding barycentric coordinates.
    (It's really just the determinant of a 2x2 matrix repackaged)
    */
    static float cross(Vector2f v1, Vector2f v2) {
        return v1.x * v2.y - v1.y * v2.x;
    }

    static Vector2f projected(Vector2f v, Vector2f to_project_on) {
        Vector2f normalized = to_project_on.normalized();
        float inner = Vector2f::inner(v, normalized);
        return normalized * inner;
    };

    float length_sqr() {
        return this->x * this->x + this->y * this->y;
    }

    float length() {
        return sqrtf(this->length_sqr());
    }

    Vector2f normalized() {
        float l = this->length();
        if (l == 0.0f) return *this;
        return *this / l;
    }

    Vector2f operator+(const Vector2f &v2) {
        return Vector2f { this->x + v2.x, this->y + v2.y };
    }

    void operator+=(const Vector2f &v2) {
        *this = (*this) + v2;
    }

    Vector2f operator-(const Vector2f &v2) {
        return Vector2f { this->x - v2.x, this->y - v2.y };
    }

    void operator-=(const Vector2f &v2) {
        *this = (*this) - v2;
    }

    Vector2f operator*(const float &scalar) {
        return Vector2f { this->x * scalar, this->y * scalar };
    }

    void operator*=(const float &scalar) {
        *this = (*this) * scalar;
    }

    Vector2f operator/(const float &scalar) {
        return Vector2f { this->x / scalar, this->y / scalar };
    }

    void operator/=(const float &scalar) {
        *this = (*this) / scalar;
    }
};

struct Vector3f {
    float x, y, z;

    static float inner(Vector3f v1, Vector3f v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    static Vector3f cross(Vector3f v1, Vector3f v2) {
        return Vector3f {
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x
        };
    }

    static Vector3f projected(Vector3f v, Vector3f to_project_on) {
        Vector3f normalized = to_project_on.normalized();
        float inner = Vector3f::inner(v, normalized);
        return normalized * inner;
    };

    float length_sqr() {
        return this->x * this->x + this->y * this->y + this->z * this->z;
    }

    float length() {
        return sqrtf(this->length_sqr());
    }

    Vector3f normalized() {
        float l = this->length();
        if (l == 0.0f) return *this;
        return *this / l;
    }

    Vector3f operator+(const Vector3f &v2) {
        return Vector3f { this->x + v2.x, this->y + v2.y, this->z + v2.z };
    }

    void operator+=(const Vector3f &v2) {
        *this = (*this) + v2;
    }

    Vector3f operator-(const Vector3f &v2) {
        return Vector3f { this->x - v2.x, this->y - v2.y, this->z - v2.z };
    }

    void operator-=(const Vector3f &v2) {
        *this = (*this) - v2;
    }

    Vector3f operator*(const float &scalar) {
        return Vector3f { this->x * scalar, this->y * scalar, this->z * scalar };
    }

    void operator*=(const float &scalar) {
        *this = (*this) * scalar;
    }

    Vector3f operator/(const float &scalar) {
        return Vector3f { this->x / scalar, this->y / scalar, this->z / scalar };
    }

    void operator/=(const float &scalar) {
        *this = (*this) / scalar;
    }
};

/*
    NOTE: This struct has only implemented support for conversion to/from Vector2f and Vector3f.
    Use this conversion if you need to send Vector4f information to your graphics API.
*/
struct Vector4f {
    float x, y, z, w;

    Vector4f(float x, float y, float z, float w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    Vector4f(Vector2f v, float z = 0.0f, float w = 1.0f) {
        this->x = v.x;
        this->y = v.y;
        this->z = z;
        this->w = w;
    }

    Vector4f(Vector3f v, float w = 1.0f) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        this->w = w;
    }

    Vector2f as_Vector2f() {
        return Vector2f { this->x, this->y };
    }

    Vector3f as_Vector3f() {
        return Vector3f { this->x, this->y, this->z };
    }
};

struct Triangle2D {
    Vector2f p1, p2, p3;

    /*
    Math for this was pulled straight from the "Barycentric coordinates on triangles" heading
    https://en.wikipedia.org/wiki/Barycentric_coordinate_system

    It's unoptimized, I'm aware. But it's mine :3
    */
    void barycentrics(Vector2f hit, float *u, float *v) {
        Vector2f e1 = p2 - p1;
        Vector2f e3 = p1 - p3;

        float total_area_parralelogram = Vector2f::cross(e1, e3 * -1);
        *u = Vector2f::cross(e3, hit - p3) / total_area_parralelogram;
        *v = Vector2f::cross(e1, hit - p1) / total_area_parralelogram;
    }

};

struct Triangle3D {
    Vector3f p1, p2, p3;

    /*
    Math for this was pulled straight from the "Barycentric coordinates on triangles" heading
    https://en.wikipedia.org/wiki/Barycentric_coordinate_system

    It's unoptimized, I'm aware. But it's mine :3
    */
    void barycentrics(Vector3f hit, float *u, float *v) {
        Vector3f e1 = p2 - p1;
        Vector3f e3 = p1 - p3;

        Vector3f perp = Vector3f::cross(e1, e3 * -1);
        float total_area_parralelogram = perp.length();
        Vector3f normalized = perp / total_area_parralelogram;

        *u = Vector3f::inner(normalized, Vector3f::cross(e3, hit - p3)) / total_area_parralelogram;
        *v = Vector3f::inner(normalized, Vector3f::cross(e1, hit - p1)) / total_area_parralelogram;
    }
};

/*
NOTE: Matrices are row-major
*/
struct Matrix2f {
    float arr[2][2];

    Matrix2f(float diagonal = 1.0f) {
        for (unsigned int i = 0; i < 2; i++) {
            for (unsigned int j = 0; j < 2; j++) {
                if (i == j) {
                    arr[i][j] = diagonal;
                } else {
                    arr[i][j] = 0.0f;
                }
            }
        }
    } 

    float det() {
        float a = this->arr[0][0];
        float b = this->arr[0][1];
        float c = this->arr[1][0];
        float d = this->arr[1][1];

        return a * d - b * c;
    }

    Matrix2f inversed() {
        Matrix2f m;
        float det = this->det();
        if (det == 0.0f) {
            return m;
        }

        float inv_det = 1.0f / det;

        m.arr[0][0] = this->arr[1][1] * inv_det;
        m.arr[0][1] = -this->arr[0][1] * inv_det;
        m.arr[1][0] = -this->arr[1][0] * inv_det;
        m.arr[1][1] = this->arr[0][0] * inv_det;

        return m;
    }

    Matrix2f transposed() {
        Matrix2f m;
        for (unsigned int i = 0; i < 2; i++) {
            for (unsigned int j = 0; j < 2; j++) {
                if (i != j) {
                    m.arr[i][j] = this->arr[j][i];
                } else {
                    m.arr[i][j] = this->arr[i][j];
                }
            }
        }

        return m;
    }

    Matrix2f operator+(const Matrix2f &m) {
        Matrix2f r;

        for (unsigned int i = 0; i < 2; i++) {
            for (unsigned int j = 0; j < 2; j++) {
                r.arr[i][j] = this->arr[i][j] += m.arr[i][j];
            }
        }

        return r;
    }

    void operator+=(const Matrix2f &m) {
        *this = (*this) + m;
    }

     Matrix2f operator-(const Matrix2f &m) {
        Matrix2f r;

        for (unsigned int i = 0; i < 2; i++) {
            for (unsigned int j = 0; j < 2; j++) {
                r.arr[i][j] = this->arr[i][j] -= m.arr[i][j];
            }
        }

        return r;
    }

    void operator-=(const Matrix2f &m) {
        *this = (*this) - m;
    }

    Matrix2f operator*(const Matrix2f &m) {
        Matrix2f r;

        for (unsigned int i = 0; i < 2; i++) {
            for (unsigned int j = 0; j < 2; j++) {
                float sum = 0.0f;
                for (unsigned int k = 0; k < 2; k++) {
                    sum += this->arr[i][k] * m.arr[k][j]; 
                }
                r.arr[i][j] = sum;
            }
        }

        return r;
    }

    void operator*=(const Matrix2f &m) {
        *this = (*this) * m;
    }

    Vector2f operator*(const Vector2f &v) {
        Vector2f r;

        r.x = this->arr[0][0] * v.x + this->arr[0][1] * v.y;
        r.y = this->arr[1][0] * v.x + this->arr[1][1] * v.y; 
        return r;
    }
};

/*
NOTE: Matrices are row-major
*/
struct Matrix3f {
    float arr[3][3];
};

/*
NOTE: Matrices are row-major
*/
struct Matrix4f {
    float arr[4][4];
};