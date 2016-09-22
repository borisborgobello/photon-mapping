#ifndef COHERENT_LIGHT_SOURCE_HPP_
#define COHERENT_LIGHT_SOURCE_HPP_

/**
 * \file coherent_light_source.hpp
 * \author B.BORGOBELLO
 * \brief Declaration of class CoherentLightSource
 */

#include "radiant_object.hpp"
#include "../geometry.hpp"
#include <scene.hpp>

/**
 * \class CoherentLightSource
 * \brief Derived from RadiantObject
 * represents a coherent light source
 *
 * Coherent light sources represents the potential
 * emitters of Photons for the photon-mapping
 * which can also direct illuminate
 * Derived from RadiantObject class
 */

class CoherentLightSource : public RadiantObject
{
public :
    /**
     * \brief Constructor
     * \param color : Color of source
     * \param power : Power of source
     * \param location : Position of source
     */
    CoherentLightSource(Color color, float power, Point3D location) :
        RadiantObject(color, power), _location(location) {}

    const Point3D& get_location() const { return _location ; } ///< Returns the position of the source

    virtual bool is_viewable_from(const Point3D, const std::vector< boost::shared_ptr<Shape> >&) const = 0; ///< Whether a source is visible from a point of space
    virtual boost::shared_ptr<Photon> random_photon() = 0; ///< Randomly generates a Photon

protected :
    Point3D _location; ///< Position of the source in space
};

#endif
