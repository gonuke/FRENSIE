//---------------------------------------------------------------------------//
//!
//! \file   tstBremsstrahlungPositronatomicReaction.cpp
//! \author Luke Kersting
//! \brief  Bremsstrahlung positron-atomic reaction unit tests
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>

// Trilinos Includes
#include <Teuchos_UnitTestHarness.hpp>
#include <Teuchos_VerboseObject.hpp>
#include <Teuchos_RCP.hpp>

// FRENSIE Includes
#include "MonteCarlo_BremsstrahlungPositronatomicReaction.hpp"
#include "MonteCarlo_BremsstrahlungElectronScatteringDistribution.hpp"
#include "MonteCarlo_BremsstrahlungAngularDistributionType.hpp"
#include "Data_ACEFileHandler.hpp"
#include "Data_XSSEPRDataExtractor.hpp"
#include "Utility_RandomNumberGenerator.hpp"
#include "Utility_HistogramDistribution.hpp"
#include "Utility_TabularDistribution.hpp"
#include "Utility_UnitTestHarnessExtensions.hpp"

//---------------------------------------------------------------------------//
// Testing Variables.
//---------------------------------------------------------------------------//

std::shared_ptr<MonteCarlo::BremsstrahlungPositronatomicReaction<Utility::LinLin> >
  ace_dipole_bremsstrahlung_reaction;

std::shared_ptr<MonteCarlo::BremsstrahlungPositronatomicReaction<Utility::LinLin> >
  ace_twobs_bremsstrahlung_reaction;

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
TEUCHOS_UNIT_TEST( BremsstrahlungPositronatomicReaction, getReactionType_ace )
{
  TEST_EQUALITY_CONST( ace_twobs_bremsstrahlung_reaction->getReactionType(),
               MonteCarlo::BREMSSTRAHLUNG_POSITRONATOMIC_REACTION );

  TEST_EQUALITY_CONST( ace_dipole_bremsstrahlung_reaction->getReactionType(),
               MonteCarlo::BREMSSTRAHLUNG_POSITRONATOMIC_REACTION );
}

//---------------------------------------------------------------------------//
// Check that the threshold energy can be returned
TEUCHOS_UNIT_TEST( BremsstrahlungPositronatomicReaction, getThresholdEnergy_ace )
{
  TEST_EQUALITY_CONST( ace_twobs_bremsstrahlung_reaction->getThresholdEnergy(),
                       1.000000000000E-05 );

  TEST_EQUALITY_CONST( ace_dipole_bremsstrahlung_reaction->getThresholdEnergy(),
                       1.000000000000E-05 );
}

//---------------------------------------------------------------------------//
// Check that the number of electrons emitted from the rxn can be returned
TEUCHOS_UNIT_TEST( BremsstrahlungPositronatomicReaction, getNumberOfEmittedElectrons_ace )
{
  TEST_EQUALITY_CONST( ace_twobs_bremsstrahlung_reaction->getNumberOfEmittedElectrons(1e-8),
                       0u );

  TEST_EQUALITY_CONST( ace_twobs_bremsstrahlung_reaction->getNumberOfEmittedElectrons(20.0),
                       0u );


  TEST_EQUALITY_CONST( ace_dipole_bremsstrahlung_reaction->getNumberOfEmittedElectrons(1e-8),
                       0u );

  TEST_EQUALITY_CONST( ace_dipole_bremsstrahlung_reaction->getNumberOfEmittedElectrons(20.0),
                       0u );
}

//---------------------------------------------------------------------------//
// Check that the number of photons emitted from the rxn can be returned
TEUCHOS_UNIT_TEST( BremsstrahlungPositronatomicReaction, getNumberOfEmittedPhotons_ace )
{
  TEST_EQUALITY_CONST( ace_twobs_bremsstrahlung_reaction->getNumberOfEmittedPhotons(1e-8),
                       0u );

  TEST_EQUALITY_CONST( ace_twobs_bremsstrahlung_reaction->getNumberOfEmittedPhotons(20.0),
                       1u );


  TEST_EQUALITY_CONST( ace_dipole_bremsstrahlung_reaction->getNumberOfEmittedPhotons(1e-8),
                       0u );

  TEST_EQUALITY_CONST( ace_dipole_bremsstrahlung_reaction->getNumberOfEmittedPhotons(20.0),
                       1u );
}

