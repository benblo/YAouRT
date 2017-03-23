//------------------------------------------------------------------------------
// Copyright Luke Titley 2017
//------------------------------------------------------------------------------
#ifndef PT_H_VECTOR
#define PT_H_VECTOR
//------------------------------------------------------------------------------
#include <cmath>
#include <cstddef>
#include <string>
#include <sstream>
//------------------------------------------------------------------------------

/// \brief Aligns the given type, field or variable to a 16 byte boundry.
/// The following examples are valid.
/// \code
/// struct X{ char a[16]; } LT_ALIGN16;
/// \endcode
/// \code
/// struct X{ char a[16] LT_ALIGN16; };
/// \endcode
/// \code
/// char a[16] LT_ALIGN16;
/// \endcode
#define LT_ALIGN16 __attribute__((aligned(16)))

//------------------------------------------------------------------------------
// Vector3
//------------------------------------------------------------------------------
/// \brief Implements a box standard vector class, with all the methods you
/// would expect from a vanilla linear algebra vector implementation.
///
/// The following examples outline how this implementation can be used.
//------------------------------------------------------------------------------
template <typename T, size_t D = 4>
class Vector3
{
public:
    /// \brief Value indices.
    ///
    /// Mainly for use with Vector3::overwrite, but can be used with
    /// Vector3::operator[] and the data member Vector3::value.
    ///
    enum
    {
        kX = 0,
        kY = 1,
        kZ = 2,
        kW = 3,
        kR = 0,
        kG = 1,
        kB = 2,
        kA = 3,
        kHeight = 0,
        kWidth = 1,
        kDepth = 2
    };
    union
    {
        /// The Vector3 members as position coordinates.
        struct
        {
            T x;
            T y;
            T z;
            T w;
        };

        /// The Vector3 members as colour values.
        struct
        {
            T r;
            T g;
            T b;
            T a;
        };
        /// The Vector3 members as dimensions.
        struct
        {
            T width;
            T height;
            T depth;
        };
        T value[D];
    };

    /// \brief Initializes a Vector3 class.
    ///
    /// At least two values must be provided.
    /// \param px: The first element of the vector.
    /// \param py: The second element of the vector.
    /// \param pz: The third element of the vector.
    /// \param pw: The fourth element of the vector.
    ///
    /// <b>Example : Normal Construction</b>
    /// \snippet test_vector.cpp test_vector constructor_XYWW
    inline Vector3(T px, T py, T pz = 0.0f, T pw = 0.0f);

    /// \brief Initializes a Vector3 class.
    ///
    /// \param px: A single value to set for all the elements in the vector.
    ///
    /// <b>Example : Setting all elements to a default value</b>
    /// \snippet test_vector.cpp test_vector constructor_All_One_Value
    inline Vector3(T default_value = 0.0f);

    inline Vector3(const Vector3 & axis, const T angle, const T two = 2.0f);

    /// \return true if 'rhs' is bit for bit identical to this Vector3
    /// instance, false otherwise.
    ///
    /// <b>Example : Comparison : Equals</b>
    /// \snippet test_vector.cpp test_vector comparison_equals
    inline bool operator==(const Vector3& rhs) const;

    /// \return true if 'rhs' is bit for bit identical to this Vector3
    /// instance, false otherwise.
    inline bool operator!=(const Vector3& rhs) const;

    /// \return true if all the elements in rhs are greater than the elements in
    /// this Vector3 instance, false if not.
    ///
    /// <b>Example : Comparison : Less than / Greater than</b>
    /// \snippet test_vector.cpp test_vector comparison_lessThanGreaterThan
    inline bool operator<(const Vector3& rhs) const;
    /// \return true if all the elements in rhs are smaller than the elements in
    /// this Vector3 instance, false if not.
    inline bool operator>(const Vector3& rhs) const;
    /// \return true if all the elements in rhs are greater than or equal to the
    /// elements in this Vector3 instance, false if not.
    inline bool operator<=(const Vector3& rhs) const;
    /// \return true if all the elements in rhs are smaller than or equal to the
    /// elements in this Vector3 instance, false if not.
    inline bool operator>=(const Vector3& rhs) const;

    /// \brief Adds the elements of rhs to the elements of this Vector3
    /// instance.
    /// \return A reference to this Vector3 instance.
    ///
    /// <b>Example : Arithmetic Non Const</b>
    /// \snippet test_vector.cpp test_vector arithmetic
    inline Vector3& operator+=(const Vector3& rhs);
    /// \brief Subtracts the elements of rhs from the elements of this
    /// Vector3 instance.
    /// \return A reference to this Vector3 instance.
    inline Vector3& operator-=(const Vector3& rhs);
    /// \brief Multiplies the elements of rhs with the elements of this
    /// Vector3 instance.
    /// \return A reference to this Vector3 instance.
    inline Vector3& operator*=(const Vector3& rhs);
    /// \brief Divides this Vector3's elements by the elements in rhs.
    /// \return A reference to this Vector3 instance.
    inline Vector3& operator/=(const Vector3& rhs);

