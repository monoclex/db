#ifndef UNIX_DB_ERRORS_C
#define UNIX_DB_ERRORS_C

enum Errors
{
	Success = 0,
	ErrorCouldntGetFullPath = 1,
	NotEnoughArguments = 2,
	ErrorSegfault = 3,
};

#endif
