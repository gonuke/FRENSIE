//---------------------------------------------------------------------------//
//!
//! \file   NuclearReactionBank.cpp
//! \author Alex Robinson
//! \brief  Nuclear reaction bank class definition
//!
//---------------------------------------------------------------------------//

// FACEMC Includes
#include "NuclearReactionBank.hpp"
#include "ContractException.hpp"

namespace FACEMC{

// Constructor
NuclearReactionBank::NuclearReactionBank( 
			 const Teuchos::Array<NuclearReactionType>& reactions )
  : d_nuclear_reaction_banks()
{
  // Make sure there is at least on nuclear reaction of interest
  testPrecondition( reactions.size() > 0 );

  // Initialize the map
  for( unsigned i = 0; i < reactions.size(); ++i )
    d_nuclear_reaction_banks[reactions[i]];
}

// Push a neutron to the bank
void NuclearReactionBank::push( const Teuchos::RCP<NeutronState>& neutron,
				const NuclearReactionType reaction )
{
  if( d_nuclear_reaction_banks.find( reaction ) != 
      d_nuclear_reaction_banks.end() )
    d_nuclear_reaction_banks[reaction].push_front( neutron );
  else
    ParticleBank::push( Teuchos::rcp_dynamic_cast<ParticleState>( neutron ) );
}

} // end FACEMC namespace

//---------------------------------------------------------------------------//
// end NuclearReactionBank.cpp
//---------------------------------------------------------------------------//
