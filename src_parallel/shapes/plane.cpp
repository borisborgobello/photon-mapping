/**
 * \file plane.cpp
 * \brief Implementation of class Plane
 * \author B.BORGOBELLO
 */

#include <cmath>
#include <cstdlib>
#include <time.h>
#include <cstdio>
#include "plane.hpp"
#include <textures/colored.hpp>
#include <textures/procedural.hpp>
#include <Eigen/Array>

using namespace std ;

/**
 * \param l : the incoming launchable to test
 * the collision with
 *
 * This function returns whether the launchable direction
 * will lead to a collision with this plane
 */
bool Plane::is_intersected_by(const Launchable& l) const
{
    static double epsilon = 1e-7;
	Point3D start = l.get_end_point() ;
	Vector3D dir = l.get_direction() ;
    if (std::abs(dir.dot(_normal)) < epsilon) return false;

    Vector3D normal_facing_the_launch = _normal * _normal.dot( start - _one_point );
    //if (normal_facing_the_launch.norm() < EPSILON) return false;

	Point3D projection_of_start = start - normal_facing_the_launch;
	//std::cout << "Projection of start [" << projection_of_start[0] << ", " << projection_of_start[1] << ", " << projection_of_start[2] << "]\n";
    normal_facing_the_launch.normalize();
	return (dir.dot(normal_facing_the_launch) < 0);
}

/**
 * \param l : the incoming launchable to find
 * intersection with
 *
 * This function returns a couple containing the nearest
 * intersection point (first param) with this plane and
 * the normal at intersection point (second param)
 */
Couple3D Plane::get_nearest_intersection_with_normal(const Launchable& l) const
{
    //using namespace std;
	Point3D start = l.get_end_point() ;
	Vector3D dir = l.get_direction() ;

	Point3D projection_of_start = start - _normal * _normal.dot( start - _one_point );
	//cout << "Projection of start" << endl << projection_of_start << endl;
    Vector3D proj_start = start - projection_of_start;
    if (std::abs(dir.dot(_normal)) == 1) return Couple3D(projection_of_start, proj_start.normalized());

    Point3D intersection = start + dir * (proj_start.norm())/(std::abs(dir.dot(_normal)));
    //std::cout << "Intersection point :\n" <<intersection << "\n" << "Normal point :\n" <<proj_start.normalized() << "\n";

	return Couple3D(intersection, proj_start.normalized());
}

/**
 * \param ph : the incoming photon to redirect
 * \param couple : first parameter contains
 * the intersection point, second contains the normal
 * at this intersection
 *
 * This function determines if a photon will be
 * aborbed, reflected or will pass through depending on the
 * corresponding probabilities specified for this sphere
 * It returns TRUE is the photon was redirected, and
 * FALSE if the photon was absorbed
 */
