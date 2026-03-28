#pragma once
#include <math.h>

struct Vector2f {
    float x, y;

    Vector2f(float x = 0.0f, float y = 0.0f) {
        this->x = x;
        this->y = y;
    }

    static float dot(Vector2f v1, Vector2f v2) {
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
        float dot = Vector2f::dot(v, normalized);
        return normalized * dot;
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
        return Vector2f(this->x + v2.x, this->y + v2.y);
    }

    void operator+=(const Vector2f &v2) {
        *this = (*this) + v2;
    }

    Vector2f operator-(const Vector2f &v2) {
        return Vector2f(this->x - v2.x, this->y - v2.y);
    }

    void operator-=(const Vector2f &v2) {
        *this = (*this) - v2;
    }

    Vector2f operator*(const float scalar) {
        return Vector2f(this->x * scalar, this->y * scalar);
    }

    void operator*=(const float scalar) {
        *this = (*this) * scalar;
    }

    Vector2f operator/(const float scalar) {
        return Vector2f(this->x / scalar, this->y / scalar);
    }

    void operator/=(const float scalar) {
        *this = (*this) / scalar;
    }
};

struct Vector3f {
    float x, y, z;

    Vector3f(float x = 0.0f, float y = 0.0f, float z = 0.0f) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    static float dot(Vector3f v1, Vector3f v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    static Vector3f cross(Vector3f v1, Vector3f v2) {
        return Vector3f(
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x
        );
    }

    static Vector3f projected(Vector3f v, Vector3f to_project_on) {
        Vector3f normalized = to_project_on.normalized();
        float dot = Vector3f::dot(v, normalized);
        return normalized * dot;
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
        return Vector3f(this->x + v2.x, this->y + v2.y, this->z + v2.z);
    }

    void operator+=(const Vector3f &v2) {
        *this = (*this) + v2;
    }

    Vector3f operator-(const Vector3f &v2) {
        return Vector3f(this->x - v2.x, this->y - v2.y, this->z - v2.z);
    }

    void operator-=(const Vector3f &v2) {
        *this = (*this) - v2;
    }

    Vector3f operator*(const float scalar) {
        return Vector3f(this->x * scalar, this->y * scalar, this->z * scalar);
    }

    void operator*=(const float scalar) {
        *this = (*this) * scalar;
    }

    Vector3f operator/(const float scalar) {
        return Vector3f(this->x / scalar, this->y / scalar, this->z / scalar);
    }

    void operator/=(const float scalar) {
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
        return Vector2f(this->x, this->y);
    }

    Vector3f as_Vector3f() {
        return Vector3f(this->x, this->y, this->z);
    }
};

struct Triangle2D {
    Vector2f p1, p2, p3;

    Triangle2D(Vector2f p1, Vector2f p2, Vector2f p3) {
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
    }

    /*
    Math for this was pulled straight from the "Barycentric coordinates on triangles" heading
    https://en.wikipedia.org/wiki/Barycentric_coordinate_system
    Will return null if triangle area is 0

    It's unoptimized, I'm aware. But it's mine :3
    */
    void barycentrics(Vector2f hit, float *u, float *v) {
        Vector2f e1 = p2 - p1;
        Vector2f e3 = p1 - p3;

        float total_area_parralelogram = Vector2f::cross(e1, e3 * -1);

        if (total_area_parralelogram < 0.0005f) {
            return;
        }

        *u = Vector2f::cross(e3, hit - p3) / total_area_parralelogram;
        *v = Vector2f::cross(e1, hit - p1) / total_area_parralelogram;
    }

};

struct Triangle3D {
    Vector3f p1, p2, p3;

    Triangle3D(Vector3f p1, Vector3f p2, Vector3f p3) {
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
    }

