PREFIX =	/usr/local
DATADIR ?=	${PREFIX}/share/tkb
MAKE_FLAGS =	DATADIR="${DATADIR}"
MAKE_ENV =	INSTALL_DATA_DIR="${INSTALL_DATA_DIR}" \
		INSTALL_DATA="${INSTALL_DATA}" \
		INSTALL_PROGRAM_DIR="${INSTALL_PROGRAM_DIR}" \
		INSTALL_PROGRAM="${INSTALL_PROGRAM}" \
		DESTDIR="${DESTDIR}" \
		PREFIX="${PREFIX}" \
		DATADIR="${DATADIR}"

INSTALL_DATA_DIR =	install -d -o root -g bin -m 755
INSTALL_PROGRAM_DIR =	install -d -o root -g bin -m 755
INSTALL_DATA =		install -c -o root -g bin -m 444
INSTALL_PROGRAM =	install -c -s -o root -g bin -m 555

all: game editor

game:
	cd src && env -i ${MAKE_ENV} ${MAKE} ${MAKE_FLAGS}

editor:
	cd src/Editor && env -i ${MAKE_ENV} ${MAKE} ${MAKE_FLAGS}

install:
	${INSTALL_PROGRAM_DIR} ${DESTDIR}${PREFIX}/bin
	${INSTALL_PROGRAM} src/tkb src/Editor/tkb-editor ${DESTDIR}${PREFIX}/bin
	${INSTALL_DATA_DIR} ${DESTDIR}${DATADIR}
	${INSTALL_DATA} COPYING ${DESTDIR}${DATADIR}/
	cd data && env -i ${MAKE_ENV} ${MAKE} ${MAKE_FLAGS} install
