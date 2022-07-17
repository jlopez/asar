// note for asar devs: autogenerated by update_c_bind.py, don't edit this
// directly! either edit asardll.c.in or interface-lib.cpp.
#include <stdbool.h>
#include <stddef.h>

#if defined(_WIN32)
#	if defined(_MSC_VER)
#		pragma warning(push)
#		pragma warning(disable : 4255)
#		pragma warning(disable : 4668)
#	endif

#	include <windows.h>
#	include <stdio.h>

#	if defined(_MSC_VER)
#		pragma warning(pop)
#	endif

inline static void * getlib(void)
{
	void * ret_val = LoadLibraryW(L"asar.dll");

	if (ret_val == NULL)
	{
		// TODO: Add a better method of error checking? This won't do much for people who are using
		// Asar with a GUI application, they probably won't see this error output.
		char buf[1024];
		sprintf(buf, "Failed to load Asar DLL! HRESULT: 0x%08x\n", (unsigned int)HRESULT_FROM_WIN32(GetLastError()));
		printf("%s", buf);
		OutputDebugStringA(buf);
	}

	return ret_val;
}

inline static void * getlibfrompath(const char * path)
{
	int required_size = MultiByteToWideChar(CP_UTF8, 0, path, -1, NULL, 0);
	if (required_size <= 0) return NULL;

	wchar_t* path_buf = (wchar_t*)malloc((size_t)required_size * sizeof(wchar_t));
	if (path_buf == NULL) return NULL;

	int converted_size = MultiByteToWideChar(CP_UTF8, 0, path, -1, path_buf, required_size);

	if (converted_size == 0)
	{
		return NULL;
	}

	void * ret_val = LoadLibraryW(path_buf);
	free(path_buf);

	if (ret_val == NULL)
	{
		// TODO: Add a better method of error checking? This won't do much for people who are using
		// Asar with a GUI application, they probably won't see this error output.
		char buf[1024];
		sprintf(buf, "Failed to load Asar DLL! HRESULT: 0x%08x\n", (unsigned int)HRESULT_FROM_WIN32(GetLastError()));
		printf("%s", buf);
		OutputDebugStringA(buf);
	}

	return ret_val;
}

inline static bool setfunction(void* target, FARPROC fn)
{
	memcpy(target, &fn, sizeof(fn));
	return fn;
}
#	define loadraw(name, target) require(setfunction(&target, GetProcAddress((HINSTANCE)asardll, name)))
#	define closelib(var) FreeLibrary((HINSTANCE)var)
#else
#	include <dlfcn.h>
#	include <stdio.h>

#	ifdef __APPLE__
#		define EXTENSION ".dylib"
#	else
#		define EXTENSION ".so"
#	endif

	inline static void * getlib(void)
	{
		const char * names[]={"./libasar"EXTENSION, "libasar", NULL};
		for (int i=0;names[i];i++)
		{
			void * rval=dlopen(names[i], RTLD_LAZY);
			const char*e=dlerror();
			if(e)puts(e);
			if (rval) return rval;
		}
		return NULL;
	}

	inline static void * getlibfrompath(const char * path)
	{
		void * rval = dlopen(path, RTLD_LAZY);
		const char*e = dlerror();
		if (e)puts(e);
		if (rval) return rval;
		return NULL;
	}

#	define loadraw(name, target) *(void **)(&target)=dlsym(asardll, name); require(target)
#	define closelib(var) dlclose(var)
#endif

#include "asardll.h"

static void * asardll=NULL;

static bool (*asar_i_init)(void);
static void(*asar_i_close)(void);
int (*asar_version)(void);
int (*asar_apiversion)(void);
bool (*asar_reset)(void);
bool (*asar_patch)(const struct patchparams *params);
int (*asar_maxromsize)(void);
const struct errordata * (*asar_geterrors)(int * count);
const struct errordata * (*asar_getwarnings)(int * count);
const char * const * (*asar_getprints)(int * count);
const struct labeldata * (*asar_getalllabels)(int * count);
int (*asar_getlabelval)(const char * name);
const char * (*asar_getdefine)(const char * name);
const char * (*asar_resolvedefines)(const char * data);
const struct definedata * (*asar_getalldefines)(int * count);
double (*asar_math)(const char * math_, const char ** error);
const struct writtenblockdata * (*asar_getwrittenblocks)(int * count);
enum mappertype (*asar_getmapper)(void);
const char * (*asar_getsymbolsfile)(const char* type);

#define require(b) if (!(b)) { asardll=NULL; return false; }

static bool asar_init_shared(void)
{
	loadraw("asar_init", asar_i_init);
	loadraw("asar_close", asar_i_close);
	loadraw("asar_version", asar_version);
	loadraw("asar_apiversion", asar_apiversion);
	loadraw("asar_reset", asar_reset);
	loadraw("asar_patch", asar_patch);
	loadraw("asar_maxromsize", asar_maxromsize);
	loadraw("asar_geterrors", asar_geterrors);
	loadraw("asar_getwarnings", asar_getwarnings);
	loadraw("asar_getprints", asar_getprints);
	loadraw("asar_getalllabels", asar_getalllabels);
	loadraw("asar_getlabelval", asar_getlabelval);
	loadraw("asar_getdefine", asar_getdefine);
	loadraw("asar_resolvedefines", asar_resolvedefines);
	loadraw("asar_getalldefines", asar_getalldefines);
	loadraw("asar_math", asar_math);
	loadraw("asar_getwrittenblocks", asar_getwrittenblocks);
	loadraw("asar_getmapper", asar_getmapper);
	loadraw("asar_getsymbolsfile", asar_getsymbolsfile);
	if (asar_apiversion() < expectedapiversion || (asar_apiversion() / 100) > (expectedapiversion / 100)) return false;
	require(asar_i_init());
	return true;
}

bool asar_init(void)
{
	if (asardll) return true;
	asardll=getlib();
	require(asardll);
	if (!asar_init_shared()) return false;
	return true;
}

bool asar_init_with_dll_path(const char * dllpath)
{
	if (asardll) return true;
	asardll = getlibfrompath(dllpath);
	require(asardll);
	if (!asar_init_shared()) return false;
	return true;
}

void asar_close(void)
{
	if (!asardll) return;
	asar_i_close();
	closelib(asardll);
	asardll=NULL;
}
