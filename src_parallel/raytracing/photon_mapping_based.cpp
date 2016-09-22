/**
 * \file photon_mapping_based.cpp
 * \brief Implementation of class PhotonMappingBased
 * \author T.FEIGLER / B.BORGOBELLO
 */

#include <iterator>
#include "photon_mapping_based.hpp"
#include "lights/coherent_light_source.hpp"
#include "shapes/volume.hpp"
#include "shapes/surface.hpp"

using boost::shared_ptr ;
using std::vector ;
using std::cout ;
using std::endl ;

/**
 * \brief Raytraces a scene returning the corresponding image
 * \param sc : the scene to raytrace
 */
Image PhotonMappingBased::render(const Scene& sc) const
{
	GlobalParameters *params = GlobalParameters::get_unique_instance() ;
	const Camera& cam = *(sc.get_camera()) ;
	int nb_rendered = 0 ;

    std::cout << "!!STARTING RAYTRACING!!" << std::endl;
	Image img(
			params->get_res_x(),
			params->get_res_y()
		) ;

    int nb_total_pixels = img.get_res_x() * img.get_res_y() ;

	for(int j = 0 ; j < img.get_res_y() ; j++)
	{
		for(int i = 0 ; i < img.get_res_x() ; i++)
		{
		    if( ((nb_rendered * 20) % nb_total_pixels) > (((nb_rendered+1) * 20) % nb_total_pixels) )
		    {
                cout << "Raytracing : " << (5 * ((nb_rendered+1) * 20) / nb_total_pixels) << "%" << endl ;
		    }

            nb_rendered++ ;

			Ray ray = cam.get_ray(
								i/((double)img.get_res_x()-1),
								j/((double)img.get_res_y()-1) // BUG ICI
							) ;

			Color col_ = get_local_color(ray, sc, params->get_raytracer_depth()) ;

			boost::shared_ptr<Color> col(new Color(
					col_.get_r(),
					col_.get_g(),
					col_.get_b()
				)) ;

			img.add_color(col, i, j) ;
		}
	}
    cout << "!!RAYTRACING TERMINATED!!" << endl;
	return img ;
}

/**
 * \brief Renders the photon-map (counter raytracing) and returns an Image
 * \param sc : the scene containing the camera
 */
Image PhotonMappingBased::render_photonmap(const Scene& sc) const
{
    GlobalParameters *params = GlobalParameters::get_unique_instance() ;
	boost::shared_ptr<Color> col;

    const Camera& cam = *(sc.get_camera()) ;
    vector< shared_ptr<Photon> > photons = _photon_mapper.get_k_nearest_photons(-1,Point3D(0,0,0)) ;

    cout << "!!RAYTRACING THE PHOTON_MAP!!" << endl;
	Image img(
			params->get_res_x(),
			params->get_res_y()
		) ;

    // Initializing the image to full_black
	for(int j = 0 ; j < img.get_res_y() ; j++)
	{
		for(int i = 0 ; i < img.get_res_x() ; i++)
		{
            img.add_color(col = boost::shared_ptr<Color>(new Color(0,0,0)), i, j) ;
		}
	}

    std::pair<double, double> coordinates;
    /*boost::shared_ptr<Photon> temp;
    photons.push_back(temp = boost::shared_ptr<Photon>(new Photon(Point3D(-2,0,0)+0.5*Point3D(1,1,1) , Vector3D(1,0,0), Color(0,0,0))));
    */
    for (unsigned k = 0; k < photons.size(); k++) {
        //cout << photons[k]->get_end_point() << endl;
        coordinates = cam.can_see(photons[k]->get_end_point());
        if (coordinates.first >=0) {
            //cout << " VS [" << (int)(100*cam.get_ray(coordinates.first, coordinates.second).get_direction()[0]) << ", " << (int)(100*cam.get_ray(coordinates.first, coordinates.second).get_direction()[1]) << ", " << (int)(100*cam.get_ray(coordinates.first, coordinates.second).get_direction()[2]) << "]" << endl;
            //cout << "Adding color !" << endl;
            img.add_color(col = boost::shared_ptr<Color>(new Color(1,1,1)), coordinates.first*img.get_res_x(), coordinates.second*img.get_res_y()) ;
        }
    }
    cout << "!!PHOTON RAYTRACING TERMINATED!!" << endl;
	return img ;
}

/**
 * \brief Aimed recursive, computes the color viewed by the given ray
 * \param ray : the ray to launch into the scene
 * \param sc : the scene containing lights/objects/camera
 * \param depth: the remaining depth of the recursion
 */
