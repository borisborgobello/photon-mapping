#ifndef GEOMETRY_HPP_
#define GEOMETRY_HPP_

/**
 * \file geometry.hpp
 * \brief Typedefs for the EIGEN used types
 * \author T.FEIGLER
 */

#include <Eigen/Core>
#include <utility>

USING_PART_OF_NAMESPACE_EIGEN ;

typedef Vector3d Vector3D ;
typedef Vector3d Point3D ;
typedef std::pair< Point3D, Vector3D > Couple3D ;

#endif /* GEOMETRY_HPP_ */
