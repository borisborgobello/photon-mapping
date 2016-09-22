/**
 * \file triangle.cpp
 * \brief Implementation of class Triangle
 * \author B.BORGOBELLO
 */

#include <cmath>
#include <cstdlib>
#include <time.h>
#include "triangle.hpp"
#include "plane.hpp"
#include <textures/colored.hpp>
#include <textures/procedural.hpp>
#include <Eigen/Array>

/**
 * \param l : the incoming launchable to test
 * the collision with
 *
 * This function returns whether the launchable direction
 * will lead to a collision with this triangle
 */
bool Triangle::is_intersected_by(const Launchable& l) const
{
    static double epsilon = 1e-7;
	Point3D start = l.get_end_point() ;
	Vector3D dir = l.get_direction() ;
	Vector3D ab = _b-_a;
	Vector3D ac = _c-_a;
	Vector3D plane_normal = (ab).cross(ac).normalized();

    if (std::abs(dir.dot(plane_normal)) < epsilon) return false;

    Plane plane(0,0,0,NULL, _a, plane_normal);
    if (!plane.is_intersected_by(l)) return false;
    Point3D intersection = plane.get_nearest_intersection_with_normal(l).first;

    Vector3D ac_dir = (_c-_a).normalized();
    Vector3D ac_proj_ofm = ac_dir.dot(intersection-_a)*ac_dir;
    Vector3D ac_perp = (intersection-_a) - ac_proj_ofm;
    if (ac_perp.dot((_b-_a).normalized()) < 0) return false;

    Vector3D ab_dir = (_b-_a).normalized();
    Vector3D ab_proj_ofm = ab_dir.dot(intersection-_a)*ab_dir;
    Vector3D ab_perp = (intersection-_a) - ab_proj_ofm;
    if (ab_perp.dot((_c-_a).normalized()) < 0) return false;

    Vector3D bc_dir = (_c-_b).normalized();
    Vector3D bc_proj_ofm = bc_dir.dot(intersection-_b)*bc_dir;
    Vector3D bc_perp = (intersection-_b) - bc_proj_ofm;
    if (bc_perp.dot((_a-_b).normalized()) < 0) return false;

	return true;
}

/**
 * \param l : the incoming launchable to find
 * intersection with
 *
 * This function returns a couple containing the nearest
 * intersection point (first param) with this triangle and
 * the normal at intersection point (second param)
 */
Couple3D Triangle::get_nearest_intersection_with_normal(const Launchable& l) const
{
	Point3D start = l.get_end_point() ;
	Vector3D dir = l.get_direction() ;
	Vector3D ab = _a-_b;
	Vector3D ac = _a-_c;
	Vector3D plane_normal = (ab).cross(ac).normalized();

	return  Plane(0,0,0,NULL, _a, plane_normal).get_nearest_intersection_with_normal(l);
}

/**
 * \param ph : the incoming photon to redirect
 * \param couple : first parameter contains
 * the intersection point, second contains the normal
 * at this intersection
 *
 * This function determines if a photon will be
 * aborbed, reflected or will pass through depending on the
 * corresponding probabilities specified for this triangle
 * It returns TRUE is the photon was redirected, and
 * FALSE if the photon was absorbed
 */
bool Triangle::redirect_photon( const Couple3D& couple, Photon& ph ) const
{
    //using namespace std;
    double number = (double)rand()/RAND_MAX;
    static double epsilon = 1e-6;

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
std::pair<Ray, Ray> Triangle::divide_ray( const Couple3D& couple, const Ray& ray) const {
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
Color Triangle::get_color_at(const Point3D& inters_point) const
{
    if (dynamic_cast<Colored*>(_texture.get())) return _texture->get_color(0,0);

    Procedural * proc = dynamic_cast<Procedural *>(_texture.get());
    Vector3D x_vector = proc->get_text_x();
    Vector3D y_vector = proc->get_text_y();
    Vector3D ab = _a-_b;
	Vector3D ac = _a-_c;

    static bool never_checked = true;
    static Vector3D normal = (ab).cross(ac).normalized();

    if (never_checked) {
        if (x_vector.norm() == 0 || y_vector.norm() == 0 || (std::abs(x_vector.dot(normal)) == 1 && std::abs(y_vector.dot(normal)) == 1)) {
            do {
                do {
                    x_vector = Vector3d::Random().normalized();

                } while (x_vector.dot(normal) <= 0.3 && x_vector.dot(normal) > 0.7);
                Launchable test_launch((_a + x_vector), -normal);
                x_vector = (get_nearest_intersection_with_normal(test_launch).first - _a).normalized();
                //std::cout << std::endl << "Normlized error " << x_vector.dot(normal);
            } while (normal.dot(x_vector) != 0);
            y_vector = x_vector.cross(normal).normalized();
            proc->set_text_x(x_vector);
            proc->set_text_y(y_vector);
        }
        else if (std::abs(x_vector.dot(normal)) != 0 && std::abs(y_vector.dot(normal)) != 0) {
            Vector3D temp_vector;
            if (std::abs(x_vector.dot(normal)) != 1) {
                temp_vector = normal.cross(x_vector);
                if (temp_vector.dot(y_vector) >= 0) y_vector = temp_vector; else  y_vector = -temp_vector;
                temp_vector = normal.cross(y_vector);
                if (temp_vector.dot(x_vector) >= 0) x_vector = temp_vector; else  x_vector = -temp_vector;
            }
            else {
                temp_vector = normal.cross(y_vector);
                if (temp_vector.dot(x_vector) >= 0) x_vector = temp_vector; else  x_vector = -temp_vector;
                temp_vector = normal.cross(x_vector);
                if (temp_vector.dot(y_vector) >= 0) y_vector = temp_vector; else  y_vector = -temp_vector;
            }
            proc->set_text_x(x_vector);
            proc->set_text_y(y_vector);
        }
        else if (std::abs(x_vector.dot(normal)) == 0 && std::abs(y_vector.dot(normal)) != 0) {
            Vector3D temp_vector = normal.cross(x_vector);
            if (temp_vector.dot(y_vector) >= 0) y_vector = temp_vector; else  y_vector = -temp_vector;
            proc->set_text_y(y_vector);
        }
        else if (std::abs(y_vector.dot(normal)) == 0 && std::abs(x_vector.dot(normal)) != 0) {
            Vector3D temp_vector = normal.cross(y_vector);
            if (temp_vector.dot(x_vector) >= 0) x_vector = temp_vector; else  x_vector = -temp_vector;
            proc->set_text_x(x_vector);
        }
        never_checked = false;
    }

    double x_value = (inters_point - _a).dot(x_vector);
    double y_value = (inters_point - _a).dot(y_vector);

    return _texture->get_color(x_value, y_value);
}
