#ifndef PHOTON_MAP_HPP_
#define PHOTON_MAP_HPP_

/**
 * \file photon_map.hpp
 * \author T.FEIGLER
 * \brief Declaration of class PhotonMap
 */

#include <vector>
#include <cstdlib>
#include <boost/smart_ptr/shared_ptr.hpp>
#define LIBSSRCKDTREE_HAVE_BOOST
#include <ssrc/spatial/kd_tree.h>
#include "launchables/photon.hpp"


/**
 * \class PhotonMap
 * \brief Determines efficiently the k-nearest-neighbour at a given point
 */
class PhotonMap
{
    typedef std::array<double, 3> ArrayPoint ; ///< Three component vector
    typedef ssrc::spatial::kd_tree< ArrayPoint, boost::shared_ptr<Photon> > Tree; ///< The new tree of photons

public:
    /**
	 * \brief Constructor optimizing the kd-tree
	 * \param list : the list of all absorbed photons during the photon-mapping
     *
     * Tidy the given photon-list efficiently for all the futur searches for the k-nearest.
	 */
    PhotonMap(std::vector< boost::shared_ptr<Photon> > list)
    {
        for(std::vector< boost::shared_ptr<Photon> >::iterator it = list.begin() ; it != list.end() ; it++)
        {
            const Point3D& point3d = (*it)->get_end_point() ;
            ArrayPoint array_point ;

            array_point[0] = point3d[0] ;
            array_point[1] = point3d[1] ;
            array_point[2] = point3d[2] ;

            _map[array_point] = (*it) ;
        }

        std::cout << "Balancing the photon KD-Tree..." << std::endl ;
        _map.optimize() ;
        std::cout << "Optimization done." << std::endl ;
    }

    /**
	 * \brief Returns a list of the given point's k-nearest-photons
	 * \param point : the center of the searching sphere
     * \param k : the number of photons to find near the point
	 */
    std::vector< boost::shared_ptr<Photon> > get_k_nearest(const Point3D& point, int k )
    {
        std::vector< boost::shared_ptr<Photon> > ret ;
        ret.clear() ;

        if( k <= 0 )
        {
            for(Tree::iterator it = _map.begin() ; it != _map.end() ; it++)
                ret.push_back(it->second) ;

            return ret ;
        }

        Tree::knn_iterator iter_debut, iter_fin ;

        ArrayPoint array_point ;

        array_point[0] = point[0] ;
        array_point[1] = point[1] ;
        array_point[2] = point[2] ;

        std::pair<Tree::knn_iterator,Tree::knn_iterator> iters =
            _map.find_nearest_neighbors(array_point, 1000) ;

        for(Tree::knn_iterator it = iters.first ; it != iters.second ; it++)
            ret.push_back( it->second ) ;

        return ret ;
    }


    void optimize()
    {
        _map.optimize() ;
    } ///< More simple method to optimize

private:
    Tree _map ; ///< The optimized tree of photons
} ;

#endif // PHOTON_MAP_HPP_
