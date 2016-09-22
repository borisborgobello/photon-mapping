#include "raytracing/photon_list_builder.hpp"
#include "lights/radiant_volume.hpp"


using namespace tbb ;
using std::vector ;
using boost::shared_ptr ;


void PhotonListBuilder::operator() (const blocked_range2d< RowType, ColType >& r) const
{
    // Concurrent vector where to save the generated photons
    concurrent_vector< shared_ptr<Photon> >& photons =
		*(const_cast< concurrent_vector< shared_ptr<Photon> >* >(&_photons) ) ;

    const vector< shared_ptr<Shape> >& shape_list = _scene.get_shape_list();

    const int photon_depth = GlobalParameters::get_unique_instance()->get_photon_depth() ;

    bool intersection_found;
    double current_distance, best_distance;
    Couple3D current_couple, best_couple;
    boost::shared_ptr<Shape> current_shape, best_shape;
    boost::shared_ptr<Photon> photon_temp, photon_radiant;
    Photon * photon;


    for ( RowType x = r.rows().begin() ; x != r.rows().end() ; x++ )
    {
        bool is_a_radiant_volume = false;

        if ( dynamic_cast<RadiantVolume*>( (*x).get() ) )
        {
            is_a_radiant_volume = true;
            // cout << "RADIANT VOLUME !" << endl;
        }

        for ( ColType y = r.cols().begin() ; y != r.cols().end() ; y++ )
        {
            photon_temp = (*x)->random_photon();
            photon = photon_temp.get();

            if (is_a_radiant_volume)
            {
                photons.push_back(photon_radiant = boost::shared_ptr<Photon>(new Photon(photon->get_end_point(), photon->get_direction(), photon->get_color())));
            }

            for (int iter = 0 ; iter < photon_depth ; iter++)
            {
                intersection_found = false;
                best_distance = -1;
                for (unsigned int k = 0; k < shape_list.size(); k++)
                {
                    current_shape = shape_list[k];
                    if (current_shape->is_intersected_by(*photon))
                    {
                        current_couple = current_shape->get_nearest_intersection_with_normal(*photon);
                        current_distance = (photon->get_end_point() - current_couple.first).norm();
                        ////cout << "Shape " << k << endl << "Intersection found !" << endl << "This distance = " << current_distance << endl;

                        if (!intersection_found || current_distance < best_distance)
                        {
                            intersection_found = true;
                            best_shape = current_shape;
                            best_couple = current_couple;
                            best_distance = current_distance;
                        }
                    }
                }
                if (intersection_found)
                {
                    //cout << "Best distance = " << best_distance << endl;
                    //cout << "Best couple :\n" << best_couple.first << endl << endl << best_couple.second << endl;
                    if (!best_shape->redirect_photon(best_couple, *photon_temp))
                    {
                        photon_temp->set_end_point(best_couple.first);
                        Color ph_c = photon_temp->get_color() ;

                        if (
                            ph_c.get_r() == ph_c.get_g()
                            && ph_c.get_r() == ph_c.get_b()
                            && ph_c.get_r() == 0.0
                        )
                            continue ;

                        double pow = (*x)->get_power() ;

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
                        break;
                    }
                    else if (iter == photon_depth-1)
                    {
                        //cout << "Maximum recu level reached, photon lost\n";
                    }
                }
                else
                {
                    //cout << "Photon lost into void\n";
                    break;
                }
            }
        }
    }
}
