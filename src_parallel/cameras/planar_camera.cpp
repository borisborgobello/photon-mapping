/**
 * \file conic_camera.cpp
 * \brief Implementation of class PlanarCamera
 * \author B.BORGOBELLO / T. FEIGLER
 */

#include <cmath>
#include <Eigen/Geometry>
#include "planar_camera.hpp"

using namespace Eigen ;

USING_PART_OF_NAMESPACE_EIGEN ;

/**
 * \brief Construction of the ray to emit
 *
 * \param x : x projection parameter between 0 et 1
 * \param y : y projection parameter between 0 et 1
 */
Ray PlanarCamera::get_ray(float x, float y) const {
    /// _param_x and _param_y are the sizes along
    /// _vector_x and _vector_y
    double translat_x, translat_y ;
    translat_x = _param_x * (x-0.5) ;
    translat_y = _param_y * (y-0.5) ;

	Vector3D direction = _vector_x.cross(_vector_y) ;
	Point3D origine = _position - translat_x*_vector_x + translat_y*_vector_y ;

	return Ray(origine, direction) ;
}

/**
 * \brief Determines the correponding pixel that sees a photon in space
 *
 * Returns two doubles between 1 and 0 reprensenting the position of the pixel
 * seeing the given position of the photon
 * If no pixel sees the photon position, the returned couple is (-1, -1)
 * Used to counter raytrace the photon-map efficiently.
 *
 * \param photon_position : the position of the photon in space
 */
std::pair<double, double> PlanarCamera::can_see(const Point3D& photon_position) const
{
    double x, y;

    Vector3D camera_photon = photon_position - _position;
    Vector3D direction = _vector_x.cross(_vector_y);

    double x_value = - camera_photon.dot(_vector_x);
    double y_value = camera_photon.dot(_vector_y);

    if (camera_photon.dot(direction) <= 0) return std::pair<double, double>(-1,-1);
    if (std::abs(x_value) > std::abs(_param_x*0.5)) return std::pair<double, double>(-1,-1);
    if (std::abs(y_value) > std::abs(_param_y*0.5)) return std::pair<double, double>(-1,-1);

    x = x_value/_param_x + 0.5;
    y = y_value/_param_y + 0.5;

    //std::cout << "Values " << x << " and " << y << std::endl;

    return std::pair<double, double>(x,y);
}
