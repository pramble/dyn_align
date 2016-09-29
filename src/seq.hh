/*
 * 2 sep 2016
 * We store sequences in potentially different forms, so we have a base class,
 * but expect to use the seq_part or maybe nseq types.
 *
 * Can only be included after
 *  string, vector
 */
#ifndef SEQ_HH
#define SEQ_HH

/* ---------------- seq_base ---------------------------------
 * Some information will be common to the sequences, so we
 * only store one copy in a static variable.
 * Do not put the file handle (ifstream object) there. It
 * makes the object hard to copy and handle.
 */
class seq_base {
private:
    std::string cmmt;
protected:
    static bool mask_lower_case;
    static std::string infile_name; /* mainly for error messages */
public:
    std::string get_cmmt() { return cmmt; }
    std::string get_seq() { return "broken"; }
    void set_infile_name (const char *s) { infile_name = s;}
    const std::string get_infile_name() { return (infile_name);}
    unsigned get_len() {return 0;}
};

class seq_n : seq_base {  /* seq_n for normal, the sequence is in a string */
private:
    std::string seq;
public:
    std::string get_seq() { return seq;}
    unsigned get_len() { return unsigned(seq.length());}
};


class seq_part : seq_base {
private:
    size_t start, len;
    static std::vector <char> seq_mother;
public:
    std::string get_seq() { return std::string(&seq_mother [start], len);}
    size_t get_len() const { return len; }
};


/* ---------------- seq_x   ----------------------------------
 * This is the fellow with coordinates.
 */
class seq_x : seq_part {
private:
    static std::vector <float> x_mother;   /* This big array is shared amongst members */
    static std::vector <float> v_mother;   /* velocities */
    static std::vector <float> f_mother;   /* force / acceleration */
public:
    static const std::vector<float> get_x() {return x_mother;}
};

#endif /* SEQ_HH */
