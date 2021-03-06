#ifndef xrDebugH
#define xrDebugH
#pragma once

typedef	void		crashhandler	(void);
class XRCORE_API	xrDebug
{
private:
	crashhandler*	handler	;

public:
	void			_initialize		();
	void			_destroy		();
	
public:
	crashhandler*	get_crashhandler	()							{ return handler;	};
	void			set_crashhandler	(crashhandler* _handler)	{ handler=_handler;	};
	LPCSTR			error2string		(long  code	);
	void			fail				(const char *e1, const char *file, int line);
	void			fail				(const char *e1, const char *e2, const char *file, int line);
	void			fail				(const char *e1, const char *e2, const char *e3, const char *file, int line);
	void			error				(long  code, const char* e1, const char *file, int line);
	void _cdecl		fatal				(const char* F,...);
	void			backend				(const char* reason, const char* file, int line);
};

extern XRCORE_API	xrDebug		Debug;

#include "xrDebug_macros.h"

#endif
