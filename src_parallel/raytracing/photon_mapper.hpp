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
	 * \param scene : the scene to photon-trace
     * \param nb_photons : the size (in number of photons) of the photon-map
     * \param photon_depth : the maximum number of reflection/refraction of a photon
     *
     * Directly calls the build_photon_tree method (photon_mapping phase)
	 */
    PhotonMapper(const Scene& sc, int nb_photons, int photon_depth) :
    	_photon_map(build_photon_tree(sc, nb_photons, photon_depth)) {}

	/**
	 * \brief k-nearest-neighbor search (kNN-search) method
	 * \param k : Number of photons to search
	 * \param point : Reference to the point where to search the nearest photons from
	 *
	 * Returns the k nearest photons (full list if k<=0) from the given point
	 */
    std::vector< boost::shared_ptr<Photon> > get_k_nearest_photons(int k, const Point3D& point) const ; ///< Returns the k nearest photons of the given point

	/**
	 * \brief KD-Tree optimization method
	 *
	 * Optimizes the photon map so as to perform faster future kNN searches
	 */
    void optimize() const { _photon_map->optimize() ; }

private:
	/**
	 * \brief Photon-tree building method
	 * \param scene : The scene where to launch the photons
	 * \param nb_photon_MAX : The maximum number of photons to launch
	 * \param photon_depth : The maximum number of reflections/refractions to perform
	 *
	 * Optimizes the photon map so as to perform faster future kNN searches
	 */
    static PhotonMap *build_photon_tree
		(const Scene& scene, int nb_photon_MAX , int photon_depth) ; ///< Photon-map the scene and creates the photon_tree

    boost::shared_ptr<PhotonMap> _photon_map; ///< List of absorbed photons
};

#endif