    /*
    Math for this was pulled straight from the "Barycentric coordinates on triangles" heading
    https://en.wikipedia.org/wiki/Barycentric_coordinate_system
    Will return null if triangle area is 0

    It's unoptimized, I'm aware. But it's mine :3
    */
    void barycentrics(Vector3f hit, float *u, float *v) {
        Vector3f e1 = p2 - p1;
        Vector3f e3 = p1 - p3;

        Vector3f perp = Vector3f::cross(e1, e3 * -1);
        float total_area_parralelogram = perp.length();

        if (total_area_parralelogram < 0.00005f) {
            return;
        }

        Vector3f normalized = perp / total_area_parralelogram;

        *u = Vector3f::dot(normalized, Vector3f::cross(e3, hit - p3)) / total_area_parralelogram;
        *v = Vector3f::dot(normalized, Vector3f::cross(e1, hit - p1)) / total_area_parralelogram;
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
                r.arr[i][j] = this->arr[i][j] + m.arr[i][j];
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
                r.arr[i][j] = this->arr[i][j] - m.arr[i][j];
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

    Matrix3f(float diagonal = 1.0f) {
        for (unsigned int i = 0; i < 3; i++) {
            for (unsigned int j = 0; j < 3; j++) {
                if (i == j) {
                    arr[i][j] = diagonal;
                } else {
                    arr[i][j] = 0.0f;
                }
            }
        }
    } 

    float det() {
        return arr[0][0] * (arr[1][1] * arr[2][2] - arr[1][2] * arr[2][1])
         - arr[0][1] * (arr[1][0] * arr[2][2] - arr[1][2] * arr[2][0])
         + arr[0][2] * (arr[1][0] * arr[2][1] - arr[1][1] * arr[2][0]);
    }

    Matrix3f inversed() {
        Matrix3f m;
        float det = this->det();
        if (det == 0.0f) {
            return m;
        }

        float inv_det = 1.0f / det;

        m.arr[0][0] =  (arr[1][1] * arr[2][2] - arr[1][2] * arr[2][1]) * inv_det;
        m.arr[0][1] = -(arr[0][1] * arr[2][2] - arr[0][2] * arr[2][1]) * inv_det;
        m.arr[0][2] =  (arr[0][1] * arr[1][2] - arr[0][2] * arr[1][1]) * inv_det;

        m.arr[1][0] = -(arr[1][0] * arr[2][2] - arr[1][2] * arr[2][0]) * inv_det;
        m.arr[1][1] =  (arr[0][0] * arr[2][2] - arr[0][2] * arr[2][0]) * inv_det;
        m.arr[1][2] = -(arr[0][0] * arr[1][2] - arr[0][2] * arr[1][0]) * inv_det;

        m.arr[2][0] =  (arr[1][0] * arr[2][1] - arr[1][1] * arr[2][0]) * inv_det;
        m.arr[2][1] = -(arr[0][0] * arr[2][1] - arr[0][1] * arr[2][0]) * inv_det;
        m.arr[2][2] =  (arr[0][0] * arr[1][1] - arr[0][1] * arr[1][0]) * inv_det;

        return m;
    }

    Matrix3f transposed() {
        Matrix3f m;
        for (unsigned int i = 0; i < 3; i++) {
            for (unsigned int j = 0; j < 3; j++) {
                if (i != j) {
                    m.arr[i][j] = this->arr[j][i];
                } else {
                    m.arr[i][j] = this->arr[i][j];
                }
            }
        }

        return m;
    }

    Matrix3f operator+(const Matrix3f &m) {
        Matrix3f r;

        for (unsigned int i = 0; i < 3; i++) {
            for (unsigned int j = 0; j < 3; j++) {
                r.arr[i][j] = this->arr[i][j] + m.arr[i][j];
            }
        }

        return r;
    }

    void operator+=(const Matrix3f &m) {
        *this = (*this) + m;
    }

     Matrix3f operator-(const Matrix3f &m) {
        Matrix3f r;

        for (unsigned int i = 0; i < 3; i++) {
            for (unsigned int j = 0; j < 3; j++) {
                r.arr[i][j] = this->arr[i][j] - m.arr[i][j];
            }
        }

        return r;
    }

    void operator-=(const Matrix3f &m) {
        *this = (*this) - m;
    }

    Matrix3f operator*(const Matrix3f &m) {
        Matrix3f r;

        for (unsigned int i = 0; i < 3; i++) {
            for (unsigned int j = 0; j < 3; j++) {
                float sum = 0.0f;
                for (unsigned int k = 0; k < 3; k++) {
                    sum += this->arr[i][k] * m.arr[k][j]; 
                }
                r.arr[i][j] = sum;
            }
        }

        return r;
    }

    void operator*=(const Matrix3f &m) {
        *this = (*this) * m;
    }

    Vector3f operator*(const Vector3f &v) {
        Vector3f r;

        r.x = this->arr[0][0] * v.x + this->arr[0][1] * v.y + this->arr[0][2] * v.z;
        r.y = this->arr[1][0] * v.x + this->arr[1][1] * v.y + this->arr[1][2] * v.z; 
        r.z = this->arr[2][0] * v.x + this->arr[2][1] * v.y + this->arr[2][2] * v.z;
        return r;
    }
};

/*
NOTE: Matrices are row-major
*/
struct Matrix4f {
    float arr[4][4];

