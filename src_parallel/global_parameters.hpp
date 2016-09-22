#ifndef GLOBAL_PARAMETERS_HPP_
#define GLOBAL_PARAMETERS_HPP_

/**
 * \file global_parameters.hpp
 * \brief Declaration of GlobalParameters
 * \author B.BORGOBELLO
 */

/**
 * \class GlobalParameters
 * \brief Contains global parameters
 *
 * There can be only one instance of this class in
 * all the program
 * Carries general information found with the ParserYAML class
 */

#include <boost/shared_ptr.hpp>

class GlobalParameters {
private :
    GlobalParameters() : _res_x(800), _res_y(600), _supersampling(false), _nb_photon_MAX(10000), _nb_photon_to_find(100), _photon_depth(20), _raytracer_depth(3) {} ///< Constructor
    ~GlobalParameters() {} ///< Destructor

public :
    void set_res_x(int res_x) {_res_x = res_x;} ///< Sets the resolution component X
    void set_res_y(int res_y) {_res_y = res_y;} ///< Sets the resolution component Y
    void set_supersampling(bool supersampling) {_supersampling = supersampling;} ///< Sets whether the anti-aliasing (supersampling) solution will be used or not
    void set_nb_photon_MAX(int nb_photon_MAX) {_nb_photon_MAX = nb_photon_MAX;} ///< Sets the maximum number of divisions (reflection/refraction) of rays
    void set_nb_photon_to_find(int nb_photon_to_find) {_nb_photon_to_find = nb_photon_to_find;} ///< Sets the maximum number of photons to look for
    void set_photon_depth(int photon_depth) {_photon_depth = photon_depth;} ///< Sets the maximum number of photons emitted during the photon-mapping
    void set_raytracer_depth(int raytracer_depth) {_raytracer_depth = raytracer_depth;} ///< Sets the maximum number of reflection/refraction of photons

    int get_res_x () {return _res_x;} ///< Returns the resolution component X
    int get_res_y () {return _res_y;} ///< Returns the resolution component Y
    bool get_supersampling () {return _supersampling;} ///< Returns whether the anti-aliasing (supersampling) solution will be used or not
    int get_nb_photon_MAX () {return _nb_photon_MAX;} ///< Returns the maximum number of photons emitted during the photon-mapping
    int get_nb_photon_to_find () {return _nb_photon_to_find;} ///< Returns the maximum number of photons to look for (indirect illumination)
    int get_photon_depth () {return _photon_depth;} ///< Returns the maximum number of reflection/refraction of photons
    int get_raytracer_depth () {return _raytracer_depth;} ///< Returns the maximum number of divisions (reflection/refraction) of rays

    static GlobalParameters * get_unique_instance() { ///< Gives the unique instance of the class
        if (_unique_instance == 0)
        	_unique_instance = new GlobalParameters;

        return _unique_instance;
    }

private :
    int     _res_x; ///< Resolution X
    int     _res_y; ///< Resolution Y
    bool    _supersampling; ///< Will supersampling antialiasing be used?
    int     _nb_photon_MAX; ///< Maximum number
    int		_nb_photon_to_find; ///< Number of photons searched in PhotonMapper::get_k_nearest_photons
    int     _photon_depth; ///< Maximum number of reflection/refraction of photons
    int     _raytracer_depth; ///< Maximum number of divisions (reflection/refraction) of rays

    static GlobalParameters * _unique_instance; ///< Pointer to the unique instance of the class
};

#endif
