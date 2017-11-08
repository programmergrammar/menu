default: m.c
	gcc -o m cJSON.c m.c
2: m.c
	gcc -std=c11 -o2 -Wall -o m cJSON.c m.c -Wall -Wextra -Wpedantic \
          -Wformat=2 -Wno-unused-parameter -Wshadow \
          -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
          -Wredundant-decls -Wnested-externs -Wmissing-include-dirs
