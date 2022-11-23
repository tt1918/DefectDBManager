
#ifdef BARCODE_VISION
#define LOT_NAME_LENGTH	10
#define BCR_POS_DATA_LENGTH 6
#define BCR_SCALE_MAX 0.06
#define BCR_SCALE_REF 0.045
#endif

enum eBCRPatRead
{
	eNone = -1,
	eFineRectOK,
	eReadDone,
	eForceReadDone,
};

void WEB_Barcode(LPVOID pParent);