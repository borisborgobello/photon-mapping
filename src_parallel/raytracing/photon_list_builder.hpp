#ifndef PHOTON_LIST_BUILDER_HPP
#define PHOTON_LIST_BUILDER_HPP

#include <vector>
#include <tbb/concurrent_vector.h>
#include <tbb/blocked_range2d.h>
#include <tbb/parallel_for.h>
#include "lights/radiant_object.hpp"
#include "launchables/photon.hpp"
#include "shapes/shape.hpp"
#include "scene.hpp"
#include "global_parameters.hpp"




class PhotonListBuilder
{
public:
    typedef std::vector< boost::shared_ptr<RadiantObject> >::iterator
    RowType ;	///< Represents an iterator to a (smart_ptr of a) RadiantObject

    typedef int
    ColType ;	///< Represents the numero of the photon to be generated

    PhotonListBuilder( tbb::concurrent_vector< boost::shared_ptr<Photon> >& photons, const Scene& sc)
	: _photons(photons), _scene(sc)
    {
	}

    void operator() (const tbb::blocked_range2d< RowType, ColType >&) const ;

    tbb::concurrent_vector< boost::shared_ptr<Photon> > get_photons() const
    {
        return _photons ;
    }

private:
    tbb::concurrent_vector< boost::shared_ptr<Photon> >& _photons ;
    const Scene& _scene ;
} ;


#endif
