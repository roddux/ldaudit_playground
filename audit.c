// roddux 20201030
// reference the rtld-audit and ld.so manpages
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h> // PRI* format strings
#include <link.h> // LD_AUDIT stuff
#include <libgen.h> // basename()

#define DEBUG false
#define DBG(...) if(DEBUG) printf(__VA_ARGS__)
#define LIST_SIZE 2
#define PATH_MAX 4096

const uint8_t *blacklist[PATH_MAX] = {
	"something_else.so",
	"testlib.so"
};

// blacklist size assumed to be <256
bool in_blacklist(uint8_t *value) {
	DBG( "checking list\n" );
	for(uint8_t c; c<LIST_SIZE; c++) {
		uint8_t *cur = blacklist[c];
		uint8_t *nam = basename(value);
		DBG( "got basename '%s'\n", nam);
		DBG( "checking if '%s' == '%s'\n", nam, cur );
		if ( strncmp(nam, cur, PATH_MAX) == 0) {
			return true;
		}
	}
	return false;
}

enum {
	LA_SER_ORIG = 0x01,    // Original name
	LA_SER_LIBPATH = 0x02, // Directory from LD_LIBRARY_PATH
	LA_SER_RUNPATH = 0x04, // Directory from RPATH/RUNPATH
	LA_SER_CONFIG = 0x08,  // Found through ldconfig
	LA_SER_DEFAULT = 0x40, // Default directory
	LA_SER_SECURE = 0x80   // Unused
};

// This function must be defined
uint32_t la_version(uint32_t version) {
	DBG( "la_version called: %d\n", version );
	return version;
}

// Called for every dynamic library that is to be loaded
uint8_t *la_objsearch(
	uint8_t *name, uintptr_t *cookie, uint32_t flag
) {
	if(DEBUG) {
		printf("la_objsearch called\n");
		printf("  name: '%s'\n", name);
		printf("cookie: '%" PRIxPTR "'\n", cookie);
		printf("  flag: '%d' (", flag);
		switch(flag) {
			case LA_SER_ORIG:
				printf("LA_SER_ORIG)\n");
				break;
			case LA_SER_LIBPATH:
				printf("LA_SER_LIBPATH)\n");
				break;
			case LA_SER_RUNPATH:
				printf("LA_SER_RUNPATH)\n");
				break;
			case LA_SER_CONFIG:
				printf("LA_SER_CONFIG)\n");
				break;
			case LA_SER_DEFAULT:
				printf("LA_SER_DEFAULT)\n");
				break;
			case LA_SER_SECURE: // unused on Linux
				printf("LA_SER_SECURE)\n");
				break;
			default:
				printf("UNKNOWN)\n");
				break;
		}
	}
	// return the pathname of the library that we want to
	// load. we'll pass everything that isn't on our blacklist.
	if ( in_blacklist(name) ) {
		DBG("library is blacklisted! returning NULL\n");
		return NULL;
	} else {
		DBG("library not blacklisted, allowing\n");
	}
	return name;
}
