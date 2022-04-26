#include <Math/Matrix4.h>
#include <cmath>

Matrix4 Matrix4::operator+(Matrix4 const &other) const {

    Matrix4 m;

    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            m.data[i][j] = this->data[i][j] + other.data[i][j];
        }
    }

    return m;

}

Matrix4 Matrix4::operator-(Matrix4 const &other) const {

    Matrix4 m;

    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            m.data[i][j] = this->data[i][j] - other.data[i][j];
        }
    }

    return m;

}

Matrix4 Matrix4::operator*(Matrix4 const &other) const {
    Matrix4 newMatrix = Matrix4();

    for (size_t i = 0; i < 4; i++) {

        for (size_t j = 0; j < 4; j++) {

            newMatrix.data[i][j] = 0;

            for (size_t k = 0; k < 4; k++) {
                newMatrix.data[i][j] += this->data[i][k] * other.data[k][j];
            }

        }

    }

    return newMatrix;
}


Matrix4 Matrix4::operator*(const float scale) const {

    Matrix4 m;

    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            m.data[i][j] = this->data[i][j] * scale;
        }
    }

    return m;

}

Vector3 Matrix4::operator*(const Vector3 other) const {

    const float x = this->data[0][0] * other.x + this->data[1][0] * other.y + this->data[2][0] * other.z;
    const float y = this->data[0][1] * other.x + this->data[1][1] * other.y + this->data[2][1] * other.z;
    const float z = this->data[0][2] * other.x + this->data[1][2] * other.y + this->data[2][2] * other.z;

    return { x, y, z };

}

void Matrix4::operator*=(const float scale) {

    for (auto & a : this->data) {
        for (auto &b : a) {
            b *= scale;
        }
    }

}

Matrix4 &Matrix4::operator=(const float newData[4][4]) {
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            this->data[i][j] = newData[i][j];
        }
    }
    return *this;
}

void Matrix4::operator*=(Matrix4 const &other) {
    *this = *this * other;
}

void Matrix4::operator+=(Matrix4 const &other) {

    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            this->data[i][j] += other.data[i][j];
        }
    }

}

void Matrix4::operator-=(Matrix4 const &other) {

    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            this->data[i][j] -= other.data[i][j];
        }
    }

}

bool Matrix4::operator==(Matrix4 const &other) const {

    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            if (this->data[i][j] != other.data[i][j]) {

                return false;

            }

        }
    }

    return true;

}

Vector4 Matrix4::operator[](const size_t i) const {
    return { data[i][0], data[i][1], data[i][2], data[i][3] };
}

void Matrix4::rotateX(const float radians) {

    const float cosT = std::cos(radians);
    const float sinT = std::sin(radians);

    Matrix4 newMatrix = identity();

    newMatrix.data[1][1] = cosT;
    newMatrix.data[1][2] = -sinT;
    newMatrix.data[2][1] = sinT;
    newMatrix.data[2][2] = cosT;

    *this *= newMatrix;

}

void Matrix4::rotateY(const float radians) {

    const float cosT = std::cos(radians);
    const float sinT = std::sin(radians);

    Matrix4 newMatrix = identity();

    newMatrix.data[0][0] = cosT;
    newMatrix.data[0][2] = sinT;
    newMatrix.data[2][0] = -sinT;
    newMatrix.data[2][2] = cosT;

    *this *= newMatrix;

}

void Matrix4::rotateZ(const float radians) {

    const float cosT = std::cos(radians);
    const float sinT = std::sin(radians);

    Matrix4 newMatrix = identity();

    newMatrix.data[0][0] = cosT;
    newMatrix.data[0][1] = sinT;
    newMatrix.data[1][0] = -sinT;
    newMatrix.data[1][1] = cosT;

    *this *= newMatrix;

}

void Matrix4::rotate(float radians, Vector3 axis) {

    const Matrix4 rotation = Matrix4::rotation(radians, axis);
    const auto r = rotation.data;

    const Vector4 a = (*this)[0] * r[0][0] + (*this)[1] * r[0][1] + (*this)[2] * r[0][2];
    const Vector4 b = (*this)[0] * r[1][0] + (*this)[1] * r[1][1] + (*this)[2] * r[1][2];
    const Vector4 c = (*this)[0] * r[2][0] + (*this)[1] * r[2][1] + (*this)[2] * r[2][2];
    const Vector4 d = (*this)[3];

    const float da[4][4] = {

            {a.x, a.y, a.z, a.w},
            {b.x, b.y, b.z, a.w},
            {c.x, c.y, c.z, c.w},
            {d.x, d.y, d.z, d.w},

    };

    *this = copyingData(da);

}


