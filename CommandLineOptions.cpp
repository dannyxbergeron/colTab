// ***********************************************************
// Adapted from https://github.com/SpektorSoft/program-options
// ***********************************************************

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "CommandLineOptions.h"

CommandLineOptions::CommandLineOptions():
      options( "" ),
      inputFile( "" ),
      userSep( "TAB" ),
      userOutSep( "same as sep" ),
      userCols( "" ),
      colIndices( false )
{
   setup();
}

CommandLineOptions::~CommandLineOptions()
{

}

void CommandLineOptions::setup()
{
   po::options_description progOptions( "Program Options" );
   progOptions.add_options()
         ( "help,h", "Display help menu." )
         ( "version,V", "Display program version number" )
         ( "indices,I", "Display column indices of the header" )
         ( "file,f", po::value< std::string >( &inputFile ),
                            "User-specified input file (use 'IN' for input stream)" )
         ( "sep,s", po::value< std::string >( &userSep )->default_value( "TAB" ),
                            "User-specified input separator")
         ( "outSep", po::value< std::string >( &userOutSep )->default_value( "same as sep" ),
                            "User-specified output separator" )
         ( "cols,c", po::value< std::string >( &userCols )->default_value( "" ),
                            "Columns to be displayed (seperated by a comma)" )
         ( "colIndices,i", po::bool_switch( &colIndices )->default_value( false ),
                            "Use column indices instead of name in -c option" );

    options.add( progOptions );
}

CommandLineOptions::statusReturn_e CommandLineOptions::parse( int argc, char* argv[] )
{
   statusReturn_e ret = OPTS_SUCCESS;

   po::variables_map varMap;
   char filename[5000];

   try
   {
      po::store( po::parse_command_line( argc, argv, options ), varMap );
      po::notify( varMap );

      // Help option
      if( varMap.count( "help" ) )
      {
         std::cout << options << std::endl;
         return OPTS_HELP;
      }

      // Version info
      if( varMap.count( "version" ) )
      {
         return OPTS_VERSION;
      }

      // Version info
      if( varMap.count( "indices" ) )
      {
         return OPTS_COL_INDICES;
      }

      // Enforce an input file argument every time
      if( !( 0 < varMap.count( "file" ) ) )
      {
         std::cout << "ERROR - Input file must be specified!!!" << std::endl;
         std::cout << options << std::endl;
         return OPTS_FAILURE;
      }
      else
      {
          if(inputFile != "IN")
         {
             // Strip whitespaces from front/back of filename string
             boost::algorithm::trim( inputFile );

             // resolve the filename to be fully-qualified
             realpath( inputFile.c_str(), filename );
             inputFile = filename;

             ret = validateFiles() ? OPTS_SUCCESS : OPTS_FAILURE;
         }
      }

      // We can check if a value is defaulted
      // if( !varMap[ "value" ].defaulted() )
      // {
      //    std::cout << "WARNING - Default value for User-Value overwritten to " << myUserValue << std::endl;
      // }
   }
   catch( std::exception &e )
   {
      std::cout << "ERROR - parsing error: " << e.what() << std::endl;
      ret = OPTS_FAILURE;
   }
   catch( ... )
   {
      std::cout << "ERROR - parsing error (unknown type)." << std::endl;
      ret = OPTS_FAILURE;
   }

   return ret;
}

bool CommandLineOptions::validateFiles()
{
   if( !boost::filesystem::is_regular_file( inputFile ) )
   {
      std::cout << "ERROR - Input file provided does not exist! [" << inputFile << "]" << std::endl;
      return false;
   }
   return true;
}
