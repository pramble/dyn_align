/*
 * 26 Sep 2016
 */

#include <cstdlib>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector> /* for bust.hh */

#include "bust.hh"
#include "param_entry.hh"
#include "param_tbl.hh"

using namespace std;

/* ----------------  ini_params ------------------------------
 */
void
param_tbl::ini_params()
{
    parameters =
    {
        {"misc.verbosity",    unsigned (1)},
        {"dynmc.temp",        100.},
        {"dynmc.nstep",       unsigned (110)},
        {"misc.compile_date", __DATE__},
        {"misc.compile_time", __TIME__},
        {"misc.dryrun",       false}
    };
}

/* ---------------- get   ------------------------------------
 */
param_entry &
param_tbl::operator[](const std::string &s)
{
    const char *get_pname = "getting parameter name ";
    const char *nfound = " not found\n";
    auto p = parameters.find (s);
    if (p == parameters.end())
        throw runtime_error(get_pname + s + nfound);
    return p->second;
}

/* ---------------- dump  ------------------------------------
 * Write parameters to a specified file handle.
 */
void
param_tbl::dump(std::ostream &of) {
    for (auto x : parameters)
        of << x.first<< ": "<< x.second<< '\n';
}
