CFLAGS +=	-DDATADIR=\"${DATADIR}\" `pkg-config --cflags SDL_image SDL_mixer sdl` -I/usr/local/include
LDFLAGS +=	`pkg-config --libs SDL_image SDL_mixer sdl` -lSDL_net

SRCS =	CBodyPart.cpp \
	CGUIStateGame.cpp \
	CGameModeDeathmatch.cpp \
	CGraphicsFader.cpp \
	CTargetSystemCrate.cpp \
	CBullet.cpp \
	CGUIStateLogo.cpp \
	CGameState.cpp \
	CIngameData.cpp \
	CWeaponCrate.cpp \
	CBulletCrate.cpp \
	CGUIStateQuit.cpp \
	CGameStatisticsData.cpp \
	CKeys.cpp \
	CWebVersionChecker.cpp \
	CCharacter.cpp \
	CGameApp.cpp \
	CGameSubStateLevelInfo.cpp \
	CLevelRuntimeData.cpp \
	Menu.cpp \
	CCrate.cpp \
	CGameData.cpp \
	CGameSubStateNextLevel.cpp \
	CMessageBoard.cpp \
	MenuItems.cpp \
	CDynGameData.cpp \
	CGameDrawManager.cpp \
	CGameSubStateOngoing.cpp \
	CMoneyCrate.cpp \
	Timer.cpp \
	CDynShopData.cpp \
	CGameDrawer.cpp \
	CGameSubStateQuit.cpp \
	COptions.cpp \
	lists.cpp \
	CEffect.cpp \
	CGameGraphicsInterface.cpp \
	CGameSubStateShop.cpp \
	CPlayer.cpp \
	main.cpp \
	CEnemy.cpp \
	CGameMenuBase.cpp \
	CGameSubStateStatistics.cpp \
	CSoundPlayer.cpp \
	network.cpp \
	CEnergyCrate.cpp \
	CGameMenuContainer.cpp \
	CGameSubStateVictory.cpp \
	CSplash.cpp \
	texts.cpp \
	CEventFilter.cpp \
	CGameMenuItem.cpp \
	CGameSystem.cpp \
	CStatViewData.cpp \
	CGUIEffectData.cpp \
	CGameModeCooperative.cpp \
	CGameVersion.cpp \
	CState.cpp \
	common/CBodyPartBase.cpp \
	common/CEnemyBase.cpp \
	common/CLevel.cpp \
	common/CSteam.cpp \
	common/CBulletBase.cpp \
	common/CEventHandler.cpp \
	common/CLightMap.cpp \
	common/CWeaponBase.cpp \
	common/CColorTable.cpp \
	common/CFonts.cpp \
	common/CLightTable.cpp \
	common/TypeInterfaces.cpp \
	common/CConfigFile.cpp \
	common/CGameTypeData.cpp \
	common/CMath.cpp \
	common/defines.cpp \
	common/CDrawArea.cpp \
	common/CGraphicsBuffer.cpp \
	common/CMouse.cpp \
	common/error.cpp \
	common/CEditableLevel.cpp \
	common/CGraphicsDevice.cpp \
	common/CPalette.cpp \
	common/files.cpp \
	common/CEffectBase.cpp \
	common/CGraphicsGenerator.cpp \
	common/CSpotlight.cpp \
	common/types.cpp

OBJS =		${SRCS:.cpp=.o}
TARGET = 	tkb

.SUFFIXES:	.cpp .o

all: ${TARGET}

${TARGET}: ${OBJS}
	${CXX} ${LDFLAGS} ${OBJS} -o ${TARGET}

.cpp.o:
	${CXX} ${CFLAGS} -c -o $@ ${@:.o=.cpp}

clean:
	rm -f ${OBJS} ${TARGET}
