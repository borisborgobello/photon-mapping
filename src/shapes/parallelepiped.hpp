#ifndef PARALLELEPIPED_HPP_
#define PARALLELEPIPED_HPP_

/**
 * \file parallelepiped.hpp
 * \brief Declaration of Parallelepiped class
 * \author B.BORGOBELLO / T.FEIGLER
 */

#include "volume.hpp"

/**
 * \class Parallelepiped
 * \brief Derived from Volume, representing a parallelepiped in space
 *
 * The parallelepiped can reflect and refract Launchables.
 * It can be intersected by a Launchable from inside
 * or outside
 */
class Parallelepiped : public Volume
{
public:
    /**
	 * \brief Constructor
	 *
	 * Constructor of class Parallelepiped
	 *
	 * \param absorp : absorption probability
	 * \param reflect : reflection probability
	 * \param refract : refraction probability
	 * \param index : index (material) of this Parallelepiped
	 * \param tex : pointer to the texture of this Parallelepiped
	 * \param corner : one point of the volume
	 * \param x, y, z : orientation of the parallelepiped (norms precise sizes)
	 */
	Parallelepiped(double absorp, double reflect, double refract, double index,
		Texture* tex, Point3D corner, Vector3D x, Vector3D y, Vector3D z) :
			Volume(absorp, reflect, refract, index, tex), _corner(corner), _x(x), _y(y), _z(z)
    {
        if ( std::abs(_x.dot(_y)) == 1 || std::abs(_y.dot(_z)) == 1 || std::abs(_x.dot(_z)) == 1 ) {
            std::cout << "CRITICAL FAILURE : One parallelepiped isn't really a parallelepiped (angle 0 or 180)" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (_x.norm() == 0 || _y.norm() == 0 || _z.norm() == 0) {
            std::cout << "CRITICAL FAILURE : One parallelepiped isn't really a parallelepiped (one null vector)" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

	bool is_intersected_by(const Launchable&) const ;  ///< Returns whether this parallelepiped is intersected by a given launchable
	Couple3D get_nearest_intersection_with_normal(const Launchable&) const ; ///< Returns the nearest intersection (point/normal) of the given launchable with this parallelepiped
	bool redirect_photon( const Couple3D&, Photon& ) const ; ///< Redirects (or not) a given photon depending on the probilities of this parallelepiped
	std::pair<Ray, Ray> divide_ray( const Couple3D&, const Ray& ) const; ///< Returns the division (reflected/refracted) of an incoming ray at the couple position
	Couple3D get_random_point_and_normal() const ; ///< Returns a random surface point and a random normal of the parallelepiped
	Color get_color_at(const Point3D&) const; ///< Returns the color at this point of the parallelepiped

private:
    inline Couple3D face_intersected_by(const Launchable& l, const Point3D& a, const Point3D& b, const Point3D& c, const Point3D& d) const; ///< Returns whether a face of the parallelepiped is intersected by the given launchable
    inline Couple3D face_random_point_and_normal(const Point3D& a, const Point3D& b, const Point3D& c, const Point3D& d) const; ///< Generates random photons from a side of the parallelepiped

	Point3D _corner; ///< One hook point
	Vector3D _x, _y, _z; ///< Direction and sizes of this parallelepiped
};

#endif /* PARALLELEPIPED_HPP_ */
