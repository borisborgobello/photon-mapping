#ifndef PHOTON_LIST_HPP_
#define PHOTON_LIST_HPP_

/**
 * \file photon_list.hpp
 * \author T.FEIGLER
 * \brief Declaration of class PhotonList
 */

#include <vector>
#include <algorithm>
#include <cstdlib>
#include "photon_nearer_from_naive.hpp"

/**
 * \class PhotonList
 * \brief Determines naively the k nearest photons around a given point
 */
class PhotonList
{
public:
	/**
	 * \brief Constructor
	 * \param list : a list of smart pointers to photons
	 */
	PhotonList(std::vector< boost::shared_ptr<Photon> > list) :
		_photons(list) {}

	/**
	 * \brief Method to get the k nearest photon of a point
	 * \param point : center of the research sphere
	 * \param k : number of photon to find
	 *
	 * The list is roughly sorted by distance of photon to point
	 * and cut to the desired length k.
	 */
	std::vector< boost::shared_ptr<Photon> > get_k_nearest(const Point3D& point, int k )
	{
	    unsigned int k_ ;

	    if(k < 0)
            return _photons ;
        else
            k_ = (unsigned int)k ;

        if(k_ > _photons.size())
            return _photons ;

	    std::vector< boost::shared_ptr<Photon> > ret(k_) ;
	    ret.clear() ;

		std::sort(_photons.begin(), _photons.end(),
				PhotonNearerFromNaive(point)) ;

		for(unsigned int i = 0 ; i < k_ ; i++ )
            ret.push_back( _photons[i] ) ;

		return ret ;
	}

private:
	std::vector< boost::shared_ptr<Photon> > _photons ;	///< Total list of Photon created with PhotonMapper
};

#endif /* PHOTON_LIST_HPP_ */
