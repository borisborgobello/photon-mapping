/**
 * \file sphere.cpp
 * \brief Implementation of class Sphere
 * \author B.BORGOBELLO / T.FEIGLER
 */

#include <cmath>
#include <cstdlib>
#include <time.h>
#include "sphere.hpp"
#include <Eigen/Array>
#include <textures/colored.hpp>
#include <textures/procedural.hpp>


/**
 * \param l : the incoming launchable to test
 * collision with
 *
 * This function returns whether the launchable direction
 * will lead to a collision with this sphere
 */
bool Sphere::is_intersected_by(const Launchable& l) const
{
	Point3D start = l.get_end_point() ;
	Vector3D dir = l.get_direction() ;

	if ((start-_center).squaredNorm() < _radius*_radius) return true; // Already inside the sphere

	Point3D center_proj = dir * dir.dot( _center - start ) + start ;

	return ( ((_center - center_proj).squaredNorm() <= _radius*_radius )
				&& (center_proj - start).dot(dir) > 0 ) ;
}

/**
 * \param l : the incoming launchable to find
 * intersection with
 *
 * This function returns a couple containing the nearest
 * intersection point (first param) with this Sphere and
 * the normal at intersection point (second param)
 */
Couple3D Sphere::get_nearest_intersection_with_normal(const Launchable& l) const
{
	Point3D start = l.get_end_point() ;
	Vector3D dir = l.get_direction() ;
	Vector3D normal;

	Point3D center_proj = dir * dir.dot( _center - start ) + start ;

	double r1_pow2 = ( _center - center_proj ).squaredNorm() ;
	double r2 = sqrt( _radius*_radius - r1_pow2 ) ;

	Point3D M1, M2, nearest ;
	M1 = center_proj + r2 * dir ;
	M2 = center_proj - r2 * dir ;

    if ((start-_center).squaredNorm() < _radius*_radius) { // Inside the sphere
        //std::cout << "INSIIIIDE THE SPHERE\n\n";
        //std::cout << "Intersections found\n" << M1 << std::endl << M2 << std::endl;
        if (dir.dot(M1-center_proj) > 0) nearest = M1;
        else nearest = M2;
        normal = -(nearest - _center).normalized();
    }
    else {
        if( (M1 - start).squaredNorm() > (M2 - start).squaredNorm())
            nearest = M2 ;
        else
            nearest = M1 ;
        normal = (nearest - _center).normalized();
    }

	return Couple3D( nearest, normal ) ;
}

/**
 * Returns a couple containing as first parameter
 * a random point of the surface of the sphere from
 * where to emit, and a corresponding normal as second
 * parameter
 */
Couple3D Sphere::get_random_point_and_normal() const
{
	/*
	 * TODO : mettre en place ici l'utilisation du générateur aléatoire
	 * parallèle [ CF trng.berlios.de ]
	*/
	Vector3D position_from_center ;
	do
	{
		position_from_center << ( rand() / (double)RAND_MAX ) - 0.5,
							( rand() / (double)RAND_MAX ) - 0.5,
							( rand() / (double)RAND_MAX ) - 0.5 ;
	}
	while( position_from_center.squaredNorm() > 1 ) ;
	position_from_center.normalize() ;
	Point3D position = _center + _radius*position_from_center ;

	Vector3D direction ;
	do
	{
		direction << 	( rand() / (double)RAND_MAX ) - 0.5,
						( rand() / (double)RAND_MAX ) - 0.5,
						( rand() / (double)RAND_MAX ) - 0.5 ;
	}
	while( direction.dot(position_from_center) < 0 ) ;

	return Couple3D( position, direction ) ;
}

/**
 * \param ph : the incoming photon to redirect
 * \param couple : first parameter contains
 * the intersection point, second contains the normal
 * at this intersection
 *
 * This function determines if a photon will be
 * aborbed, reflected or refracted depending on the
 * corresponding probabilities specified for this sphere
 * It returns TRUE is the photon was redirected, and
 * FALSE if the photon was absorbed.
 */
bool Sphere::redirect_photon( const Couple3D& couple, Photon& ph ) const
{
	//using namespace std;
	Point3D intersection_point = couple.first;
	Vector3D intersection_normal = couple.second;

    double number = (double)rand()/RAND_MAX;
    static double epsilon = 1e-6;

    if (number < _reflection_prob) {
        ph.set_end_point(intersection_point);
        ph.set_direction(ph.get_reflected(couple));
        ph.set_end_point(ph.get_end_point() + ph.get_direction()*epsilon);
        //cout << "Getting reflected" << endl;

        Color ph_c = ph.get_color() ;
        Color shape_c = get_color_at(couple.first);
        ph.set_color(
            Color(
                ph_c.get_r()*shape_c.get_r(),
                ph_c.get_g()*shape_c.get_g(),
                ph_c.get_b()*shape_c.get_b()
            )
        ) ;
    }
    else if (number < _reflection_prob + _refraction_prob) {
        double ratio = (intersection_normal.dot((intersection_point - _center).normalized()) > 0) ? 1.0/_ref_index : _ref_index;
        //std::cout << ((ratio <= 1) ? "Entering surface\n" : "Getting out\n");
        ph.set_end_point(intersection_point);
        ph.set_direction(ph.get_refracted(couple, ratio));
        ph.set_end_point(ph.get_end_point() + ph.get_direction()*epsilon);
        //cout << "Getting refracted" << endl;

        Color ph_c = ph.get_color() ;
        Color shape_c = get_color_at(couple.first);
        ph.set_color(
            Color(
                ph_c.get_r()*shape_c.get_r(),
                ph_c.get_g()*shape_c.get_g(),
                ph_c.get_b()*shape_c.get_b()
            )
        ) ;
    }
    else {
        //cout << "Absorbed" << endl;
        return false;
    }
	return true;
}

