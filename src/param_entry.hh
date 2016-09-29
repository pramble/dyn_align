/*
 * 9 Sep 2016
 */
#ifndef PARAM_ENTRY_HH
#define PARAM_ENTRY_HH

#ifdef __clang__  /* The param_entry gets 3 bytes of padding. Hard to avoid */
#   pragma clang diagnostic ignored "-Wpadded"
#endif /* __clang__ */
/*
 * When a variable is born
    param_entry p = 5;
 *  it invokes the constructor.
 *  When we reassign to it, p = 7;, it invokes the assignment operator.
 */
#include <csignal>
class param_entry {
public:
    static const unsigned char invalid_t  = 0;
    static const unsigned char int_t      = 1;
    static const unsigned char float_t    = 2;
    static const unsigned char double_t   = 3;
    static const unsigned char unsigned_t = 4;
    static const unsigned char bool_t     = 5;
    static const unsigned char string_t   = 6;
private:
    std::string s;
    union mixed_t {
        int i;
        float f;
        unsigned u;
        bool b;
    } the_var;
    unsigned char this_t;
    const union mixed_t i_v = {-1};

    void set (const int i);
    void set (const float f   )     {the_var.f = f;}
    void set (const double d  )     {the_var.f = float(d); }
    void set (const unsigned u)     {the_var.u = u;}
    void set (const bool b)         {the_var.b = b;}
    void set (const char *ss)       {set (std::string (ss));}
    void set (const std::string &ss);
public:
    param_entry ()                : this_t(invalid_t),  s(),   the_var(i_v) {};
    param_entry (const int i)     : this_t(int_t),      s(),   the_var(i_v) { set(i);}
    param_entry (const float f)   : this_t(float_t),    s(),   the_var(i_v) { set(f);}
    param_entry (const double d)  : this_t(float_t),    s(),   the_var(i_v) { set(float(d));}
    param_entry (const unsigned u): this_t(unsigned_t), s(),   the_var(i_v) { set(u);}
    param_entry (const bool b)    : this_t(bool_t),     s(),   the_var(i_v) { set(b);}
    param_entry (const char *ss)  : this_t(string_t),   s(ss), the_var(i_v) { set(ss);}
//  param_entry (const std::string *ss) : this_t (string_t) s(ss), the_var(i_v) {set(ss);}

    /* If I clean this up, set should return the right type */

    float          operator =(float  f)         {set(f);   return f;}
    int            operator =(int    i)         {set(i);   return i;}
    double         operator =(double d)         {set(d);   return d;}
    unsigned       operator =(unsigned u )      {set(u);   return u;}
    bool           operator =(bool b)           {set(b);   return b;}
    char *         operator =(char *cp)         {set(cp);  return cp;}
    const char *   operator =(const char *cp)   {set (cp); return cp;}
    std::string    &operator =(std::string &ss) {set(ss);  return ss;}

    operator int()         { return the_var.i;} /* This funny overloading of  */
    operator float()       { return the_var.f;} /* the cast operator let's us */
    operator unsigned()    { return the_var.u;} /* return different types.    */
    operator bool()        { return the_var.b;}
    operator std::string() { return s;}

    friend std::ostream& operator<<(std::ostream& out, const param_entry o);
};

#endif /* PARAM_ENTRY_HH */
