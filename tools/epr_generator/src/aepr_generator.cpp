//---------------------------------------------------------------------------//
//!
//! \file   aepr_generator.cpp
//! \author Alex Robinson
//! \brief  adjoint epr_generator tool
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>
#include <sstream>
#include <cstdio>

// Trilinos Includes
#include <Teuchos_ParameterList.hpp>
#include <Teuchos_XMLParameterListCoreHelpers.hpp>
#include <Teuchos_CommandLineProcessor.hpp>
#include <Teuchos_FancyOStream.hpp>
#include <Teuchos_VerboseObject.hpp>

// FRENSIE Includes
#include "MonteCarlo_TwoDInterpolationType.hpp"
#include "DataGen_StandardElectronPhotonRelaxationDataGenerator.hpp"
#include "DataGen_StandardAdjointElectronPhotonRelaxationDataGenerator.hpp"
#include "Data_CrossSectionsXMLProperties.hpp"
#include "Data_XSSEPRDataExtractor.hpp"
#include "Data_ElectronPhotonRelaxationDataContainer.hpp"
#include "Data_AdjointElectronPhotonRelaxationDataContainer.hpp"
#include "Utility_PhysicalConstants.hpp"
#include "Utility_StaticOutputFormatter.hpp"
#include "Utility_ExceptionCatchMacros.hpp"