    Matrix4f(float diagonal = 1.0f) {
        for (unsigned int i = 0; i < 4; i++) {
            for (unsigned int j = 0; j < 4; j++) {
                if (i == j) {
                    arr[i][j] = diagonal;
                } else {
                    arr[i][j] = 0.0f;
                }
            }
        }
    }

    /*
    * Ripped from: https://stackoverflow.com/questions/2937702/i-want-to-find-determinant-of-4x4-matrix-in-c-sharp
    */
    float det() { 
        return
        arr[0][3] * arr[1][2] * arr[2][1] * arr[3][0] - arr[0][2] * arr[1][3] * arr[2][1] * arr[3][0] -
        arr[0][3] * arr[1][1] * arr[2][2] * arr[3][0] + arr[0][1] * arr[1][3] * arr[2][2] * arr[3][0] +
        arr[0][2] * arr[1][1] * arr[2][3] * arr[3][0] - arr[0][1] * arr[1][2] * arr[2][3] * arr[3][0] -
        arr[0][3] * arr[1][2] * arr[2][0] * arr[3][1] + arr[0][2] * arr[1][3] * arr[2][0] * arr[3][1] +
        arr[0][3] * arr[1][0] * arr[2][2] * arr[3][1] - arr[0][0] * arr[1][3] * arr[2][2] * arr[3][1] -
        arr[0][2] * arr[1][0] * arr[2][3] * arr[3][1] + arr[0][0] * arr[1][2] * arr[2][3] * arr[3][1] +
        arr[0][3] * arr[1][1] * arr[2][0] * arr[3][2] - arr[0][1] * arr[1][3] * arr[2][0] * arr[3][2] -
        arr[0][3] * arr[1][0] * arr[2][1] * arr[3][2] + arr[0][0] * arr[1][3] * arr[2][1] * arr[3][2] +
        arr[0][1] * arr[1][0] * arr[2][3] * arr[3][2] - arr[0][0] * arr[1][1] * arr[2][3] * arr[3][2] -
        arr[0][2] * arr[1][1] * arr[2][0] * arr[3][3] + arr[0][1] * arr[1][2] * arr[2][0] * arr[3][3] +
        arr[0][2] * arr[1][0] * arr[2][1] * arr[3][3] - arr[0][0] * arr[1][2] * arr[2][1] * arr[3][3] -
        arr[0][1] * arr[1][0] * arr[2][2] * arr[3][3] + arr[0][0] * arr[1][1] * arr[2][2] * arr[3][3];
    }

