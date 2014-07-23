//---------------------------------------------------------------------------//
//! 
//! \file   Facemc_IncoherentPhotonScatteringDistribution.hpp
//! \author Alex Robinson
//! \brief  The incoherent photon scattering distribution
//!
//---------------------------------------------------------------------------//

#ifndef FACEMC_INCOHERENT_PHOTON_SCATTERING_DISTRIBUTION_HPP
#define FACEMC_INCOHERENT_PHOTON_SCATTERING_DISTRIBUTION_HPP

// Trilinos Includes
#include <Teuchos_RCP.hpp>
#include <Teuchos_ArrayView.hpp>

// FRENSIE Includes
#include "Facemc_PhotonScatteringDistribution.hpp"
#include "Utility_OneDDistribution.hpp"
#include "Utility_Tuple.hpp"

namespace Facemc{

//! The inelastic photon scattering distribution class
class IncoherentPhotonScatteringDistribution : public PhotonScatteringDistribution
{

public:

  //! The electron momentum distribution array
  Teuchos::Array<Teuchos::RCP<Utility::OneDDistribution> >
  ElectronMomentumDistArray;

  //! Constructor 
  IncoherentPhotonScatteringDistribution( 
	      const Teuchos::ArrayView<const double>& recoil_electron_momentum,
	      const Tuechos::ArrayView<const double>& scattering_function,
	      const ElectronMomentumDistArray& electron_momentum_dist_array );

  //! Destructor
  ~IncoherntPhotonScatteringDistribution()
  { /* ... */ }

  //! Randomly scatter the photon
  void scatterPhoton( PhotonState& photon ) const;

  //! Randomly scatter the photon
  void scatterPhoton( PhotonState& photon,
		      unsigned& shell_of_interaction ) const;

private:

  // The scattering function
  Teuchos::Array<Utility::Pair<double,double> > d_scattering_function;

  // The electron momentum dist array
  // Note: Every electron shell should have a momentum distribution array
  ElectronMomentumDistArray d_electron_momentum_distribution;
}

} // end Facemc namespace

#endif // end FACEMC_INCOHERENT_PHOTON_SCATTERING_DISTRIBUTION_HPP

//---------------------------------------------------------------------------//
// end Facemc_IncoherentPhotonScatteringDistribution.hpp
//---------------------------------------------------------------------------//
