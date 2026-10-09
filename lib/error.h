#ifndef ERROR_H
#define ERROR_H

/* Nonfatal error related to system call
 * Print message and return */
void err_ret(const char *, ...);

/* Fatal error related to system call
 * Print message and terminate */
void err_sys(const char *, ...);

/* Fatal error related to system call
 * Print message, dump core, and terminate */
void err_dump(const char *, ...);

/* Nonfatal error unrelated to system call
 * Print message and return */
void err_msg(const char *, ...);

/* Fatal error unrelated to system call
 * Print message and terminate */
void err_quit(const char *, ...);

#endif