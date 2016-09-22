/**
 * \file launchable.cpp
 * \brief Implementation of class Launchable
 * \author T.FEIGLER / B.BORGOBELLO
 */

#include <cmath>
#include "launchable.hpp"

/***
 * \param couple : first member is the intersection point,
 * second is the normal at this point
 *
 * This function takes the intersection couple and returns
 * the new direction of the reflected launchable
 */
Vector3D Launchable::get_reflected(const Couple3D& couple) const
{
	Vector3D reflection_normal = (couple.second).normalized() ;
	return _direction - 2.0 * ( _direction.dot( reflection_normal ) * reflection_normal ) ;
}


/***
 * \param couple : first member is the intersection point,
 * second is the normal at this point
 * \param refraction_ratio : ratio incoming index/outcoming index
 *
 * This function takes the intersection couple and returns
 * the new direction of the refracted launchable
 * If we are in a case of total reflection, it returns the reflected
 * direction
 */
Vector3D Launchable::get_refracted(const Couple3D& couple, double refraction_ratio) const
{
    Vector3D normal = couple.second.normalized();

    if (refraction_ratio == 1) return _direction; // Surfaces and same index
    if (normal.dot(_direction) + 1 < 1e-9) return _direction; // Incoming normally

    double sin_i = (normal.cross(_direction)).norm();
    double sin_r = refraction_ratio*sin_i;

    //std::cout << "In angle " << asin(sin_i)*180/M_PI << std::endl;

    if (sin_r > 1) { // full reflexion
        return get_reflected(couple);
    }
    Vector3D tangent = (_direction + normal).normalized();

    //std::cout << "Out angle " << asin(sin_r)*180/M_PI << std::endl;

    Vector3D new_direction = -normal*sqrt(1-sin_r*sin_r) + tangent * sin_r;

    return new_direction.normalized();
}
