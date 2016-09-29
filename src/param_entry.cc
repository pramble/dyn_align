/*
 * 9 Sep 2016
 */

#include <iostream>
#include <limits>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include <csignal>  /* Only in debugging */

#include "bust.hh"
#include "param_entry.hh"
#include "param_tbl.hh"
using namespace std;

/* ---------------- operator << ------------------------------
 */

std::ostream& operator<<(std::ostream& out, const param_entry o) {
    switch (o.this_t) {
    case param_entry::int_t:      out << o.the_var.i; break;
    case param_entry::float_t:    out << o.the_var.f; break;
    case param_entry::unsigned_t: out << o.the_var.u; break;
    case param_entry::bool_t:     out << o.the_var.b; break;
    case param_entry::string_t:   out << o.s;         break;
    default: bust_die ({__func__, "unknown data type\n"});
    }

#   ifdef want_to_show_types
    if (1 == 1) {
        switch (o.this_t) {
        case param_entry::int_t:      out << " (int)";      break;
        case param_entry::float_t:    out << " (float)";    break;
        case param_entry::unsigned_t: out << " (unsigned)"; break;
        case param_entry::bool_t:     out << " (bool)";     break;
        case param_entry::string_t:   out << " (string)";   break;
        default: bust_die ({__func__, "unknown data type\n"});
        }
    }
#   endif /* want_to_show_types */
    return out;
}

/* ---------------- stob  ------------------------------------
 * Given a string, return true / false
 */
static bool
stob (const string &s)
{
    string tmp = s;
    for (char &it : tmp)
        it = char (std::tolower (it));
    if (tmp == "0" || tmp == "false")
        return false;
    if ( tmp == "1" || tmp == "true")
        return true;
    throw runtime_error ("unable to make a bool from " + s + "\n");
}

/* ---------------- stou  ------------------------------------
 * convert string to unsigned, but check the range and throw
 * an exception if it is out of range
 */
static unsigned
stou (const string &s)
{
    unsigned long ul = stoul (s);
    unsigned u;
    cout << "max unsigned is " << numeric_limits<unsigned>::max() << "\n";
    if (ul > numeric_limits<unsigned>::max() )
        throw out_of_range ("out of range converting " + s + " to unsigned\n");
    return (u = unsigned (ul));
}

/* ---------------- set --------------------------------------
 * If we are given a string, we might have to convert it.
 */

void param_entry::set (const std::string &ss)
{
    switch (this_t) {

    case int_t:      set(stoi(ss));                          break;
    case float_t:    set(stof(ss));                          break;
    case unsigned_t: set(stou(ss));                          break;
    case bool_t:     set(stob(ss));                          break;
    case string_t:   s = ss;  s.shrink_to_fit();             break;
    case invalid_t:  throw logic_error ("unknown type");
    default:         throw logic_error ("stupid andrew");
    }
}

/* ---------------- set (int) --------------------------------
 * We land here, even if we have an unsigned. Better check.
 */
void param_entry::set (const int i)
{
    if (this_t == int_t) {
        the_var.i = i;
    } else if (this_t == unsigned_t) {
        if (i < 0)
            throw (runtime_error (string(__func__) + " attempting to set unsigned to negative value\n"));
        the_var.u = unsigned (i);
    }
}
/* ---------------- testing only -----------------------------
 */
#undef want_main
#ifdef want_main
int
main ()
{
    param_entry pi, pf, pu, pb, pcharp, pconstcharp, ps;
    pi.set(3);
    pf.set(3.7);
    pu.set(3);
    pb.set(false);
    ps.set(string("hello from string"));

    char charp [40] = "hello from char pointer";
    pcharp.set( charp);
    const char *constcharp = "hello from constcharp";
    pconstcharp.set( constcharp);

    std::cout << "Size of my union is "<< sizeof (pi)<< "\n";
    std::cout << "i   is "<< pi << "\n";
    std::cout << "f   is "<< pf << "\n";
    std::cout << "u   is "<< pu << "\n";
    std::cout << "b   is "<< pb << "\n";
    std::cout << "ps  is "<< ps << "\n";
    std::cout << "pc  is "<< pcharp << "\n"
              << "pcc is "<< pconstcharp << "\n";
    /* Now try some assignments */
    float g = pf;
    int   j = pi;
    string t = ps;
    std::cout << "float val is "<< g<< " int is "<< j<< " and a string is "<< t<< "\n";
    /* Now the overloaded assignment operator */
    param_entry ai = 3;
    param_entry af = 3.1415;
    param_entry au = 3, ub = true, us = "hello string";
    std::cout << "Trying the assignment / constructor\n"
              << "int is " << ai << " float is "<< af << " unsigned is "<< au << "bool (1) is "
              << ub << "\n" << "us is " << us<< "\n";

    /* Now, some reassignments */
    ai = 77;
    std::cout << "int has been reassigned to be "<< ai <<"\n";
    us = "new value for us";
    std::cout << "string is now \""<< us << "\"\n";

    return EXIT_SUCCESS;
}
#endif /* want_main */