    /// \brief Adds the elements of rhs to the elements of this Vector3
    /// instance.
    /// \return A new Vector3 instance containing the result.
    ///
    /// <b>Example : Arithmetic</b>
    /// \snippet test_vector.cpp test_vector arithmetic_newResult
    inline Vector3 operator+(const Vector3& rhs) const;
    /// \brief Subtracts the elements of rhs from the elements of this
    /// Vector3 instance.
    /// \return A new Vector3 instance containing the result.
    inline Vector3 operator-(const Vector3& rhs) const;
    /// \brief Multiplies the elements of rhs with the elements of this
    /// Vector3 instance.
    /// \return A new Vector3 instance containing the result.
    inline Vector3 operator*(const Vector3& rhs) const;
    /// \brief Divides this Vector3's elements by the elements in rhs.
    /// \return A new Vector3 instance containing the result.
    inline Vector3 operator/(const Vector3& rhs) const;

    /// \param index: The position of the element to return. If index is greater
    /// than the number of elements in this Vector3 instance, then it will
    /// wrap around. So that index=3 for a vector containing only three elements
    /// will be the same as index=0. For a Vector3 where D=4, index=-1 is
    /// the same as index=3.
    /// \return A reference to the element at the position given by 'index'.
    ///
    /// <b>Example : Indices</b>
    /// \snippet test_vector.cpp test_vector indices
    inline T& operator[](size_t index);

    /// \param index: The position of the element to return. If index is greater
    /// than the number of elements in this Vector3 instance, then it will
    /// wrap around. So that index=3 for a vector containing only three elements
    /// will be the same as index=0. For a Vector3 where D=4, index=-1 is
    /// the same as index=3.
    /// \return A reference to the element at the position given by 'index'.
    inline const T& operator[](size_t index) const;

    /// \return The scalar product of this Vector3 instance and 'rhs'.
    ///
    /// <b>Example : Dot Product</b>
    /// \snippet test_vector.cpp test_vector dot
    inline T dot(const Vector3& rhs) const;

    /// \return The vector product of this Vector3 instance and 'rhs'.
    /// \usage Only applies to 3 dimensional Vector3 instances.
    ///
    /// <b>Example : Cross Product</b>
    /// \snippet test_vector.cpp test_vector cross
    inline Vector3 cross(const Vector3& rhs) const;

    /// \return The scalar triple product if this Vector3 instance with 'a'
    /// and 'b'. Can be used to compute the volume of the parallelepiped defined
    /// by this, b, and c. Used by the intersection routines.
    ///
    /// <b>Example : Scalar Triple Product</b>
    /// \snippet test_vector.cpp test_vector scalarTriple
    inline T scalarTriple(const Vector3& b, const Vector3& c) const;

    /// \param index: The index of the element to overwrite.
    /// \param value: The new value to replace element with.
    /// \return A copy of this Vector3 instance, with the value of the element
    /// given by 'index' replaced by the value given by 'value'.
    ///
    /// <b>For Example</b>
    /// \code
    /// Vector3<int> opaque = Vector3<int>(255);
    /// Vector3<int> transparent = opaque.overwrite(Vector3<int>::kA,0);
    /// \endcode
    ///
    /// <b>Example : Overwrite</b>
    /// \snippet test_vector.cpp test_vector overwrite
    inline Vector3 overwrite(size_t index, T value) const;

    /// \param epsilon: The amount of permitted difference between the values of
    /// 'rhs' and this Vector3 instance.
    /// \return true if the values of 'rhs' are very similar to the ones in this
    /// Vector3 instance. false if they are too different.
    ///
    /// <b>Example : Equals</b>
    /// \snippet test_vector.cpp test_vector equals
    inline bool equals(const Vector3& rhs, const T epsilon = 0.001f) const;

    inline Vector3 slice(const size_t end) const;

    /// \param rhs: The Vector3 to interpolate with.
    /// \param t: The interpolation factor.
    /// \param one: If interpolation Vector3 instances where T != float then
    /// one must be given. It should be the value for '1' for type T.
    /// \return The linear interpolation of this Vector3 instance with
    /// 'rhs'.
    ///
    /// <b>Example : Linear Interpolation</b>
    /// \snippet test_vector.cpp test_vector lerp
    inline Vector3 lerp(const Vector3& rhs, T t, const T one = 1.0f) const;

    /// \brief Compute the vectors that are orthogonal to this vector.
    ///
    /// Useful for creating a coordinate frame around this vector.
    /// A matrix can be constructed from these results in the following manner.
    ///
    /// \code
    /// vector.tangentAndBitangent(tangent, biTrangent);
    /// Vector3<float> x = tangent
    /// Vector3<float> y = vector
    /// Vector3<float> z = biTangent
    /// Matrix<float> frame(x,y,z)
    /// \endcode
    ///
    /// \param resultTangent[out] : The resulting tangent vector.
    /// \param resultBiTangent[out] : The resulting bi-tangent vector.
    /// \param zero: If working with Vector3 instances where T != float then
    /// zero must be given. It should be the value for zero for type T. For
    /// example when T=size zero=0, when T=double zero=0.0, when T=float
    /// zero=0.0f.
    /// \param one: If working with Vector3 instances where T != float then
    /// one must be given. It should be the value for one for type T. For
    /// example when T=size one=1, when T=double one=1.0, when T=float one=1.0f
    ///
    /// <b>Example : Tangents and Bi-Tangents</b>
    /// \snippet test_vector.cpp test_vector tangentAndBitangent
    inline void tangentAndBitangent(Vector3& resultTangent,
                                    Vector3& resultBitangent,
                                    const T zero = 0.0f,
                                    const T one = 1.0f) const;

