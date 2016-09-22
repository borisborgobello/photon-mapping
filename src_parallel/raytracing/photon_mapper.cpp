/**
 * \file photon_mapper.cpp
 * \brief Implementation of class PhotonMapper
 * \author B.BORGOBELLO
 */

#include <tbb/blocked_range2d.h>
#include <tbb/parallel_for.h>

#include "photon_list_builder.hpp"
#include "photon_mapper.hpp"
#include "global_parameters.hpp"
#include <lights/radiant_object.hpp>
#include <lights/radiant_volume.hpp>


using namespace tbb ;
using std::vector ;
using std::cout ;
using std::endl ;
using boost::shared_ptr ;
typedef vector< shared_ptr<RadiantObject> >::iterator RowType ;
typedef int ColType ;

/**
 * \brief Creates the photon map with the given scene
 * \param scene : the scene to photon-trace
 * \param nb_photon_MAX : the size (in number of photons) of the photon-map
 * \param photon_depth : the maximum number of reflection/refraction of a photon
 *
 * Returns the list of absorbed photons
 */
PhotonMap * PhotonMapper::build_photon_tree(const Scene& scene, int nb_photon_MAX , int photon_depth)
{
	vector< shared_ptr<RadiantObject> > radiants ;
	radiants.clear() ;
	const vector< shared_ptr<Light> >& lights = scene.get_light_list() ;

	for( vector< shared_ptr<Light> >::const_iterator it = lights.begin() ; it != lights.end() ; it++ )
	{
		if( RadiantObject* rad = dynamic_cast<RadiantObject*>( it->get() ) )
			radiants.push_back( shared_ptr< RadiantObject >(rad) ) ;
	}

	concurrent_vector< shared_ptr<Photon> > photons ;
	photons.clear() ;
	PhotonListBuilder plb(photons, scene) ;

	cout << "Radiants : " << radiants.size() << endl ;

	parallel_for(
		blocked_range2d<RowType, ColType>(
			radiants.begin(), radiants.end(),
			0, GlobalParameters::get_unique_instance()->get_nb_photon_MAX() / radiants.size()
		),
		plb
	) ;

	vector< shared_ptr<Photon> > photons_ ;
	photons_.clear() ;

	std::cout << "Size : " << photons.size() << std::endl ;

	for( concurrent_vector< shared_ptr<Photon> >::iterator it = photons.begin() ; it != photons.end() ; it++ )
	{
		photons_.push_back( *it ) ;
	}

	return new PhotonMap(photons_) ;
}

/**
 * \param k : number of photon to find around the point
 * \param pt : center of the searching sphere
 *
 * Returns a list of the k nearest photons
 */
std::vector< boost::shared_ptr<Photon> > PhotonMapper::get_k_nearest_photons(int k, const Point3D& pt) const
{
	return _photon_map->get_k_nearest(pt, k) ;
}
