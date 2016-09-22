#ifndef RAY_HPP_
#define RAY_HPP_

/**
 * \file ray.hpp
 * \brief Description of class Ray
 * \author T.FEIGLER / B.BORGOBELLO
 */

#include "launchable.hpp"

/**
 * \class Ray
 * \brief Derived class of Launchable
 *
 * Class Ray is a launchable
 */
class Ray : public Launchable
{
public:
    /**
	 * \brief Constructor
	 *
	 * Constructor of class Ray
	 *
	 * \param point : initial position of the Ray
	 * \param vector : initial direction of the Ray
	 */
	Ray( Point3D point, Vector3D vector) :
		Launchable(point, vector) {}
};

#endif