int main( int argc, char** argv )
{
  std::shared_ptr<const DataGen::AdjointElectronPhotonRelaxationDataGenerator>
    aepr_generator;

  Teuchos::RCP<Teuchos::FancyOStream> out =
    Teuchos::VerboseObjectBase::getDefaultOStream();

  // Set up the command line options
  Teuchos::CommandLineProcessor aepr_generator_clp;

  // General table options
  std::string forward_file_name;
  std::string cross_section_directory, cross_section_alias;
  std::string subdirectory_name = "native";
  std::string table_notes;
  bool modify_cs_xml_file;

  // Table energy limits options
  double min_photon_energy = 1e-3, max_photon_energy = 20.0;
  double min_electron_energy = 1e-5, max_electron_energy = 1e5;

  // Adjoint photon options
  double adjoint_pp_energy_dist_norm_const_eval_tol = 1e-3;
  double adjoint_pp_energy_dist_norm_const_nudge_value = 1e-6;
  double adjoint_tp_energy_dist_norm_const_eval_tol = 1e-3;
  double adjoint_tp_energy_dist_norm_const_nudge_value = 1e-6;
  double adjoint_incoherent_max_energy_nudge_value = 0.2;
  double adjoint_incoherent_energy_to_max_energy_nudge_value = 1e-6;
  double adjoint_incoherent_evaluation_tol = 1e-3;
  double adjoint_incoherent_grid_convergence_tol = 1e-3;
  double adjoint_incoherent_grid_absolute_diff_tol = 1e-42;
  double adjoint_incoherent_grid_distance_tol = 1e-16;

  // Adjoint electron options
  double cutoff_angle_cosine = 1.0;
  int number_of_moment_preserving_angles = 0;
  double electron_tabular_evaluation_tol = 1e-8;
  std::string electron_two_d_interp = "Log-Log-Log";
  MonteCarlo::TwoDInterpolationType electron_interp =
                                        MonteCarlo::LOGLOGLOG_INTERPOLATION;
  bool electron_correlated_sampling_mode = true;
  bool electron_unit_based_interpolation_mode = true;
  double adjoint_electron_grid_convergence_tol = 0.001;
  double adjoint_electron_grid_absolute_diff_tol = 1e-16;
  double adjoint_electron_grid_distance_tol = 1e-8;
  double adjoint_bremsstrahlung_max_energy_nudge_value = 0.2;
  double adjoint_bremsstrahlung_energy_to_outgoing_energy_nudge_value = 1e-7;
  double adjoint_bremsstrahlung_evaluation_tol = 1e-6;
  double adjoint_bremsstrahlung_grid_convergence_tol = 0.001;
  double adjoint_bremsstrahlung_grid_absolute_diff_tol = 1e-12;
  double adjoint_bremsstrahlung_grid_distance_tol = 1e-14;
  double adjoint_electroionization_evaluation_tol = 1e-6;
  double adjoint_electroionization_grid_convergence_tol = 0.001;
  double adjoint_electroionization_grid_absolute_diff_tol = 1e-12;
  double adjoint_electroionization_grid_distance_tol = 1e-14;

  // General grid generation options
  double grid_convergence_tol = 1e-3;
  double grid_absolute_diff_tol = 1e-42;
  double grid_distance_tol = 1e-16;

  // Set the general table option names
  aepr_generator_clp.setDocString( "Adjoint Electron-Photon-Relaxation Native "
                                   "Data File Generator\n" );
  aepr_generator_clp.setOption( "forward_file",
                                &forward_file_name,
                                "The forward file that will be used to "
                                "generate the adjoint epr table. Note: "
                                "If this option is used the "
                                "cross_sec_dir, cross_sec_alis, "
                                "modify_cs_xml_file, and subdir options will "
                                "be ignored." );
  aepr_generator_clp.setOption( "cross_sec_dir",
                                &cross_section_directory,
                                "Directory containing desired cross section "
                                "tables. Note: If the forward_file option "
                                "is used this option will be ignored." );
  aepr_generator_clp.setOption( "cross_sec_alias",
                                &cross_section_alias,
                                "Photon cross section table alias. Note: If "
                                "the cross_sec_dir option is used this option "
                                "must also be used. If the forward_file "
                                "option is used this option will be ignored.");
  aepr_generator_clp.setOption( "subdir",
                                &subdirectory_name,
                                "Subdirectory in the cross section directory "
                                "where the generated table will be placed. If "
                                "the foward_file option is used this option "
                                "will be ignored." );
  aepr_generator_clp.setOption( "notes",
                                &table_notes,
                                "Notes about this table (e.g. date of "
                                "generation, owner, copyright, etc.)" );
  aepr_generator_clp.setOption( "modify_cs_xml_file",
                                "do_not_modify_cs_xml_file",
                                &modify_cs_xml_file,
                                "Modify the cross_sections.xml file?" );

  // Set the table energy limits option names
  aepr_generator_clp.setOption( "min_photon_energy",
                                &min_photon_energy,
                                "Min photon energy for table." );
  aepr_generator_clp.setOption( "max_photon_energy",
                                &max_photon_energy,
                                "Max photon energy for table." );
  aepr_generator_clp.setOption( "min_electron_energy",
                                &min_electron_energy,
                                "Min electron energy for table." );
  aepr_generator_clp.setOption( "max_electron_energy",
                                &max_electron_energy,
                                "Max electron energy for table." );

  // Set the photon option names
  aepr_generator_clp.setOption( "adjoint_pp_edist_eval_tol",
                                &adjoint_pp_energy_dist_norm_const_eval_tol,
                                "Adjoint pair production energy distribution "
                                "norm constant evaluation tolerance "
                                "(0.0<tol<1.0)" );
  aepr_generator_clp.setOption( "adjoint_pp_edist_nudge_val",
                                &adjoint_pp_energy_dist_norm_const_nudge_value,
                                "Adjoint pair production energy distribution "
                                "norm constant nudge value (val>= 0.0)" );
  aepr_generator_clp.setOption( "adjoint_tp_edist_eval_tol",
                                &adjoint_tp_energy_dist_norm_const_eval_tol,
                                "Adjoint triplet production energy "
                                "distribution norm constant evaluation "
                                "tolerance (0.0<tol<1.0)" );
  aepr_generator_clp.setOption( "adjoint_tp_edist_nudge_val",
                                &adjoint_tp_energy_dist_norm_const_nudge_value,
                                "Adjoint triplet production energy "
                                "distribution norm constant nudge value "
                                "(val>=0.0)" );
  aepr_generator_clp.setOption( "adjoint_incoherent_max_e_nudge_val",
                                &adjoint_incoherent_max_energy_nudge_value,
                                "Adjoint incoherent max energy nudge value "
                                "(val>0.0)" );
  aepr_generator_clp.setOption( "adjoint_incoherent_e_to_max_e_nudge_val",
                                &adjoint_incoherent_energy_to_max_energy_nudge_value,
                                "Adjoint incoherent energy to max energy "
                                "nudge value (val>=0.0)" );
  aepr_generator_clp.setOption( "adjoint_incoherent_eval_tol",
                                &adjoint_incoherent_evaluation_tol,
                                "Adjoint incoherent evaluation tolerance "
                                "(0.0<tol<1.0)" );
  aepr_generator_clp.setOption( "adjoint_incoherent_grid_convergence_tol",
                                &adjoint_incoherent_grid_convergence_tol,
                                "Adjoint incoherent grid convergence "
                                "tolerance (0.0<tol<1.0)" );
  aepr_generator_clp.setOption( "adjoint_incoherent_grid_absolute_diff_tol",
                                &adjoint_incoherent_grid_absolute_diff_tol,
                                "Adjoint incoherent grid absolute difference "
                                "tolerance (0.0<tol<1.0)" );
  aepr_generator_clp.setOption( "adjoint_incoherent_grid_dist_tol",
                                &adjoint_incoherent_grid_distance_tol,
                                "Adjoint incoherent grid distance tolerance "
                                "(0.0<tol<1.0)" );
  
  // Set the electron option names
  aepr_generator_clp.setOption( "cutoff_angle_cosine",
                                &cutoff_angle_cosine,
                                "Cutoff angle cosine for table "
                                "(-1.0<=mu<=1.0" );
  aepr_generator_clp.setOption( "number_of_moment_preserving_angles",
                                &number_of_moment_preserving_angles,
                                "Number of moment preserving angles for table"
                                "(angles>=0)" );
  aepr_generator_clp.setOption( "electron_tabular_evaluation_tol",
                                &electron_tabular_evaluation_tol,
                                "Electron tabular evaluation tolerance "
                                "(0.0<tol<1.0)" );
  aepr_generator_clp.setOption( "electron_interp_policy",
                                &electron_two_d_interp,
                                "The electron 2D interpolation policy" );
  aepr_generator_clp.setOption( "electron_correlated_sampling",
                                "electron_stochastic_sampling",
                                &electron_correlated_sampling_mode,
                                "Electron correlated/stochastic sampling mode" );
  aepr_generator_clp.setOption( "electron_unit_based_interp",
                                "electron_exact_interp",
                                &electron_unit_based_interpolation_mode,
                                "Electron unit based/exact interpolation mode" );
  aepr_generator_clp.setOption( "adjoint_electron_grid_convergence_tol",
                                &adjoint_electron_grid_convergence_tol,
                                "Adjoint electron grid convergence "
                                "tolerance (0.0<tol<1.0)" );
  aepr_generator_clp.setOption( "adjoint_electron_grid_absolute_diff_tol",
                                &adjoint_electron_grid_absolute_diff_tol,
                                "Adjoint electron grid absolute difference "
                                "tolerance (0.0<tol<1.0)" );
  aepr_generator_clp.setOption( "adjoint_electron_dist_tol",
                                &adjoint_electron_grid_distance_tol,
                                "Adjoint electron grid distance tolerance "
                                "(0.0<tol<1.0)" );
  aepr_generator_clp.setOption( "adjoint_bremsstrahlung_max_e_nudge_val",
                                &adjoint_bremsstrahlung_max_energy_nudge_value,
                                "Adjoint bremsstrahlung max energy nudge value "
                                "(val>0.0)" );
  aepr_generator_clp.setOption( "adjoint_bremsstrahlung_e_to_outgoing_e_nudge_val",
                                &adjoint_bremsstrahlung_energy_to_outgoing_energy_nudge_value,
                                "Adjoint bremsstrahlung energy to outgoing energy "
                                "nudge value (val>=0.0)" );
  aepr_generator_clp.setOption( "adjoint_bremsstrahlung_eval_tol",
                                &adjoint_bremsstrahlung_evaluation_tol,
                                "Adjoint bremsstrahlung evaluation tolerance "
                                "(0.0<tol<1.0)" );
  aepr_generator_clp.setOption( "adjoint_bremsstrahlung_grid_convergence_tol",
                                &adjoint_bremsstrahlung_grid_convergence_tol,
                                "Adjoint bremsstrahlung grid convergence "
                                "tolerance (0.0<tol<1.0)" );
  aepr_generator_clp.setOption( "adjoint_bremsstrahlung_grid_absolute_diff_tol",
                                &adjoint_bremsstrahlung_grid_absolute_diff_tol,
                                "Adjoint bremsstrahlung grid absolute difference "
                                "tolerance (0.0<tol<1.0)" );
  aepr_generator_clp.setOption( "adjoint_bremsstrahlung_dist_tol",
                                &adjoint_bremsstrahlung_grid_distance_tol,
                                "Adjoint bremsstrahlung grid distance tolerance "
                                "(0.0<tol<1.0)" );
  aepr_generator_clp.setOption( "adjoint_electroionization_eval_tol",
                                &adjoint_electroionization_evaluation_tol,
                                "Adjoint electroionization evaluation "
                                "tolerance (0.0<tol<1.0)" );
  aepr_generator_clp.setOption( "adjoint_electroionization_grid_convergence_tol",
                                &adjoint_electroionization_grid_convergence_tol,
                                "Adjoint electroionization grid convergence "
                                "tolerance (0.0<tol<1.0)" );
  aepr_generator_clp.setOption( "adjoint_electroionization_grid_absolute_diff_tol",
                                &adjoint_electroionization_grid_absolute_diff_tol,
                                "Adjoint electroionization grid absolute "
                                "difference tolerance (0.0<tol<1.0)" );
  aepr_generator_clp.setOption( "adjoint_electroionization_dist_tol",
                                &adjoint_electroionization_grid_distance_tol,
                                "Adjoint electroionization grid distance "
                                "tolerance (0.0<tol<1.0)" );
  
  // Set the grid generation option names
  aepr_generator_clp.setOption( "grid_convergence_tol",
                                &grid_convergence_tol,
                                "Grid convergence tolerance (0.0<tol<1.0)" );
  aepr_generator_clp.setOption( "grid_absolute_diff_tol",
                                &grid_absolute_diff_tol,
                                "Grid absolute difference tolerance"
                                "(0.0<tol<1.0)" );
  aepr_generator_clp.setOption( "grid_absolute_dist_tol",
                                &grid_distance_tol,
                                "Grid absolute distance tolerance"
                                "(0.0<tol<1.0)" );

  aepr_generator_clp.throwExceptions( false );

  // Parse the command line
  Teuchos::CommandLineProcessor::EParseCommandLineReturn
    parse_return = aepr_generator_clp.parse( argc, argv );

  if( parse_return != Teuchos::CommandLineProcessor::PARSE_SUCCESSFUL )
  {
    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return parse_return;
  }

  // Make sure the user's requested option values are valid
  // 1.) Either the forward_file option OR the cross_sec_dir and
  //     cross_sec_alias options MUST be specified. If both are specified
  //     warn the user that cross_sec_dir, cross_sec_alias and
  //     modify_cs_xml_file options will be ignored.
  if( forward_file_name.size() == 0 && cross_section_alias.size() == 0 )
  {
    std::cerr << Utility::BoldRed( "Error: " )
              << "either the forward_file option or the cross_sec_alias option"
              << " must be used!"
              << std::endl;
    
    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }
  else if( forward_file_name.size() != 0 && cross_section_alias.size() != 0 )
  {
    std::cerr << Utility::BoldMagenta( "Warning: " )
              << "both the foward_file option and the cross_sec_alias option "
              << "were used. The cross_sec_alias option will be ignored!"
              << std::endl;
  }

  // 2.) The min photon energy must be > 0.0
  if( min_photon_energy <= 0.0 )
  {
    std::cerr << Utility::BoldRed( "Error: " )
              << "the min photon energy is not valid!"
              << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 3.) The max photon energy must be > min photon energy
  if( max_photon_energy <= min_photon_energy )
  {
    std::cerr << Utility::BoldRed( "Error: " )
              << "the max photon energy is not valid!"
              << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 4.) The min electron energy must be > 0.0
  if( min_electron_energy <= 0.0 )
  {
    std::cerr << Utility::BoldRed( "Error: " )
              << "the min electron energy is not valid!"
              << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 5.) The max electron energy must be > min electron energy
  if( max_electron_energy <= min_electron_energy )
  {
    std::cerr << Utility::BoldRed( "Error: " )
              << "the max electron energy is not valid!"
              << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );
  }

  // 6.) The pair production energy distribution norm constant evaluation
  //     tolerance must be in the valid range
  if( adjoint_pp_energy_dist_norm_const_eval_tol <= 0.0 ||
      adjoint_pp_energy_dist_norm_const_eval_tol >= 1.0 )
  {
    std::cerr << Utility::BoldRed( "Error: " )
              << "the adjoint pair production energy distribution evaluation "
              << "tolerance is not valid!"
              << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 7.) The pair production energy distribution norm constant nudge value
  //     must be >= 0.0
  if( adjoint_pp_energy_dist_norm_const_nudge_value < 0.0 )
  {
    std::cerr << Utility::BoldRed( "Error: " )
              << "the adjoint pair production energy distribution nudge value "
              << "is not valid!"
              << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 8.) The triplet production energy distribution norm constant evaluation
  //     tolerance must be in the valid range
  if( adjoint_tp_energy_dist_norm_const_eval_tol <= 0.0 ||
      adjoint_tp_energy_dist_norm_const_eval_tol >= 1.0 )
  {
    std::cerr << Utility::BoldRed( "Error: " )
              << "the adjoint triplet production energy distribution "
              << "evaluation tolerance is not valid!"
              << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 9.) The triplet production energy distribution norm constant nudge value
  //     must be >= 0.0
  if( adjoint_tp_energy_dist_norm_const_nudge_value < 0.0 )
  {
    std::cerr << Utility::BoldRed( "Error: " )
              << "the adjoint triplet production energy distribution nudge "
              << "value is not valid!"
              << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 10.) The incoherent max energy nudge value must be > 0.0
  if( adjoint_incoherent_max_energy_nudge_value <= 0.0 )
  {
    std::cerr << Utility::BoldRed( "Error: " )
              << "the adjoint incoherent max energy nudge value is not valid!"
              << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 11.) The incoherent energy to max energy nudge value must be >= 0.0
  if( adjoint_incoherent_energy_to_max_energy_nudge_value < 0.0 )
  {
    std::cerr << Utility::BoldRed( "Error: " )
              << "the adjoint incoherent energy to max energy nudge value is "
              << "not valid!"
              << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 12.) The adjoint incoherent evaluation tolerance must be in the valid range
  if( adjoint_incoherent_evaluation_tol <= 0.0 ||
      adjoint_incoherent_evaluation_tol >= 1.0 )
  {
    std::cerr << Utility::BoldRed( "Error: " )
              << "the adjoint incoherent evaluation tolerance is not valid!"
              << std::endl;
    
    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 13.) The adjoint incoherent grid convergence tolerance must be in the
  //     valid range
  if( adjoint_incoherent_grid_convergence_tol <= 0.0 ||
      adjoint_incoherent_grid_convergence_tol >= 1.0 )
  {
    std::cerr << Utility::BoldRed( "Error: " )
              << "the adjoint incoherent grid convergence tolerance is not "
              << "valid!"
              << std::endl;
    
    aepr_generator_clp.printHelpMessage( argv[0], *out );
    
    return 1;
  }

  // 14.) The adjoint incoherent grid absolute difference tolerance must be
  //      in the valid range
  if( adjoint_incoherent_grid_absolute_diff_tol <= 0.0 ||
      adjoint_incoherent_grid_absolute_diff_tol >= 1.0 )
  {
    std::cerr << Utility::BoldRed( "Error: " )
              << "the adjoint incoherent grid absolute difference tolerance "
              << "is not valid!"
              << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 15.) The adjoint incoherent grid distance tolerance must be in the
  //      valid range
  if( adjoint_incoherent_grid_distance_tol <= 0.0 ||
      adjoint_incoherent_grid_distance_tol >= 1.0 )
  {
    std::cerr << Utility::BoldRed( "Error: " )
              << "the adjoint incoherent grid distance tolerance is not valid!"
              << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 16.) The electron TwoDInterpPolicy
  if( !Data::isTwoDInterpPolicyValid( electron_two_d_interp ) )
  {
    std::cerr << Utility::BoldRed( "Error: " )
              << "the electron 2D interpolation policy is not valid!"
              << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }
  else
  {
    electron_interp =
      MonteCarlo::convertStringToTwoDInterpolationType( electron_two_d_interp );
  }

  // 17.) The cutoff angle cosine must be in the valid range
  if( cutoff_angle_cosine < -1.0 ||
      cutoff_angle_cosine > 1.0 )
  {
    std::cerr << Utility::BoldRed( "Error: " )
              << "the cutoff angle cosine is not valid!"
              << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 18.) The number of moment preserving angles must be >= 0
  if( number_of_moment_preserving_angles < 0 )
  {
    std::cerr << Utility::BoldRed( "Error: " )
              << "the number of moment preserving angles is not valid!"
              << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 19.) The electron tabular evaluation tolerance must be in the valid range
  if( electron_tabular_evaluation_tol <= 0.0 ||
      electron_tabular_evaluation_tol >= 1.0 )
  {
    std::cerr << Utility::BoldRed( "Error: " )
              << "the electron tabular evaluation tolerance is not valid!"
              << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 20.) The adjoint electron grid convergence tolerance must be in the valid range
  if( adjoint_electron_grid_convergence_tol <= 0.0 ||
      adjoint_electron_grid_convergence_tol >= 1.0 )
  {
    (*out) << Utility::BoldRed( "Error: " )
           << "the adjoint electron grid convergence tolerance is not valid!"
           << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 21.) The adjoint electron grid absolute difference tolerance must be in the valid range
  if( adjoint_electron_grid_absolute_diff_tol <= 0.0 ||
      adjoint_electron_grid_absolute_diff_tol >= 1.0 )
  {
    (*out) << Utility::BoldRed( "Error: " )
           << "the adjoint electron grid absolute difference tolerance is not valid!"
           << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }
  
  // 22.) The adjoint electron grid distance tolerance must be in the valid range
  if( adjoint_electron_grid_distance_tol <= 0.0 ||
      adjoint_electron_grid_distance_tol >= 1.0 )
  {
    (*out) << Utility::BoldRed( "Error: " )
           << "the adjoint electron grid distance tolerance is not valid!"
           << std::endl;
    
    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 23.) The bremsstrahlung max energy nudge value must be > 0.0
  if( adjoint_bremsstrahlung_max_energy_nudge_value <= 0.0 )
  {
    (*out) << Utility::BoldRed( "Error: " )
           << "the adjoint bremsstrahlung max energy nudge value is not valid!"
           << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 24.) The bremsstrahlung energy to oloutgoing energy nudge value must be >= 0.0
  if( adjoint_bremsstrahlung_energy_to_outgoing_energy_nudge_value < 0.0 )
  {
    (*out) << Utility::BoldRed( "Error: " )
           << "the adjoint bremsstrahlung energy to outgoing energy nudge value is not "
           << "valid!"
           << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 25.) The adjoint bremsstrahlung evaluation tolerance must be in the valid range
  if( adjoint_bremsstrahlung_evaluation_tol <= 0.0 ||
      adjoint_bremsstrahlung_evaluation_tol >= 1.0 )
  {
    (*out) << Utility::BoldRed( "Error: " )
           << "the adjoint bremsstrahlung evaluation tolerance is not valid!"
           << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 26.) The adjoint bremsstrahlung grid convergence tolerance must be in the
  //     valid range
  if( adjoint_bremsstrahlung_grid_convergence_tol <= 0.0 ||
      adjoint_bremsstrahlung_grid_convergence_tol >= 1.0 )
  {
    (*out) << Utility::BoldRed( "Error: " )
           << "the adjoint bremsstrahlung grid convergence tolerance is not valid!"
           << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 27.) The adjoint bremsstrahlung grid absolute difference tolerance must be
  //      in the valid range
  if( adjoint_bremsstrahlung_grid_absolute_diff_tol <= 0.0 ||
      adjoint_bremsstrahlung_grid_absolute_diff_tol >= 1.0 )
  {
    (*out) << Utility::BoldRed( "Error: " )
           << "the adjoint bremsstrahlung grid absolute difference tolerance is "
           << "not valid!"
           << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 28.) The adjoint bremsstrahlung grid distance tolerance must be in the
  //      valid range
  if( adjoint_bremsstrahlung_grid_distance_tol <= 0.0 ||
      adjoint_bremsstrahlung_grid_distance_tol >= 1.0 )
  {
    (*out) << Utility::BoldRed( "Error: " )
           << "the adjoint bremsstrahlung grid distance tolerance is not valid!"
           << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 29.) The adjoint electroionization evaluation tolerance must be in the valid range
  if( adjoint_electroionization_evaluation_tol <= 0.0 ||
      adjoint_electroionization_evaluation_tol >= 1.0 )
  {
    (*out) << Utility::BoldRed( "Error: " )
           << "the adjoint electroionization evaluation tolerance is not valid!"
           << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 30.) The adjoint electroionization grid convergence tolerance must be in the
  //     valid range
  if( adjoint_electroionization_grid_convergence_tol <= 0.0 ||
      adjoint_electroionization_grid_convergence_tol >= 1.0 )
  {
    (*out) << Utility::BoldRed( "Error: " )
           << "the adjoint electroionization grid convergence tolerance is not valid!"
           << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 31.) The adjoint electroionization grid absolute difference tolerance must be
  //      in the valid range
  if( adjoint_electroionization_grid_absolute_diff_tol <= 0.0 ||
      adjoint_electroionization_grid_absolute_diff_tol >= 1.0 )
  {
    (*out) << Utility::BoldRed( "Error: " )
           << "the adjoint electroionization grid absolute difference tolerance is "
           << "not valid!"
           << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 32.) The adjoint electroionization grid distance tolerance must be in the
  //      valid range
  if( adjoint_electroionization_grid_distance_tol <= 0.0 ||
      adjoint_electroionization_grid_distance_tol >= 1.0 )
  {
    (*out) << Utility::BoldRed( "Error: " )
           << "the adjoint electroionization grid distance tolerance is not valid!"
           << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 33.) The grid convergence tolerance must be in the valid range
  if( grid_convergence_tol <= 0.0 || grid_convergence_tol >= 1.0 )
  {
    std::cerr << Utility::BoldRed( "Error: " )
              << "the grid convergence tolerance is not valid!"
              << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // 34.) The grid absolute difference tolerance must be in the valid range
  if( grid_absolute_diff_tol <= 0.0 || grid_absolute_diff_tol >= 1.0 )
  {
    std::cerr << Utility::BoldRed( "Error: " )
              << "the grid absolute difference tolerance is not valid!"
              << std::endl;

    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }
  
  // 35.) The grid distance tolerance must be in the valid range
  if( grid_distance_tol <= 0.0 || grid_distance_tol >= 1.0 )
  {
    std::cerr << Utility::BoldRed( "Error: " )
              << "the grid distance tolerance is not valid!"
              << std::endl;
    
    aepr_generator_clp.printHelpMessage( argv[0], *out );

    return 1;
  }

  // Get the file info
  std::string data_file_path, cross_sections_xml_file;
  Teuchos::RCP<Teuchos::ParameterList> cross_sections_table_info;

  if( forward_file_name.size() > 0 )
    data_file_path = forward_file_name;
  else
  {
    // Open the cross_sections.xml file
    cross_sections_xml_file = cross_section_directory;
    cross_sections_xml_file += "/cross_sections.xml";

    cross_sections_table_info =
      Teuchos::getParametersFromXmlFile( cross_sections_xml_file );

    std::string data_file_type, data_table_name;
    int data_file_start_line;
    double atomic_weight;

    try{
      Data::CrossSectionsXMLProperties::extractInfoFromPhotoatomTableInfoParameterList(
                                                    cross_section_directory,
                                                    cross_section_alias,
                                                    *cross_sections_table_info,
                                                    data_file_path,
                                                    data_file_type,
                                                    data_table_name,
                                                    data_file_start_line,
                                                    atomic_weight );
    }
    EXCEPTION_CATCH_AND_EXIT( std::exception,
                              "Error: Unable to load the requested cross "
                              "section table metadata!" );

    // The data file type must be a native file
    if( data_file_type != "Native" )
    {
      std::cerr << Utility::BoldRed( "Error: " )
                << "the cross section alias provided does not correspond to a "
                << "Native photon data file!"
                << std::endl;

      return 1;
    }
  }

  // Create the data generator and populate the new data table
  Data::AdjointElectronPhotonRelaxationVolatileDataContainer data_container;
  int atomic_number;
  
  {
  // Export the temp data to an XML file
  std::string temp_forward_file_name = "epr_native_temp.xml";

    // Recalculate the elastic electron data with the desired parameters
    {
      Data::ElectronPhotonRelaxationVolatileDataContainer temp_data_container(
            data_file_path,
            Utility::ArchivableObject::XML_ARCHIVE );

      try{
        DataGen::StandardElectronPhotonRelaxationDataGenerator::repopulateElectronElasticData(
          temp_data_container,
          max_electron_energy,
          cutoff_angle_cosine,
          electron_tabular_evaluation_tol,
          number_of_moment_preserving_angles,
          electron_interp );
      }
      EXCEPTION_CATCH_AND_EXIT( std::exception,
                                "Error: Unable to repopulate the elastic "
                                "electron data!" );

      temp_data_container.exportData( temp_forward_file_name,
                                      Utility::ArchivableObject::XML_ARCHIVE );
    }

    std::shared_ptr<const Data::ElectronPhotonRelaxationDataContainer>
      forward_data_container( new Data::ElectronPhotonRelaxationDataContainer(
                                                     temp_forward_file_name ) );

    atomic_number = forward_data_container->getAtomicNumber();

    DataGen::StandardAdjointElectronPhotonRelaxationDataGenerator
      generator( forward_data_container,
                 min_photon_energy,
                 max_photon_energy,
                 min_electron_energy,
                 max_electron_energy );

    // Set the general properties
    generator.setDefaultGridConvergenceTolerance( grid_convergence_tol );
    generator.setDefaultGridAbsoluteDifferenceTolerance( grid_absolute_diff_tol );
    generator.setDefaultGridDistanceTolerance( grid_distance_tol );

    // Set the adjoint photon properties
    generator.setAdjointPairProductionEnergyDistNormConstEvaluationTolerance( adjoint_pp_energy_dist_norm_const_eval_tol );
    generator.setAdjointPairProductionEnergyDistNormConstNudgeValue( adjoint_pp_energy_dist_norm_const_nudge_value );
    generator.setAdjointTripletProductionEnergyDistNormConstEvaluationTolerance( adjoint_tp_energy_dist_norm_const_eval_tol );
    generator.setAdjointTripletProductionEnergyDistNormConstNudgeValue( adjoint_tp_energy_dist_norm_const_nudge_value );
    generator.setAdjointIncoherentMaxEnergyNudgeValue( adjoint_incoherent_max_energy_nudge_value );
    generator.setAdjointIncoherentEnergyToMaxEnergyNudgeValue( adjoint_incoherent_energy_to_max_energy_nudge_value );
    generator.setAdjointIncoherentCrossSectionEvaluationTolerance( adjoint_incoherent_evaluation_tol );
    generator.setAdjointIncoherentGridConvergenceTolerance( adjoint_incoherent_grid_convergence_tol );
    generator.setAdjointIncoherentGridAbsoluteDifferenceTolerance( adjoint_incoherent_grid_absolute_diff_tol );
    generator.setAdjointIncoherentGridDistanceTolerance( adjoint_incoherent_grid_distance_tol );

    // Set the adjoint electron properties
    generator.setAdjointElectronGridConvergenceTolerance( adjoint_electron_grid_convergence_tol );
    generator.setAdjointElectronAbsoluteDifferenceTolerance( adjoint_electron_grid_absolute_diff_tol );
    generator.setAdjointElectronDistanceTolerance( adjoint_electron_grid_distance_tol );
    generator.setAdjointBremsstrahlungMaxEnergyNudgeValue( adjoint_bremsstrahlung_max_energy_nudge_value );
    generator.setAdjointBremsstrahlungEnergyToOutgoingEnergyNudgeValue( adjoint_bremsstrahlung_energy_to_outgoing_energy_nudge_value );
    generator.setAdjointBremsstrahlungEvaluationTolerance( adjoint_bremsstrahlung_evaluation_tol );
    generator.setAdjointBremsstrahlungGridConvergenceTolerance( adjoint_bremsstrahlung_grid_convergence_tol );
    generator.setAdjointBremsstrahlungAbsoluteDifferenceTolerance( adjoint_bremsstrahlung_grid_absolute_diff_tol );
    generator.setAdjointBremsstrahlungDistanceTolerance( adjoint_bremsstrahlung_grid_distance_tol );
    generator.setAdjointElectroionizationEvaluationTolerance( adjoint_electroionization_evaluation_tol );
    generator.setAdjointElectroionizationGridConvergenceTolerance( adjoint_electroionization_grid_convergence_tol );
    generator.setAdjointElectroionizationAbsoluteDifferenceTolerance( adjoint_electroionization_grid_absolute_diff_tol );
    generator.setAdjointElectroionizationDistanceTolerance( adjoint_electroionization_grid_distance_tol );
    generator.setTabularEvaluationTolerance( electron_tabular_evaluation_tol );
    generator.setElectronTwoDInterpPolicy( electron_interp );
    if( electron_correlated_sampling_mode )
      generator.setElectronCorrelatedSamplingModeOn();
    else
      generator.setElectronCorrelatedSamplingModeOff();
    if( electron_unit_based_interpolation_mode )
      generator.setElectronUnitBasedInterpolationModeOn();
    else
      generator.setElectronUnitBasedInterpolationModeOff();

    // Populate the new data container
    try{
      generator.populateEPRDataContainer( data_container );
    }
    EXCEPTION_CATCH_AND_EXIT( std::exception,
                              "Error: The adjoint EPR data could not be "
                              "generated!" );

    // Add the notes to the data container
    if( table_notes.size() > 0 )
      data_container.setNotes( table_notes );

    const char *cstr = temp_forward_file_name.c_str();

    std::remove( cstr );
  }
std::cout << std::setprecision(17) << std::scientific << "\ndata genernated" << std::endl;
  // Export the generated data to an XML file
  std::ostringstream oss;
  oss << "aepr_" << atomic_number << "_native.xml";

  std::string new_file_name;

  if( forward_file_name.size() > 0 )
    new_file_name = oss.str();
  else
  {
    new_file_name = cross_section_directory;

    if( subdirectory_name.size() > 0 )
    {
      new_file_name += "/";
      new_file_name += subdirectory_name;
    }

    new_file_name += "/";
    new_file_name += oss.str();
  }
std::cout << std::setprecision(17) << std::scientific << "\ndata ready for export to " << new_file_name << std::endl;
  data_container.exportData( new_file_name,
                             Utility::ArchivableObject::XML_ARCHIVE );
std::cout << std::setprecision(17) << std::scientific << "\ndata exported" << std::endl;
  // Update the cross_sections.xml file if requested
  if( forward_file_name.size() == 0 && modify_cs_xml_file )
  {
std::cout << std::setprecision(17) << std::scientific << "\nUpdate the cross_sections.xml file if requested" << std::endl;
    Teuchos::ParameterList& alias_table =
      cross_sections_table_info->sublist( cross_section_alias );

    std::string local_file_name;

    if( subdirectory_name.size() > 0 )
    {
      local_file_name = subdirectory_name;
      local_file_name += "/";
    }

    local_file_name += oss.str();

    alias_table.set(
          Data::CrossSectionsXMLProperties::adjoint_photoatomic_file_path_prop,
          local_file_name );
    alias_table.set(
          Data::CrossSectionsXMLProperties::adjoint_photoatomic_file_type_prop,
          Data::CrossSectionsXMLProperties::native_file );
    alias_table.set(
         Data::CrossSectionsXMLProperties::adjoint_photoatomic_file_start_line_prop,
         -1 );
    alias_table.set(
         Data::CrossSectionsXMLProperties::adjoint_photoatomic_table_name_prop,
         "" );

    alias_table.set(
          Data::CrossSectionsXMLProperties::adjoint_electroatomic_file_path_prop,
          local_file_name );
    alias_table.set(
          Data::CrossSectionsXMLProperties::adjoint_electroatomic_file_type_prop,
          Data::CrossSectionsXMLProperties::native_file );
    alias_table.set(
         Data::CrossSectionsXMLProperties::adjoint_electroatomic_file_start_line_prop,
         -1 );
    alias_table.set(
         Data::CrossSectionsXMLProperties::adjoint_electroatomic_table_name_prop,
         "" );

    Teuchos::writeParameterListToXmlFile( *cross_sections_table_info,
                                          cross_sections_xml_file );
  }
  std::cout << std::setprecision(17) << std::scientific << " =\twhat's the problem?" << std::endl;
  return 0;
  std::cout << std::setprecision(17) << std::scientific << " =\thow are we here?" << std::endl;
}

//---------------------------------------------------------------------------//
// end aepr_generator.cpp
//---------------------------------------------------------------------------//
