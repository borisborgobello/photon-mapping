
/**
 * \file parallelepiped.cpp
 * \brief Implementation of class Parallelepiped
 * \author B.BORGOBELLO
 */

#include <cmath>
#include <cstdlib>
#include <time.h>
#include "parallelepiped.hpp"
#include "plane.hpp"
#include <Eigen/Array>
#include <textures/colored.hpp>
#include <textures/procedural.hpp>
#include <vector>


/**
 * \param l : the incoming launchable to test
 * collision with
 * \param a,b,c,d : corners of the side to be tested
 *
 * This function returns whether the launchable direction
 * will lead to a collision with this side of the parallelepiped
 */
inline Couple3D Parallelepiped::face_intersected_by(const Launchable& l, const Point3D& a, const Point3D& b, const Point3D& c, const Point3D& d) const
{
    static double epsilon = 1e-7;

    static Couple3D nil = Couple3D(Point3D(0,0,0), Vector3D(0,0,0));

	Point3D start = l.get_end_point() ;
	Vector3D dir = l.get_direction() ;
	Vector3D ab = b-a;
	Vector3D bc = c-b;
	Vector3D cd = d-c;
	Vector3D da = a-d;
	Vector3D ba = a-b;
	Vector3D cb = b-c;
	Vector3D dc = c-d;
	Vector3D ad = d-a;

	Vector3D plane_normal = (ab).cross(ad).normalized();

    if (std::abs(dir.dot(plane_normal)) < epsilon) { return nil;}

    Couple3D intersection_couple;
    //std::cout << "Point of plane : " << a << std::endl;
    Plane plane(0,0,0,NULL, a, plane_normal);
    if (!plane.is_intersected_by(l)) return nil;

    intersection_couple = plane.get_nearest_intersection_with_normal(l);
    //std::cout << intersection_couple.first << std::endl;
    Point3D intersection = intersection_couple.first;

    // AB OK
    Vector3D ab_dir = ab.normalized();
    Vector3D ab_proj_ofm = ab_dir.dot(intersection-a)*ab_dir;
    Vector3D ab_perp = (intersection-a) - ab_proj_ofm;
    if (ab_perp.dot(ad.normalized()) < 0) return nil;

    // AD OK
    Vector3D ad_dir = ad.normalized();
    Vector3D ad_proj_ofm = ad_dir.dot(intersection-a)*ad_dir;
    Vector3D ad_perp = (intersection-a) - ad_proj_ofm;
    if (ad_perp.dot(ab.normalized()) < 0) return nil;

    // CB OK
    Vector3D cb_dir = cb.normalized();
    Vector3D cb_proj_ofm = cb_dir.dot(intersection-c)*cb_dir;
    Vector3D cb_perp = (intersection-c) - cb_proj_ofm;
    if (cb_perp.dot(cd.normalized()) < 0) return nil;

    // CD OK
    Vector3D cd_dir = cd.normalized();
    Vector3D cd_proj_ofm = cd_dir.dot(intersection-c)*cd_dir;
    Vector3D cd_perp = (intersection-c) - cd_proj_ofm;
    if (cd_perp.dot(cb.normalized()) < 0) return nil;

    //std::cout << "FOUND !";

	return intersection_couple;
}

/**
 * \param l : the incoming launchable to test
 * collision with
 *
 * This function returns whether the launchable direction
 * will lead to a collision with this parallelepiped
 */
bool Parallelepiped::is_intersected_by(const Launchable& l) const
{
	Point3D start = l.get_end_point() ;
	Vector3D dir = l.get_direction() ;

	Point3D a = _corner;
	Point3D b = _corner+_z;
	Point3D c = _corner+_z+_y;
	Point3D d = _corner+_y;

	Point3D e = _corner+_x;
	Point3D f = _corner+_x+_z;
	Point3D g = _corner+_x+_y+_z;
	Point3D h = _corner+_x+_y;

    if (face_intersected_by(l,a,b,c,d).second.norm() != 0) return true;
    if (face_intersected_by(l,e,f,g,h).second.norm() != 0) return true;
    if (face_intersected_by(l,b,f,g,c).second.norm() != 0) return true;
    if (face_intersected_by(l,c,g,h,d).second.norm() != 0) return true;
    if (face_intersected_by(l,d,h,e,a).second.norm() != 0) return true;
    if (face_intersected_by(l,a,b,f,e).second.norm() != 0) return true;

	return false;
}

/**
 * \param l : the incoming launchable to find
 * intersection with
 *
 * This function returns a couple containing the nearest
 * intersection point (first param) with this Parallelepiped and
 * the normal at intersection point (second param)
 */
