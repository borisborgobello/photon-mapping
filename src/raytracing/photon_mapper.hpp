#ifndef PHOTON_MAPPER_HPP_
#define PHOTON_MAPPER_HPP_

/**
 * \file photon_mapper.hpp
 * \author B.BORGOBELLO
 * \brief Declaration of class PhotonList
 */

#include <string>
#include <scene.hpp>
#include "photon_map.hpp"
#include <boost/shared_ptr.hpp>

/**
 * \class PhotonMapper
 * \brief Creates the photon maps
 */
class PhotonMapper {
public:
    /**
	 * \brief Constructor
	 * \param sc : the scene to photon-trace
     * \param nb_photons : the size (in number of photons) of the photon-map
     * \param photon_depth : the maximum number of reflection/refraction of a photon
     *
     * Directly calls the build_photon_tree method (photon_mapping phase)
	 */
    PhotonMapper(const Scene& sc, int nb_photons, int photon_depth) :
    	_photon_map(build_photon_tree(sc, nb_photons, photon_depth)) {}

    std::vector< boost::shared_ptr<Photon> > get_k_nearest_photons(int, const Point3D&) const ; ///< Returns the k nearest photons of the given point
    void optimize() const { _photon_map->optimize() ; } ///< Optimize the Photon-Map

private:
    static PhotonMap *build_photon_tree
		(const Scene& scene, int nb_photon_MAX , int photon_depth) ; ///< Photon-map the scene and creates the photon_tree

    boost::shared_ptr<PhotonMap> _photon_map; ///< List of absorbed photons
};

#endif
