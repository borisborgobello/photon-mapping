#ifndef PHOTON_MAPPING_BASED_HPP_
#define PHOTON_MAPPING_BASED_HPP_

#include <string>
#include <boost/shared_ptr.hpp>
#include "raytracer.hpp"
#include "photon_mapper.hpp"
#include "global_parameters.hpp"
#include "launchables/ray.hpp"

/**
 * \file photon_mapping_based.hpp
 * \brief Declaration of class PhotonMappingBased
 * \author B.BORGOBELLO/ T.FEIGLER
 */

/**
 * \class PhotonMappingBased
 * \brief Derived from Raytracer, using photon maps
 */
class PhotonMappingBased : Raytracer {
public:
    /**
	 * \brief Constructor
	 * \param scene : the scene to photon-trace
     *
     * Directly calls the build_photon_tree method (photon_mapping phase)
     * thus creating the photon_maps
	 */
    PhotonMappingBased(const Scene& sc) :
    	_photon_mapper(
    			sc,
    			GlobalParameters::get_unique_instance()->get_nb_photon_MAX(),
    			GlobalParameters::get_unique_instance()->get_photon_depth()
    		) {}
    Image render(const Scene&) const ;              ///< Returns an Image with the given scene
    Image render_photonmap(const Scene&) const ;    ///< Returns an Image of the photon-map of the scene

private:
    PhotonMapper _photon_mapper; ///< Contains the photon_mapper used for the scene

    Color get_local_color(Ray, const Scene&, int depth_level) const ; ///< Aimed recursive, calculates the color of a point
};

#endif