Couple3D Parallelepiped::get_nearest_intersection_with_normal(const Launchable& l) const
{
	Point3D start = l.get_end_point() ;
	Vector3D dir = l.get_direction() ;
    std::vector<Couple3D> intersection_points;
    Couple3D temp_couple;

	Point3D a = _corner;
	Point3D b = _corner+_z;
	Point3D c = _corner+_z+_y;
	Point3D d = _corner+_y;

	Point3D e = _corner+_x;
	Point3D f = _corner+_x+_z;
	Point3D g = _corner+_x+_y+_z;
	Point3D h = _corner+_x+_y;

    if ((temp_couple = face_intersected_by(l,a,b,c,d)).second.norm() != 0) intersection_points.push_back(temp_couple);
    if ((temp_couple = face_intersected_by(l,e,f,g,h)).second.norm() != 0) intersection_points.push_back(temp_couple);
    if ((temp_couple = face_intersected_by(l,b,f,g,c)).second.norm() != 0) intersection_points.push_back(temp_couple);
    if ((temp_couple = face_intersected_by(l,c,g,h,d)).second.norm() != 0) intersection_points.push_back(temp_couple);
    if ((temp_couple = face_intersected_by(l,d,h,e,a)).second.norm() != 0) intersection_points.push_back(temp_couple);
    if ((temp_couple = face_intersected_by(l,a,b,f,e)).second.norm() != 0) intersection_points.push_back(temp_couple);

    //std::cout << intersection_points.size() << std::endl;

    if (intersection_points.size() == 1) return intersection_points[0];

	return ((l.get_end_point() - intersection_points[0].first).norm() <= (l.get_end_point() - intersection_points[1].first).norm()) ?
	 intersection_points[0] : intersection_points[1];
}


/**
 * \param a, b, c, d : the corners of the selected
 * parallelepiped's side
 *
 * Returns a couple containing as first parameter
 * a random point of the selected side of the parallelepiped from
 * where to emit, and a corresponding normal as second
 * parameter
 */
inline Couple3D Parallelepiped::face_random_point_and_normal(const Point3D& a, const Point3D& b, const Point3D& c, const Point3D& d) const
{
    Vector3D ab = b-a;
    Vector3D ad = d-a;

    Vector3D normal = ab.cross(ad).normalized();
    double x = ((double)rand())/RAND_MAX*ab.norm();
    double y = (double)rand()/RAND_MAX*ad.norm();

    return Couple3D(a + x*ab.normalized() + y*ad.normalized(), normal);
}

/**
 * Returns a couple containing as first parameter
 * a random point of the surface of the parallelepiped from
 * where to emit, and a corresponding normal as second
 * parameter
 */
Couple3D Parallelepiped::get_random_point_and_normal() const
{
	int face = ((double)rand()/RAND_MAX)*6.0;
	if (face == 6) face = 0;
	Couple3D couple;

	Point3D a = _corner;
	Point3D b = _corner+_z;
	Point3D c = _corner+_z+_y;
	Point3D d = _corner+_y;

	Point3D e = _corner+_x;
	Point3D f = _corner+_x+_z;
	Point3D g = _corner+_x+_y+_z;
	Point3D h = _corner+_x+_y;

    switch (face) {
        case 0 : {
            couple = face_random_point_and_normal(a,b,c,d);
            if ((e-a).dot(couple.second) > 0) couple.second = -couple.second;
            break;
        }
        case 1 : {
            couple = face_random_point_and_normal(e,f,g,h);
            if ((d-e).dot(couple.second) > 0) couple.second = -couple.second;
            break;
        }
        case 2 : {
            couple = face_random_point_and_normal(b,f,g,c);
            if ((a-b).dot(couple.second) > 0) couple.second = -couple.second;
            break;
        }
        case 3 : {
            couple = face_random_point_and_normal(c,g,h,d);
            if ((a-c).dot(couple.second) > 0) couple.second = -couple.second;
            break;
        }
        case 4 : {
            couple = face_random_point_and_normal(d,h,e,a);
            if ((c-a).dot(couple.second) > 0) couple.second = -couple.second;
            break;
        }
        case 5 : {
            couple = face_random_point_and_normal(a,b,f,e);
            if ((c-a).dot(couple.second) > 0) couple.second = -couple.second;
            break;
        }
    }
    Vector3D temp_vector;
    do {
         temp_vector = Vector3d::Random();
    } while (temp_vector.dot(couple.second) <= 0);
    couple.second = temp_vector;
    return couple;
}

/**
 * \param ph : the incoming photon to redirect
 * \param couple : first parameter contains
 * the intersection point, second contains the normal
 * at this intersection
 *
 * This function determines if a photon will be
 * aborbed, reflected or refracted depending on the
 * corresponding probabilities specified for this parallelepiped
 * It returns TRUE is the photon was redirected, and
 * FALSE if the photon was absorbed.
 */
bool Parallelepiped::redirect_photon( const Couple3D& couple, Photon& ph ) const
{
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
        bool entering = (couple.second.dot(ph.get_direction()) < 0);
        double ratio = (entering) ? 1.0/_ref_index : _ref_index;
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
 * the intersection point, second parameter contains
 * the normal at this intersection
 *
 * This function divides a ray into a reflected and refracted ray
 * only if the new ray can exist (probabilities not null).
 * In case of a total reflection, the refracted ray has a null direction
 */
std::pair<Ray, Ray> Parallelepiped::divide_ray( const Couple3D& couple, const Ray& ray) const
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
        bool entering = (couple.second.dot(ray.get_direction()) < 0);
        double ratio = (entering) ? 1.0/_ref_index : _ref_index;
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
Color Parallelepiped::get_color_at(const Point3D& inters_point) const
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

    return _texture->get_color((inters_point-_corner).dot(x_vector), (inters_point-_corner).dot(y_vector));
}
