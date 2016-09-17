CFLAGS +=	`pkg-config --cflags SDL_image sdl gtk+-2.0` -I/usr/local/include
LDFLAGS +=	`pkg-config --libs SDL_image sdl gtk+-2.0` -lSDL_net

SRCS = 	EditorBrush.cpp \
	EditorGraphics.cpp \
	EditorTools.cpp \
	StateEditLevel.cpp \
	StateNewLevel.cpp \
	EditorClipBoard.cpp \
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

ifeq ($(OS), Windows_NT)
	SRCS += EditorLevelIO_win32.cpp
else
	SRCS += EditorLevelIO_gtk.cpp
endif

OBJS =		${SRCS:.cpp=.o}
TARGET = 	tkb-editor

.SUFFIXES:	.cpp .o

all: ${TARGET}

${TARGET}: ${OBJS}
	${CXX} ${OBJS} ${LDFLAGS} -o ${TARGET}

.cpp.o:
	${CXX} ${CFLAGS} -c -o $@ ${@:.o=.cpp}

clean:
	rm -f ${OBJS} ${TARGET}
