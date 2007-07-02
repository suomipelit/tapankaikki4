CC=g++
CXXFLAGS=$(shell sdl-config --cflags) -D__LINUX__ -DEF_DEBUG=0 -D_snprintf=snprintf -D_getcwd=getcwd -Dstricmp=strcasecmp -g `pkg-config --cflags gtk+-2.0`
LDFLAGS=$(shell sdl-config --libs) -lSDL_image $(shell pkg-config --libs gtk+-2.0) -L../common/ -ltk4common

TARGETS = editorapp.o\
	 EditorBrush.o\
	 EditorClipBoardBlock.o\
	 EditorClipBoard.o\
	 EditorDrawer.o\
	 EditorEvent.o\
	 EditorGraphics.o\
	 EditorLevelIO.o\
	 EditorMagicSelection.o\
	 EditorSelection.o\
	 EditorState.o\
	 EditorTools.o\
	 EventFilter.o\
	 main.o\
	 StateAddObjects.o\
	 StateEditGeneralData.o\
	 StateEditHeights.o\
	 StateEditLevel.o\
	 StateEditRCData.o\
	 StateGetBlock.o\
	 StateHandler.o\
	 StateHelp.o\
	 StateNewLevel.o\
	 StateRemoveObjects.o

all: $(TARGETS)
	$(CC) $(TARGETS) $(CFLASG) $(LDFLAGS) -o editor

clean:
	rm -f $(TARGETS)


editorapp.o: editorapp.h ../common/CGraphicsDevice.h ../common/CEventHandler.h ../common/error.h ../common/CEditableLevel.h ../common/CMath.h EventFilter.h EditorGraphics.h EditorDrawer.h StateHandler.h EditorState.h EditorLevelIO.h EditorBrush.h EditorSelection.h EditorMagicSelection.h EditorTools.h EditorClipBoard.h ../common/CGraphicsBuffer.h ../common/CDrawArea.h ../common/CPalette.h ../common/CMouse.h ../common/defines.h ../common/CLevel.h ../common/IEventInterface.h ../common/CFonts.h EditorClipBoardBlock.h ../common/CRect.h ../common/linux.h ../common/windows.h ../common/CGameTypeData.h ../common/CCoord.h

EditorBrush.o: EditorBrush.h EditorGraphics.h EditorSelection.h ../common/CEditableLevel.h ../common/error.h ../common/CLevel.h ../common/defines.h ../common/CGameTypeData.h ../common/CCoord.h ../common/linux.h ../common/windows.h ../common/CMath.h

EditorClipBoardBlock.o: EditorClipBoardBlock.h ../common/CEditableLevel.h ../common/CLevel.h ../common/CGameTypeData.h ../common/CCoord.h ../common/defines.h ../common/CMath.h ../common/linux.h ../common/windows.h

EditorClipBoard.o: EditorClipBoard.h EditorSelection.h ../common/CEditableLevel.h EditorClipBoardBlock.h ../common/CLevel.h ../common/CGameTypeData.h ../common/CCoord.h ../common/defines.h ../common/CMath.h ../common/linux.h ../common/windows.h

EditorDrawer.o: EditorDrawer.h EditorGraphics.h editorapp.h EditorSelection.h EditorBrush.h EditorClipBoardBlock.h ../common/CGraphicsBuffer.h ../common/CGraphicsDevice.h ../common/CEditableLevel.h ../common/error.h ../common/CFonts.h ../common/CSpotlight.h ../common/CSteam.h ../common/CMath.h ../common/CGameTypeData.h ../common/files.h ../common/IBulletType.h ../common/IEnemyType.h ../common/IWeaponType.h ../common/CLevel.h ../common/defines.h ../common/CRect.h ../common/CDrawArea.h ../common/CPalette.h ../common/CCoord.h ../common/CLinkedList.h ../common/linux.h ../common/windows.h

EditorEvent.o: EditorEvent.h

EditorGraphics.o: EditorGraphics.h ../common/CGraphicsGenerator.h ../common/CGraphicsBuffer.h ../common/CPalette.h ../common/CFonts.h ../common/CConfigFile.h ../common/files.h ../common/defines.h ../common/CRect.h ../common/error.h ../common/linux.h ../common/windows.h ../common/CCoord.h ../common/CMath.h

EditorLevelIO.o: ../common/CEditableLevel.h EditorLevelIO.h ../common/CGraphicsDevice.h ../common/error.h ../common/files.h EditorDrawer.h EditorGraphics.h ../common/CLevel.h ../common/CGraphicsBuffer.h ../common/CDrawArea.h ../common/CPalette.h ../common/defines.h ../common/CFonts.h ../common/CGameTypeData.h ../common/CCoord.h ../common/CRect.h ../common/linux.h ../common/windows.h ../common/CMath.h

EditorMagicSelection.o: EditorMagicSelection.h ../common/CEditableLevel.h EditorSelection.h ../common/CLevel.h ../common/CGameTypeData.h ../common/CCoord.h ../common/defines.h ../common/CMath.h ../common/linux.h ../common/windows.h

EditorSelection.o: EditorSelection.h

EditorState.o: EditorState.h

EditorTools.o: EditorTools.h

EventFilter.o: EventFilter.h ../common/CGraphicsDevice.h EditorDrawer.h EditorLevelIO.h StateHandler.h EditorState.h editorapp.h EditorSelection.h ../common/IEventInterface.h ../common/CGraphicsBuffer.h ../common/CDrawArea.h ../common/CPalette.h ../common/CFonts.h ../common/defines.h ../common/CRect.h ../common/error.h ../common/linux.h ../common/windows.h ../common/CCoord.h ../common/CMath.h

