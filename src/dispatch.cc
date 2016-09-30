/*
 * 27 Sep 2016
 */
#include <csignal>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "mgetline.hh"
#include "param_entry.hh"
#include "param_tbl.hh"

using namespace std;
/* ---------------- structures -------------------------------
 */

struct string_pair {
    string first, second;
};
    
/* ---------------- split_two --------------------------------
 * Given a string, return the first and second parts, split by
 * a space. Is is guaranteed by the caller, that there are no leading
 * white spaces.
 */
static string_pair
split_two (const string s)
{
    struct string_pair s_p = { "", ""};
    {
        unsigned i;
        for ( i = 0; i < s.length() && (!bool(isspace(s[i]))) ; i++)
            /* just count */;
        s_p.first  = s.substr (0, i);
        i++;
        if (i < s.length()) {
            unsigned j;
            s_p.second = s.substr (i);
            for (j = 0; (j < s_p.second.length()) && isspace (s_p.second[j]); j++)
                ;
            if (j)
                s_p.second.erase (0,j);
        } /* Else we only got one string, but leave that to the caller */
    }
    return s_p;
}

typedef int gen_func (const string &to_do, string &ret_msg, param_tbl &params, void *misc_data);
typedef gen_func * gen_func_p;

/* ---------------- m_lower  ---------------------------------
 * lower case from->to and return "to" so we can use it in
 * function calls
 */
static string &
m_lower (const string &from, string &to)
{
    to = from;
    for (unsigned i = 0; i < to.length(); i++)
        to[i] = char (tolower(to[i]));
    return to;
}

static int
f1 (const string &to_do, string &ret_msg, param_tbl &params, void *misc_data)
{
    cout << __func__ << " was called";
    ret_msg = string(__func__) + " says " + to_do + "\n";
    ret_msg += "compiled on " + string (params["misc.compile_date"]);
    if (!misc_data) { cout << "";} /* stop compiler warnings */
    return EXIT_SUCCESS;
}


static int
f2 (const string &to_do, string &ret_msg, param_tbl &params, void *misc_data)
{
    cout << __func__ << " was called";
    ret_msg = string(__func__) + " says " + to_do +"\n";
    ret_msg += "compiled on " + string(params["misc.compile_date"]);
    if (!misc_data) { cout << "";} /* stop compiler warnings */
    return EXIT_FAILURE;
}

static int
cmd_set (const string &to_do, string &ret_msg, param_tbl &params, void *misc_data)
{
    cout << "hello from "<< __func__ << "\n";
    string_pair s_p = split_two (to_do);
    if (misc_data)
        ret_msg = "";
    params[s_p.first] = s_p.second;
    return EXIT_SUCCESS;
}
static int
cmd_param_dump (const string &to_do, string &ret_msg, param_tbl &params, void *misc_data)
{
    cout << "hello from "<< __func__ << "\n";
    if (misc_data ==  & to_do)
        ret_msg = "";
    params.dump(cout);
    return EXIT_SUCCESS;
}


/* ---------------- dispatch ---------------------------------
 * This reads commands and does something with them.
 */
static int
dispatch()
{
    param_tbl params;

    map <const string, gen_func_p> commands;
    commands = {
        { "f1",   &f1 },
        { "f2",   &f2 },
        { "set",  &cmd_set },
        { "dump", &cmd_param_dump }
    };
    unsigned nl = 0;
    for ( string s; n_getline(cin, s, '#', nl); ) {
        try {
            string_pair s_p;
            s_p = split_two (s);
            string tmp, ret_msg;
            auto p_cmmd = commands.find (m_lower (s_p.first, tmp));
            if ( p_cmmd == commands.end()) {
                throw runtime_error (s_p.first + " is not a valid command\n");
            } else {
                if (p_cmmd->second(s_p.second, ret_msg, params, nullptr) == EXIT_FAILURE) {
                    cerr << ret_msg << '\n';
                    return EXIT_FAILURE;
                }
            }
        } catch (runtime_error &e) {
            cerr << "Caught: "<< e.what();;
            if (bool(params["misc.dryrun"]) == true)
                cerr << "Continuing since misc.dryrun is true\n";
            else
                return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

/* ---------------- main  ------------------------------------
 */
int
main(int argc, char *argv[])
{
    streambuf *old_stdin_sbuf = nullptr;
    ifstream infile;
    if (argc < 2) {
        cout << "using stdin for input\n";
    } else {
        infile.open (argv[1]);
        old_stdin_sbuf = cin.rdbuf();
        cin.rdbuf( infile.rdbuf() );
    }

    if (dispatch() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if (old_stdin_sbuf) {
        infile.close(); /* need both the close() and rdbuf(),but the order does not matter */
        cin.rdbuf(old_stdin_sbuf);
    }
    return EXIT_SUCCESS;
}



#ifdef notes_on_redirecting_stdout
    streambuf *old_stdout_sbuf = nullptr;
    ofstream out;
    if (argc < 2) {
        cout << "using stdout for output\n";
    } else {
        out.open (argv[1]);
        old_stdout_sbuf = cout.rdbuf();
        cout.rdbuf( out.rdbuf() );
    }

    cout << "written to of\n";
    out.close(); /* need both the close() and rdbuf(),but the order does not matter */
    cout.rdbuf(old_stdout_sbuf);
#endif /* notes_on_redirecting_stdout */
