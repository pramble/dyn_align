/*
 * 14 Jan 2016
 * You must include
 *   stdexcept, string
 * before this one
 */

#ifndef BUST_HH
#define BUST_HH

#ifdef use_old_vararg_version

int bust (const char *func, ...);
void bust_void (const char *func, ...);
#else

int bust (const std::vector<const char *> &m);
void bust_void (const std::vector<const char *> &m);
[[ noreturn ]]
void bust_die  (const std::vector<const char *> &m);
#endif /* use_old_vararg_version */


class file_open_read_excpt : std::runtime_error {
    std::string msg;
public:
    file_open_read_excpt (int err, const char *func_name, const std::string fname);
    const char* what() const noexcept; /* clang does not like override */
};

#endif /* BUST_HH */
