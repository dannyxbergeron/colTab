// ***********************************************************
// Adapted from https://github.com/SpektorSoft/program-options
// ***********************************************************

#ifndef _COMMAND_LINE_OPTIONS_H__
#define _COMMAND_LINE_OPTIONS_H__

#include <string>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

// ----------------------------------------------------------------------------
// Handle command line options
//
// Options:
//  [-h|--help]                     Show help menu
//  [-V|--version]                  Display version information
//  [-I|--indices]                  Display column indices of the header
//  [-s|--sep] ARG (string)         User-supplied separator (default = TAB)
//  [--OutSep] ARG (string)         User-supplied output separator (default = TAB)
//  [-c|--cols] ARG (string)        Columns to be displayed (seperated by a comma)
//  [-i|--colIndices]               Use column indices instead of name in -c option
//  [-f|--file] ARG (string)   Input file
// ----------------------------------------------------------------------------

class CommandLineOptions
{
public:
   enum statusReturn_e
   {
      OPTS_SUCCESS,
      OPTS_VERSION,
      OPTS_HELP,
      OPTS_COL_INDICES,
      OPTS_FAILURE
   };

   CommandLineOptions();
   ~CommandLineOptions();
   statusReturn_e parse( int argc, char* argv[] );

   inline const std::string & getInputFile() const;
   inline const std::string & getUserSep() const{ return userSep; };
   inline const std::string & getUserOutSep() const{ return userOutSep; };
   inline const std::string & getUserCols() const{ return userCols; };
   inline bool getColIndices(){ return colIndices; };

protected:
   void setup();
   bool validateFiles();

private:
   // Not implemented -- not for use
   CommandLineOptions( const CommandLineOptions &rhs );
   CommandLineOptions &operator=( const CommandLineOptions &rhs );

   po::options_description options;
   std::string inputFile;
   std::string userSep;
   std::string userOutSep;
   std::string userCols;
   bool colIndices;
};

inline
const std::string & CommandLineOptions::getInputFile() const
{
   static const std::string emptyString;
   return ( 0 < inputFile.size() ) ? inputFile : emptyString;
}

#endif // _COMMAND_LINE_OPTIONS_H__
