/*
 * 3 Sep 2016
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "bust.hh"
#include "seq.hh"

using namespace std;

/* ---------------- seq_part_set -----------------------------
 * Get a set of sequences.
 */
class seq_part_set {
private:
    vector <seq_part> seqs;
public:
    seq_part_set (const string infile_name);
};

/* ---------------- seq_part_set -----------------------------
 * Later, we have to add options for lower case masking,
 * ignoring or considering gaps.
 */
seq_part_set::seq_part_set (const string infile_name) {
    ifstream seq_file (infile_name);
    if (seq_file.fail()) {
        throw file_open_read_excpt (errno, __func__, infile_name); }

    seq_file.close();
}

/* ---------------- main    ----------------------------------
 * This is the fellow with coordinates.
 */
int main ()
{
    cout << "sizof base_seq is " << sizeof (seq_base)
         << "\nsizeof seq_part is "<< sizeof (seq_part) << "\n"
         << "\nsizeof seq_n    is "<< sizeof (seq_n)     << "\n"
         << "\nsizeof seq_x    is "<< sizeof (seq_x)    << "\n";

    const string infile_name = "/work/torda/people/petri/arc/allign_noseed/list_860.fa";
    try {
        seq_part_set seqs (infile_name);
    } catch (file_open_read_excpt &e) {
        return (bust ({e.what(), "\nStopping\n"} ));
    }
    return EXIT_SUCCESS;
}
