//---------------------------------------------------------------------------//
//!
//! \file   tstHybridElasticElectroatomicReaction.cpp
//! \author Luke Kersting
//! \brief  Hybrid Elastic electroatomic reaction unit tests
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>

// Trilinos Includes
#include <Teuchos_UnitTestHarness.hpp>
#include <Teuchos_VerboseObject.hpp>
#include <Teuchos_RCP.hpp>
#include <Teuchos_ArrayRCP.hpp>

// FRENSIE Includes
#include "Data_ElectronPhotonRelaxationDataContainer.hpp"
#include "MonteCarlo_HybridElasticElectroatomicReaction.hpp"
#include "MonteCarlo_HybridElasticElectronScatteringDistribution.hpp"
#include "MonteCarlo_ElasticElectronScatteringDistributionNativeFactory.hpp"
#include "Utility_RandomNumberGenerator.hpp"
#include "Utility_HistogramDistribution.hpp"
#include "Utility_UnitTestHarnessExtensions.hpp"
#include "Utility_TabularDistribution.hpp"

//---------------------------------------------------------------------------//
// Testing Variables.
//---------------------------------------------------------------------------//

  typedef MonteCarlo::ElasticElectronScatteringDistributionNativeFactory::TwoDFunction
            TwoDFunction;

  typedef MonteCarlo::ElasticElectronScatteringDistributionNativeFactory::TwoDDist
            TwoDDist;

  Teuchos::RCP<MonteCarlo::HybridElasticElectroatomicReaction<Utility::LinLinLog,Utility::LinLin> >
    hybrid_elastic_reaction;

//---------------------------------------------------------------------------//
// Testing Functions.
//---------------------------------------------------------------------------//
bool notEqualZero( double value )
{
  return value != 0.0;
}

//---------------------------------------------------------------------------//
// Tests
//---------------------------------------------------------------------------//
// Check that the reaction type can be returned
TEUCHOS_UNIT_TEST( HybridElasticElectroatomicReaction, getReactionType )
{
  TEST_EQUALITY_CONST( hybrid_elastic_reaction->getReactionType(),
		       MonteCarlo::HYBRID_ELASTIC_ELECTROATOMIC_REACTION );
}

//---------------------------------------------------------------------------//
// Check that the threshold energy can be returned
TEUCHOS_UNIT_TEST( HybridElasticElectroatomicReaction, getThresholdEnergy )
{
  TEST_EQUALITY_CONST( hybrid_elastic_reaction->getThresholdEnergy(),
                       1.0e-5 );
}

//---------------------------------------------------------------------------//
// Check that the number of electrons emitted from the rxn can be returned
TEUCHOS_UNIT_TEST( HybridElasticElectroatomicReaction, getNumberOfEmittedElectrons )
{
  TEST_EQUALITY_CONST( hybrid_elastic_reaction->getNumberOfEmittedElectrons(1e-3),
		       0u );

  TEST_EQUALITY_CONST( hybrid_elastic_reaction->getNumberOfEmittedElectrons(20.0),
		       0u );
}

//---------------------------------------------------------------------------//
// Check that the number of photons emitted from the rxn can be returned
TEUCHOS_UNIT_TEST( HybridElasticElectroatomicReaction, getNumberOfEmittedPhotons )
{
  TEST_EQUALITY_CONST( hybrid_elastic_reaction->getNumberOfEmittedPhotons(1e-3),
		       0u );

  TEST_EQUALITY_CONST( hybrid_elastic_reaction->getNumberOfEmittedPhotons(20.0),
		       0u );
}


//---------------------------------------------------------------------------//
// Check that the hybrid cross section can be returned
TEUCHOS_UNIT_TEST( HybridElasticElectroatomicReaction,
                   getCrossSection )
{

  double cross_section = hybrid_elastic_reaction->getCrossSection( 1.0E-05 );
  TEST_FLOATING_EQUALITY( cross_section, 3444568722.2843613625 + 1.611494138359350E+08, 1e-12 );

  cross_section = hybrid_elastic_reaction->getCrossSection( 1.0E-03 );
  TEST_FLOATING_EQUALITY( cross_section, 18557880.33652209118 + 5.730253976136980E+07, 1e-12 );

  cross_section = hybrid_elastic_reaction->getCrossSection( 1.0E+05 );
  TEST_FLOATING_EQUALITY( cross_section, 1.9264754607947520206e-08 + 6.808061009771560E-05, 1e-12 );
}

//---------------------------------------------------------------------------//
// Check that the elastic reaction can be simulated
TEUCHOS_UNIT_TEST( HybridElasticElectroatomicReaction, react )
{
  MonteCarlo::ElectronState electron( 0 );
  electron.setEnergy( 20.0 );
  electron.setDirection( 0.0, 0.0, 1.0 );

  MonteCarlo::ParticleBank bank;

  Data::SubshellType shell_of_interaction;

  hybrid_elastic_reaction->react( electron, bank, shell_of_interaction );

  TEST_EQUALITY_CONST( electron.getEnergy(), 20.0 );
  TEST_ASSERT( electron.getZDirection() < 2.0 );
  TEST_ASSERT( electron.getZDirection() > 0.0 );
  TEST_ASSERT( bank.isEmpty() );
  TEST_EQUALITY_CONST( shell_of_interaction, Data::UNKNOWN_SUBSHELL );
}

