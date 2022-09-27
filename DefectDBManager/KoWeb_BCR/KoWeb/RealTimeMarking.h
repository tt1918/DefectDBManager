//-------------------------------------------------------------------------------------------
//	가변마킹 설정 데이타
//-------------------------------------------------------------------------------------------

#define MAX_DEFECT_SORT		9		//불량종류9종
#define MINY_INTERVAL		10		//USE 0 에서 사용하는 최소단위 10mm
#define MIN_PERIOD			0.33	//주기 최소단위 M
#define MAX_FOV				500		//400mm  ,FOV 최대 500mm
#define MIN_DEVIDE_FOV		2		//2mm    
#define RING_BUFFER_MM		2000.0	// 여유공간 2000mm
#define RING_BUFFER_M		2.0		// 여유공간, 2M

extern int l_nPeriodMarkingX[MAX_DEFECT_SORT][MAX_FOV/MIN_DEVIDE_FOV];		//1이면 마킹
extern int l_nMaxX;

void ChangeableMarkingReset();

void UpdateMarking(double dCurrentY_mm);
void InsertDefectData(int N, int *pType, float dLength, float *pY);

void InsertDefectDataForPeriod(int N, int *pType, float *pX, float dLength, float *pY);
void UpdateMarkingPeriod(double dCurrentY_M);