#ifndef CAMERA_HPP_
#define CAMERA_HPP_

/**
 * \file camera.hpp
 * \brief Description of class Camera
 * \author T.FEIGLER
 */

#include "../geometry.hpp"
#include "../launchables/ray.hpp"

/**
 * \class Camera
 * \brief Class (abstract) representing a camera in space
 *
 * Abstract class Camera describes the projection method used
 * to render a scene into space
 */
class Camera {

public :
	/**
	 * \brief Constructor
	 *
	 * Constructor of class Camera
	 *
	 * \param position : position of the camera (abstract)
	 * \param vector_x : a first vector of projection (abstract)
	 * \param vector_y : a second vector of projection (abstract)
	 * \param param_x : a first parameter of projection (abstract)
	 * \param param_y : a second parameter of projection (abstract)
	 */
    Camera(Point3D position, Vector3D vector_x, Vector3D vector_y, float param_x, float param_y) :
        _position(position), _vector_x(vector_x.normalized()), _vector_y(vector_y.normalized()), _param_x(param_x), _param_y(param_y)
    {}

    virtual Ray get_ray(float, float) const = 0;	///< Ray construction method
    virtual std::pair<double, double> can_see(const Point3D&) const = 0;	///< Returns which pixel sees the given point

protected :

    Point3D _position;	///< Position of the camera
    Vector3D _vector_x;	///< Vector x of projection
    Vector3D _vector_y;	///< Vector y of projection
    float _param_x;		///< Parameter of projection for x
    float _param_y;		///< Parameter of projection for y
};

#endif
