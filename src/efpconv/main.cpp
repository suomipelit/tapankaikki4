#include <stdlib.h>
#include "../common/CGraphicsBuffer.h"
#include "../common/CPalette.h"
#include "../common/error.h"


int main(int argc,char **argv)
{
	CGraphicsBuffer gbuf;
	CPalette pal;

	if (argc<3||argc>3) 
		error("Syntax:\n%s <fromfile> <tofile>\ne.g. efpconv file.gif file.efp\nformats supported: efp,efp2,pcx,8-bit png (loading only),gif (loading only),8-bit jpg (loading only), sci", argv[0]);
	gbuf.Load(argv[1],&pal);
	gbuf.Save(argv[2],&pal);
	return 0;
}
