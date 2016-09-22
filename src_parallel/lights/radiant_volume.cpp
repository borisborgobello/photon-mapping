/**
 * \file radiant_volume.cpp
 * \brief Implementation of class RadiantVolume
 * \author B.BORGOBELLO / T.FEIGLER
 */

#include <iostream>
#include "radiant_volume.hpp"
#include <Eigen/Array>

/**
 * \brief Generates a photon with a random direction
 * from a random point of the volume
 */
boost::shared_ptr<Photon> RadiantVolume::random_photon() {
    using namespace std;
    boost::shared_ptr<Photon> photon;
    static double epsilon = 1e-6;

    Couple3D couple = _volume.get_random_point_and_normal();
    photon = boost::shared_ptr<Photon>(new Photon(couple.first, couple.second, _color));
    photon->set_end_point(photon->get_end_point() + photon->get_direction()*epsilon);

    return photon;
}
