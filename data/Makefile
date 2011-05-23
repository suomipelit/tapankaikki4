.PHONY: genlist

.include "Makefile.inc"

install:
.       for I in ${DIRS}
		${INSTALL_DATA_DIR} ${DESTDIR}${DATADIR}/$I
.       endfor
.       for I in ${FILES}
		${INSTALL_DATA} $I ${DESTDIR}${DATADIR}/$I
.       endfor

genlist:
	@set -e ; \
	echo 'DIRS = \' > Makefile.inc; find * -type d | \
		sed 's/^/	/;$$!s/$$/ \\/' >> Makefile.inc ; \
	echo 'FILES = \' >> Makefile.inc; find * -type f -a \! \( -name Makefile.inc -o -name Makefile \) | \
	sed 's/^/	/;$$!s/$$/ \\/' >> Makefile.inc

