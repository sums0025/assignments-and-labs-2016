#include <iostream>
#include <cmath>
#include <string>
#include <sys/types.h>
using namespace std;

typedef unsigned int Vaddr_32;//32位
const int block_size = 1024;//磁盘块大小为1024B
const int element_block_count = 10;//一个间接块包含10个元素



struct single_indirect
{
	int iaddr[element_block_count];

	single_indirect()
	{
		for (int i = 0; i < element_block_count; ++i)
			iaddr[i] = -1;
	}
};


struct double_indirect
{
	struct single_indirect single_indirects[element_block_count];
};


struct triple_indirect
{
	struct double_indirect double_indirects[element_block_count];
};

struct inode
{
	string owners; // 持有者
	int timeStamps; // 创建时间
	int iaddr[element_block_count];
	struct single_indirect sib;
	struct double_indirect dib;
	struct triple_indirect tib;

	inode() :  owners(""), timeStamps(-1)
	{
		for (int i = 0; i < element_block_count; ++i)
			iaddr[i] = -1;
	}
};

void initInode(struct inode& node)
{

	node.owners = "zhangshuang";
	node.timeStamps = 20160610;
	int count = 0;
	for (int i = 0; i < element_block_count; ++i)
		node.iaddr[i] = count++;
	for (int i = 0; i < element_block_count; ++i)
		node.sib.iaddr[i] = count++;
	for (int i = 0; i < element_block_count; ++i)
		for (int j = 0; j < element_block_count; ++j)
			node.dib.single_indirects[i].iaddr[j] = count++;

}

void printInode(const struct inode& node)
{

	cout << "所有者：" << node.owners << endl;
	cout << "创建时间: " << node.timeStamps << endl;
	cout << "直接地址块： ";
	for (int i = 0; node.iaddr[i] != -1 && i < element_block_count; ++i)
		cout << node.iaddr[i] << " ";
	cout << endl;
}

int SWITCH(Vaddr_32 logicAddr, const struct inode& node,
	Vaddr_32& physicalBlockNo, Vaddr_32& physicalBlockOffset)
{
	Vaddr_32 idx = logicAddr / block_size;
	physicalBlockOffset = logicAddr % block_size;


	if (idx < 10)
	{
		if (node.iaddr[idx] == -1)
			return -1;
		physicalBlockNo = node.iaddr[idx];
		return 0;
	}

	else if (idx < 10 + element_block_count)
	{
		idx = idx - 10;
		if (node.sib.iaddr[idx] == -1)
			return -1;
		physicalBlockNo = node.sib.iaddr[idx];
		return 0;
	}

	else if (idx < 10 + element_block_count + element_block_count * element_block_count)
	{
		int i, j;
		idx = idx - 10 - element_block_count;
		i = idx / element_block_count;
		j = idx % element_block_count;
		if (node.dib.single_indirects[i].iaddr[j] == -1)
			return -1;
		physicalBlockNo = node.dib.single_indirects[i].iaddr[j];
		return 0;
	}

	else if (10 + element_block_count + element_block_count * element_block_count + element_block_count * element_block_count * element_block_count)
	{
		int i, j, k;
		idx = idx - 10 - element_block_count - element_block_count * element_block_count;

		i = idx / (element_block_count * element_block_count);
		idx %= (element_block_count * element_block_count);
		j = idx / element_block_count;
		k = idx % element_block_count;
		if (node.tib.double_indirects[i].single_indirects[j].iaddr[k] == -1)
			return -1;
		physicalBlockNo = node.tib.double_indirects[i].single_indirects[j].iaddr[k];
		return 0;
	}
	else
		return -1;

	return 0;
}

int main(int argc, char *argv[])
{
	struct inode node;
	initInode(node);
	printInode(node);

	Vaddr_32 logicAddr;
	Vaddr_32 physicalBlockNo;
	Vaddr_32 physicalBlockOffset;

	cout << "请输入文件逻辑地址" << endl;
	cin >> hex >> logicAddr;

	int ret = SWITCH(logicAddr, node, physicalBlockNo, physicalBlockOffset);

	cout << "******************************结果****************************" << ret << endl;
	if (ret == 0)
	{
		cout << "相对应的物理块号是： " << physicalBlockNo << endl;
		cout << "块内偏移量是：" << physicalBlockOffset << endl;
	}

	return 0;
}

