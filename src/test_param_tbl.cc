/*
 * 26 Sep 2016
 * Check the interface to param_tbl
 */

#include <csignal>
#include <map>
#include <iostream>

#include "param_entry.hh"
#include "param_tbl.hh"

using namespace std;

/* ---------------- main       -------------------------------
 */
int
main ()
{
    param_tbl parameters;
    parameters.dump (cout);
    cout << "\n those are the hard coded parameters\n";
    cout << "Now, get a parameter\n";
    param_entry &p_e = parameters["misc.compile_date"];
    cout << "I just read the compile date as "<< p_e<< "\n";
    {
        string s = parameters["misc.compile_date"];
        cout << "and going via a temporary string gives.. "<< s<< endl;
    }
    cout << "after reading parameters, dump them: \n";

    parameters.dump (cout);
    try {
        parameters["misc.verbosity"] = 2;
        unsigned u = 3;
        parameters["misc.verbosity"] = u;
        int i = 4;
        parameters["misc.verbosity"] = i;
        typedef const char * c_c_p;
        c_c_p junk = "this is a constant char*";
        parameters["misc.compile_date"] = junk;
        cout << "this should say "<<junk << parameters["misc.compile_date"]<< endl;
    } catch (runtime_error &e) {
        cerr << e.what() << "should stop\n";
    } catch (out_of_range &e) {
        cerr << e.what() << "Should stop\n";
    }
    cout << "setting a boolean\n";
    {
        string s = "trUe";
        parameters["misc.dryrun"] = s;
    }
    cout << "setting a string from a const string";
    {
        const string t = "this_was a const string";
        raise (SIGTRAP);
        parameters["misc.compile_date"] = t;
    }
    cout << "After setting..\n";
    parameters.dump (cout);
    int i = parameters["misc.verbosity"];
    float f = parameters ["dynmc.temp"];
    cout << __func__ << " via the square brackets, temp is "<< f<< " and vbsty is " << i<< "\n";
    try {
        cout << "This should provoke a runtime error\n";
        parameters["silly"] = 2;
    } catch (runtime_error &e) {
        cout << "caught an error: "<< e.what()<<'\n';
    }
    cout << "setting an unsigned to a negative number   ";
    try {
        parameters["misc.verbosity"] = -1;
    } catch (runtime_error &e) {
        cout << "caught "<< e.what() << '\n';
    }
    parameters["dynmc.temp"] = float (33.3);
    parameters.dump(cout);
    return EXIT_SUCCESS;
}
