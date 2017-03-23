//------------------------------------------------------------------------------
// Copyright Luke Titley 2017
//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D>::Vector3(T px, T py, T pz, T pw)
    : x(px), y(py), z(pz), w(pw)
{
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D>::Vector3(T default_value)
{
    for (size_t i = 0; i != D; ++i)
    {
        value[i] = default_value;
    }
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D>::Vector3(const Vector3 & axis, const T angle, const T two)
{
    T halfAngle = angle / two;

    (*this) = axis * sin(halfAngle);
    this->w = cos(halfAngle);
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
bool Vector3<T, D>::operator==(const Vector3& rhs) const
{
    for (size_t i = 0; i != D; ++i)
    {
        if (value[i] != rhs.value[i])
        {
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
bool Vector3<T, D>::operator!=(const Vector3& rhs) const
{
    return !operator==(rhs);
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
bool Vector3<T, D>::operator<(const Vector3& rhs) const
{
    return x < rhs.x && y < rhs.y && z < rhs.z;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
bool Vector3<T, D>::operator>(const Vector3& rhs) const
{
    return x > rhs.x && y > rhs.y && z > rhs.z;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
bool Vector3<T, D>::operator<=(const Vector3& rhs) const
{
    return x <= rhs.x && y <= rhs.y && z <= rhs.z;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
bool Vector3<T, D>::operator>=(const Vector3& rhs) const
{
    return x >= rhs.x && y >= rhs.y && z >= rhs.z;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D> Vector3<T, D>::operator+(const Vector3& rhs) const
{
    Vector3 result = *this;

    for (size_t i = 0; i != D; ++i)
    {
        result.value[i] += rhs.value[i];
    }
    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D> Vector3<T, D>::operator-(const Vector3& rhs) const
{
    Vector3 result = *this;

    for (size_t i = 0; i != D; ++i)
    {
        result.value[i] -= rhs.value[i];
    }
    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D> Vector3<T, D>::operator*(const Vector3& rhs) const
{
    Vector3 result = *this;

    for (size_t i = 0; i != D; ++i)
    {
        result.value[i] *= rhs.value[i];
    }
    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D> Vector3<T, D>::operator/(const Vector3& rhs) const
{
    Vector3 result = *this;

    for (size_t i = 0; i != D; ++i)
    {
        result.value[i] /= rhs.value[i];
    }
    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D>& Vector3<T, D>::operator+=(const Vector3& rhs)
{
    for (size_t i = 0; i != D; ++i)
    {
        value[i] += rhs.value[i];
    }
    return *this;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D>& Vector3<T, D>::operator-=(const Vector3& rhs)
{
    for (size_t i = 0; i != D; ++i)
    {
        value[i] -= rhs.value[i];
    }
    return *this;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D>& Vector3<T, D>::operator*=(const Vector3& rhs)
{
    for (size_t i = 0; i != D; ++i)
    {
        value[i] *= rhs.value[i];
    }
    return *this;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D>& Vector3<T, D>::operator/=(const Vector3& rhs)
{
    for (size_t i = 0; i != D; ++i)
    {
        value[i] /= rhs.value[i];
    }
    return *this;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
T& Vector3<T, D>::operator[](size_t index)
{
    return value[index % D];  // Wrap around
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
const T& Vector3<T, D>::operator[](size_t index) const
{
    return value[index % D];  // Wrap around
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D> Vector3<T, D>::overwrite(size_t index, T value) const
{
    Vector3 result = *this;

    result[index % D] = value;  // Wrap around
    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
T Vector3<T, D>::dot(const Vector3& rhs) const
{
    T result = 0;

    for (size_t i = 0; i != D; ++i)
    {
        result += value[i] * rhs.value[i];
    }
    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D> Vector3<T, D>::cross(const Vector3& rhs) const
{
    T rx = (y * rhs.z) - (z * rhs.y);
    T ry = (z * rhs.x) - (x * rhs.z);
    T rz = (x * rhs.y) - (y * rhs.x);

    Vector3<T> result(rx, ry, rz);
    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
T Vector3<T, D>::scalarTriple(const Vector3& b, const Vector3& c) const
{
    return this->dot(b.cross(c));
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
bool Vector3<T, D>::equals(const Vector3& rhs, const T epsilon) const
{
    for (size_t i = 0; i != D; ++i)
    {
        if (fabs(value[i] - rhs.value[i]) > epsilon)
        {
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D> Vector3<T, D>::slice(const size_t end) const
{
    const T max = end < D ? end : D;

    Vector3<T, D> result(0.0f);

    for (size_t i = 0; i != max; ++i)
    {
        result[i] = (*this)[i];
    }
    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D> Vector3<T, D>::lerp(const Vector3& rhs, T t, const T one) const
{
    Vector3<T> result = ((*this) * Vector3(one - t)) + (rhs * Vector3(t));
    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
void Vector3<T, D>::tangentAndBitangent(Vector3& resultTangent,
                                        Vector3& resultBitangent, const T zero,
                                        const T one) const
{
    Vector3 up(zero, one, zero);
    T vector_dot_up = this->dot(up);

    if (vector_dot_up == one)
    {
        up = Vector3(one, zero, zero);
    }
    else if (vector_dot_up == -one)
    {
        up = Vector3(-one, zero, zero);
    }

    resultTangent = this->cross(up).normalized();
    resultBitangent = resultTangent.cross(*this);
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D> Vector3<T, D>::transformNormalized(const Vector3& i,
                                                 const Vector3& j,
                                                 const Vector3& k) const
{
    const Vector3 result(((i * x) + (j * y) + (k * z)).normalized());

    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D> Vector3<T, D>::transform(const Vector3& i, const Vector3& j,
                                       const Vector3& k) const
{
    const Vector3 result(((i * x) + (j * y) + (k * z)));

    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
T Vector3<T, D>::magSq() const
{
    T result = dot(*this);

    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
T Vector3<T, D>::mag(const T zero) const
{
    T lengthSq = magSq();

    if (lengthSq == zero)
    {
        return zero;
    }
    T result = sqrt(magSq());
    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D> Vector3<T, D>::normalized() const
{
    Vector3<T> result = *this;
    T mag = sqrt(result.magSq());
    for (size_t i = 0; i != D; ++i)
    {
        result.value[i] /= mag;
    }
    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D> Vector3<T, D>::inverse(const T one) const
{
    Vector3<T> result = *this;
    for (size_t i = 0; i != D; ++i)
    {
        result.value[i] *= -one;
    }
    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
T Vector3<T, D>::volume() const
{
    return x * y * z;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
T Vector3<T, D>::area() const
{
    return Vector3(x, y, 1).volume();
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
T Vector3<T, D>::surfaceArea() const
{
    const T areaX = depth * height;
    const T areaY = width * depth;
    const T areaZ = width * height;
    const T halfSurfaceArea = areaX + areaY + areaZ;
    const T surfaceArea = halfSurfaceArea * 2.0f;

    return surfaceArea;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D> Vector3<T, D>::quatProd(const Vector3 & rhs) const
{
    const Vector3 v1 = this->slice(3);
    const Vector3 v2 = rhs.slice(3);
    const T s1 = this->w;
    const T s2 = rhs.w;
    const T s = (s1 * s2) - v1.dot(v2);
    const Vector3 v = (v2 * s1) + (v1 * s2) + (v1.cross(v2));
    const Vector3 result(v.x, v.y, v.z, s);

    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D> Vector3<T, D>::quatInverse() const
{
    Vector3 result =
        Vector3(-this->x, -this->y, -this->z, this->w) / this->magSq();

    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D> Vector3<T, D>::rotateBy(const Vector3 & axis, const T angle) const
{
    Vector3 rotation(axis, angle);
    Vector3 result = rotateBy(rotation);

    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D> Vector3<T, D>::rotateBy(const Vector3 & rotation) const
{
    Vector3 result = rotation.quatProd(*this).quatProd(rotation.quatInverse());

    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
template <typename M>
Vector3<T, D> Vector3<T, D>::map(M t) const
{
    Vector3 result;

    for (size_t i = 0; i != D; ++i)
    {
        result.value[i] = t(value[i]);
    }
    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
template <typename M>
Vector3<M, D> Vector3<T, D>::cast() const
{
    Vector3<M, D> result;
    for (size_t i = 0; i != D; ++i)
    {
        result.value[i] = static_cast<T>(value[i]);
    }
    return result;
}

//------------------------------------------------------------------------------
template <typename T, size_t D>
Vector3<T, D>::operator std::string() const
{
    std::stringstream ss;
    ss << x;
    for (size_t i = 1; i < D; ++i)
    {
        ss << " " << value[i];
    }
    return ss.str();
}

