# Tool to easily work with table files containing header
Simple tool to display only a sub portion of the columns in a file containing
a table.

__Author__ : Danny Bergeron

__Email__ :  _<danny.bergeron@usherbrooke.ca>_


### Dependencies
For the tool to work, you need to have installed the boost library
```bash
sudo apt-get install libboost-all-dev
```

### Comipling the tool
```bash
./activate main
```

### Usage
```bash
 colTab -f file -c columns [options]
```

In the -c option, this could be either the names of the columns (that is found
in the header (first line) of the table) or it could be the indices of the
columns, starting from 1, but the latter must have the -i flag.

You can easily find the indices of the columns in your file using the -I flag.

Help section:
```
Program Options:
  -h [ --help ]                Display help menu.
  -V [ --version ]             Display program version number
  -I [ --indices ]             Display column indices of the header
  -f [ --file ] arg            User-specified input file (use 'IN' for input
                               stream)
  -s [ --sep ] arg (=TAB)      User-specified input separator
  --outSep arg (=same as sep)  User-specified output separator
  -c [ --cols ] arg            Columns to be displayed (seperated by a comma)
  -i [ --colIndices ]          Use column indices instead of name in -c option

```
