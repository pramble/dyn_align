/*
 * 27 Sep 2016
 */
#ifndef MGETLINE_HH
#define MGETLINE_HH

unsigned
mc_getline ( std::istream& is, std::string& str, const char cmmt);
std::istream&
n_getline (std::istream& is, std::string& str, char cmmt, unsigned &nl);
#endif /* MGETLINE_HH */