/**
 * \param ray : the incoming ray to divide
 * \param couple : first parameter contains
 * the intersection point, second contains the normal
 * at this intersection
 *
 * This function divides a ray into a reflected and refracted ray
 * only if the new ray can exist (probabilities not null).
 * In case of a total reflection, the refracted ray has a null direction
 */
std::pair<Ray, Ray> Sphere::divide_ray( const Couple3D& couple, const Ray& ray) const
{
    Point3D intersection_point = couple.first;
	Vector3D intersection_normal = couple.second;
	static double epsilon = 1e-6;

    Ray reflected_ray(Point3D(0,0,0), Vector3D(0,0,0));
    Ray refracted_ray(Point3D(0,0,0), Vector3D(0,0,0));

    if (_reflection_prob == 0)
        reflected_ray = Ray(Point3D(0,0,0), Vector3D(0,0,0));
    else {
        reflected_ray = Ray(intersection_point, ray.get_reflected(couple));
        reflected_ray.set_end_point(reflected_ray.get_end_point() + reflected_ray.get_direction()*epsilon);
    }

    if (_refraction_prob == 0)
        refracted_ray = Ray(Point3D(0,0,0), Vector3D(0,0,0));
    else {
        double ratio = (intersection_normal.dot((intersection_point - _center).normalized()) > 0) ? 1.0/_ref_index : _ref_index;
        //std::cout << ((ratio <= 1) ? "Entering surface\n" : "Getting out\n");
        refracted_ray = Ray(intersection_point, ray.get_refracted(couple, ratio));
        refracted_ray.set_end_point(refracted_ray.get_end_point() + refracted_ray.get_direction()*epsilon);
        if (refracted_ray.get_direction()[0] == reflected_ray.get_direction()[0]
            && refracted_ray.get_direction()[1] == reflected_ray.get_direction()[1]
             && refracted_ray.get_direction()[2] == reflected_ray.get_direction()[2])
            refracted_ray.set_direction(Vector3D(0,0,0));
        //cout << "Getting refracted" << endl;
    }

    return std::pair<Ray, Ray>(reflected_ray, refracted_ray);
}

/**
 * \param inters_point : an intersection point where we seek the color
 *
 * This function determines the color at the given point of the surface
 * It is related to the get_color(double, double) of the Texture where the
 * two doubles represent coordinates for the texture map.
 */
Color Sphere::get_color_at(const Point3D& inters_point) const
{
    if (dynamic_cast<Colored*>(_texture.get())) return _texture->get_color(0,0);

    Procedural * proc = dynamic_cast<Procedural *>(_texture.get());
    Vector3D x_vector = proc->get_text_x();
    Vector3D y_vector = proc->get_text_y();

    if (y_vector.norm() == 0 && x_vector.norm() == 0) {
        y_vector = Vector3d::Random().normalized();
        do {
            x_vector = Vector3d::Random().normalized();
        } while(std::abs(x_vector.dot(y_vector)) == 1);
        x_vector = y_vector.cross(x_vector).normalized();
        proc->set_text_x(y_vector);
        proc->set_text_y(x_vector);
    }
    else if (y_vector.norm() == 0) {
        do {
            y_vector = Vector3d::Random().normalized();
        } while(std::abs(x_vector.dot(y_vector)) == 1);
        y_vector = y_vector.cross(x_vector).normalized();
        proc->set_text_x(y_vector);
    }
    else if (x_vector.norm() == 0) {
        do {
            x_vector = Vector3d::Random().normalized();
        } while(std::abs(x_vector.dot(y_vector)) == 1);
        x_vector = y_vector.cross(x_vector).normalized();
        proc->set_text_y(x_vector);
    }

    Vector3D current_vector = (inters_point-_center).normalized();
    if (std::abs(y_vector.dot(current_vector)) == 1) return _texture->get_color(0, 0);
    double angle_x = acos(y_vector.dot(current_vector));
    double x_value = angle_x*_radius;

    Point3D proj_vect_y = (current_vector - current_vector.dot(y_vector)*y_vector).normalized();
    double angle_y = acos(x_vector.dot(proj_vect_y));
    double y_value = angle_y*_radius;
    //if (x_value < 0 && y_value < 0) x_value = -x_value;

    return _texture->get_color(x_value, y_value);
}
