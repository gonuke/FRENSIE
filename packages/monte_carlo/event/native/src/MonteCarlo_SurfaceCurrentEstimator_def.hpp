//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_SurfaceCurrentEstimator_def.hpp
//! \author Alex Robinson
//! \brief  Surface current estimator class definition
//!
//---------------------------------------------------------------------------//

#ifndef MONTE_CARLO_SURFACE_CURRENT_ESTIMATOR_DEF_HPP
#define MONTE_CARLO_SURFACE_CURRENT_ESTIMATOR_DEF_HPP

// Std Lib Includes
#include <iostream>

// FRENSIE Includes
#include "Utility_ExplicitTemplateInstantiationMacros.hpp"
#include "Utility_LoggingMacros.hpp"
#include "Utility_ContractException.hpp"

namespace MonteCarlo{

// Explicit instantiation (extern declaration)
EXTERN_EXPLICIT_TEMPLATE_CLASS_INST( SurfaceCurrentEstimator<WeightMultiplier> );
EXTERN_EXPLICIT_TEMPLATE_CLASS_INST( SurfaceCurrentEstimator<WeightAndEnergyMultiplier> );

// Constructor
template<typename ContributionMultiplierPolicy>
template<typename<typename,typename...> class STLCompliantArray>
SurfaceCurrentEstimator<ContributionMultiplierPolicy>::SurfaceCurrentEstimator(
              const Estimator::idType id,
              const double multiplier,
              const STLCompliantArray<StandardSurfaceEstimator::surfaceIdType>&
              surface_ids )
  : StandardSurfaceEstimator( id, multiplier, surface_ids )
{ /* ... */ }

// Add current history estimator contribution
/*! \details It is unsafe to call this function directly! This function will
 * be called by the appropriate dispatcher whan an event of interest occurs.
 * If calling this function directly, make sure that the cell of surface of
 * interest is actually assigned to this estimator (otherwise seg. faults are
 * likely!).
 */
template<typename ContributionMultiplierPolicy>
void SurfaceCurrentEstimator<ContributionMultiplierPolicy>::updateFromParticleCrossingSurfaceEvent(
		const ParticleState& particle,
		const StandardSurfaceEstimator::surfaceIdType surface_crossing,
		const double angle_cosine )
{
  // Make sure the surface is assigned to this estimator
  testPrecondition( isEntityAssigned( surface_crossing ) );
  // Make sure the angle cosine is valid
  testPrecondition( angle_cosine <= 1.0 );
  testPrecondition( angle_cosine >= -1.0 );

  if( this->isParticleTypeAssigned( particle.getParticleType() ) )
  {
    double contribution =
      ContributionMultiplierPolicy::multiplier( particle );

    EstimatorParticleStateWrapper particle_state_wrapper( particle );
    particle_state_wrapper.setAngleCosine( angle_cosine );

    this->addPartialHistoryPointContribution( surface_crossing,
                                              particle_state_wrapper,
                                              contribution );
  }
}

// Print the estimator data
template<typename ContributionMultiplierPolicy>
void SurfaceCurrentEstimator<ContributionMultiplierPolicy>::printSummary(
                                                       std::ostream& os ) const
{
  os << "Surface Current Estimator: " << getId() << std::endl;

  this->printImplementation( os, "Surface" );
}

// Set the response functions
template<typename ContributionMultiplierPolicy>
void SurfaceCurrentEstimator<ContributionMultiplierPolicy>::assignParticleType( const ParticleType particle_type )
{
  FRENSIE_LOG_TAGGED_WARNING( "Estimator",
                              "response functions cannot be set for surface "
                              "current estimators. The response function "
                              "requested for surface current estimator "
                              << getId() << " will be ignored." );
}

} // end MonteCarlo namespace

#endif // end MONTE_CARLO_SURFACE_CURRENT_ESTIMATOR_DEF_HPP

//---------------------------------------------------------------------------//
// end MonteCarlo_SurfaceCurrentEstimator_def.hpp
//---------------------------------------------------------------------------//
