CC ?=		g++
LD ?=		g++

DATADIR ?=	/usr/local/share/tkb
CFLAGS ?=	-Wall -O2
CFLAGS +=	-DDATADIR=\"${DATADIR}\" `pkg-config --cflags SDL_image sdl gtk+-2.0` -I/usr/local/include
LDFLAGS +=	`pkg-config --libs SDL_image sdl gtk+-2.0` -lSDL_net

SRCS = 	EditorBrush.cpp \
	EditorGraphics.cpp \
	EditorTools.cpp \
	StateEditLevel.cpp \
	StateNewLevel.cpp \
	EditorClipBoard.cpp \
	EditorLevelIO.cpp \
	EventFilter.cpp \
	StateEditRCData.cpp \
	StateRemoveObjects.cpp \
	EditorClipBoardBlock.cpp \
	EditorMagicSelection.cpp \
	StateAddObjects.cpp \
	StateGetBlock.cpp \
	editorapp.cpp \
	EditorDrawer.cpp \
	EditorSelection.cpp \
	StateEditGeneralData.cpp \
	StateHandler.cpp \
	main.cpp \
	EditorEvent.cpp \
	EditorState.cpp \
	StateEditHeights.cpp \
	StateHelp.cpp \
	../common/CBodyPartBase.cpp \
	../common/CEnemyBase.cpp \
	../common/CLevel.cpp \
	../common/CSteam.cpp \
	../common/CBulletBase.cpp \
	../common/CEventHandler.cpp \
	../common/CLightMap.cpp \
	../common/CWeaponBase.cpp \
	../common/CColorTable.cpp \
	../common/CFonts.cpp \
	../common/CLightTable.cpp \
	../common/TypeInterfaces.cpp \
	../common/CConfigFile.cpp \
	../common/CGameTypeData.cpp \
	../common/CMath.cpp \
	../common/defines.cpp \
	../common/CDrawArea.cpp \
	../common/CGraphicsBuffer.cpp \
	../common/CMouse.cpp \
	../common/error.cpp \
	../common/CEditableLevel.cpp \
	../common/CGraphicsDevice.cpp \
	../common/CPalette.cpp \
	../common/files.cpp \
	../common/CEffectBase.cpp \
	../common/CGraphicsGenerator.cpp \
	../common/CSpotlight.cpp \
	../common/types.cpp

OBJS =		${SRCS:.cpp=.o}
TARGET = 	tkb-editor

.SUFFIXES:	.cpp .o

all: ${TARGET}

${TARGET}: ${OBJS}
	${LD} ${LDFLAGS} ${OBJS} -o ${TARGET}

.cpp.o:
	${CC} ${CFLAGS} -c -o $@ ${@:.o=.cpp}

clean:
	rm -f ${OBJS} ${TARGET}
