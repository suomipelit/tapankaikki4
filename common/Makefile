CC=g++

CXXFLAGS=$(shell sdl-config --cflags) -D__LINUX__ -DEF_DEBUG=0 -D_snprintf=snprintf -D_getcwd=getcwd -Dstricmp=strcasecmp -g
LDFLAGS=$(sdl-config --libs) -lSDL_image

TARGETS = CBodyPartBase.o\
	 CBulletBase.o\
	 CColorTable.o\
	 CConfigFile.o\
	 CDrawArea.o\
	 CEditableLevel.o\
	 CEffectBase.o\
	 CEnemyBase.o\
	 CEventHandler.o\
	 CFonts.o\
	 CGameTypeData.o\
	 CGraphicsBuffer.o\
	 CGraphicsDevice.o\
	 CGraphicsGenerator.o\
	 CLevel.o\
	 CLightMap.o\
	 CLightTable.o\
	 CMath.o CMouse.o\
	 CPalette.o\
	 CSpotlight.o\
	 CSteam.o\
	 CWeaponBase.o\
	 defines.o\
	 error.o\
	 files.o\
	 TypeInterfaces.o\
	 types.o

all: libtk4common.so

libtk4common.so: $(TARGETS)
	$(CC) $(TARGETS) $(CXXFLAGS) $(LDFLAGS) -o libtk4common.so

clean:
	rm -f $(TARGETS)

CBodyPartBase.o: CBodyPartBase.h IBodyPartType.h defines.h linux.h windows.h

CBulletBase.o: CBulletBase.h defines.h CColorTable.h IBulletType.h linux.h windows.h CLightMap.h CLightTable.h CGameTypeData.h

CColorTable.o: CColorTable.h CPalette.h files.h CLightMap.h CLightTable.h defines.h error.h linux.h windows.h

CConfigFile.o: CConfigFile.h error.h defines.h linux.h windows.h

CDrawArea.o: CDrawArea.h CRect.h error.h CCoord.h defines.h CMath.h linux.h windows.h

CEditableLevel.o: CEditableLevel.h error.h CSpotlight.h CSteam.h CLevel.h defines.h CCoord.h CLinkedList.h CGameTypeData.h linux.h windows.h CMath.h

CEffectBase.o: CEffectBase.h defines.h IEffectType.h linux.h windows.h CGameTypeData.h

CEnemyBase.o: CLevel.h CEnemyBase.h CGameTypeData.h CCoord.h IEnemyType.h defines.h CMath.h linux.h windows.h

CEventHandler.o: CEventHandler.h CGraphicsDevice.h IEventInterface.h CMouse.h CGraphicsBuffer.h CDrawArea.h CPalette.h defines.h CRect.h error.h linux.h windows.h CCoord.h CMath.h

CFonts.o: CFonts.h CGraphicsBuffer.h CRect.h defines.h error.h CCoord.h linux.h windows.h CMath.h

CGameTypeData.o: error.h CGameTypeData.h Types.h defines.h IShield.h CEnemyBase.h CBulletBase.h CEffectBase.h CBodyPartBase.h CWeaponBase.h linux.h windows.h IEnemyType.h CColorTable.h IBulletType.h IEffectType.h IBodyPartType.h IWeaponType.h CLightMap.h CLightTable.h

CGraphicsBuffer.o: CGraphicsBuffer.h CPalette.h files.h defines.h CRect.h error.h linux.h windows.h CCoord.h CMath.h

CGraphicsDevice.o: CGraphicsDevice.h files.h CGraphicsBuffer.h CDrawArea.h CPalette.h defines.h CRect.h error.h linux.h windows.h CCoord.h CMath.h

CGraphicsGenerator.o: CGraphicsGenerator.h CGraphicsBuffer.h CLevel.h CPalette.h CLightTable.h CMath.h defines.h CRect.h CGameTypeData.h CCoord.h error.h linux.h windows.h

CLevel.o: CLevel.h error.h CSpotlight.h CSteam.h CMath.h CGameTypeData.h CCoord.h defines.h CLinkedList.h linux.h windows.h

CLightMap.o: CLightMap.h defines.h linux.h windows.h

CLightTable.o: CLightTable.h CPalette.h defines.h error.h linux.h windows.h

CMath.o: CMath.h defines.h linux.h windows.h

CMouse.o: CMouse.h CMath.h error.h defines.h linux.h windows.h

CPalette.o: CPalette.h CMath.h defines.h error.h linux.h windows.h

CSpotlight.o: CSpotlight.h error.h defines.h CCoord.h CLinkedList.h linux.h windows.h CMath.h

CSteam.o: CSteam.h CMath.h error.h defines.h CCoord.h CLinkedList.h linux.h windows.h

CWeaponBase.o: CWeaponBase.h error.h CGameTypeData.h IWeaponType.h defines.h IBulletType.h linux.h windows.h

defines.o: defines.h linux.h windows.h

error.o: error.h defines.h linux.h windows.h

files.o: files.h defines.h linux.h windows.h

TypeInterfaces.o: IShield.h IWeaponType.h IEnemyType.h IBodyPartType.h IEffectType.h IBulletType.h defines.h CGameTypeData.h linux.h windows.h

types.o: Types.h defines.h CColorTable.h CGameTypeData.h IShield.h CEnemyBase.h CBulletBase.h CEffectBase.h CBodyPartBase.h CWeaponBase.h linux.h windows.h CLightMap.h CLightTable.h IEnemyType.h IBulletType.h IEffectType.h IBodyPartType.h IWeaponType.h
