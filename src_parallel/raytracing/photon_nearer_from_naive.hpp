/**
 * \file photon_nearer_from_naive.hpp
 * \author T.FEIGLER
 * \brief Declaration of class PhotonNearerFromNaive
 */

#ifndef PHOTON_NEARER_FROM_NAIVE_HPP_
#define PHOTON_NEARER_FROM_NAIVE_HPP_

#include <boost/shared_ptr.hpp>
#include "launchables/photon.hpp"
#include "geometry.hpp"

/**
 * \class PhotonNearerFromNaive
 * \brief Fonctor class determining the nearest photons
 *
 * PhotonNearerFromNaive is used to determine is a photon
 * is closer to a given point than another
 */
class PhotonNearerFromNaive
{
public:
	/**
	 * \brief Constructor
	 * \param cent : central point
	 */
	PhotonNearerFromNaive(Point3D cent) : central(cent) {}

	/**
	 * \brief Fonctor
	 * \param ph1, ph2 : smart pointers to photons
	 * \return True if ph1 is closer to the central point than ph2
	 */
	bool operator()(boost::shared_ptr<Photon> ph1, boost::shared_ptr<Photon> ph2)
	{
		return (((*ph1).get_end_point() - central).squaredNorm()
				<
				((*ph2).get_end_point() - central).squaredNorm()) ;
	}

private:
	Point3D central ;	///< Point from which we compare the distances
};


#endif /* PHOTON_NEARER_FROM_NAIVE_HPP_ */