//---------------------------------------------------------------------------//
// Check that the cross section can be returned
TEUCHOS_UNIT_TEST( BremsstrahlungPositronatomicReaction, getCrossSection_ace )
{
  // 2BS
  double cross_section =
    ace_twobs_bremsstrahlung_reaction->getCrossSection( 9.000000000000E-05 );

  TEST_FLOATING_EQUALITY( cross_section, 7.249970966838E+03, 1e-12 );

  cross_section =
    ace_twobs_bremsstrahlung_reaction->getCrossSection( 4.000000000000E-04 );

  TEST_FLOATING_EQUALITY( cross_section, 8.914234996439E+03, 1e-12 );

  cross_section =
    ace_twobs_bremsstrahlung_reaction->getCrossSection( 2.000000000000E-03 );

  TEST_FLOATING_EQUALITY( cross_section, 9.258661418255E+03, 1e-12 );


  // Dipole
  cross_section =
    ace_dipole_bremsstrahlung_reaction->getCrossSection( 9.000000000000E-05 );

  TEST_FLOATING_EQUALITY( cross_section, 7.249970966838E+03, 1e-12 );

  cross_section =
    ace_dipole_bremsstrahlung_reaction->getCrossSection( 4.000000000000E-04 );

  TEST_FLOATING_EQUALITY( cross_section, 8.914234996439E+03, 1e-12 );

  cross_section =
    ace_dipole_bremsstrahlung_reaction->getCrossSection( 2.000000000000E-03 );

  TEST_FLOATING_EQUALITY( cross_section, 9.258661418255E+03, 1e-12 );
}

//---------------------------------------------------------------------------//
// Check that the differential cross section can be returned
TEUCHOS_UNIT_TEST( BremsstrahlungPositronatomicReaction, getDifferentialCrossSection_ace )
{
  // 2BS
  double diff_cross_section =
    ace_twobs_bremsstrahlung_reaction->getDifferentialCrossSection(
        1.0e-5,
        9.0e-6 );

  UTILITY_TEST_FLOATING_EQUALITY( diff_cross_section,
                                  8.859383971725880E+08,
                                  1e-12 );

  diff_cross_section =
    ace_twobs_bremsstrahlung_reaction->getDifferentialCrossSection(
        3.16228E-01,
        3.16115596E-01 );

  UTILITY_TEST_FLOATING_EQUALITY( diff_cross_section,
                                  9.504071722591320E+05,
                                  1e-12 );

  diff_cross_section =
    ace_twobs_bremsstrahlung_reaction->getDifferentialCrossSection(
        1.0e5,
        8.0E4 );

  UTILITY_TEST_FLOATING_EQUALITY( diff_cross_section,
                                  2.665370886148930E-03,
                                  1e-12 );


  // Dipole
  diff_cross_section =
    ace_dipole_bremsstrahlung_reaction->getDifferentialCrossSection(
        1.0e-5,
        9.0e-6 );

  UTILITY_TEST_FLOATING_EQUALITY( diff_cross_section,
                                  8.859383971725880E+08,
                                  1e-12 );

  diff_cross_section =
    ace_dipole_bremsstrahlung_reaction->getDifferentialCrossSection(
        3.16228E-01,
        3.16115596E-01 );

  UTILITY_TEST_FLOATING_EQUALITY( diff_cross_section,
                                  9.504071722591320E+05,
                                  1e-12 );

  diff_cross_section =
    ace_dipole_bremsstrahlung_reaction->getDifferentialCrossSection(
        1.0e5,
        8.0E4 );

  UTILITY_TEST_FLOATING_EQUALITY( diff_cross_section,
                                  2.665370886148930E-03,
                                  1e-12 );
}

