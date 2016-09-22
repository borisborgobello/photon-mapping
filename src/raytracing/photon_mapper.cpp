/**
 * \file photon_mapper.cpp
 * \brief Implementation of class PhotonMapper
 * \author B.BORGOBELLO
 */

#include "photon_mapper.hpp"
#include "global_parameters.hpp"
#include <lights/radiant_object.hpp>
#include <lights/radiant_volume.hpp>

using std::vector ;
using boost::shared_ptr ;

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
    using namespace std;
    // Photon list building code...
	std::vector< boost::shared_ptr<Photon> > photons ;

    std::vector< boost::shared_ptr<Shape> > shape_list = scene.get_shape_list();
    std::vector< boost::shared_ptr<Light> > light_list = scene.get_light_list();

    int cpt = 0;
    bool intersection_found;
    double current_distance, best_distance;
    Couple3D current_couple, best_couple;
    boost::shared_ptr<Shape> current_shape, best_shape;
    boost::shared_ptr<Light> current_light;
    RadiantObject * current_radiant;
    boost::shared_ptr<Photon> photon_temp, photon_radiant;
    Photon * photon;
    bool is_a_radiant_volume = false;


    cout << "!!STARTING PHOTON-MAPPING WITH LEVEL " << photon_depth << " !!" << endl <<endl;

    int nb_radiant = 0 ;
    for (unsigned int j = 0; j < light_list.size(); j++)
        if( dynamic_cast<RadiantObject*>(light_list[j].get()) != 0 )
            nb_radiant++ ;

    for (unsigned int j = 0; j < light_list.size(); j++)
    {
        is_a_radiant_volume = false;
        cout << "Random photoning light " << j << endl;
        current_radiant = dynamic_cast<RadiantObject*>(light_list[j].get());

        //check
        if (current_radiant == NULL) continue;
        if (dynamic_cast<RadiantVolume*>(current_radiant)) {
            is_a_radiant_volume = true;
            cout << "RADIANT VOLUME !" << endl;
        }
        // fin test

        for (int i=0; i<nb_photon_MAX/nb_radiant; i++)
        {
            photon_temp = current_radiant->random_photon();
            photon = photon_temp.get();

            if (is_a_radiant_volume) {
                photons.push_back(photon_radiant = boost::shared_ptr<Photon>(new Photon(photon->get_end_point(), photon->get_direction(), photon->get_color())));
            }
            for (int x = 0; x < photon_depth; x++) {
                intersection_found = false;
                best_distance = -1;
                for (unsigned int k = 0; k < shape_list.size(); k++) { // Find nearest shape
                    current_shape = shape_list[k];
                    if (current_shape->is_intersected_by(*photon)) {
                        current_couple = current_shape->get_nearest_intersection_with_normal(*photon);
                        current_distance = (photon->get_end_point() - current_couple.first).norm();

                        if (!intersection_found || current_distance < best_distance) {
                            intersection_found = true;
                            best_shape = current_shape;
                            best_couple = current_couple;
                            best_distance = current_distance;
                        }
                    }
                }
                if (intersection_found) {
                    //cout << "Best distance = " << best_distance << endl;
                    //cout << "Best couple :\n" << best_couple.first << endl << endl << best_couple.second << endl;
                    if (!best_shape->redirect_photon(best_couple, *photon_temp)) { // absorbed
                        //cout << "A new absorbed photon !\n"; // save in photon_map
                        photon_temp->set_end_point(best_couple.first);
                        Color ph_c = photon_temp->get_color() ;

                        if(
                            ph_c.get_r() == ph_c.get_g()
                            && ph_c.get_r() == ph_c.get_b()
                            && ph_c.get_r() == 0.0
                        )
                            continue ;

                        double pow = current_radiant->get_power() ;

                        Color final_ph_c(
                            ph_c.get_r() * pow,
                            ph_c.get_g() * pow,
                            ph_c.get_b() * pow
                        ) ;

                        boost::shared_ptr<Photon> photon_final(
                            new Photon(
                                photon_temp->get_end_point(),
                                photon_temp->get_direction(),
                                final_ph_c
                            )
                        ) ;

                        photons.push_back(photon_final);
                        cpt++;
                        break;
                    }
                    else if (x == photon_depth-1) {
                        //cout << "Maximum recu level reached, photon lost\n";
                    }
                }
                else {
                    //cout << "Photon lost into void\n";
                    break;
                }
            }
        }
    }

    cout << endl << cpt << endl << endl;

	return new PhotonMap(photons) ;
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