main.o: editorapp.h ../common/defines.h ../common/linux.h ../common/windows.h

StateAddObjects.o: StateAddObjects.h EditorDrawer.h EditorBrush.h StateHandler.h ../common/CColorTable.h ../common/CEditableLevel.h ../common/CMath.h ../common/CEventHandler.h editorapp.h EditorState.h ../common/CFonts.h ../common/CLightMap.h ../common/CLightTable.h ../common/CLevel.h ../common/defines.h ../common/CMouse.h ../common/CRect.h ../common/CGameTypeData.h ../common/CCoord.h ../common/linux.h ../common/windows.h ../common/error.h

StateEditGeneralData.o: StateEditGeneralData.h EditorDrawer.h EditorBrush.h StateHandler.h ../common/CEditableLevel.h ../common/CEventHandler.h EditorState.h editorapp.h ../common/CFonts.h ../common/CLevel.h ../common/CMouse.h ../common/CRect.h ../common/CGameTypeData.h ../common/CCoord.h ../common/defines.h ../common/error.h ../common/CMath.h ../common/linux.h ../common/windows.h

StateEditHeights.o: StateEditHeights.h EditorDrawer.h EditorBrush.h EditorGraphics.h StateHandler.h EditorSelection.h EditorMagicSelection.h EditorTools.h ../common/CEditableLevel.h ../common/CEventHandler.h ../common/CMath.h EditorState.h ../common/CFonts.h editorapp.h ../common/CLevel.h ../common/CMouse.h ../common/defines.h ../common/CRect.h ../common/CGameTypeData.h ../common/CCoord.h ../common/linux.h ../common/windows.h ../common/error.h

StateEditLevel.o: StateEditLevel.h EditorDrawer.h EditorBrush.h StateHandler.h editorapp.h EditorTools.h EditorMagicSelection.h EditorSelection.h EditorClipBoard.h ../common/CEditableLevel.h ../common/CEventHandler.h ../common/CMath.h EditorState.h ../common/CFonts.h EditorClipBoardBlock.h ../common/CLevel.h ../common/CMouse.h ../common/defines.h ../common/CRect.h ../common/CGameTypeData.h ../common/CCoord.h ../common/linux.h ../common/windows.h ../common/error.h

StateEditRCData.o: editorapp.h StateEditRCData.h EditorDrawer.h EditorBrush.h StateHandler.h ../common/CEditableLevel.h ../common/CEventHandler.h EditorState.h ../common/CFonts.h ../common/CLevel.h ../common/CMouse.h ../common/CRect.h ../common/CGameTypeData.h ../common/CCoord.h ../common/defines.h ../common/error.h ../common/CMath.h ../common/linux.h ../common/windows.h

StateGetBlock.o: StateEditLevel.h EditorDrawer.h StateGetBlock.h EditorBrush.h StateHandler.h ../common/CEventHandler.h EditorState.h ../common/CFonts.h ../common/CEditableLevel.h editorapp.h ../common/CMouse.h ../common/CRect.h ../common/CLevel.h ../common/defines.h ../common/error.h ../common/CCoord.h ../common/CGameTypeData.h ../common/linux.h ../common/windows.h ../common/CMath.h

StateHandler.o: StateHandler.h StateEditLevel.h StateGetBlock.h StateEditHeights.h StateAddObjects.h StateRemoveObjects.h StateNewLevel.h StateEditRCData.h StateEditGeneralData.h StateHelp.h EditorTools.h EditorClipBoard.h ../common/error.h editorapp.h EditorState.h ../common/CEditableLevel.h EditorClipBoardBlock.h ../common/defines.h ../common/CLevel.h ../common/linux.h ../common/windows.h ../common/CGameTypeData.h ../common/CCoord.h ../common/CMath.h

StateHelp.o: StateHelp.h EditorDrawer.h EditorBrush.h StateHandler.h ../common/CEditableLevel.h ../common/CEventHandler.h EditorState.h editorapp.h ../common/CFonts.h ../common/CLevel.h ../common/CMouse.h ../common/CRect.h ../common/CGameTypeData.h ../common/CCoord.h ../common/defines.h ../common/error.h ../common/CMath.h ../common/linux.h ../common/windows.h

StateNewLevel.o: StateNewLevel.h EditorDrawer.h EditorBrush.h EditorGraphics.h StateHandler.h ../common/CEditableLevel.h ../common/CEventHandler.h EditorState.h ../common/CFonts.h editorapp.h ../common/CLevel.h ../common/CMouse.h ../common/CRect.h ../common/CGameTypeData.h ../common/CCoord.h ../common/defines.h ../common/error.h ../common/CMath.h ../common/linux.h ../common/windows.h

StateRemoveObjects.o: StateRemoveObjects.h EditorDrawer.h StateHandler.h ../common/CEditableLevel.h ../common/CMath.h ../common/CEventHandler.h ../common/CSpotlight.h ../common/CSteam.h editorapp.h EditorState.h ../common/CFonts.h ../common/CLevel.h ../common/defines.h ../common/CMouse.h ../common/CCoord.h ../common/CLinkedList.h ../common/CRect.h ../common/CGameTypeData.h ../common/linux.h ../common/windows.h ../common/error.h