//---------------------------------------------------------------------------//
// Custom setup
//---------------------------------------------------------------------------//
UTILITY_CUSTOM_TEUCHOS_UNIT_TEST_SETUP_BEGIN();

std::string test_native_file_name;

UTILITY_CUSTOM_TEUCHOS_UNIT_TEST_COMMAND_LINE_OPTIONS()
{
  clp().setOption( "test_native_file",
                   &test_native_file_name,
                   "Test Native file name" );
}

UTILITY_CUSTOM_TEUCHOS_UNIT_TEST_DATA_INITIALIZATION()
{
  double evaluation_tol = 1e-7;

  // Get native data container
  Data::ElectronPhotonRelaxationDataContainer data_container =
        Data::ElectronPhotonRelaxationDataContainer( test_native_file_name );

  Teuchos::ArrayRCP<double> energy_grid;
  energy_grid.assign(
        data_container.getElectronEnergyGrid().begin(),
        data_container.getElectronEnergyGrid().end() );

  // Construct the grid searcher
  Teuchos::RCP<Utility::HashBasedGridSearcher> grid_searcher(
    new Utility::StandardHashBasedGridSearcher<Teuchos::ArrayRCP<const double>,false>(
                    energy_grid,
                    energy_grid[0],
                    energy_grid[energy_grid.size()-1],
                    energy_grid.size()/10+1 ) );

  Teuchos::ArrayRCP<double> cutoff_cross_section;
  cutoff_cross_section.assign(
        data_container.getCutoffElasticCrossSection().begin(),
        data_container.getCutoffElasticCrossSection().end() );

  // Reduced cutoff elastic cross section ratio
  std::vector<double> reduced_cutoff_ratio =
    data_container.getReducedCutoffCrossSectionRatios();

  Teuchos::ArrayRCP<double> mp_cross_section;
  mp_cross_section.assign(
        data_container.getMomentPreservingCrossSection().begin(),
        data_container.getMomentPreservingCrossSection().end() );

  // Calculate the hybrid cross section
  unsigned hybrid_threshold_energy_index =
    std::min( data_container.getMomentPreservingCrossSectionThresholdEnergyIndex(),
              data_container.getCutoffElasticCrossSectionThresholdEnergyIndex() );

  unsigned mp_threshold_diff =
    data_container.getMomentPreservingCrossSectionThresholdEnergyIndex() -
    hybrid_threshold_energy_index;
  unsigned cutoff_threshold_diff =
    data_container.getCutoffElasticCrossSectionThresholdEnergyIndex() -
    hybrid_threshold_energy_index;

  Teuchos::Array<double> combined_cross_section(
                           energy_grid.size() - hybrid_threshold_energy_index );

  for (unsigned i = 0; i < combined_cross_section.size(); ++i )
  {
    double energy = energy_grid[i + hybrid_threshold_energy_index];

    if ( i < mp_threshold_diff )
    {
      combined_cross_section[i] = cutoff_cross_section[i]*reduced_cutoff_ratio[i];
    }
    else if ( i < cutoff_threshold_diff )
    {
      combined_cross_section[i] = mp_cross_section[i];
    }
    else
    {
      combined_cross_section[i] =
        cutoff_cross_section[i-cutoff_threshold_diff]*reduced_cutoff_ratio[i] +
        mp_cross_section[i-mp_threshold_diff];
    }
  }

  Teuchos::ArrayRCP<double> hybrid_cross_section;
  hybrid_cross_section.assign( combined_cross_section.begin(),
                               combined_cross_section.end() );

  // Create hybrid distribution
  std::shared_ptr<const MonteCarlo::HybridElasticElectronScatteringDistribution>
        hybrid_elastic_distribution;

  MonteCarlo::ElasticElectronScatteringDistributionNativeFactory::createHybridElasticDistribution<Utility::LinLinLog>(
        hybrid_elastic_distribution,
        grid_searcher,
        energy_grid,
        cutoff_cross_section,
        mp_cross_section,
        data_container,
        data_container.getCutoffAngleCosine(),
        evaluation_tol );

  // Create the reaction
  hybrid_elastic_reaction.reset(
    new MonteCarlo::HybridElasticElectroatomicReaction<Utility::LinLinLog,Utility::LinLin>(
            energy_grid,
            hybrid_cross_section,
            hybrid_threshold_energy_index,
            data_container.getCutoffAngleCosine(),
            hybrid_elastic_distribution ) );

  // Initialize the random number generator
  Utility::RandomNumberGenerator::createStreams();
}

UTILITY_CUSTOM_TEUCHOS_UNIT_TEST_SETUP_END();

//---------------------------------------------------------------------------//
// end tstHybridElasticElectroatomicReaction.cpp
//---------------------------------------------------------------------------//