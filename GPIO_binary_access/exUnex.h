#ifndef _EXUNEX_H_
#define _EXUNEX_H_

#define ERROR			-1
#define INSUFFICIENT_ARGS	-2
#define EXP_PATH_ERROR		-3
#define EXP_WRITE_ERROR		-4
#define UNEXP_PATH_ERROR	-5
#define UNEXP_WRITE_ERROR	-6

#define EXPORT_PATH	"/sys/class/gpio/export"
#define UNEXPORT_PATH	"/sys/class/gpio/unexport"

#ifndef FILE_WRITE
#define FILE_WRITE	"w"
#endif

#ifndef FILE_READ
#define FILE_READ	"r"
#endif

#endif	// _EXUNEX_H_