    /*
    * Ripped from: https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
    */
    Matrix4f inversed() {
        float inv[4][4];

        inv[0][0] = arr[1][1] * arr[2][2] * arr[3][3] -
                    arr[1][1] * arr[2][3] * arr[3][2] -
                    arr[2][1] * arr[1][2] * arr[3][3] +
                    arr[2][1] * arr[1][3] * arr[3][2] +
                    arr[3][1] * arr[1][2] * arr[2][3] -
                    arr[3][1] * arr[1][3] * arr[2][2];

        inv[1][0] = -arr[1][0] * arr[2][2] * arr[3][3] +
                    arr[1][0] * arr[2][3] * arr[3][2] +
                    arr[2][0] * arr[1][2] * arr[3][3] -
                    arr[2][0] * arr[1][3] * arr[3][2] -
                    arr[3][0] * arr[1][2] * arr[2][3] +
                    arr[3][0] * arr[1][3] * arr[2][2];

        inv[2][0] = arr[1][0] * arr[2][1] * arr[3][3] -
                    arr[1][0] * arr[2][3] * arr[3][1] -
                    arr[2][0] * arr[1][1] * arr[3][3] +
                    arr[2][0] * arr[1][3] * arr[3][1] +
                    arr[3][0] * arr[1][1] * arr[2][3] -
                    arr[3][0] * arr[1][3] * arr[2][1];

        inv[3][0] = -arr[1][0] * arr[2][1] * arr[3][2] +
                    arr[1][0] * arr[2][2] * arr[3][1] +
                    arr[2][0] * arr[1][1] * arr[3][2] -
                    arr[2][0] * arr[1][2] * arr[3][1] -
                    arr[3][0] * arr[1][1] * arr[2][2] +
                    arr[3][0] * arr[1][2] * arr[2][1];

        inv[0][1] = -arr[0][1] * arr[2][2] * arr[3][3] +
                    arr[0][1] * arr[2][3] * arr[3][2] +
                    arr[2][1] * arr[0][2] * arr[3][3] -
                    arr[2][1] * arr[0][3] * arr[3][2] -
                    arr[3][1] * arr[0][2] * arr[2][3] +
                    arr[3][1] * arr[0][3] * arr[2][2];

        inv[1][1] = arr[0][0] * arr[2][2] * arr[3][3] -
                    arr[0][0] * arr[2][3] * arr[3][2] -
                    arr[2][0] * arr[0][2] * arr[3][3] +
                    arr[2][0] * arr[0][3] * arr[3][2] +
                    arr[3][0] * arr[0][2] * arr[2][3] -
                    arr[3][0] * arr[0][3] * arr[2][2];

        inv[2][1] = -arr[0][0] * arr[2][1] * arr[3][3] +
                    arr[0][0] * arr[2][3] * arr[3][1] +
                    arr[2][0] * arr[0][1] * arr[3][3] -
                    arr[2][0] * arr[0][3] * arr[3][1] -
                    arr[3][0] * arr[0][1] * arr[2][3] +
                    arr[3][0] * arr[0][3] * arr[2][1];

        inv[3][1] = arr[0][0] * arr[2][1] * arr[3][2] -
                    arr[0][0] * arr[2][2] * arr[3][1] -
                    arr[2][0] * arr[0][1] * arr[3][2] +
                    arr[2][0] * arr[0][2] * arr[3][1] +
                    arr[3][0] * arr[0][1] * arr[2][2] -
                    arr[3][0] * arr[0][2] * arr[2][1];

        inv[0][2] = arr[0][1] * arr[1][2] * arr[3][3] -
                    arr[0][1] * arr[1][3] * arr[3][2] -
                    arr[1][1] * arr[0][2] * arr[3][3] +
                    arr[1][1] * arr[0][3] * arr[3][2] +
                    arr[3][1] * arr[0][2] * arr[1][3] -
                    arr[3][1] * arr[0][3] * arr[1][2];

        inv[1][2] = -arr[0][0] * arr[1][2] * arr[3][3] +
                    arr[0][0] * arr[1][3] * arr[3][2] +
                    arr[1][0] * arr[0][2] * arr[3][3] -
                    arr[1][0] * arr[0][3] * arr[3][2] -
                    arr[3][0] * arr[0][2] * arr[1][3] +
                    arr[3][0] * arr[0][3] * arr[1][2];

        inv[2][2] = arr[0][0] * arr[1][1] * arr[3][3] -
                    arr[0][0] * arr[1][3] * arr[3][1] -
                    arr[1][0] * arr[0][1] * arr[3][3] +
                    arr[1][0] * arr[0][3] * arr[3][1] +
                    arr[3][0] * arr[0][1] * arr[1][3] -
                    arr[3][0] * arr[0][3] * arr[1][1];

        inv[3][2] = -arr[0][0] * arr[1][1] * arr[3][2] +
                    arr[0][0] * arr[1][2] * arr[3][1] +
                    arr[1][0] * arr[0][1] * arr[3][2] -
                    arr[1][0] * arr[0][2] * arr[3][1] -
                    arr[3][0] * arr[0][1] * arr[1][2] +
                    arr[3][0] * arr[0][2] * arr[1][1];

        inv[0][3] = -arr[0][1] * arr[1][2] * arr[2][3] +
                    arr[0][1] * arr[1][3] * arr[2][2] +
                    arr[1][1] * arr[0][2] * arr[2][3] -
                    arr[1][1] * arr[0][3] * arr[2][2] -
                    arr[2][1] * arr[0][2] * arr[1][3] +
                    arr[2][1] * arr[0][3] * arr[1][2];

        inv[1][3] = arr[0][0] * arr[1][2] * arr[2][3] -
                    arr[0][0] * arr[1][3] * arr[2][2] -
                    arr[1][0] * arr[0][2] * arr[2][3] +
                    arr[1][0] * arr[0][3] * arr[2][2] +
                    arr[2][0] * arr[0][2] * arr[1][3] -
                    arr[2][0] * arr[0][3] * arr[1][2];

        inv[2][3] = -arr[0][0] * arr[1][1] * arr[2][3] +
                    arr[0][0] * arr[1][3] * arr[2][1] +
                    arr[1][0] * arr[0][1] * arr[2][3] -
                    arr[1][0] * arr[0][3] * arr[2][1] -
                    arr[2][0] * arr[0][1] * arr[1][3] +
                    arr[2][0] * arr[0][3] * arr[1][1];

        inv[3][3] = arr[0][0] * arr[1][1] * arr[2][2] -
                    arr[0][0] * arr[1][2] * arr[2][1] -
                    arr[1][0] * arr[0][1] * arr[2][2] +
                    arr[1][0] * arr[0][2] * arr[2][1] +
                    arr[2][0] * arr[0][1] * arr[1][2] -
                    arr[2][0] * arr[0][2] * arr[1][1];

        float det = arr[0][0] * inv[0][0] + arr[0][1] * inv[1][0] + arr[0][2] * inv[2][0] + arr[0][3] * inv[3][0];

        if (det == 0.0f) {
            return Matrix4f(1.0f);
        }

        float inv_det = 1.0f / det;

        Matrix4f result(0.0f);
        for (unsigned int i = 0; i < 4; i++) {
            for (unsigned int j = 0; j < 4; j++) {
                result.arr[i][j] = inv[i][j] * inv_det;
            }
        }

        return result;
    }