Matrix4 Matrix4::rotation(const float radians, Vector3 axis) {

    axis.normalize();
    const float c = cos(radians);
    const float s = sin(radians);
    const float o = 1.0f - c;

    Matrix4 rotate = identity();

    rotate.data[0][0] = c + (axis.x * axis.x) * o;
    rotate.data[1][0] = (axis.x * axis.y * o) - (axis.z * s);
    rotate.data[2][0] = axis.x * axis.z * o + axis.y * s;

    rotate.data[0][1] = (axis.x * axis.y * o) + (axis.z * s);
    rotate.data[1][1] = c + (axis.y * axis.y) * o;
    rotate.data[2][1] = axis.y * axis.z * o - axis.x * s;

    rotate.data[0][2] = (axis.x * axis.z * o) - (axis.y * s);
    rotate.data[1][2] = axis.y * axis.z * o + axis.x * s;
    rotate.data[2][2] = c + (axis.z * axis.z) * o;

    return rotate;

}

// https://en.wikipedia.org/wiki/Orthographic_projection
Matrix4 Matrix4::orthographic(const float left, const float right, const float bottom, const float top, const float near, const float far) {

    Matrix4 newMatrix = empty();

    newMatrix.data[0][0] = 2.0f / (right - left);
    newMatrix.data[1][1] = 2.0f / (top - bottom);
    newMatrix.data[2][2] = -2.0f / (far - near);
    newMatrix.data[3][0] = -(right + left) / (right - left);
    newMatrix.data[3][1] = -(top + bottom) / (top - bottom);
    newMatrix.data[3][2] = -(far + near) / (far - near);
    newMatrix.data[3][3] = 1.0f;

    return newMatrix;

}

Matrix4 Matrix4::perspective(const float fov, const float aspectRatio, const float near, const float far) {

    Matrix4 newMatrix = empty();

    const float tanFov = tan(Math::toRadians(fov) / 2.0f);
    const float fmn = (far - near);

    newMatrix.data[0][0] = 1.0f / (aspectRatio * tanFov);
    newMatrix.data[1][1] = 1.0f / tanFov;
    newMatrix.data[2][2] = (far + near) / fmn;
    newMatrix.data[2][3] = 1.0f;
    newMatrix.data[3][2] = -(near * far) / fmn;

    return newMatrix;

}

Matrix4 Matrix4::lookAt(Vector3 eye, Vector3 at, const Vector3 up) {

    Vector3 zAxis = Vector3::normalize(at - eye);
    const Vector3 xAxis = Vector3::normalize(Vector3::cross(up, zAxis));
    const Vector3 yAxis = Vector3::cross(zAxis, xAxis);

    zAxis.negate();

    Matrix4 newMatrix = empty();

    newMatrix.data[0][0] = xAxis.x;
    newMatrix.data[1][0] = xAxis.y;
    newMatrix.data[2][0] = xAxis.z;
    newMatrix.data[3][0] = -Vector3::dot(xAxis, eye);

    newMatrix.data[0][1] = yAxis.x;
    newMatrix.data[1][1] = yAxis.y;
    newMatrix.data[2][1] = yAxis.z;
    newMatrix.data[3][1] = -Vector3::dot(yAxis, eye);

    newMatrix.data[0][2] = -zAxis.x;
    newMatrix.data[1][2] = -zAxis.y;
    newMatrix.data[2][2] = -zAxis.z;
    newMatrix.data[3][2] = Vector3::dot(zAxis, eye);
    newMatrix.data[3][3] = 1.0f;

    return newMatrix;

}

void Matrix4::scale(Vector3 const &factor) {

    this->data[0][0] *= factor.x;
    this->data[1][1] *= factor.y;
    this->data[2][2] *= factor.z;

}

Matrix4 Matrix4::modelMatrix(Transform const &transform) {

    Matrix4 m = Matrix4::identity();

    m.rotateX(Math::toRadians(transform.rotation.x));
    m.rotateY(Math::toRadians(transform.rotation.y));
    m.rotateZ(Math::toRadians(transform.rotation.z));
    m.translate(transform.position);
    m.scale(transform.scale);

    return m;
}

Matrix4 Matrix4::empty() {

    Matrix4 m;
    memset(m.data, 0, 16 * sizeof(float));
    return m;

}

Matrix4 Matrix4::identity() {

    Matrix4 m;

    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            m.data[i][j] = (i == j);
        }
    }

    return m;

}

Matrix4 Matrix4::fill(const float value) {

    Matrix4 m;

    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            m.data[i][j] = value;
        }
    }

    return m;

}

void Matrix4::translate(Vector3 const &translate) {

    this->data[3][0] += translate.x;
    this->data[3][1] += translate.y;
    this->data[3][2] += translate.z;

}


Matrix4 Matrix4::translation(Vector3 const &translate) {

    Matrix4 m = Matrix4::identity();

    m.data[3][0] = translate.x;
    m.data[3][1] = translate.y;
    m.data[3][2] = translate.z;

    return m;

}

Matrix4 Matrix4::copyingData(const float newData[4][4]) {

    Matrix4 newMatrix;
    memcpy(newMatrix.data, newData, 16 * sizeof(float));
    return newMatrix;

}

Matrix4 Matrix4::copyingData(const float *newData) {

    Matrix4 newMatrix;
    memcpy(newMatrix.data, newData, 16 * sizeof(float));
    return newMatrix;

}
