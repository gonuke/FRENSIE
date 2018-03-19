//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_NeutronState.cpp
//! \author Alex Robinson
//! \brief  Neutron state class definition
//!
//---------------------------------------------------------------------------//

// Boost Includes
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>
#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>

// FRENSIE Includes
#include "MonteCarlo_NeutronState.hpp"
#include "Utility_HDF5IArchive.hpp"
#include "Utility_HDF5OArchive.hpp"
#include "Utility_PhysicalConstants.hpp"
#include "Utility_ArchiveHelpers.hpp"
#include "Utility_ContractException.hpp"

namespace MonteCarlo{

// Constructor
NeutronState::NeutronState()
  : MassiveParticleState()
{ /* ... */ }

// Constructor
NeutronState::NeutronState(
		        const ParticleState::historyNumberType history_number )
  : MassiveParticleState( history_number, NEUTRON, 0 )
{ /* ... */ }

// Copy constructor (with possible creation of new generation)
NeutronState::NeutronState( const ParticleState& existing_base_state,
			    const bool increment_generation_number,
			    const bool reset_collision_number )
  : MassiveParticleState( existing_base_state,
			  NEUTRON,
			  0,
			  Utility::PhysicalConstants::neutron_rest_mass_energy,
			  increment_generation_number,
			  reset_collision_number )
{ /* ... */ }

// Copy constructor (with possible creation of new generation)
NeutronState::NeutronState( const NeutronState& existing_state,
			    const bool increment_generation_number,
			    const bool reset_collision_number )
  : MassiveParticleState( existing_state,
			  NEUTRON,
			  0,
			  Utility::PhysicalConstants::neutron_rest_mass_energy,
			  increment_generation_number,
			  reset_collision_number )
{ /* ... */ }

// Return the rest mass energy of the neutron (MeV)
double NeutronState::getRestMassEnergy() const
{
  return Utility::PhysicalConstants::neutron_rest_mass_energy;
}

// Clone the particle state (do not use to generate new particles!)
NeutronState* NeutronState::clone() const
{
  return new NeutronState( *this, false, false );
}

// Print the neutron state
void NeutronState::toStream( std::ostream& os ) const
{
  os << "Particle Type: Neutron" << std::endl;

  this->printImplementation<NeutronState>( os );
}

EXPLICIT_MONTE_CARLO_CLASS_SERIALIZE_INST( NeutronState );

} // end MonteCarlo namespace

BOOST_CLASS_EXPORT_IMPLEMENT( MonteCarlo::NeutronState );

//---------------------------------------------------------------------------//
// end MonteCarlo_NeutronState.cpp
//---------------------------------------------------------------------------//
