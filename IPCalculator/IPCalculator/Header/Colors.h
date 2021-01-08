#ifndef COLORS_H
#define COLORS_H

#define RESET		 "\033[0m"		   // &0
#define BLACK		 "\033[30m"		   // &1
#define RED			 "\033[31m"		   // &2
#define GREEN		 "\033[32m"		   // &3
#define YELLOW		 "\033[33m"		   // &4
#define BLUE		 "\033[34m"		   // &5
#define MAGENTA		 "\033[35m"		   // &6
#define CYAN		 "\033[36m"		   // &7
#define WHITE		 "\033[37m"		   // &8

#define BOLD_BLACK   "\033[1m\033[30m" // &a
#define BOLD_RED     "\033[1m\033[31m" // &b
#define BOLD_GREEN   "\033[1m\033[32m" // &c
#define BOLD_YELLOW  "\033[1m\033[33m" // &d
#define BOLD_BLUE    "\033[1m\033[34m" // &e
#define BOLD_MAGENTA "\033[1m\033[35m" // &f
#define BOLD_CYAN    "\033[1m\033[36m" // &g
#define BOLD_WHITE   "\033[1m\033[37m" // &h

#define UNDERLINED_WHITE   "\033[4;37m" // &i

constexpr unsigned int _hash(const char *s, int off = 0) {
	return !s[off] ? 5381 : (_hash(s, off + 1) * 33) ^ s[off];
}

#endif // COLORS_H
