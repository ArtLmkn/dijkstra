#ifndef __CONST_H__
#define __CONST_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

// Program constants
#define DEF_SIZE 30         // Default allocation size
#define LIM_SIZE 1000000    // Fast reallocations limit
#define BUF_SIZE 4096       // Write buffer size
#define BUF_STOCK 64        // Write buffer stock
#define NONE -1             // Empty value
#define DIGITS "0123456789" // Decimal digits
#define NEG '-'             // "Negative sign" character
#define EOL '\n'            // "End of line" character
#define WSC ' '             // "Whitespace" character

// Micro functions
#define PARENT(i) ((i - 1) >> 1)    // Get parrent of element with index i
#define CHILD(i) ((i << 1) + 1)     // Get left child of element with index i

// Error texts
#define MEM_ERR_MSG "Error: Can't allocate memory!%c", EOL
#define FILE_OPEN_ERR_MSG "Error: Can't open file!%c", EOL
#define FILE_CLOSE_ERR_MSG "Error: Can't close file!%c", EOL

#endif // __CONST_H__
