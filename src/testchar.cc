/*
 * I have to work out the rules about type promotion and what gives warnings.
 */

#include <cstdlib>
#include <iostream>

using std::cout;
using std::string;
/* ---------------- main    ----------------------------------
 */
int main ()
{
    int x[100];
#   ifdef __clang__
#       pragma clang diagnostic ignored "-Wchar-subscripts"
#   endif
    x['A'] = 2;
    typedef unsigned char uchar_t;
    unsigned char u = uchar_t ('A');
    cout << "char is A value is " << x[u]<< " and the unsigned char is " << unsigned ('A') << "\n";
    return EXIT_SUCCESS;
}