    Matrix4f transposed() {
        Matrix4f m;
        for (unsigned int i = 0; i < 4; i++) {
            for (unsigned int j = 0; j < 4; j++) {
                m.arr[i][j] = this->arr[j][i];
            }
        }
        return m;
    }

    Matrix4f operator+(const Matrix4f &m) {
        Matrix4f r;
        for (unsigned int i = 0; i < 4; i++) {
            for (unsigned int j = 0; j < 4; j++) {
                r.arr[i][j] = this->arr[i][j] + m.arr[i][j];
            }
        }
        return r;
    }

    void operator+=(const Matrix4f &m) {
        *this = (*this) + m;
    }

    Matrix4f operator-(const Matrix4f &m) {
        Matrix4f r;
        for (unsigned int i = 0; i < 4; i++) {
            for (unsigned int j = 0; j < 4; j++) {
                r.arr[i][j] = this->arr[i][j] - m.arr[i][j];
            }
        }
        return r;
    }

    void operator-=(const Matrix4f &m) {
        *this = (*this) - m;
    }

    Matrix4f operator*(const Matrix4f &m) {
        Matrix4f r(0.0f);
        for (unsigned int i = 0; i < 4; i++) {
            for (unsigned int j = 0; j < 4; j++) {
                float sum = 0.0f;
                for (unsigned int k = 0; k < 4; k++) {
                    sum += this->arr[i][k] * m.arr[k][j]; 
                }
                r.arr[i][j] = sum;
            }
        }
        return r;
    }

    void operator*=(const Matrix4f &m) {
        *this = (*this) * m;
    }

