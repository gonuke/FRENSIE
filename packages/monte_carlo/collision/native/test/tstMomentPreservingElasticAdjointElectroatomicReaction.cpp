//---------------------------------------------------------------------------//
//!
//! \file   tstMomentPreservingElasticAdjointElectroatomicReaction.cpp
//! \author Luke Kersting
//! \brief  Moment preserving Elastic adjoint electroatomic reaction unit tests
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
#include "Data_AdjointElectronPhotonRelaxationDataContainer.hpp"
#include "MonteCarlo_MomentPreservingElasticAdjointElectroatomicReaction.hpp"
#include "MonteCarlo_ElasticElectronScatteringDistributionNativeFactory.hpp"
#include "Utility_RandomNumberGenerator.hpp"
#include "Utility_UnitTestHarnessExtensions.hpp"
#include "Utility_DiscreteDistribution.hpp"

typedef MonteCarlo::ElasticElectronScatteringDistributionNativeFactory 
    NativeFactory;


//---------------------------------------------------------------------------//
// Testing Variables.
//---------------------------------------------------------------------------//

Teuchos::RCP<MonteCarlo::AdjointElectroatomicReaction> mp_elastic_reaction;

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
TEUCHOS_UNIT_TEST( MomentPreservingElasticAdjointElectroatomicReaction,
                   getReactionType )
{
  TEST_EQUALITY_CONST( mp_elastic_reaction->getReactionType(),
		       MonteCarlo::MOMENT_PRESERVING_ELASTIC_ADJOINT_ELECTROATOMIC_REACTION );
}

//---------------------------------------------------------------------------//
// Check that the threshold energy can be returned
TEUCHOS_UNIT_TEST( MomentPreservingElasticAdjointElectroatomicReaction,
                   getThresholdEnergy )
{
  TEST_EQUALITY_CONST( mp_elastic_reaction->getThresholdEnergy(),
                       1e-5 );
}

//---------------------------------------------------------------------------//
// Check that the number of electrons emitted from the rxn can be returned
TEUCHOS_UNIT_TEST( MomentPreservingElasticAdjointElectroatomicReaction,
                   getNumberOfEmittedElectrons )
{
  TEST_EQUALITY_CONST( mp_elastic_reaction->getNumberOfEmittedElectrons(1e-3),
                       0u );

  TEST_EQUALITY_CONST( mp_elastic_reaction->getNumberOfEmittedElectrons(20.0),
                       0u );
}

//---------------------------------------------------------------------------//
// Check that the number of photons emitted from the rxn can be returned
TEUCHOS_UNIT_TEST( MomentPreservingElasticAdjointElectroatomicReaction,
                   getNumberOfEmittedPhotons )
{
  TEST_EQUALITY_CONST( mp_elastic_reaction->getNumberOfEmittedPhotons(1e-3),
                       0u );

  TEST_EQUALITY_CONST( mp_elastic_reaction->getNumberOfEmittedPhotons(20.0),
                       0u );
}

//---------------------------------------------------------------------------//
// Check that the moment preserving cross section can be returned
TEUCHOS_UNIT_TEST( MomentPreservingElasticAdjointElectroatomicReaction,
                   getCrossSection )
{
  double cross_section = mp_elastic_reaction->getCrossSection( 1.0E-05 );
  TEST_FLOATING_EQUALITY( cross_section, 1.2217606103336416e+07, 1e-12 );

  cross_section = mp_elastic_reaction->getCrossSection( 1.0E-03 );
  TEST_FLOATING_EQUALITY( cross_section, 1.6718090775280627e+06, 1e-12 );

  cross_section = mp_elastic_reaction->getCrossSection( 20.0 );
  TEST_FLOATING_EQUALITY( cross_section, 2.0498802209908908, 1e-12 );
}


//---------------------------------------------------------------------------//
// Check that the elastic reaction can be simulated
TEUCHOS_UNIT_TEST( MomentPreservingElasticAdjointElectroatomicReaction, react )
{
  MonteCarlo::AdjointElectronState electron( 0 );
  electron.setEnergy( 20.0 );
  electron.setDirection( 0.0, 0.0, 1.0 );

  MonteCarlo::ParticleBank bank;

  Data::SubshellType shell_of_interaction;

  mp_elastic_reaction->react( electron, bank, shell_of_interaction );

  TEST_EQUALITY_CONST( electron.getEnergy(), 20.0 );
  TEST_ASSERT( electron.getZDirection() < 1.0 );
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
  // Get native data container
  Data::AdjointElectronPhotonRelaxationDataContainer data_container =
        Data::AdjointElectronPhotonRelaxationDataContainer( test_native_file_name );

  // Extract the energy grid and cross section
  Teuchos::ArrayRCP<double> energy_grid;
  energy_grid.assign(
        data_container.getAdjointElectronEnergyGrid().begin(),
        data_container.getAdjointElectronEnergyGrid().end() );

  double cutoff_angle_cosine = 0.9;
  double evaluation_tol = 1e-15;

  // Moment preserving elastic cross section
  std::vector<double> moment_preserving_cross_sections;
  unsigned threshold_index;
  NativeFactory::calculateMomentPreservingCrossSections<Utility::LogLogCosLog,Utility::Exact>(
                                moment_preserving_cross_sections,
                                threshold_index,
                                data_container,
                                energy_grid,
                                evaluation_tol );

  Teuchos::ArrayRCP<double> cross_section;
  cross_section.assign(
    moment_preserving_cross_sections.begin(),
    moment_preserving_cross_sections.end() );

  // Create the distribution
  std::shared_ptr<const MonteCarlo::MomentPreservingElasticElectronScatteringDistribution>
        discrete_elastic_distribution;

  NativeFactory::createMomentPreservingElasticDistribution<Utility::LogLogCosLog,Utility::Exact>(
        discrete_elastic_distribution,
        data_container,
        cutoff_angle_cosine,
        evaluation_tol );

  // Create the reaction
  mp_elastic_reaction.reset(
      new MonteCarlo::MomentPreservingElasticAdjointElectroatomicReaction<Utility::LinLin>(
                energy_grid,
                cross_section,
                threshold_index,
                discrete_elastic_distribution ) );

  // Initialize the random number generator
  Utility::RandomNumberGenerator::createStreams();
}

UTILITY_CUSTOM_TEUCHOS_UNIT_TEST_SETUP_END();

//---------------------------------------------------------------------------//
// end tstCutoffElasticAdjointElectroatomicReaction.cpp
//---------------------------------------------------------------------------//
