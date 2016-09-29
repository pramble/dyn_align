/*
 * 14 Jan 2016
 * the c varargs stuff could be replaced by  <initializer_list>
 */

#include <cerrno>
#include <cstdarg>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>

#include "bust.hh"

using std::cerr;
using std::cout;
using std::vector;

/* ---------------- bust_void --------------------------------
 * Print out vector members to cerr. Look at the instructions
 * below.
 */
void
bust_void (const vector <const char *> &m) {
    for (auto c: m)
        cerr<< c;
    cerr<< "\n";
}

/* ---------------- bust  ------------------------------------
 * Print out vector members to cerr. Each must be a C char *.
 * The use would be something like this,
 * return (bust ({"function: ", __func__, ", problem with file ", filename, "\n"}).
 * The braces are necessary to get promoted to a vector.
 */
int
bust (const vector <const char *> &m) {
    bust_void (m);
    return EXIT_FAILURE;
}

/* ---------------- bust_die ---------------------------------
 * Print out an error messaage and drop dead.
 */
[[ noreturn ]]
void
bust_die (const vector <const char *> &m) {
    bust_void (m);
    exit (EXIT_FAILURE);
}


#ifdef use_old_vararg_version
/* ---------------- bust  ------------------------------------
 * Something broke.
 * Print out the name of the function and then a series of const char *args.
 * The caller must terminate the list with a zero (0).
 */

void
bust_void (const char *func, ...)
{
    std::string errmsg = std::string (func) + ": ";
    va_list tl;
    va_start (tl, func);
    const char *t = va_arg (tl, const char *);
    while (t != nullptr) {
        errmsg += t; errmsg += ' ';
        t = va_arg (tl, const char *);
    }
    va_end (tl);
    errmsg += '\n';
    std::cerr << errmsg;
}

int
bust (const char *func, ...)
{
    va_list sl;
    std::string errmsg = std::string (func) + ": ";
    va_start (sl, func);
    for ( const char *s = va_arg (sl, const char *); s != nullptr; s = va_arg (sl, const char *)) {
        errmsg += s;
        errmsg += ' ';
    }
    va_end (sl);
    errmsg += '\n';
    std::cerr << errmsg;
    return EXIT_FAILURE;
}
#endif /* use_old_vararg_version */

/* ---------------- file_open_read_excpt ---------------------
 * When opening a file fails.
 */
file_open_read_excpt::file_open_read_excpt (int err, const char *func_name,
                                            std::string fname) : runtime_error ("")
{
    std::error_code ec (err, std::generic_category());
    msg = std::string ("function: ") + func_name + ": error opening " + fname
        + " for reading:\n"+ ec.message();
}
const char*
file_open_read_excpt::what() const noexcept
{
    return msg.c_str();
}
