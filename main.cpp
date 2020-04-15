#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <vector>
#include <unordered_map>

#include "CommandLineOptions.h"

using namespace std;

bool USE_IN_STREAM = false; // If the user want to use the input stream
char SEP = '\t'; // Default separator
char OUT_SEP; // Output separator (assign to SEP if not specified)
int nbCol; // Number of columns in the header
unordered_map<string, int> columns; // map of header_name/index
vector<string> headerNames; // Header names (first line)
vector<int> indices; // Indices of the desired col (default all)
vector<string> values; // vector used to assign the parsed values

// Get the separator specified by the user
void getSep(const string& sep, bool inSep)
{
    // Assigne a new separator if specified by the user
    if(!(sep == "\\t" || sep == "TAB"))
    {
        if(sep.size() != 1)
        {
            cout << "ERROR - Separator must be one character" << endl;
            exit(0);
        }
        else
            if(inSep)
                SEP = sep[0];
            else
                OUT_SEP = sep[0];
    }
    // In case that the SEP is not TAB and the user wants OUT_SEP = TAB
    else if(!inSep)
        OUT_SEP = '\t';
}

// Get nb of columns and the actual values of the header
void getHeader(const string& file)
{
    string header;

    // Use the input stream if specified
    if(USE_IN_STREAM)
    {
        getline(cin, header);
    }
    else // use the specified input file
    {
        ifstream fs(file);
        getline(fs, header);
    }

    istringstream ss(header);
    string val;
    int count = 0;

    // Parse the first line to get the names of the columns
    while(getline(ss, val, SEP))
    {
        count++;
        // Add the header names to the headerNames vector
        headerNames.push_back(val);
        // Assigne the index of each column to its name
        columns[val] = count;
    }
    // Get the total number of columns
    nbCol = columns.size();
    // Get enough space in the values vector to prevent unwanted resizing
    // and be able to assign values with indices
    values.resize(nbCol + 1);
}

void getIndices(bool useIndices, const string& cols)
{
    istringstream ss(cols);
    string colVal;

    if(cols == "")
    {
        // To print all the columns
        for(int i = 1; i <= nbCol; i++)
            indices.push_back(i);
    }
    else if(useIndices) // Configure program to use col indices instead of names
    {
        int idx;
        while(getline(ss, colVal, ','))
        {
            // Validate that the values given for -c are really intergers
            // in the range of the columns
            try
            {
                idx = stoi(colVal);
                if(idx > 0 && idx <= nbCol)
                    indices.push_back(idx);
                else
                {
                    cout << "ERROR - Columns index \"" << idx
                    << "\" out of bound ! " << std::endl;
                    exit(0);
                }
            }
            catch(invalid_argument& e){
                cout << "ERROR - Columns passed are not intergers ! " << std::endl;
                exit(0);
            }
        }
    }
    else
    {
        // Parse the wanted columns
        while(getline(ss, colVal, ','))
        {
            // If the columns are in the header, add the index to the
            // indices vector
            if(columns.find(colVal) != columns.end())
                indices.push_back(columns[colVal]);
            else
            {
                cout << "ERROR - Column \"" << colVal << "\" not in the header !\n";
                exit(0);
            }
        }
    }
}

void printColValues(const vector<string>& v, int offset)
{
    // Go through all the desired columns and print the value in the
    // desired order
    for(int i = 0; i < indices.size(); i++)
    {
        cout << v[indices[i] - offset];
        if(i != indices.size() - 1)
            cout << OUT_SEP;
    }
    cout << endl;
}

void printUserColumns(const string& file)
{
    ifstream ifs = ifstream(file);
    istream *fs;
    fs = &ifs;

    // Use cin to get the input stream if wanted
    if(USE_IN_STREAM)
    {
        fs = &cin;
        // Print the header that was already read by cin
        printColValues(headerNames, 1);
    }

    string line;

    istringstream ss;
    string val;
    int idx = 1;

    // Read line by line and assign the values to the values vector
    while(getline(*fs, line))
    {
        ss.str(line);
        while(getline(ss, val, SEP))
        {
            values[idx] = val;
            idx++;
        }
        // Print the value of the wanted col for each row
        printColValues(values, 0);

        idx = 1;
        ss.str("");
        ss.clear();
    }
}

void init(CommandLineOptions& opts, int argc, char* argv[], const string& version)
{
    // Get the status of the commande line parser
    CommandLineOptions::statusReturn_e temp = opts.parse( argc, argv );

    if( CommandLineOptions::OPTS_SUCCESS == temp )
    {
        // If the input comes from a PIPE
        if(opts.getInputFile() == "IN")
            USE_IN_STREAM = true;

        // Get the separators to use
        getSep(opts.getUserSep(), true);
        if(opts.getUserOutSep() != "same as sep")
            getSep(opts.getUserOutSep(), false);
        else
            OUT_SEP = SEP;

        // Read and get infos on the header
        getHeader(opts.getInputFile());

        // Get the indices of the columns to print
        getIndices(opts.getColIndices(),
                    opts.getUserCols());

        // Print the desired columns
        printUserColumns(opts.getInputFile());
    }
    else{
        if( CommandLineOptions::OPTS_VERSION == temp )
            cout << "Version: " << version << endl;
        else if( CommandLineOptions::OPTS_COL_INDICES == temp )
        {
            // If the input comes from a PIPE
            if(opts.getInputFile() == "IN")
                USE_IN_STREAM = true;

            // Get the separator to use
            getSep(opts.getUserSep(), true);

            // Read and get infos on the header
            getHeader(opts.getInputFile());

            // Print the column names with their indices
            if(opts.getInputFile() != "")
            {
                for(string& n: headerNames)
                    cout << n << " -> " << columns[n] << endl;
            }
        }
        else if( CommandLineOptions::OPTS_HELP == temp ){}
        else
            cout << "ERROR - problem with options" << endl;

        exit(0);
    }
}


int main( int argc, char* argv[] )
{
    // Version of the script
    const string version = "0.2";

    CommandLineOptions opts;

    // Initialise the command line parser
    init(opts, argc, argv, version);

    return 0;
}
