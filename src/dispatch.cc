/*
 * 27 Sep 2016
 */
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
class action_base {
public:
    param_tbl *params;
    virtual int action (const string s) {cout <<s;return 0;}
    action_base(param_tbl *p): params (p) {}
    virtual void nothing();
    virtual ~action_base() = default;
};
void action_base::nothing() {} /* This quietens a compiler warning */

class action_msg : public action_base{
private:
public:
    int action (const string s);
};
int action_msg::action(const string s) {
  cout << "msg: "<< s<<endl; return EXIT_SUCCESS;}

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
        for (i = 0; s.length() && ! bool(isspace(s[i])); i++)
            /* count */ ;
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
int action_base::i_s = 77;
/* ---------------- dispatch ---------------------------------
 * This reads commands and does something with them.
 */
static int
dispatch()
{
    param_tbl pp;
    
    action_base a_b;
    a_b.set_params (&pp);

    action_msg a_m;

    a_m.action("hello a_m");
    
    unsigned nl = 0;
    for ( string s; n_getline(cin, s, '#', nl); ) {
        string_pair s_p;
        cout << "line "<<nl<< " is \""<< s<< "\"\n";
        s_p = split_two (s);
        if (! (s_p.first.length() && s_p.second.length())) {
            cerr << "Only one word in "<< s<<endl;
        } else {
            cout << "I found \""<< s_p.first<< "\" and \""<< s_p.second<<"\"\n";
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
