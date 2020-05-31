#define _CRT_SECURE_NO_WARNINGS//����scanf����
#include"Graph.h"
#include"Tourism.h"
#include<cstring>
#include<iostream>
#define INF 32767;
using namespace std;
CCgraph m_Graph;
//��ʼ��ͼ��Ϣ
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
//�����
bool InsertVex(Vex sVex)
{
	if (m_Graph.m_nVexNum == 32767)
		return false;
	m_Graph.m_aVexs[m_Graph.m_nVexNum++] = sVex;
	return true;
}
//�����
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
Ѱ����Χ���� �����ߴ����aEdge������
��Χ������Ŀ
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
//������ ��¼ĳ�������Ƿ񱻱����� ��������� �����õ��Ľ��
void DFS(int nVex, bool bVisited[], int &nIndex, PathList &pList) {
	bVisited[nVex] = true; //��Ϊ�ѷ���
	pList->cVex[nIndex++] = nVex; //���ʶ���nVex
	int nVexNum = 0;
	for (int i = 0; i < m_Graph.m_nVexNum; i++) {
		if (bVisited[i]) nVexNum++;
	}
	//�ж��Ƿ����ж��㶼���ʹ�
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
				//�ݹ���õõ�һ��·��
				DFS(i, bVisited, nIndex, pList);
				bVisited[i] = false; //�õ�һ��·���󽫷��ʵ�i����Ϊfalse
				nIndex--;//����
			}
		}
	}
}
//����4.�������·�� Dijkstra̰���㷨 ̰�Ĳ��ԡ������ӽṹ
int FindShortPath(int nVexStart, int nVexEnd, Edge aPath[]) {
	//��ʼ�����·��
	int flag[20],pre[20]; //������飬ǰ�ö���
	int dist[20],k;
	for (int i = 0; i < m_Graph.m_nVexNum; i++){ 
		flag[i] = 0; //Ĭ�϶�û���뼯��
		pre[i] = -1; 
		if (m_Graph.m_aAdjMatrix[nVexStart][i] > 0||i == nVexStart) {
			dist[i] = m_Graph.m_aAdjMatrix[nVexStart][i];//�������
			pre[i] = nVexStart;			//��������ֱ������ʱ��pre��ʼ��Ϊ���
		}
		else{
			dist[i] = INF;//�������
		}
	}
	flag[nVexStart] = 1; //��һ�������Ӧ����nStartVex��
	int min;
	//���� m_Graph.m_nVexNum-1�Σ�ÿ���ҳ�һ����������·��
	for (int i = 1; i < m_Graph.m_nVexNum; i++) {
		//Ѱ�ҵ�ǰ��С��·��
		min = INF;
		for (int j = 0; j < m_Graph.m_nVexNum; j++){
			if (flag[j]==0 && dist[j]<min){
				min = dist[j];
				k = j; //k���Ѿ�������·�����Ǹ���
			}
		}
		flag[k] = 1;
		if (k == nVexEnd) { //kΪ�յ㣬���رߵ���Ŀ
			break;
		}
		//��k��Ϊ�м�����nVexSart�����ж�������·�����������伯��
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
	int Num = 0; //�ܹ�Num��·��
	int i = nVexEnd;//���յ㿪ʼ����
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
//����5.�����·�滮
int FindMinTree(int nStart,Edge aEdge[]) {
	int flag[20],closest[20];//������� ǰ�ö���
	int lowcost[20];//�������
	for (int i = 0; i < m_Graph.m_nVexNum; i++) { //��ʼ��
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
	//�ҳ�m_Graph.m_nVexNum-1����
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