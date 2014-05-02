//---------------------------------------------------------------------------//
//!
//! \file   ElasticScatteringDistribution.cpp
//! \author Alex Robinson
//! \brief  The elastic scattering distribution class definition
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <math.h>

// FACEMC Includes
#include "ElasticScatteringDistribution.hpp"
#include "DirectionHelpers.hpp"
#include "ContractException.hpp"
#include "SearchAlgorithms.hpp"

namespace FACEMC{

// Constructor
ElasticScatteringDistribution::ElasticScatteringDistribution(
		 const double atomic_weight_ratio,
		 Teuchos::Array<Pair<double,Teuchos::RCP<OneDDistribution> > >&
		 angular_scattering_distribution )
  : ScatteringDistribution( atomic_weight_ratio ),
    d_angular_scattering_distribution( angular_scattering_distribution )
{ 
  // Make sure the array has at least one value
  testPrecondition( angular_scattering_distribution.size() > 0 );
}

// Randomly scatter the particle
/*! \details The energy of the neutron in the lab frame is used to sample
 * a center-of-mass scattering angle cosine (this is also done in openmc). It
 * may be more appropriate to instead use the energy of the neutron in the
 * target-at-rest frame as recommended in the MCNP manual.
 */ 
void ElasticScatteringDistribution::scatterParticle( 
					      BasicParticleState& particle,
					      const double temperature ) const
{
  // Calculate the neutron velocity
  double neutron_velocity[3] = {particle.getXDirection()*particle.getSpeed(),
				particle.getYDirection()*particle.getSpeed(),
				particle.getZDirection()*particle.getSpeed()};
    
  // Sample the target velocity
  double target_velocity[3];

  sampleTargetVelocity( particle, temperature, target_velocity );

  // Calculate the center-of-mass velocity
  double center_of_mass_velocity[3];

  calculateCenterOfMassVelocity( neutron_velocity,
				 target_velocity,
				 center_of_mass_velocity );

  // Transform the neutron velocity to the center-of-mass frame
  transformVelocityToCenterOfMassFrame( center_of_mass_velocity, 
					neutron_velocity );

  // Calculate the neutron speed in the center-of-mass
  double cm_neutron_speed = vectorMagnitude( neutron_velocity );

  // Calculate the neutron direction in the center-of-mass frame
  double cm_neutron_direction[3] = {neutron_velocity[0]/cm_neutron_speed,
				    neutron_velocity[1]/cm_neutron_speed,
				    neutron_velocity[2]/cm_neutron_speed};

  // Sample the scattering angle 
  double cm_scattering_angle_cosine = 
    sampleCMScatteringAngleCosine( particle.getEnergy() );

  // Rotate the neutron velocity vector to the new angle
  // Note: The speed of the neutron does not change in the center-of-mass
  double cm_outgoing_neutron_direction[3];

  rotateDirectionThroughPolarAndAzimuthalAngle( cm_scattering_angle_cosine,
						sampleAzimuthalAngle(),
						cm_neutron_direction,
						cm_outgoing_neutron_direction);

  neutron_velocity[0] = cm_neutron_speed*cm_outgoing_neutron_direction[0];
  neutron_velocity[1] = cm_neutron_speed*cm_outgoing_neutron_direction[1];
  neutron_velocity[2] = cm_neutron_speed*cm_outgoing_neutron_direction[2];

  // Transform back to the lab frame
  transformVelocityToLabFrame( center_of_mass_velocity, neutron_velocity );

  // Calculate the outgoing neutron speed
  double outgoing_neutron_speed = vectorMagnitude( neutron_velocity );

  // Set the new neutron direction
  particle.setDirection( neutron_velocity[0]/outgoing_neutron_speed,
			 neutron_velocity[1]/outgoing_neutron_speed,
			 neutron_velocity[2]/outgoing_neutron_speed );

  // Calculate and set the outgoing neutron energy  
  double speed_ratio = 
    outgoing_neutron_speed/PhysicalConstants::speed_of_light;
  
  particle.setEnergy( PhysicalConstants::neutron_rest_mass_energy*speed_ratio/
		      sqrt(1.0 - speed_ratio*speed_ratio) );
}

// Sample a center-of-mass scattering angle cosine
double ElasticScatteringDistribution::sampleCMScatteringAngleCosine(
						    const double energy ) const
{
  double cm_scattering_angle_cosine;

  if( energy < d_angular_scattering_distribution.front().first )
  {
    cm_scattering_angle_cosine = 
      d_angular_scattering_distribution.front().second->sample();
  }
  else if( energy >= d_angular_scattering_distribution.back().first )
  {
    cm_scattering_angle_cosine = 
      d_angular_scattering_distribution.back().second->sample();
  }
  else
  {
    Teuchos::Array<Pair<double,
			Teuchos::RCP<OneDDistribution> > >::const_iterator
      lower_bin_boundary, upper_bin_boundary;
    
    lower_bin_boundary = d_angular_scattering_distribution.begin();
    upper_bin_boundary = d_angular_scattering_distribution.end();
    
    lower_bin_boundary = Search::binarySearchContinuousData<FIRST>( 
							    lower_bin_boundary,
							    upper_bin_boundary,
							    energy );

    upper_bin_boundary = lower_bin_boundary;
    ++upper_bin_boundary;
    
    double lower_bin_boundary_probability = 
      (upper_bin_boundary->first - energy)/
      (upper_bin_boundary->first - lower_bin_boundary->first);
    
    if( RandomNumberGenerator::getRandomNumber<double>() < 
	lower_bin_boundary_probability )
      cm_scattering_angle_cosine = lower_bin_boundary->second->sample();
    else
      cm_scattering_angle_cosine = upper_bin_boundary->second->sample();
  }

  // Due to floating-point roundoff, it is possible for the scattering angle
  // cosine to be outside [-1,1]. When this occurs, manually set to -1 or 1.
  if(Teuchos::ScalarTraits<double>::magnitude(cm_scattering_angle_cosine) >1.0)
    cm_scattering_angle_cosine = copysign( 1.0, cm_scattering_angle_cosine );

  // Make sure that the scattering angle cosine is valid
  testPrecondition( cm_scattering_angle_cosine >= -1.0 );
  testPrecondition( cm_scattering_angle_cosine <= 1.0 );

  return cm_scattering_angle_cosine;
}								    

} // end FACEMC namespace

//---------------------------------------------------------------------------//
// end ElasticScatteringDistribution.cpp
//---------------------------------------------------------------------------//