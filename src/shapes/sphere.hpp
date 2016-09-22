
#ifndef SPHERE_HPP_
#define SPHERE_HPP_

/**
 * \file sphere.hpp
 * \brief Declaration of Sphere class
 * \author B.BORGOBELLO / T.FEIGLER
 */

#include "volume.hpp"

/**
 * \class Sphere
 * \brief Derived from Volume, representing a sphere in space
 *
 * The sphere can reflect and refract Launchables.
 * It can be intersected by a Launchable from inside
 * or outside
 */
class Sphere : public Volume
{
public:
    /**
	 * \brief Constructor
	 *
	 * Constructor of class Sphere
	 *
	 * \param absorp : absorption probability
	 * \param reflect : reflection probability
	 * \param refract : refraction probability
	 * \param index : index (material) of this Sphere
	 * \param tex : pointer to the texture of this Sphere
	 * \param center : center of the Sphere in space
	 * \param radius : radius of the Sphere
	 */
	Sphere(double absorp, double reflect, double refract, double index,
		Texture* tex, Point3D center, double radius) :
			Volume(absorp, reflect, refract, index, tex),
			_center(center), _radius(radius) {}

	bool is_intersected_by(const Launchable&) const ;  ///< Returns whether this sphere is intersected by a given launchable
	Couple3D get_nearest_intersection_with_normal(const Launchable&) const ; ///< Returns the nearest intersection (point/normal) of the given launchable with this sphere
	bool redirect_photon( const Couple3D&, Photon& ) const ; ///< Redirects (or not) a given photon depending on the probilities of this sphere
	std::pair<Ray, Ray> divide_ray( const Couple3D&, const Ray& ) const; ///< Returns the division (reflected/refracted) of an incoming ray at the couple position
	Couple3D get_random_point_and_normal() const ; ///< Returns a random surface point and a random normal of the sphere
	Color get_color_at(const Point3D&) const; ///< Returns the color at this point of the sphere

private:
	Point3D _center ; ///< The center of the sphere in space
	double _radius ; ///< The radius of the sphere
};

#endif /* SPHERE_HPP_ */
