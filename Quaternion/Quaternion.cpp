#include "Quaternion.h"
#include <unordered_map>

#define SQR(x) ((x)*(x))

Quaternion::Quaternion(double w, double x, double y, double z)
        : m_w(w), m_x(x), m_y(y), m_z(z)
{}

Quaternion::Quaternion(double x, double y, double z, std::string order)
{
    std::unordered_map<char, Quaternion> m = {
            {'x', Quaternion{std::cos(x/2.0), std::sin(x/2.0), 0,          0}},
            {'y', Quaternion{std::cos(y/2.0), 0,          std::sin(y/2.0), 0}},
            {'z', Quaternion{std::cos(z/2.0), 0,          0,          std::sin(z/2.0) }}
    };

    Quaternion q{ 1,0,0,0 };
    for (int i = 0; i < 3; ++i)
        q = q * m[order[i]];
    *this = q;
}

Quaternion ToQuaternion(double yaw, double pitch, double roll) // yaw (Z), pitch (Y), roll (X)
{
    // Abbreviations for the various angular functions
    double cy = std::cos(yaw * 0.5);
    double sy = std::sin(yaw * 0.5);
    double cp = std::cos(pitch * 0.5);
    double sp = std::sin(pitch * 0.5);
    double cr = std::cos(roll * 0.5);
    double sr = std::sin(roll * 0.5);

    return Quaternion{
            cr * cp * cy + sr * sp * sy,
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy
    };
}

Quaternion& Quaternion::Scale(double val)
{
    m_w *= val;
    m_x *= val;
    m_y *= val;
    m_z *= val;

    return *this;
}

double Quaternion::Length()const
{
    return std::sqrt(SQR(m_w) + SQR(m_x) + SQR(m_y) + SQR(m_z));
}

Quaternion& Quaternion::Normalize()
{
    auto length = Length();
    if(length > Accuracy)
        return Scale(1.0 / length);
    return *this;
}

Quaternion Quaternion::Invert() const
{
    return Quaternion{m_w, -m_x, -m_y, -m_z}.Normalize();
}

Quaternion operator*(const Quaternion& a, double b)
{
    return Quaternion{ a.m_w * b, a.m_x * b, a.m_y * b, a.m_z * b };
}

Quaternion operator+(const Quaternion& a, const Quaternion& b)
{
    return Quaternion{
            a.m_w + b.m_w,
            a.m_x + b.m_x,
            a.m_y + b.m_y,
            a.m_z + b.m_z
    };
}

Quaternion operator*(const Quaternion& a, const Quaternion& b)
{
    return Quaternion{
            a.m_w * b.m_w - a.m_x * b.m_x - a.m_y * b.m_y - a.m_z * b.m_z,
            a.m_w * b.m_x + a.m_x * b.m_w + a.m_y * b.m_z - a.m_z * b.m_y,
            a.m_w * b.m_y - a.m_x * b.m_z + a.m_y * b.m_w + a.m_z * b.m_x,
            a.m_w * b.m_z + a.m_x * b.m_y - a.m_y * b.m_x + a.m_z * b.m_w
    };
}
