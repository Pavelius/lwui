#pragma once

namespace log {
extern bool	allowparse;
void		close();
int			geterrors();
void		error(const char* position, const char* format, ...);
void		errorv(const char* position, const char* format);
const char*	read(const char* url, bool error_if_not_exist = true);
void        setfile(const char* v);
void        seturl(const char* v);
}