    /// \return A copy of this Vector3 instance transformed by the
    /// coordinate frame defined by i, j and k. This is a 3x3 matrix
    /// transformation where i,j and k are the rows of the matrix.
    ///
    /// <b>Example : Transform</b>
    /// \snippet test_vector.cpp test_vector transform
    inline Vector3 transformNormalized(const Vector3& i, const Vector3& j,
                                       const Vector3& k) const;

    /// \return A copy of this Vector3 instance transformed by the
    /// coordinate frame defined by i, j and k. This is a 3x3 matrix
    /// transformation where i,j and k are the rows of the matrix.
    ///
    /// <b>Example : Transform</b>
    /// \snippet test_vector.cpp test_vector transform
    inline Vector3 transform(const Vector3& i, const Vector3& j,
                             const Vector3& k) const;

    /// \return The square of the magnitude of this Vector3 instance.
    ///
    /// <b>Example : Magnitude Squared</b>
    /// \snippet test_vector.cpp test_vector magSq
    inline T magSq() const;

    /// \return The magnitude of this Vector3 instance.
    ///
    /// <b>Example : Magnitude</b>
    /// \snippet test_vector.cpp test_vector mag
    inline T mag(const T zero = 0.0f) const;

    /// \return A copy of this Vector3 instance with all elements
    /// normalized.
    ///
    /// <b>Example : Normalization</b>
    /// \snippet test_vector.cpp test_vector normalized
    inline Vector3 normalized() const;

    /// \return The inverse of this Vector3 class.
    ///
    /// <b>Example : Inverse Vector</b>
    /// \snippet test_vector.cpp test_vector inverse
    inline Vector3 inverse(const T one = 1.0f) const;

    /// \return The product of the first three elements of this Vector3
    /// instance.
    ///
    /// <b>Example : Volume</b>
    /// \snippet test_vector.cpp test_vector volume
    inline T volume() const;

    /// \return The product of the first two elements of this Vector3
    /// instance.
    ///
    /// <b>Example : Area</b>
    /// \snippet test_vector.cpp test_vector area
    inline T area() const;

    /// \return The surface area of a prism whose width, height and depth are
    /// the width, height and depth stored in this Vector3 instance.
    ///
    /// <b>Example : SurfaceArea</b>
    /// \snippet test_vector.cpp test_vector surfaceArea
    inline T surfaceArea() const;

    /// The quaternion product of this vector
    inline Vector3 quatProd(const Vector3 & rhs) const;

    ///  The quaternion inverse
    inline Vector3 quatInverse() const;

    /// Rotate this vector around the given axis by the given angle.
    ///
    /// This is the same as: rotateBy(Vector3(axis, angle))
    ///
    Vector3<T, D> rotateBy(const Vector3 & axis, const T angle) const;

    /// Rotate this vector by the given quaternion
    Vector3<T, D> rotateBy(const Vector3 & rotation) const;

    /// \param t: A function or functor that will be used to transform a copy
    /// of the elements in this Vector3 instance.
    /// \return A copy of this Vector3 instance, where each of the elements
    /// have been transformed by the function or functor given by 't'.
    ///
    /// <b>Example : Map</b>
    /// \snippet test_vector.cpp test_vector map
    template <typename M>
    Vector3 map(M t) const;

    /// \return A new Vector3 instance. Where each of the elements is of
    /// type 'M' and is the result of casting the equivalent element in this
    /// Vector3 instance from type T to type M.
    ///
    /// <b>Example : Cast</b>
    /// \snippet test_vector.cpp test_vector cast
    template <typename M>
    Vector3<M, D> cast() const;

    /// \return This Vector3 instance converted to a std::string
    /// representation.
    ///
    /// <b>Example : Convert to std::string</b>
    /// \snippet test_vector.cpp test_vector operator_str_string
    inline operator std::string() const;
};// LT_ALIGN16;

typedef Vector3<float> Vec3f;
typedef Vector3<float> Vec2f;
typedef Vector3<int> Vec3i;
typedef Vector3<int> Vec2i;
typedef Vector3<unsigned int> Vec3u;
typedef Vector3<unsigned int> Vec2u;
typedef Vector3<float> Color;
typedef Vector3<unsigned char> RGBA;
// typedef Vector3<unsigned char, 3> RGB;

//------------------------------------------------------------------------------
// A box standard type agnostic implementation
#include "vector_impl.h"

#endif
