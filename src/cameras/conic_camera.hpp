#ifndef CONIC_CAMERA_HPP_
#define CONIC_CAMERA_HPP_

/**
 * \file conic_camera.hpp
 * \brief Description of class ConicCamera
 * \author T.FEIGLER / B.BORGOBELLO
 */

#include "camera.hpp"

/**
 * \class ConicCamera
 * \brief Class representing a camera similar to eye vision
 *
 * Class ConicCamera describes the projection method used
 * to render a scene into space like the eye does
 */

class ConicCamera : public Camera {
public :
    /**
	 * \brief Constructor
	 *
	 * Constructor of class ConicCamera
	 *
	 * \param position : position of the camera
	 * \param vector_x : a first vector of projection
	 * \param vector_y : a second vector of projection
	 * \param param_x : a first parameter of projection
	 * \param param_y : a second parameter of projection
	 */
    ConicCamera(Point3D position, Vector3D vector_x, Vector3D vector_y, float param_x, float param_y) : ///< Constructor calls Camera 's constructor
        Camera(position, vector_x, vector_y, param_x, param_y)
    {}
    Ray get_ray(float, float) const ; ///< Ray construction method
    std::pair<double, double> can_see(const Point3D&) const;	///< Returns which pixel sees the given point
};

#endif
