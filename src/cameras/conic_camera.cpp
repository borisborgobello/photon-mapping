/**
 * \file conic_camera.cpp
 * \brief Implementation of class ConicCamera
 * \author B.BORGOBELLO / T. FEIGLER
 */

#include <cmath>
#include <Eigen/Geometry>
#include "conic_camera.hpp"

using namespace Eigen ;

USING_PART_OF_NAMESPACE_EIGEN ; ///< Using namespace Eigen

/**
 * \brief Construction of the ray to emit
 *
 * \param x : x projection parameter between 0 et 1
 * \param y : y projection parameter between 0 et 1
 */
Ray ConicCamera::get_ray(float x, float y) const {
    /// _param_x and _param_y are the opening angles in radian
    /// along axis X and Y
    double angle_x, angle_y ;
    angle_x = - _param_x * (x-0.5) ;
    angle_y = _param_y * (y-0.5) ;

    Vector3D direction = _vector_x.cross(_vector_y) ;
    Vector3D direction_y = direction*cos(angle_y)+(1.0-cos(angle_y))*direction.dot(_vector_x)*_vector_x+sin(angle_y)*_vector_x.cross(direction);
    Vector3D direction_x = direction*cos(angle_x)+(1.0-cos(angle_x))*direction.dot(_vector_y)*_vector_y+sin(angle_x)*_vector_y.cross(direction);
    direction = (direction_y + direction_x).normalized();

	return Ray(_position, direction) ;
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
std::pair<double, double> ConicCamera::can_see(const Point3D& photon_position) const
{
    double x, y;

    Vector3D camera_photon = photon_position - _position;
    Vector3D direction = _vector_x.cross(_vector_y).normalized();

    Vector3D x_projection = (camera_photon - _vector_x.dot(camera_photon)*_vector_x).normalized(); // vector's projet on plane with normal x
    Vector3D y_projection = (camera_photon - _vector_y.dot(camera_photon)*_vector_y).normalized(); // vector's projet on plane with normal x

    double y_angle = acos(x_projection.dot(direction));
    double x_angle = acos(y_projection.dot(direction));

    x_angle = 1.9*x_angle;
    y_angle = 1.9*y_angle;

    if (x_projection.dot(_vector_y) >= 0) { // For y angle
        y_angle = -y_angle;
    }
    if (y_projection.dot(_vector_x) >= 0) { // For x angle, inverted because we are behind the camera
        x_angle = -x_angle;
    }

    if (std::abs(x_angle) > std::abs(_param_x*0.5)) return std::pair<double, double>(-1,-1);
    if (std::abs(y_angle) > std::abs(_param_y*0.5)) return std::pair<double, double>(-1,-1);

    x = x_angle/_param_x + 0.5;
    y = y_angle/_param_y + 0.5;

    //std::cout << "Values " << x << " and " << y << std::endl;

    return std::pair<double, double>(x,y);
}
