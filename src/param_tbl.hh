/*
 * 26 Sep 2016
 * Can only be included after param_entry.hh
 */
#ifndef PARAM_TBL_HH
#define PARAM_TBL_HH

/* ---------------- param_tbl_class --------------------------
 */
class param_tbl {
private:
    typedef std::map<std::string, param_entry> p_map;
    p_map parameters;
    void ini_params();
    p_map::iterator find_and_throw(const std::string &s);
public:
    param_tbl () {ini_params();}
    void set(const std::string &s, const std::string &val);
    void dump(std::ostream &of);
    param_entry & operator[](const std::string &s);


};

#endif  /* PARAM_TBL_HH */
