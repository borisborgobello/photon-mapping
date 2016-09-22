#ifndef TRIANGLE_HPP_
#define TRIANGLE_HPP_

/**
 * \file triangle.hpp
 * \brief Declaration of Triangle class
 * \author B.BORGOBELLO / T.FEIGLER
 */

#include "surface.hpp"

/**
 * \class Triangle
 * \brief Derived from Surface, representing a triangle
 *
 * The triangle can reflect and refract (through) Launchables
 * It can be intersected by a Launchable
 */
class Triangle : public Surface
{
public:
    /**
	 * \brief Constructor
	 *
	 * Constructor of class Triangle
	 *
	 * \param absorp : absorption probability
	 * \param reflect : reflection probability
	 * \param transp : transparency probability
	 * \param tex : pointer to the texture of this surface
	 * \param a, b, c : the three points of this triangle in space
	 * \param for_volume : whether this triangle is refracting (part of a volume ?)
	 */
	Triangle(double absorp, double reflect, double transp, Texture* tex,
            Point3D a, Point3D b, Point3D c, bool for_volume = false) :
			Surface(absorp, reflect, transp, tex), _a(a), _b(b), _c(c), _for_volume(for_volume)
    {
        if ( std::abs((_b-_a).dot(_c-_a)) == 1 || std::abs((_a-_b).dot(_c-_b)) == 1 ) {
            std::cout << "CRITICAL FAILURE : One triangle isn't really a triangle (angle 0 or 180)" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

	bool is_intersected_by(const Launchable&) const ; ///< Returns whether this triangle is intersected by a given launchable
	Couple3D get_nearest_intersection_with_normal(const Launchable&) const ; ///< Returns the nearest intersection (point/normal) of the given launchable with this triangle
	bool redirect_photon( const Couple3D&, Photon& ) const ; ///< Redirects (or not) a given photon depending on the probilities of this triangle
	std::pair<Ray, Ray> divide_ray( const Couple3D&, const Ray& ) const; ///< Returns the division (reflected/refracted) of an incoming ray at the couple position
	Color get_color_at(const Point3D&) const; ///< Returns the color at this point of the triangle

private:
	Point3D _a, _b, _c ; ///< Corners of the triangle
	bool _for_volume; ///< Whether this triangle is refracting (part of a volume ?)
};

#endif /* TRIANGLE_HPP_ */
