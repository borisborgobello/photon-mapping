#ifndef LAUNCHABLE_HPP_
#define LAUNCHABLE_HPP_

/**
 * \file launchable.hpp
 * \brief Description of class Launchable
 * \author T.FEIGLER / B.BORGOBELLO
 */

#include "geometry.hpp"

/**
 * \class Launchable
 * \brief Base class for Photon and Ray
 *
 * Class Launchable describes the different
 * possible interactions of a Photon or a Ray
 * with things in space
 */
class Launchable
{
public:
    /**
	 * \brief Constructor
	 *
	 * Constructor of class Launchable
	 *
	 * \param point : initial position of the launchable
	 * \param vector : initial direction of the launchable
	 */
	Launchable( Point3D point, Vector3D vector) : _end_point(point)
		{_direction = (vector.norm() == 0) ? vector: vector.normalized();}

	const Point3D& get_end_point() const { return _end_point ; } ///< Returns the actual position of the launchable
	const Vector3D& get_direction() const { return _direction ; } ///< Returns the actual direction of the launchable

	Vector3D get_reflected(const Couple3D &) const ; ///< Returns the reflected direction
	Vector3D get_refracted(const Couple3D &, double) const ; ///< Returns the refracted direction
	void set_end_point(Point3D end_point) { _end_point = end_point;} ///< Sets the current position of the launchable
	void set_direction(Vector3D direction) { _direction = direction;} ///< Sets the current direction of the launchable

private:
	Point3D _end_point ; ///< Current position of the launchable
	Vector3D _direction ; ///< Current direction of the launchable
};

#endif /* LAUNCHABLE_HPP_ */
