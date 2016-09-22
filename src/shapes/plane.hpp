#ifndef PLANE_HPP_
#define PLANE_HPP_

/**
 * \file plane.hpp
 * \brief Declaration of Plane class
 * \author B.BORGOBELLO / T.FEIGLER
 */

#include "surface.hpp"

/**
 * \class Plane
 * \brief Derived from Surface, representing a plane
 *
 * The plane is considered infinite, and can
 * reflect and refract (through) Launchables
 * It can be intersected by a Launchable
 */
class Plane : public Surface
{
public:
    /**
	 * \brief Constructor
	 *
	 * Constructor of class Plane
	 *
	 * \param absorp : absorption probability
	 * \param reflect : reflection probability
	 * \param transp : transparency probability
	 * \param tex : pointer to the texture of this surface
	 * \param one_point : one point of this plane used to position it into space
	 * \param normal : a normal of the plane
	 */
	Plane(double absorp, double reflect, double transp, Texture* tex,
            Point3D one_point, Vector3D normal) :
			Surface(absorp, reflect, transp, tex),
			_one_point(one_point), _normal(normal.normalized()) {}

	bool is_intersected_by(const Launchable&) const ; ///< Returns whether this plane is intersected by a given launchable
	Couple3D get_nearest_intersection_with_normal(const Launchable&) const ; ///< Returns the nearest intersection (point/normal) of the given launchable with this plane
	bool redirect_photon( const Couple3D&, Photon& ) const ; ///< Redirects (or not) a given photon depending on the probilities of this plane
	std::pair<Ray, Ray> divide_ray( const Couple3D&, const Ray& ) const; ///< Returns the division (reflected/refracted) of an incoming ray at the couple position
	Color get_color_at(const Point3D&) const; ///< Returns the color at this point of the plane

private:
	Point3D _one_point ; ///< A point of the plane
	Vector3D _normal ; ///< The normal of the plane
};

#endif /* PLANE_HPP_ */
