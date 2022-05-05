#include <Math/Matrix3.h>
#include <cstring>

Matrix3 Matrix3::operator+(Matrix3 const &other) const {
    Matrix3 m;

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            m.data[i][j] = this->data[i][j] + other.data[i][j];
        }
    }

    return m;
}

Matrix3 &Matrix3::operator=(Matrix3 const &other) {

    memcpy(this->data, other.data, 9 * sizeof(float));
    return *this;

}

Matrix3 Matrix3::operator-(Matrix3 const &other) const {
    Matrix3 m;

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            m.data[i][j] = this->data[i][j] - other.data[i][j];
        }
    }

    return m;
}

Matrix3 Matrix3::operator*(Matrix3 const &other) const {
    Matrix3 m;

    for (size_t i = 0; i < 3; i++) {

        for (size_t j = 0; j < 3; j++) {
            m.data[i][j] = 0;

            for (size_t k = 0; k < 3; k++) {
                m.data[i][j] += this->data[i][k] * other.data[k][j];
            }

        }

    }

    return m;
}

Matrix3 Matrix3::operator*(const float scale) const {
    Matrix3 m;

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            m.data[i][j] = this->data[i][j] * scale;
        }
    }

    return m;
}

Vector3 Matrix3::operator*(const Vector3 other) const {
    Vector3 v;

    v.x = this->data[0][0] * other.x + this->data[0][1] * other.y + this->data[0][2] * other.z;
    v.y = this->data[1][0] * other.x + this->data[1][1] * other.y + this->data[1][2] * other.z;
    v.z = this->data[2][0] * other.x + this->data[2][1] * other.y + this->data[2][2] * other.z;

    return v;
}

void Matrix3::operator+=(Matrix3 const &other) {
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            this->data[i][j] += other.data[i][j];
        }
    }
}

void Matrix3::operator-=(Matrix3 const &other) {
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            this->data[i][j] -= other.data[i][j];
        }
    }
}

void Matrix3::operator*=(Matrix3 const &other) {
    *this = *this * other;
}

void Matrix3::operator*=(const float scale) {

    for (auto & a : this->data) {
        for (auto &b : a) {
            b *= scale;
        }
    }

}

Matrix3 &Matrix3::operator=(const float newData[3][3]) {

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            this->data[i][j] = newData[i][j];
        }
    }

    return *this;
}

bool Matrix3::operator==(Matrix3 const &other) const {

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            if (this->data[i][j] != other.data[i][j]) {
                return false;
            }
        }
    }

    return true;
}

Matrix3 Matrix3::empty() {
    Matrix3 m;
    memset(m.data, 0, 9 * sizeof(float));
    return m;
}

Matrix3 Matrix3::identity() {

    Matrix3 m;

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            m.data[i][j] = (i == j);
        }
    }

    return m;
}

Matrix3 Matrix3::fill(float value) {
    Matrix3 m;

    for (auto & a : m.data) {
        for (auto &b : a) {
            b *= value;
        }
    }

    return m;
}