    Vector3f operator*(const Vector3f &v) {
        return Vector3f {
            arr[0][0] * v.x + arr[0][1] * v.y + arr[0][2] * v.z + arr[0][3],
            arr[1][0] * v.x + arr[1][1] * v.y + arr[1][2] * v.z + arr[1][3],
            arr[2][0] * v.x + arr[2][1] * v.y + arr[2][2] * v.z + arr[2][3],
        };
    }
};

struct Quaternion {
    float x, y, z, w;

    Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) {
        this->x = x;
        this->y = y; 
        this->z = z;
        this->w = w;
    }

    static float dot(Quaternion q1, Quaternion q2) {
        return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
    }

    /*
    Angle expected in radians
    Also assumes a normalized Quaternion
    */
    static Quaternion from_axis_angle(Vector3f axis, float angle) {
        float h_angle = angle * 0.5f;
        float c = cosf(h_angle);
        float s = sinf(h_angle);

        Quaternion q(axis.x * s, axis.y * s, axis.z * s, c);
        return q;
    }

    /*
    Angles expected in radians
    Order of rotation will be YXZ
    Also assumes a normalized Quaternion
    */
    static Quaternion from_euler(float x, float y, float z) {
        Quaternion qx = Quaternion::from_axis_angle(Vector3f(1.0f, 0.0f, 0.0f), x);
        Quaternion qy = Quaternion::from_axis_angle(Vector3f(0.0f, 1.0f, 0.0f), y);
        Quaternion qz = Quaternion::from_axis_angle(Vector3f(0.0f, 0.0f, 1.0f), z);

        return qy * qx * qz;
    };

    /*
    Assumes normalized Quaternions
    Math ripped from: https://en.wikipedia.org/wiki/Spherical_linear_interpolation
    */
    static Quaternion slerp(Quaternion q1, Quaternion q2, float t) {
        float dot = Quaternion::dot(q1, q2);

        /*
        Additional shortest path condition
        */
        if (dot < 0.0f) {
            dot *= -1.0f;
            q2 *= -1.0f;
        }

        float angle = acosf(dot);
        float s = sinf(angle);

        // If quaternions are nearly identical fall back to nlerp
        if (dot > 0.9995f) {
            Quaternion r = q1 * (1.0f - t) + q2 * t;
            return r.normalized();
        }

        return q1 * (sinf((1.0f - t) * angle) / s) + q2 * (sinf(t * angle) / s);
    }

    float length() {
        return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
    }

    Quaternion normalized() {
        float l = this->length();

        if (l == 0.0f) {
            return *this;
        }

        float inv_l = 1.0f / l;
        return Quaternion(this->x * inv_l , this->y * inv_l, this->z * inv_l, this->w * inv_l);
    }

    Quaternion operator+(const Quaternion &q) {
        return Quaternion(this->x + q.x, this->y + q.y, this->z + q.z, this->w + q.w);
    }

    void operator+=(const Quaternion &q) {
        *this = (*this) + q;
    }

    Quaternion operator-(const Quaternion &q) {
        return Quaternion(this->x - q.x, this->y - q.y, this->z - q.z, this->w - q.w);
    }

    void operator-=(const Quaternion &q) {
        *this = (*this) - q;
    }

    Quaternion operator*(float s) {
        return Quaternion(this->x * s, this->y * s, this->z * s, this->w * s);
    }

    void operator*=(float s) {
        *this = (*this) * s;
    }

    Quaternion operator*(const Quaternion &q) {
        return Quaternion(
            this->w * q.x + this->x * q.w + this->y * q.z - this->z * q.y,
            this->w * q.y - this->x * q.z + this->y * q.w + this->z * q.x,
            this->w * q.z + this->x * q.y - this->y * q.x + this->z * q.w,
            this->w * q.w - this->x * q.x - this->y * q.y - this->z * q.z
        );
    }

    void operator*=(const Quaternion &q) {
        *this = (*this) * q;
    }

    /*
    Assumes a normalized quaternion
    */
    Vector3f operator*(const Vector3f &v) {
        Quaternion qv(v.x, v.y, v.z, 0.0f);
        Quaternion conj(-this->x, -this->y, -this->z, this->w);

        qv = (*this) * qv * conj;

        return Vector3f(qv.x, qv.y, qv.z);
    }
};