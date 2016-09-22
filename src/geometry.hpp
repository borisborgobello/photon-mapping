#ifndef GEOMETRY_HPP_
#define GEOMETRY_HPP_

/**
 * \file geometry.hpp
 * \brief Typedefs for the EIGEN used types
 * \author T.FEIGLER
 */

#include <Eigen/Core>
#include <utility>

USING_PART_OF_NAMESPACE_EIGEN ; ///< Using namespace Eigen

typedef Vector3d Vector3D ; ///< 3D vector typedef
typedef Vector3d Point3D ; ///< 3D point typedef
typedef std::pair< Point3D, Vector3D > Couple3D ; ///< Typedef of a couple, containing a point and a vector

#endif /* GEOMETRY_HPP_ */
