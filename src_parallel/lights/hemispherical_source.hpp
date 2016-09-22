#ifndef HEMISPHERICAL_SOURCE_HPP_
#define HEMISPHERICAL_SOURCE_HPP_

/**
 * \file hemispherical_source.hpp
 * \author B.BORGOBELLO
 * \brief Declaration of class HemisphericalSource
 */

#include "coherent_light_source.hpp"

/**
 * \class HemisphericalSource
 * \brief Derived from CoherentLightSource,
 * representing a hemispherical light source
 *
 * Class HemisphericalSource is a random light source
 * that emits in an hemisphere from its center
 * and uniformly
 */

class HemisphericalSource : public CoherentLightSource
{
public :
    /**
     * \brief Constructor
     * \param color : Color of source
     * \param power : Power of source
     * \param location : Position of source
     * \param direction : Direction of the hemisphere
     */
    HemisphericalSource(Color color, float power, Point3D location, Vector3D direction) :
        CoherentLightSource(color, power, location), _direction(direction.normalized()) {}
    ~HemisphericalSource() {}

    virtual bool is_viewable_from(Point3D, const std::vector< boost::shared_ptr<Shape> >&) const ; ///< Whether the center is visible from a point of space
    virtual boost::shared_ptr<Photon> random_photon(); ///< Randomly generates a Photon
private :
    Vector3D _direction;
};

#endif
