
#ifndef RADIANT_VOLUME_HPP_
#define RADIANT_VOLUME_HPP_

/**
 * \file radiant_volume.hpp
 * \author B.BORGOBELLO
 * \brief Declaration of class RadiantVolume
 */

#include "radiant_object.hpp"
#include "../geometry.hpp"
#include <scene.hpp>
#include <shapes/volume.hpp>

/**
 * \class RadiantVolume
 * \brief Derived from RadiantObject
 * represents a volumic light
 *
 * Radiant volumes represents the potential
 * volumes emitting Photons for the photon-mapping
 * (can not direct illuminate)
 * Derived from RadiantObject class
 */

class RadiantVolume : public RadiantObject
{
public :
    /**
     * \brief Constructor
     * \param color : Color of source
     * \param power : Power of source
     * \param volume : Volume of the source
     */
    RadiantVolume(Color color, float power, Volume& volume) :
        RadiantObject(color, power), _volume(volume) {}

    const Volume& get_volume() const { return _volume ; } ///< Returns the volume used
    boost::shared_ptr<Photon> random_photon(); ///< Randomly generates a Photon

protected :
    Volume& _volume; ///< Volume emitting light
};

#endif
