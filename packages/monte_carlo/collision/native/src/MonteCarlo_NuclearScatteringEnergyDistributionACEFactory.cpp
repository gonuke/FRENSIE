//---------------------------------------------------------------------------//
//! 
//! \file   MonteCarlo_NuclearScatteringEnergyDistributionACEFactory.cpp
//! \author Alex Robinson, Alex Bennett
//! \brief  Nuclear scattering energy distribution factory class declaration
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <stdexcept>

// Trilinos Includes
#include <Teuchos_ArrayView.hpp>
#include <Teuchos_Array.hpp>

// FRENSIE Includes
#include "MonteCarlo_NuclearScatteringEnergyDistributionACEFactory.hpp"
#include "MonteCarlo_NuclearScatteringAngularDistributionACEFactory.hpp"
#include "MonteCarlo_AceLaw1NuclearScatteringEnergyDistribution.hpp"
#include "MonteCarlo_AceLaw3NuclearScatteringEnergyDistribution.hpp"
#include "Utility_HistogramDistribution.hpp"
#include "Utility_TabularDistribution.hpp"

namespace MonteCarlo{

// Create the energy distribution
void NuclearScatteringEnergyDistributionACEFactory::createDistribution(
	     const Teuchos::ArrayView<const double>& dlw_block_array,
	     const unsigned dlw_block_array_start_index,
	     const std::string& table_name,
	     const unsigned reaction,
	     Teuchos::RCP<NuclearScatteringEnergyDistribution>& distribution )
{
  // Make sure the dlw block array is valid
  testPrecondition( dlw_block_array.size() > 0 );

  // Verify that only one law is present
  TEST_FOR_EXCEPTION( dlw_block_array[0] != 0,
		      std::runtime_error,
		      "Error: MT# " << reaction << "in ACE table "
		      << table_name << " has multiple ENDF laws associated "
		      " with it, which is not currently supported!\n" );

  // Extract the ENDF law number
  unsigned endf_law = dlw_block_array[1];

  // Create the energy distribution based on the stated ENDF law
  switch( endf_law )
  {
  case 1u:
    createAceLaw1EnergyDistribution( dlw_block_array,
				     dlw_block_array_start_index,
				     table_name,
				     reaction,
				     distribution );
    break;

  case 3u:
    createAceLaw3EnergyDistribution( dlw_block_array,
				     dlw_block_array_start_index,
				     table_name,
				     reaction,
				     distribution );
     break;

  case 4u:
    createAceLaw4EnergyDistribution( dlw_block_array,
				     dlw_block_array_start_index,
				     table_name,
				     reaction,
				     distribution );
    break;
    
  default:
    // This law is not currently supported
    TEST_FOR_EXCEPTION( true,
			std::runtime_error,
			"Error: Unsupported ENDF law number " << endf_law <<
			" found in ACE table " << table_name << " for MT# "
			<< reaction << "!\n" );
  }
}

// Create a AceLaw 1 energy distribution
void NuclearScatteringEnergyDistributionACEFactory::createAceLaw1EnergyDistribution( 
	     const Teuchos::ArrayView<const double>& dlw_block_array,
	     const unsigned dlw_block_array_start_index,
	     const std::string& table_name,
	     const unsigned reaction,
	     Teuchos::RCP<NuclearScatteringEnergyDistribution>& distribution )
{
  // Start index for ldat data
    int ldat_start_index = dlw_block_array[2] - dlw_block_array_start_index - 2;

 
    // Verify that there isn't multiple interpolation regions
    TEST_FOR_EXCEPTION( dlw_block_array[ldat_start_index + 1] != 0,
                        std::runtime_error,
                        "Error: MT# " << reaction << " in ACE table "
                        << table_name << " has multiple interpolation regions "
                        " with it, which is not currently supported in AceLaw 1!\n" );
  
    // Number of incident energies
    int incoming_energies = dlw_block_array[ldat_start_index + 2];

    // Initialize the energy grid
    AceLaw1NuclearScatteringEnergyDistribution::EnergyDistArray energy_grid;

    energy_grid.resize(incoming_energies);

    // Loop through the incident energies
    for(int i = 0; i != incoming_energies; i++)
    {
      energy_grid[i].first = dlw_block_array[ldat_start_index + 3 + i];
    }

    // Number of outgoing energies
    double outgoing_energies = dlw_block_array[ldat_start_index + 3 + incoming_energies];

    // Loop through the incoming energies
    for(int i = 0; i != incoming_energies; i++)
    {
      energy_grid[i].second.resize(outgoing_energies,0);
      
      // Loop through the outgoing energies
      for(int j = 0; i != outgoing_energies; j++)
      {
        energy_grid[i].second[j] = dlw_block_array[ldat_start_index + 4 + incoming_energies +
                                        i * outgoing_energies + j];
      }
    }

    // Create the equiprobable bin scattering energy distriubtion (law 1)
    distribution.reset( 
	       new AceLaw1NuclearScatteringEnergyDistribution( energy_grid ) );
}

// Create a AceLaw 3 energy distribution
void NuclearScatteringEnergyDistributionACEFactory::createAceLaw3EnergyDistribution(
	     const Teuchos::ArrayView<const double>& dlw_block_array,
	     const unsigned dlw_block_array_start_index,
	     const std::string& table_name,
	     const unsigned reaction,
	     Teuchos::RCP<NuclearScatteringEnergyDistribution>& distribution )
{
  // Verify that there isn't multiple interpolation regions
  TEST_FOR_EXCEPTION( dlw_block_array[3] != 0,
		      std::runtime_error,
		      "Error: MT# " << reaction << "in ACE table "
		      << table_name << " has multiple interpolation regions "
		      " with it, which is not possible in LAW 3!\n" );

  // Parse data
  double ldat1 = 
    dlw_block_array[dlw_block_array[2] - dlw_block_array_start_index - 1];
  double ldat2 = 
    dlw_block_array[dlw_block_array[2] - dlw_block_array_start_index];

  // Create the inelastic level scattering energy distribution (law 3)
  distribution.reset( 
	      new AceLaw3NuclearScatteringEnergyDistribution( ldat1, ldat2 ) );
}

// Create a AceLaw 4 energy distribution
void NuclearScatteringEnergyDistributionACEFactory::createAceLaw4EnergyDistribution(
	     const Teuchos::ArrayView<const double>& dlw_block_array,
	     const unsigned dlw_block_array_start_index,
	     const std::string& table_name,
	     const unsigned reaction,
	     Teuchos::RCP<NuclearScatteringEnergyDistribution>& distribution )
{
  // Verify that it is law 4
     TEST_FOR_EXCEPTION( dlw_block_array[1] != 4,
           	      std::runtime_error,
           	      "Error: MT# " << reaction << " in ACE table "
           	      << table_name << " should be law 4!\n" );

     // Start index for ldat data
     int ldat_start_index = (int)dlw_block_array[2] - dlw_block_array_start_index - 1;

     // Verify that only one law is present
     TEST_FOR_EXCEPTION( dlw_block_array[ldat_start_index] != 0,
           	      std::runtime_error,
           	      "Error: MT# " << reaction << " in ACE table "
           	      << table_name << " has multiple interpolation schemes "
           	      " with it, which is not currently supported!\n" );

     // Number of incident energies
     double incoming_energies = dlw_block_array[ldat_start_index + 1];

     // Array of incoming energies
     Teuchos::Array<double> incoming_energies_array = dlw_block_array( ldat_start_index + 2,
                                                                       incoming_energies);

     // Array of distribution locations
     Teuchos::Array<double> distribution_locations = dlw_block_array(ldat_start_index + 2 + incoming_energies,
                                                                  incoming_energies); 

     // Initialize the energy distribution array
     AceLaw4NuclearScatteringEnergyDistribution::EnergyDistribution 
       energy_distribution( incoming_energies );

     // Loop through the incoming energies
     for(int i = 0; i != incoming_energies; i++)
     {
       energy_distribution[i].first = incoming_energies_array[i];

       int distribution_index = static_cast<int>( distribution_locations[i] ) - dlw_block_array_start_index - 1;

       int interpolation_flag = dlw_block_array[distribution_index];

       // Check if discrete lines are present 
       TEST_FOR_EXCEPTION( interpolation_flag > 10,
           	        std::runtime_error,
           	        "Error: MT# " << reaction << "in ACE table "
           	        << table_name << " has discrete lines in continuous"
           	        " tabular data, which is not currently supported!\n" );

       int number_points_distribution = dlw_block_array[distribution_index + 1];

       Teuchos::ArrayView<const double> outgoing_energy_grid = 
         dlw_block_array( distribution_index + 2, number_points_distribution );

       Teuchos::ArrayView<const double> pdf;

       switch( interpolation_flag )
       {
       case 1: // histogram interpolation
         pdf = dlw_block_array( distribution_index +2+ number_points_distribution,
           		     number_points_distribution - 1 );

         energy_distribution[i].second.reset( 
           	      new Utility::HistogramDistribution( outgoing_energy_grid,
           						  pdf ) );

         break;
 
       case 2: // Linear-Linear interpolation
         pdf = dlw_block_array( distribution_index +2+ number_points_distribution,
           		     number_points_distribution );

         energy_distribution[i].second.reset( 
           		     new Utility::TabularDistribution<Utility::LinLin>(
           					 outgoing_energy_grid, pdf ) );

         break;
 
       default:
         THROW_EXCEPTION( std::runtime_error,
			  "Unknown interpolation flag in table "
			  << table_name << 
			  " for energy distribution of MT = "
			  << reaction << ": "
			  << interpolation_flag << "\n" );
       } 
     }

     distribution.reset( 
      new AceLaw4NuclearScatteringEnergyDistribution( energy_distribution ) );
}

} // end MonteCarlo namespace

//---------------------------------------------------------------------------//
// end MonteCarlo_NuclearScatteringEnergyDistributionACEFactory.cpp
//---------------------------------------------------------------------------//