bool Plane::redirect_photon( const Couple3D& couple, Photon& ph ) const
{
    //using namespace std;
    double number = (double)rand()/RAND_MAX;
    static double epsilon = 1e-7;

    if (number < _reflection_prob) {
        ph.set_end_point(couple.first);
        ph.set_direction(ph.get_reflected(couple));
        ph.set_end_point(ph.get_end_point() + ph.get_direction()*epsilon);

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
    else if (number < _reflection_prob + _transparency_prob) {
        ph.set_end_point(couple.first);
        ph.set_direction(ph.get_refracted(couple, 1.0));
        ph.set_end_point(ph.get_end_point() + ph.get_direction()*epsilon);

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
 * only if the new ray can exist (probabilities not null)
 */
std::pair<Ray, Ray> Plane::divide_ray( const Couple3D& couple, const Ray& ray) const {
    Point3D intersection_point = couple.first;
	Vector3D intersection_normal = couple.second;
	static double epsilon = 1e-7;

    Ray reflected_ray(Point3D(0,0,0), Vector3D(0,0,0));
    Ray refracted_ray(Point3D(0,0,0), Vector3D(0,0,0));

    if (_reflection_prob == 0)
        reflected_ray = Ray(Point3D(0,0,0), Vector3D(0,0,0));
    else {
        reflected_ray = Ray(intersection_point, ray.get_reflected(couple));
        reflected_ray.set_end_point(reflected_ray.get_end_point() + reflected_ray.get_direction()*epsilon);
    }

    if (_transparency_prob == 0)
        refracted_ray = Ray(Point3D(0,0,0), Vector3D(0,0,0));
    else {
        //std::cout << ((ratio <= 1) ? "Entering surface\n" : "Getting out\n");
        refracted_ray = Ray(intersection_point, ray.get_refracted(couple, 1.0));
        refracted_ray.set_end_point(refracted_ray.get_end_point() + refracted_ray.get_direction()*epsilon);
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
Color Plane::get_color_at(const Point3D& inters_point) const
{
    if (dynamic_cast<Colored*>(_texture.get())) return _texture->get_color(0,0);

    Procedural * proc = dynamic_cast<Procedural *>(_texture.get());
    Vector3D x_vector = proc->get_text_x();
    Vector3D y_vector = proc->get_text_y();
    static bool never_checked = true;

    if (never_checked) {
        if (x_vector.norm() == 0 || y_vector.norm() == 0 || (std::abs(x_vector.dot(_normal)) == 1 && std::abs(y_vector.dot(_normal)) == 1)) {
            do {
                do {
                    x_vector = Vector3d::Random().normalized();

                } while (x_vector.dot(_normal) <= 0.3 && x_vector.dot(_normal) > 0.7);
                Launchable test_launch((_one_point + x_vector), -_normal);
                x_vector = (get_nearest_intersection_with_normal(test_launch).first - _one_point).normalized();
                //std::cout << std::endl << "Normlized error " << x_vector.dot(_normal);
            } while (_normal.dot(x_vector) != 0);
            y_vector = x_vector.cross(_normal).normalized();
            proc->set_text_x(x_vector);
            proc->set_text_y(y_vector);
        }
        else if (std::abs(x_vector.dot(_normal)) != 0 && std::abs(y_vector.dot(_normal)) != 0) {
            Vector3D temp_vector;
            if (std::abs(x_vector.dot(_normal)) != 1) {
                temp_vector = _normal.cross(x_vector);
                if (temp_vector.dot(y_vector) >= 0) y_vector = temp_vector; else  y_vector = -temp_vector;
                temp_vector = _normal.cross(y_vector);
                if (temp_vector.dot(x_vector) >= 0) x_vector = temp_vector; else  x_vector = -temp_vector;
            }
            else {
                temp_vector = _normal.cross(y_vector);
                if (temp_vector.dot(x_vector) >= 0) x_vector = temp_vector; else  x_vector = -temp_vector;
                temp_vector = _normal.cross(x_vector);
                if (temp_vector.dot(y_vector) >= 0) y_vector = temp_vector; else  y_vector = -temp_vector;
            }
            proc->set_text_x(x_vector);
            proc->set_text_y(y_vector);
        }
        else if (std::abs(x_vector.dot(_normal)) == 0 && std::abs(y_vector.dot(_normal)) != 0) {
            Vector3D temp_vector = _normal.cross(x_vector);
            if (temp_vector.dot(y_vector) >= 0) y_vector = temp_vector; else  y_vector = -temp_vector;
            proc->set_text_y(y_vector);
        }
        else if (std::abs(y_vector.dot(_normal)) == 0 && std::abs(x_vector.dot(_normal)) != 0) {
            Vector3D temp_vector = _normal.cross(y_vector);
            if (temp_vector.dot(x_vector) >= 0) x_vector = temp_vector; else  x_vector = -temp_vector;
            proc->set_text_x(x_vector);
        }
        never_checked = false;
    }

    double x_value = (inters_point - _one_point).dot(x_vector);
    double y_value = (inters_point - _one_point).dot(y_vector);

    return _texture->get_color(x_value, y_value);
}
