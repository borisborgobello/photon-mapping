
/**
 * \file hemispherical_source.cpp
 * \brief Implementation of class HemisphericalSource
 * \author B.BORGOBELLO / T.FEIGLER
 */

#include <iostream>
#include "hemispherical_source.hpp"
#include <Eigen/Array>

/**
 * Returns whether this source is visible from a point
 * in space
 * \param point : point in space
 * \param shapes : list of all the shapes of the scene
 * which may eclipse the source
 */
bool HemisphericalSource::is_viewable_from(Point3D point, const std::vector< boost::shared_ptr<Shape> >& shapes) const
{
	using namespace std ;

    Vector3D direction = (point - _location).normalized() ;
    if (direction.dot(_direction) <= 0) return false;

    Launchable launch_test(_location, direction) ;

    double nearest_distance_2 = -1.0 ;

    for (unsigned int i = 0; i < shapes.size(); i++) {
        if (shapes[i]->is_intersected_by(launch_test))
        {
        	Point3D intersection =
        			shapes[i]->get_nearest_intersection_with_normal(launch_test).first ;
        	double this_distance_2 = ( intersection - _location ).squaredNorm() ;

        	if(nearest_distance_2 < 0.0 || this_distance_2 < nearest_distance_2)
        		nearest_distance_2 = this_distance_2 ;
        }
    }

    double result = nearest_distance_2 - (point - _location).squaredNorm() ;
    const double epsilon = 1.0e-12 ;

    return (result < epsilon) && (result > -epsilon) ;
}

/**
 * \brief Generates a photon with a random direction in the hemisphere
 * starting from this source's center
 */
boost::shared_ptr<Photon> HemisphericalSource::random_photon() {
    using namespace std;
    boost::shared_ptr<Photon> photon;

    Vector3D vector;
    do {
        vector = (Vector3d::Random()).normalized();
    } while (vector.dot(_direction) <= 0);
    photon = boost::shared_ptr<Photon>(new Photon(_location, vector, _color));

    return photon;
}
