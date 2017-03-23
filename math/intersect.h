//------------------------------------------------------------------------------
// Copyright Luke Titley 2017
//------------------------------------------------------------------------------
#include "vector.h"
#include <limits>

//------------------------------------------------------------------------------
// sq
//------------------------------------------------------------------------------
template<typename T>
T sq(T a)
{
    return a * a;
}

//------------------------------------------------------------------------------
// intersect_sphere
//------------------------------------------------------------------------------
inline bool intersect_sphere(float & t, const Vec3f & rayDirection,
                             const Vec3f & rayPosition,
                             const Vec3f & spherePosition,
                             const float sphereRadius)
{
    const Vec3f center = spherePosition - rayPosition;
    const Vec3f centerInverse = center.inverse();
    const float r = sphereRadius;
    const float root = sq(rayDirection.dot(centerInverse)) -
                          center.inverse().dot(centerInverse) + sq(r);

    if (root < 0)
    {
        return false;
    }

    float d_a = -rayDirection.dot(center.inverse()) + sqrt(root);
    float d_b = -rayDirection.dot(center.inverse()) - sqrt(root);

    // Intersections behind the camera
    // don't count.
    //
    //const float MAX = std::numeric_limits<float>::max();
    const float MAX = FLT_MAX;
    d_a = d_a < 0.0f ? MAX : d_a;
    d_b = d_b < 0.0f ? MAX : d_b;
    const float d = d_a < d_b ? d_a : d_b;

    // Which result is the closest ?
    //
    if (d < 0 || d > t)
    {
        return false;
    }

    t = d;

    return true;
}

//------------------------------------------------------------------------------
// intersect_plane
//------------------------------------------------------------------------------
inline bool intersect_plane(float & t, const Vec3f & rayDirection,
                            const Vec3f & rayPosition, const size_t planeAxis,
                            const float planePosition)
{
    Vec3f plane(0.0f);
    plane[planeAxis] = planePosition;

    Vec3f minus_one(-1.0f);
    Vec3f plane_normal(0.0f);
    if (planePosition == 0.0f)
    {
        plane_normal[planeAxis] = minus_one[planeAxis];
    }
    else
    {
        plane_normal[planeAxis] = -planePosition;
        plane_normal = plane_normal.normalized();
    }

    const float bottom = rayDirection.dot(plane_normal);

    // If the ray is parrelel to the plane.
    // They'll be no intersection
    if (bottom == 0.0f)
    {
        return false;
    }
    const float top = (plane - rayPosition).dot(plane_normal);
    const float d = top / bottom;

    // If we're behind the ray
    // or something closer
    // has already intersected
    // then fail to intersect.
    if (d < 0.0f || d > t)
    {
        return false;
    }

    // Fill in the information
    t = d;

    return true;
}

