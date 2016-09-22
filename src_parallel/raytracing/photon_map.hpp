#ifndef PHOTON_MAP_HPP_
#define PHOTON_MAP_HPP_

#include <vector>
#include <cstdlib>
#include <boost/shared_ptr.hpp>
#define LIBSSRCKDTREE_HAVE_BOOST
#include <ssrc/spatial/kd_tree.h>
#include "launchables/photon.hpp"

class PhotonMap
{
    typedef std::array<double, 3> ArrayPoint ;
    typedef ssrc::spatial::kd_tree< ArrayPoint, boost::shared_ptr<Photon> > Tree;

public:
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
    }

private:
    Tree _map ;
} ;

#endif // PHOTON_MAP_HPP_
