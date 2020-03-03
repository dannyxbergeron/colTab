#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <vector>
#include <unordered_map>

#include "CommandLineOptions.h"


using namespace std;

char SEP = '\t';
char OUT_SEP;
int nbCol;
unordered_map<string, int> columns;
vector<string> headerNames;
vector<int> indices;
vector<string> values;

void getSep(const string& sep, bool inSep)
{
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
}

void getHeader(const string& file)
{
    ifstream fs(file);
    string header;

    getline(fs, header);

    istringstream ss(header);
    string val;
    int count = 0;

    while(getline(ss, val, SEP))
    {
        count++;
        headerNames.push_back(val);
        columns[val] = count;
    }
    nbCol = columns.size();
    values.reserve(nbCol + 1);
}

void getIndices(bool useIndices, const string& cols)
{
    istringstream ss(cols);
    string colVal;

    if(cols == "")
    {
        for(int i = 1; i <= nbCol; i++)
            indices.push_back(i);
    }
    else if(useIndices) // Configure program to use col indices instead of names
    {
        int idx;
        while(getline(ss, colVal, ','))
        {
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
        unordered_map<string, int>::iterator it;
        while(getline(ss, colVal, ','))
        {
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

void printUserColumns(const string& file)
{
    ifstream fs(file);
    string line;

    istringstream ss;
    string val;
    int idx = 1;

    while(getline(fs, line))
    {
        ss.str(line);
        while(getline(ss, val, SEP))
        {
            values[idx] = val;
            idx++;
        }

        for(int i = 0; i < indices.size(); i++)
        {
            cout << values[indices[i]];
            if(i != indices.size() - 1)
                cout << OUT_SEP;
        }
        cout << endl;

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
    const string version = "0.1";

    CommandLineOptions opts;

    // Initialise the command line parser
    init(opts, argc, argv, version);

    return 0;
}
