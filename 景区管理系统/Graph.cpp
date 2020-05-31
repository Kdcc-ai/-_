#define _CRT_SECURE_NO_WARNINGS//消除scanf警告
#include"Graph.h"
#include"Tourism.h"
#include<cstring>
#include<iostream>
#define INF 32767;
using namespace std;
CCgraph m_Graph;
//初始化图信息
void Init(void)
{
	m_Graph.m_nVexNum = 0;
	int i;
	for (i = 0; i < 20; i++)
	{
		m_Graph.m_aVexs[i].num = -1;
		strcpy(m_Graph.m_aVexs[i].name, "");
		strcpy(m_Graph.m_aVexs[i].desc, "");
		for (int j = 0; j < 20; j++)
			m_Graph.m_aAdjMatrix[i][j] = 0;
		     
	}
}
//插入点
bool InsertVex(Vex sVex)
{
	if (m_Graph.m_nVexNum == 32767)
		return false;
	m_Graph.m_aVexs[m_Graph.m_nVexNum++] = sVex;
	return true;
}
//插入边
bool InsertEdge(Edge sEdge)
{
	if (sEdge.vex1 < 0 || sEdge.vex1 >= m_Graph.m_nVexNum ||
		sEdge.vex2 < 0 || sEdge.vex2 >= m_Graph.m_nVexNum)
		return false;
	m_Graph.m_aAdjMatrix[sEdge.vex1][sEdge.vex2] = sEdge.weight;
	m_Graph.m_aAdjMatrix[sEdge.vex2][sEdge.vex1] = sEdge.weight;
	return true;
}
Vex GetVex(int nVex){
	return m_Graph.m_aVexs[nVex];
}
/*
寻找周围景区 相连边存放在aEdge数组中
周围景区数目
*/
int FindEdge(int nVex, Edge aEdge[]) {
	int k = 0;
	for (int i = 0; i < m_Graph.m_nVexNum; i++)
	{
		if (m_Graph.m_aAdjMatrix[nVex][i] != 0)
		{
			aEdge[k].vex1 = nVex;
			aEdge[k].vex2 = i;
			aEdge[k].weight = m_Graph.m_aAdjMatrix[nVex][i];
			k++;
		}
	}
	return k;
}
void DFSTraverse(int nVex, PathList &pList) {
	bool bVisited[32767] = { false };
	int nIndex = 0;
	DFS(nVex, bVisited, nIndex, pList);
}
//顶点编号 记录某个顶点是否被遍历过 遍历的深度 遍历得到的结果
void DFS(int nVex, bool bVisited[], int &nIndex, PathList &pList) {
	bVisited[nVex] = true; //改为已访问
	pList->cVex[nIndex++] = nVex; //访问顶点nVex
	int nVexNum = 0;
	for (int i = 0; i < m_Graph.m_nVexNum; i++) {
		if (bVisited[i]) nVexNum++;
	}
	//判断是否所有顶点都访问过
	if (nVexNum == m_Graph.m_nVexNum) {
		pList->next = (PathList)malloc(sizeof(Path));
		for (int i = 0; i < m_Graph.m_nVexNum; i++){
			pList->next->cVex[i] = pList->cVex[i];
		}
		pList = pList->next;
		pList->next = NULL;
	}
	else{
		for (int i = 0; i < m_Graph.m_nVexNum; i++)
		{
			if (!bVisited[i]&&(m_Graph.m_aAdjMatrix[nVex][i]>0)&& (m_Graph.m_aAdjMatrix[nVex][i]!=32767)){
				//递归调用得到一条路径
				DFS(i, bVisited, nIndex, pList);
				bVisited[i] = false; //得到一条路径后将访问的i点置为false
				nIndex--;//回溯
			}
		}
	}
}
//功能4.搜索最短路径 Dijkstra贪心算法 贪心策略、最优子结构
int FindShortPath(int nVexStart, int nVexEnd, Edge aPath[]) {
	//初始化最短路径
	int flag[20],pre[20]; //标记数组，前置顶点
	int dist[20],k;
	for (int i = 0; i < m_Graph.m_nVexNum; i++){ 
		flag[i] = 0; //默认都没置入集合
		pre[i] = -1; 
		if (m_Graph.m_aAdjMatrix[nVexStart][i] > 0||i == nVexStart) {
			dist[i] = m_Graph.m_aAdjMatrix[nVexStart][i];//两点距离
			pre[i] = nVexStart;			//当两个边直接相连时，pre初始化为起点
		}
		else{
			dist[i] = INF;//两点距离
		}
	}
	flag[nVexStart] = 1; //第一个并入的应该是nStartVex点
	int min;
	//遍历 m_Graph.m_nVexNum-1次：每次找出一个顶点的最短路径
	for (int i = 1; i < m_Graph.m_nVexNum; i++) {
		//寻找当前最小的路径
		min = INF;
		for (int j = 0; j < m_Graph.m_nVexNum; j++){
			if (flag[j]==0 && dist[j]<min){
				min = dist[j];
				k = j; //k是已经获得最短路径的那个点
			}
		}
		flag[k] = 1;
		if (k == nVexEnd) { //k为终点，返回边的数目
			break;
		}
		//将k作为中间点计算nVexSart到所有顶点的最短路径，进行扩充集合
		for (int j = 0; j < m_Graph.m_nVexNum; j++) {
			int tmp;
			if (m_Graph.m_aAdjMatrix[k][j] == 0){
				tmp = INF;
			}
			else{
				tmp= min + m_Graph.m_aAdjMatrix[k][j];
			}
			if (flag[j]==0 && (tmp<dist[j])){
				dist[j] = tmp;
				pre[j] = k;
			}
		}
	}
	//cout << dist[nVexEnd] << endl;
	int Num = 0; //总共Num条路径
	int i = nVexEnd;//从终点开始回溯
	while (i != nVexStart){
		aPath[Num].vex2 = i;
		aPath[Num].vex1 = pre[i];
		aPath[Num].weight = m_Graph.m_aAdjMatrix[pre[i]][i];
		i = pre[i];
		Num++;
	}
	//cout << Num << endl;
	return Num;
}
//功能5.铺设电路规划
int FindMinTree(int nStart,Edge aEdge[]) {
	int flag[20],closest[20];//标记数组 前置顶点
	int lowcost[20];//最近顶点
	for (int i = 0; i < m_Graph.m_nVexNum; i++) { //初始化
		closest[i] = -1;
		flag[i] =  0 ;
		if (m_Graph.m_aAdjMatrix[nStart][i] > 0 || i == nStart) {
		  lowcost[i] = m_Graph.m_aAdjMatrix[nStart][i];
		  closest[i] = nStart;
		}
		else{
			lowcost[i] = INF;
		}
	}
	flag[nStart] = 1;
	int Num = 0;
	//找出m_Graph.m_nVexNum-1条边
	int min,k;
	for (int i = 1; i < m_Graph.m_nVexNum; i++) {
		min = INF;
		for (int j = 0; j < m_Graph.m_nVexNum; j++){
			if (flag[j]==0&&lowcost[j]<min){
				min = lowcost[j];
				k = j;
			}
		}
		flag[k] = 1;
		for (int j = 0; j <m_Graph.m_nVexNum ; j++){
			if (flag[j] == 0 && m_Graph.m_aAdjMatrix[k][j] < lowcost[j]&&m_Graph.m_aAdjMatrix[k][j]!=0) {
				lowcost[j] = m_Graph.m_aAdjMatrix[k][j];
				closest[j] = k;
			}
		}
		flag[closest[k]] = 1;
		aEdge[Num].vex1 = closest[k];
		aEdge[Num].vex2 = k;
		aEdge[Num].weight = m_Graph.m_aAdjMatrix[closest[k]][k];
		Num++;
	}
	return Num;
}