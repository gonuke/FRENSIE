//---------------------------------------------------------------------------//
//!
//! \file   Facemc_PhotoelectricPhotoatomicReaction.hpp
//! \author Alex Robinson
//! \brief  The pair production photoatomic reaction class decl.
//!
//---------------------------------------------------------------------------//

#ifndef FACEMC_PHOTOELECTRIC_PHOTOATOMIC_REACTION_HPP
#define FACEMC_PHOTOELECTRIC_PHOTOATOMIC_REACTION_HPP

// Trilinos Includes
#include <Teuchos_RCP.hpp>

// FRENSIE Includes
#include "Facemc_PhotoatomicReaction.hpp"
#include "Facemc_PhotoelectricPhotonScatteringDistribution.hpp"

namespace Facemc{

//! The pair production photoatomic reaction class
class PhotoelectricPhotoatomicReaction : public PhotoatomicReaction
{

public:

  //! Constructor
  PhotoelectricPhotoatomicReaction(
		const Teuchos::ArrayRCP<const double>& incoming_energy_grid,
		const Teuchos::ArrayRCP<const double>& cross_section );

  //! Destructor
  ~PhotoelectricPhotoatomicReaction()
  { /* ... */ }

  //! Return the number of photons emitted from the rxn at the given energy
  unsigned getNumberOfEmittedPhotons( const double energy ) const;

  //! Simulate the reaction
  void react( PhotonState& photon, ParticleBank& bank ) const;

private:
  
  // The pair production scattering distribution
  PhotoelectricPhotonScatteringDistribution d_scattering_distribution;	      
};

} // end Facemc namespace

#endif // end FACEMC_PHOTOELECTRIC_PHOTOATOMIC_REACTION_HPP

//---------------------------------------------------------------------------//
// end Facemc_PhotoelectricPhotoatomicReaction.hpp
//---------------------------------------------------------------------------//
