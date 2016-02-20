CXX ?=			g++
CFLAGS ?=		-Wall -O2

PREFIX =		/usr/local
DATADIR ?=		${PREFIX}/share/tkb

INSTALL_DATA_DIR =	install -d -m 755
INSTALL_PROGRAM_DIR =	install -d -m 755
INSTALL_DATA =		install -c -m 444
INSTALL_PROGRAM =	install -c -s -m 555

MY_MAKE_ENV =	CXX="${CXX}" \
		CFLAGS="${CFLAGS}" \
		INSTALL_DATA_DIR="${INSTALL_DATA_DIR}" \
		INSTALL_DATA="${INSTALL_DATA}" \
		INSTALL_PROGRAM_DIR="${INSTALL_PROGRAM_DIR}" \
		INSTALL_PROGRAM="${INSTALL_PROGRAM}" \
		DESTDIR="${DESTDIR}" \
		PREFIX="${PREFIX}" \
		DATADIR="${DATADIR}"
MY_MAKEFLAGS =

all: game editor

game:
	cd src && env ${MY_MAKE_ENV} ${MAKE} ${MAKEFLAGS} ${MY_MAKEFLAGS}

editor:
	cd src/Editor && env ${MY_MAKE_ENV} ${MAKE} ${MAKEFLAGS} ${MY_MAKEFLAGS}

install:
	${INSTALL_PROGRAM_DIR} ${DESTDIR}${PREFIX}/bin
	${INSTALL_PROGRAM} src/tkb ${DESTDIR}${PREFIX}/bin
	if test -s src/Editor/tkb-editor; then \
	${INSTALL_PROGRAM} src/Editor/tkb-editor ${DESTDIR}${PREFIX}/bin; fi
	${INSTALL_DATA_DIR} ${DESTDIR}${DATADIR}
	${INSTALL_DATA} COPYING ${DESTDIR}${DATADIR}/
	cd data && env ${MY_MAKE_ENV} ${MAKE} ${MAKEFLAGS} ${MY_MAKEFLAGS} install