//---------------------------------------------------------------------------//
// Check that the detailed 2BS bremsstrahlung reaction can be simulated
TEUCHOS_UNIT_TEST( BremsstrahlungPositronatomicReaction, react_detailed_2BS_ace )
{
  MonteCarlo::PositronState positron( 0 );
  positron.setEnergy( 20 );
  positron.setDirection( 0.0, 0.0, 1.0 );

  MonteCarlo::ParticleBank bank;

  Data::SubshellType shell_of_interaction;

  ace_twobs_bremsstrahlung_reaction->react( positron, bank, shell_of_interaction );

  TEST_ASSERT( positron.getEnergy() < 20.0 );
  TEST_EQUALITY_CONST( positron.getZDirection(), 1.0 );
  TEST_ASSERT( !bank.isEmpty() );
  TEST_EQUALITY_CONST( shell_of_interaction, Data::UNKNOWN_SUBSHELL );

  // Test the Bremsstrahlung photon
  TEST_EQUALITY_CONST( bank.top().getParticleType(), MonteCarlo::PHOTON );
  TEST_FLOATING_EQUALITY( bank.top().getEnergy(),
                          20.0 - positron.getEnergy(), 1e-12 );
  TEST_ASSERT( bank.top().getZDirection() < 1.0 );

  // Remove the banked photon
  bank.pop();
  TEST_ASSERT( bank.isEmpty() );

  // Set energy to get an annihilation
  positron.setEnergy( 1e-5 );

  ace_twobs_bremsstrahlung_reaction->react( positron, bank, shell_of_interaction );

  TEST_ASSERT( positron.getEnergy() < 1e-5 );
  TEST_FLOATING_EQUALITY( positron.getZDirection(), 1.0, 1e-12 );
  TEST_EQUALITY_CONST( shell_of_interaction, Data::UNKNOWN_SUBSHELL );
  TEST_ASSERT( !bank.isEmpty() );

  // Test the Bremsstrahlung photon
  TEST_EQUALITY_CONST( bank.top().getParticleType(), MonteCarlo::PHOTON );
  TEST_FLOATING_EQUALITY( bank.top().getEnergy(),
                          1e-5 - positron.getEnergy(), 1e-12 );
  TEST_ASSERT( bank.top().getZDirection() < 1.0 );

  bank.pop();

  // Test the first annihilation photon
  TEST_EQUALITY_CONST( bank.top().getParticleType(), MonteCarlo::PHOTON );
  TEST_FLOATING_EQUALITY( bank.top().getEnergy(),
                          positron.getRestMassEnergy(), 1e-12 );
  TEST_FLOATING_EQUALITY( bank.top().getZDirection(), 0.0, 1e-12 );
  double x_direction = bank.top().getXDirection();
  double y_direction = bank.top().getYDirection();

  bank.pop();

  // Test the second annihilation photon
  TEST_EQUALITY_CONST( bank.top().getParticleType(), MonteCarlo::PHOTON );
  TEST_FLOATING_EQUALITY( bank.top().getEnergy(),
                          positron.getRestMassEnergy(), 1e-12 );
  TEST_FLOATING_EQUALITY( bank.top().getZDirection(), 0.0, 1e-12 );
  TEST_FLOATING_EQUALITY( bank.top().getXDirection(), -1.0*x_direction, 1e-12 );
  TEST_FLOATING_EQUALITY( bank.top().getYDirection(), -1.0*y_direction, 1e-12 );
}

