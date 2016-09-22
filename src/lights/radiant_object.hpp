#ifndef RADIENT_OBJECT_HPP_
#define RADIENT_OBJECT_HPP_

/**
 * \file radiant_object.hpp
 * \author B.BORGOBELLO
 * \brief Declaration of class RadiantObject
 */

#include "light.hpp"
#include "../launchables/photon.hpp"
#include <boost/smart_ptr/shared_ptr.hpp>

/**
 * \class RadiantObject
 * \brief Class RadiantObject (derived from Light) are light emitting objects
 *
 * Class RadiantObject contains all potential photon sources
 * Derived from Light class
 */

class RadiantObject : public Light
{
public :
    /**
     * \brief Constructor
     * \param color : Color of source
     * \param power : Power of source
     */
    RadiantObject(Color color, float power) : Light(color, power) {}

    virtual boost::shared_ptr<Photon> random_photon() = 0; ///< Randomly generates a Photon
};

#endif
