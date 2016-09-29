/*
 * 27 Sep 2016
 */
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "mgetline.hh"

using namespace std;




struct string_pair {
    string first, second;
};



/* ---------------- split_two --------------------------------
 * Given a string, return the first and second parts, split by
 * a space
 */
static string_pair
split_two (const string s)
{
    string_pair s_p;
    string t;
    /* leading blanks */
    return s_p;
}

/* ---------------- dispatch ---------------------------------
 * This reads commands and does something with them.
 */
static int
dispatch()
{
    unsigned nl = 0;
    for ( string s; n_getline(cin, s, '#', nl); ) {
        string_pair s_p;
        cout << "line "<<nl<< " is \""<< s<< "\"\n";
//        s_p = split_two (s);

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
