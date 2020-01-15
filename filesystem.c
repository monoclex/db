#ifndef UNIX_DB_FILESYSTEM_C
#define UNIX_DB_FILESYSTEM_C
#include "errors.c"

#if _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	//#include <WinBase.h> // CreateSymbolicLinkW, MoveFile
	//#include <errhandlingapi.h> // GetLastError
	//#include <fileapi.h> // GetFullPathName
	#include <direct.h> // _mkdir
#else
	#include <unistd.h> // unix standard stuff, eg. rename, symlink
	#include <errno.h> // errno - for getting error result in make_link
	#include <sys/stat.h> // mkdir
#endif

static char* path_full(const char* const path);
static int32_t make_dir(const char* const path);
static int32_t make_link(const char* const source_file, const char* const linked_file_name);
static int32_t move(const char* const source, const char* const destination);

#if _WIN32
static char* path_full(const char* const input)
{
	FUNCTION_IN("path_full");
	ULONG length = GetFullPathNameW((LPCWSTR)input, 0, 0, 0);

	if (length)
	{
		LPSTR buffer = (LPSTR)malloc((length) * sizeof(WCHAR));

		GetFullPathName(input, length * sizeof(WCHAR), buffer, NULL);
		FUNCTION_OUT("path_full");
		return (char*)buffer;
	}

	exit(ErrorCouldntGetFullPath);
	FUNCTION_OUT("path_full (NULL)");
	return NULL;
}

static int32_t make_dir(const char* const path)
{
	FUNCTION_IN("make_dir");
	int32_t result = _mkdir(path);
	FUNCTION_OUT("make_dir");
	return result;
}

static int32_t make_link(const char* source_file, const char* const linked_file_name)
{
	size_t error_code =
		CreateSymbolicLinkW((LPCWSTR)source_file, (LPCWSTR)linked_file_name, 0);

	if (error_code == 0) {
		return GetLastError();
	}

	return error_code;
}

static int32_t move(const char* const source, const char* const destination)
{
	return MoveFile((LPCTSTR)source, (LPCTSTR)destination);
}
#else
static char* path_full(const char* const path)
{
	char* resolved = malloc(10000);
	realpath(path, resolved);
	return resolved;
}

static int32_t make_dir(const char* const path)
{
	return mkdir(path, 0755);
}

static int32_t make_link(const char* const source_file, const char* const linked_file_name)
{
	if (symlink(source_file, linked_file_name) == 0) {
		return 0;
	}
	
	return errno;
}

static int32_t move(const char* const source, const char* const destination)
{
	return rename(source, destination);
}
#endif

#endif