//---------------------------------------------------------------------------//
// Check that the basic dipole bremsstrahlung reaction can be simulated
TEUCHOS_UNIT_TEST( BremsstrahlungPositronatomicReaction, react_basic_ace )
{
  MonteCarlo::PositronState positron( 0 );
  positron.setEnergy( 20 );
  positron.setDirection( 0.0, 0.0, 1.0 );

  MonteCarlo::ParticleBank bank;

  Data::SubshellType shell_of_interaction;

  ace_dipole_bremsstrahlung_reaction->react( positron, bank, shell_of_interaction );

  TEST_ASSERT( positron.getEnergy() < 20.0 );
  TEST_EQUALITY_CONST( positron.getZDirection(), 1.0 );
  TEST_ASSERT( !bank.isEmpty() );
  TEST_EQUALITY_CONST( shell_of_interaction, Data::UNKNOWN_SUBSHELL );

  // Test the Bremsstrahlung photon
  TEST_EQUALITY_CONST( bank.top().getParticleType(), MonteCarlo::PHOTON );
  TEST_FLOATING_EQUALITY( bank.top().getEnergy(),
                          20.0 - positron.getEnergy(), 1e-9 );
  TEST_ASSERT( bank.top().getZDirection() < 1.0 );

  // Remove the banked photon
  bank.pop();
  TEST_ASSERT( bank.isEmpty() );

  // Set energy to get an annihilation
  positron.setEnergy( 1e-5 );

  ace_dipole_bremsstrahlung_reaction->react( positron, bank, shell_of_interaction );

  TEST_ASSERT( positron.getEnergy() < 1e-5 );
  TEST_FLOATING_EQUALITY( positron.getZDirection(), 1.0, 1e-12 );
  TEST_EQUALITY_CONST( shell_of_interaction, Data::UNKNOWN_SUBSHELL );
  TEST_ASSERT( !bank.isEmpty() );

  // Test the Bremsstrahlung photon
  TEST_EQUALITY_CONST( bank.top().getParticleType(), MonteCarlo::PHOTON );
  TEST_FLOATING_EQUALITY( bank.top().getEnergy(),
                          1e-5 - positron.getEnergy(), 1e-12 );
  TEST_ASSERT( bank.top().getZDirection() < 1.0 );

  bank.pop();

  // Test the first annihilation photon
  TEST_EQUALITY_CONST( bank.top().getParticleType(), MonteCarlo::PHOTON );
  TEST_FLOATING_EQUALITY( bank.top().getEnergy(),
                          positron.getRestMassEnergy(), 1e-12 );
  TEST_FLOATING_EQUALITY( bank.top().getZDirection(), 0.0, 1e-12 );
  double x_direction = bank.top().getXDirection();
  double y_direction = bank.top().getYDirection();

  bank.pop();

  // Test the second annihilation photon
  TEST_EQUALITY_CONST( bank.top().getParticleType(), MonteCarlo::PHOTON );
  TEST_FLOATING_EQUALITY( bank.top().getEnergy(),
                          positron.getRestMassEnergy(), 1e-12 );
  TEST_FLOATING_EQUALITY( bank.top().getZDirection(), 0.0, 1e-12 );
  TEST_FLOATING_EQUALITY( bank.top().getXDirection(), -1.0*x_direction, 1e-12 );
  TEST_FLOATING_EQUALITY( bank.top().getYDirection(), -1.0*y_direction, 1e-12 );
}

//---------------------------------------------------------------------------//
// Custom setup
//---------------------------------------------------------------------------//
UTILITY_CUSTOM_TEUCHOS_UNIT_TEST_SETUP_BEGIN();

std::string test_ace_file_name, test_ace_table_name;

UTILITY_CUSTOM_TEUCHOS_UNIT_TEST_COMMAND_LINE_OPTIONS()
{
  clp().setOption( "test_ace_file",
                    &test_ace_file_name,
                    "Test ACE file name" );
  clp().setOption( "test_ace_table",
                    &test_ace_table_name,
                    "Test ACE table name" );
}

