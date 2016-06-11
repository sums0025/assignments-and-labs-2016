#include <iostream>
#include <cmath>
using namespace std;
typedef unsigned long long Vaddr_64;
const unsigned int SPACE_SIZE = 64;

void* searchPageTable(Vaddr_64 pageNo, void* pageTable = NULL)
{
	return NULL;
}


Vaddr_64 getBitData(Vaddr_64 value, int start, int end)
{
	if (start < 0 || end < 0 || end - start <= 0)
		return 0;
	Vaddr_64 mask = pow(2, end - start + 1) - 1;
	value >>= start;

	return value & mask;
}


Vaddr_64 convertPagingLogicAddrToPhysicalAddr
(
Vaddr_64 pageItemSize /* Byte */,
Vaddr_64 pageSize /* Byte */,
Vaddr_64 pageTableLevel,
Vaddr_64 logicAddr,
Vaddr_64 &physicalBlockNo,
Vaddr_64 &physicalBlockOffset
)
{

	if (pageItemSize <= 0 || pageSize <= 0 || pageTableLevel <= 0)
		return -1;


	Vaddr_64 pageSizeBitCount = log(pageSize) / log(2);

	Vaddr_64 itemCountPerPage = pageSize / pageItemSize;
	Vaddr_64 itemCountPerPageBitCount = log(itemCountPerPage) / log(2);

	Vaddr_64 firstPageTableBitCount = SPACE_SIZE
		- itemCountPerPageBitCount * (pageTableLevel - 1)
		- pageSizeBitCount;

	int start = SPACE_SIZE - firstPageTableBitCount;
	int end = SPACE_SIZE - 1;
	Vaddr_64 firstPageNo = getBitData(logicAddr, start, end);
	Vaddr_64 lastPageNo = firstPageNo;
	void* lastPageTable = NULL;
	for (Vaddr_64 i = 1; i < pageTableLevel; ++i)
	{
		void* nextPageTable = searchPageTable(lastPageNo, lastPageTable);
		start = SPACE_SIZE - firstPageTableBitCount - itemCountPerPageBitCount * i;
		end = start + itemCountPerPageBitCount - 1;
		Vaddr_64 nextPageNo = getBitData(logicAddr, start, end);
		lastPageNo = nextPageNo;
		lastPageTable = nextPageTable;
	}

	physicalBlockNo = lastPageNo;
	physicalBlockOffset = getBitData(logicAddr, 0, pageSizeBitCount - 1);

	return 0;
}

int main(int argc, char *argv[])
{
	Vaddr_64 pageItemSize = 0;
	Vaddr_64 pageSize = 0;
	Vaddr_64 pageTableLevel = 0;
	Vaddr_64 logicAddr = 0;
	Vaddr_64 physicalBlockNo = 0;
	Vaddr_64 physicalBlockOffset = 0;
	cout << "请输入页记录大小" << endl;
	cin >> pageItemSize;
	cout << "请输入文件页面大小"  << endl;
	cin >> pageSize;
	cout << "请输入页表级数" << endl;
	cin >> pageTableLevel;
	cout << "请输入逻辑地址 " << endl;
	cin >> hex;
	cin >> logicAddr;

	Vaddr_64 ret = convertPagingLogicAddrToPhysicalAddr(pageItemSize, pageSize, pageTableLevel, logicAddr,
		physicalBlockNo, physicalBlockOffset);
	cout << "**********************结果************************* " << ret << endl;
	cout << "物理块号为： " << physicalBlockNo << endl;
	cout << "块内偏移为： " << physicalBlockOffset << endl;

	return 0;
}

