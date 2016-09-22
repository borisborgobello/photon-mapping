#ifndef PUNCTUAL_SOURCE_HPP_
#define PUNCTUAL_SOURCE_HPP_

/**
 * \file punctual_source.hpp
 * \author B.BORGOBELLO
 * \brief Declaration of class PunctualSource
 */

#include "coherent_light_source.hpp"

/**
 * \class PunctualSource
 * \brief Derived from CoherentLightSource,
 * representing a punctual light source
 *
 * Class PunctualSource is a random light source
 * that emits in all direction from its center
 * and uniformly
 */

class PunctualSource : public CoherentLightSource
{
public :
    /**
     * \brief Constructor
     * \param color : Color of source
     * \param power : Power of source
     * \param location : Position of source
     */
    PunctualSource(Color color, float power, Point3D location) :
        CoherentLightSource(color, power, location) {}
    ~PunctualSource() {}

    virtual bool is_viewable_from(Point3D, const std::vector< boost::shared_ptr<Shape> >&) const ; ///< Whether the center is visible from a point of space
    virtual boost::shared_ptr<Photon> random_photon(); ///< Randomly generates a Photon
};

#endif
