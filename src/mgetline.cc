/*
 * 27 Sep 2016
 * Read lines, with variations.
 * - ignore everything after a comment indicator.
 * - add a count for the line number
 */

#include <cstring>
#include <istream>
#include <limits>
#include <stdexcept>
#include <string>

#include "mgetline.hh"

/* ---------------- rmv_white_start_end ----------------------
 * Remove leading and trailing space.
 * This modifies the string it is given.
 * We could use regex, but
 *  1. There are very few spaces to remove and
 *  2. Regex support is so messy amongst the different compilers I have tried.
 */
static std::string
rmv_white_start_end (std::string &s)
{
    size_t i, j;
    for (i = 0; i < s.length() && bool (isspace(s[i])); i++)
        /* just count */;

    if (i)
        s.erase(0, i);

    j = s.length() - 1;

    while (isspace(s[j]))
        s.erase(j--,1);
    return s;
}

/* ---------------- n_getline  -------------------------------
 * Getline, but
 * keep track of the line number
 * remove leading and trailing spaces
 * jump over empty lines
 * remove anything after the comment character
 */
std::istream&
n_getline (std::istream& is, std::string& str, char cmmt, unsigned &nl)
{
    do {
        unsigned i;
        if (getline (is, str))
            nl++;
        else
            return is;
        for (i = 0; i < str.length(); i++) /* delete after first */
            if (str[i] == cmmt)            /* comment */
                break;
        str.erase (i);
        rmv_white_start_end (str);
    } while (str.length() == 0);
    return is;
}

#ifdef want_mc_getline
#include <fstream>

/* ---------------- line_by_bytes ----------------------------
 * This is another variation, trying to avoid the problems of
 * contention within libstdc's getline(). We can avoid the
 * slowness of the mutex, but this does very slow input,
 * character by character from the file pointer.
 */
static void
line_by_bytes (std::istream &is, char *buf, const unsigned BSIZ)
{
    unsigned got = 0;
    char c = '\0';

    do {
        is.read (&c, 1);
        buf[got++] = c;
    } while ((got < BSIZ - 1) && (! is.eof()) && (c != '\n'));

    if (is.eof()) {
        buf[ got - 1] = '\0';
    } else if (c == '\n') {
        buf [ got -1] = '\0'; /* this overwrites the newline */
    } else {  /* our buffer was too small */
        buf[got] = '\0';
        is.setstate (is.rdstate() | std::ifstream::failbit);
    }

    return;
}


/* ---------------- mc_getline -------------------------------
 * This version of getline throws away anything after a comment
 * character
 * Usually, we will only have to read one line, but if necessary
 * we go in the do loop and read on.
 */
unsigned
mc_getline ( std::istream& is, std::string& str, const char cmmt)
{
    static const unsigned BSIZ = 256;
    char buf[BSIZ];
    str.clear();
    bool blank = false;
    bool do_comment = false;
    if (cmmt != '\0')
        do_comment = true;
    do {
        is.clear();
        blank = false;
#       ifdef use_slow_locks
            mtx.lock();
            is.getline (buf, BSIZ);
            mtx.unlock();
#       else    /* use the slow character reading, but no locks */
            line_by_bytes(is, buf, BSIZ);
#       endif /* use_slow_locks */
        if (do_comment) {
            const size_t len = strlen (buf);
            const char *end = buf + len;
            for (char *b = buf; b < end; b++)
                if (*b == cmmt) {
                    *b = '\0'; break; }
        }
        str.append (buf);        /* In the very rare cases that the line is */
        if (is.eof())            /* too long for the buffer, we will enter */
            break;               /* this loop. */
        if (is.bad())
            break; /* this should not happen. */
        if (buf[0] == '\0')      /* Jump over blank lines */
            blank = true;
    } while ( is.fail() || blank);

    if (str.size() > std::numeric_limits<unsigned>::max())
        throw std::runtime_error (std::string(__func__) + "Line too Long, starting " + str.substr(0, 10) );

    return (unsigned(str.size()));
}
#endif /* want_mc_getline */