Color PhotonMappingBased::get_local_color(Ray ray, const Scene& sc, int depth) const
{
	Couple3D nearest_couple ;
	double nearest_distance_2 = -1.0;
	vector< shared_ptr<Shape> >::const_iterator shape_it ;
	vector< shared_ptr<Shape> >::const_iterator final_it ;

	final_it = sc.get_shape_list().end() ;

	for(shape_it = sc.get_shape_list().begin() ; shape_it != sc.get_shape_list().end() ; shape_it++)
	{
		if((*shape_it)->is_intersected_by(ray))
		{
			Couple3D couple =
					(*shape_it)->get_nearest_intersection_with_normal(ray) ;

			double this_distance_2 = (ray.get_end_point() - couple.first).squaredNorm() ;

			if(nearest_distance_2 < 0.0
				|| nearest_distance_2 > this_distance_2)
			{
				nearest_couple = couple ;
				nearest_distance_2 = this_distance_2 ;
				final_it = shape_it ;
			}
		}
	}

	if(final_it != sc.get_shape_list().end())
	{
		// An intersection has been found
		// Now determining its illumination
		vector< shared_ptr<Light> >::const_iterator it ;

		Point3D nearest_intersection = nearest_couple.first ;
		Color c = (*final_it)->get_color_at(nearest_intersection) ;

		vector< shared_ptr<Photon> > photons ;
		GlobalParameters *params = GlobalParameters::get_unique_instance() ;
		photons = _photon_mapper.get_k_nearest_photons(
				params->get_nb_photon_to_find(),
				nearest_intersection
			) ;

		double r_, g_, b_ ;
		r_ = g_ = b_ = 0.0 ;
		for(vector< shared_ptr<Photon> >::iterator it = photons.begin() ; it != photons.end() ; it++)
		{
			// TODO : verify the coherence of the flux calculated
			Color ph_color = (*it)->get_color() ;
			r_ += ph_color.get_r() ;
			g_ += ph_color.get_g() ;
			b_ += ph_color.get_b() ;
		}

        double inner_photon_power =
            1.0 / GlobalParameters::get_unique_instance()->get_nb_photon_MAX() ;

        double furthest_distance_2 = (nearest_intersection - photons[photons.size()-1]->get_end_point()).squaredNorm() ;
		double flux_coef = inner_photon_power / furthest_distance_2 ;
		//cout << "couleur : " << r_ << " " << g_ << " " << b_ << endl ;
		//cout << "flux coeff : " << flux_coef << endl ;
		r_ *= flux_coef * c.get_r() ;
		g_ *= flux_coef * c.get_g() ;
		b_ *= flux_coef * c.get_b() ;

		Color absorbed_illumination(r_, g_, b_) ;

		// Now we evaluate recursively the global illumination
		// TODO : verify the recursively-built rays are correct

		Color reflected_illumination( 0.0, 0.0, 0.0) ;
		Color refracted_illumination(0.0, 0.0, 0.0) ;

        std::pair<Ray, Ray> double_ray = (*final_it)->divide_ray(nearest_couple, ray);
        Ray reflected_ray = double_ray.first;
        Ray refracted_ray = double_ray.second;

        if (depth > 0)
        {
            if (reflected_ray.get_direction().norm() != 0)
                reflected_illumination = get_local_color(reflected_ray, sc, depth - 1);

            if (refracted_ray.get_direction().norm() != 0)
                refracted_illumination = get_local_color(refracted_ray, sc, depth - 1);
        }

		if(depth == 0)
			return absorbed_illumination ;
		else
		{
		    double r, g, b ;
		    r = g = b = 0.0 ;

			double refraction_prob = -1.0 ;
			Volume *vol ;
			Surface *surf ;

			vol = dynamic_cast< Volume* >( (*final_it).get() ) ;
			if(vol != 0)
				refraction_prob = vol->get_refraction_prob() ;

			surf = dynamic_cast< Surface* >( (*final_it).get() ) ;
			if(surf != 0)
				refraction_prob = surf->get_transparency_prob() ;

			if(refraction_prob <= 0.0)
				refraction_prob = 0.0 ;

			double reflection_prob = (*final_it)->get_reflection_prob() ;
			double absorption_prob = (*final_it)->get_absorption_prob() ;

			r =
					reflected_illumination.get_r() * reflection_prob
					+ refracted_illumination.get_r() * refraction_prob
					+ absorbed_illumination.get_r() * absorption_prob ;

			g =
					reflected_illumination.get_g() * reflection_prob
					+ refracted_illumination.get_g() * refraction_prob
					+ absorbed_illumination.get_g() * absorption_prob ;

			b =
					reflected_illumination.get_b() * reflection_prob
					+ refracted_illumination.get_b() * refraction_prob
					+ absorbed_illumination.get_b() * absorption_prob ;

			return Color(r, g, b) ;
		}
	}
	else
		return Color(0.0, 0.0, 0.0) ;
}
