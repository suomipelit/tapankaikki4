#ifndef __CCOLORTABLE_H_
#define __CCOLORTABLE_H_

#include "CLightMap.h"
#include "CLightTable.h"

const int KLightAmount=10;

class CPalette;

class CColorTable
{
public:
	EXPORT CColorTable();

	EXPORT const CLightMap& LightMap(int aLightMap) const;
	EXPORT const CLightTable& NormalLight() const;
	EXPORT const CLightTable& RedLight() const;
	EXPORT const CLightTable& YellowLight() const;
	EXPORT const CLightTable& ExplosionLight() const;

private:
	void Init();
	void CreateTables(char *aPaletteFilename);
	int  ReadTables(char *aFilename);
	void WriteTables(char *aFilename);
	void GenerateShadowTable(CPalette& aPalette);
	void GenerateTransparencyTable(CPalette& aPalette);

public:
	unsigned char TransparencyTable[256][256];
	unsigned char ShadowTable[256][16];
private:
	CLightMap iLightMap[KLightAmount];
	CLightTable iNormalLight,iRedLight,iYellowLight,iExplosionLight;

};

#endif