UTILITY_CUSTOM_TEUCHOS_UNIT_TEST_DATA_INITIALIZATION()
{
  // Create a file handler and data extractor
  Teuchos::RCP<Data::ACEFileHandler> ace_file_handler(
                 new Data::ACEFileHandler( test_ace_file_name,
                               test_ace_table_name,
                               1u ) );
  Teuchos::RCP<Data::XSSEPRDataExtractor> xss_data_extractor(
                            new Data::XSSEPRDataExtractor(
                      ace_file_handler->getTableNXSArray(),
                      ace_file_handler->getTableJXSArray(),
                      ace_file_handler->getTableXSSArray() ) );

  // Extract the energy grid and cross section
  Teuchos::ArrayRCP<double> energy_grid;
  energy_grid.deepCopy( xss_data_extractor->extractElectronEnergyGrid() );

  Teuchos::ArrayView<const double> raw_bremsstrahlung_cross_section =
    xss_data_extractor->extractBremsstrahlungCrossSection();

  Teuchos::ArrayView<const double>::iterator start =
    std::find_if( raw_bremsstrahlung_cross_section.begin(),
                  raw_bremsstrahlung_cross_section.end(),
                  notEqualZero );

  Teuchos::ArrayRCP<double> bremsstrahlung_cross_section;
  bremsstrahlung_cross_section.assign( start, raw_bremsstrahlung_cross_section.end() );

  unsigned bremsstrahlung_threshold_index =
    energy_grid.size() - bremsstrahlung_cross_section.size();

  // Extract the elastic scattering information data block (BREMI)
  Teuchos::ArrayView<const double> bremi_block(
                      xss_data_extractor->extractBREMIBlock() );

  // Extract the number of tabulated distributions
  int N = bremi_block.size()/3;

  // Extract the electron energy grid for bremsstrahlung energy distributions
  Teuchos::Array<double>bremsstrahlung_energy_grid(bremi_block(0,N));

  // Extract the table lengths for bremsstrahlung energy distributions
  Teuchos::Array<double> table_length(bremi_block(N,N));

  // Extract the offsets for bremsstrahlung energy distributions
  Teuchos::Array<double> offset(bremi_block(2*N,N));

  // Extract the bremsstrahlung photon energy distributions block (BREME)
  Teuchos::ArrayView<const double> breme_block =
    xss_data_extractor->extractBREMEBlock();

  // Create the bremsstrahlung scattering distributions
  Utility::FullyTabularTwoDDistribution::DistributionType function_data( N );

  for( unsigned n = 0; n < N; ++n )
  {
    function_data[n].first = bremsstrahlung_energy_grid[n];

    function_data[n].second.reset(
      new Utility::HistogramDistribution(
         breme_block( offset[n], table_length[n] ),
         breme_block( offset[n] + 1 + table_length[n], table_length[n]-1 ),
         true ) );
  }

  // Create the scattering function
  std::shared_ptr<Utility::FullyTabularTwoDDistribution> scattering_function(
    new Utility::InterpolatedFullyTabularTwoDDistribution<Utility::LinLinLin,Utility::Correlated>(
            function_data ) );

  std::shared_ptr<const MonteCarlo::BremsstrahlungElectronScatteringDistribution>
        dipole_scattering_distribution, twobs_scattering_distribution;

  // Create the distributions
  dipole_scattering_distribution.reset(
   new MonteCarlo::BremsstrahlungElectronScatteringDistribution(
        scattering_function ) );

  twobs_scattering_distribution.reset(
   new MonteCarlo::BremsstrahlungElectronScatteringDistribution(
        xss_data_extractor->extractAtomicNumber(),
        scattering_function ) );

  double min_electron_energy = 1e-5;

  // Create the reactions
  ace_dipole_bremsstrahlung_reaction.reset(
    new MonteCarlo::BremsstrahlungPositronatomicReaction<Utility::LinLin>(
                              energy_grid,
                              bremsstrahlung_cross_section,
                              bremsstrahlung_threshold_index,
                              dipole_scattering_distribution,
                              min_electron_energy ) );

  ace_twobs_bremsstrahlung_reaction.reset(
        new MonteCarlo::BremsstrahlungPositronatomicReaction<Utility::LinLin>(
                              energy_grid,
                              bremsstrahlung_cross_section,
                              bremsstrahlung_threshold_index,
                              twobs_scattering_distribution,
                              min_electron_energy ) );

  // Clear setup data
  ace_file_handler.reset();
  xss_data_extractor.reset();

  // Initialize the random number generator
  Utility::RandomNumberGenerator::createStreams();
}

UTILITY_CUSTOM_TEUCHOS_UNIT_TEST_SETUP_END();

//---------------------------------------------------------------------------//
// end tstBremsstrahlungPositronatomicReaction.cpp
//---------------------------------------------------------------------------//