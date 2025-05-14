#include <doctors.h>
#include <stdlib.h>
#include <string.h>

#define IsDigit(c)  (c >= '0' && c <= '9')

typedef struct DOCTOR_NODE DOCTOR_NODE;

struct DOCTOR_NODE
{
  DOCTOR_NODE *pLeft;
  DOCTOR_NODE *pRight;
  unsigned nDepth;
  DOCTOR Doctor;
};

struct DOCTOR_TREE
{
  DOCTOR_NODE *pRoot;
};

bool IsValidSchedule(const char *pSchedule)
{
  if (!IsDigit(pSchedule[0]) || !IsDigit(pSchedule[1]) || pSchedule[2] != ':'
      || !IsDigit(pSchedule[3]) || !IsDigit(pSchedule[4]) || pSchedule[5] != '-'
      || !IsDigit(pSchedule[6]) || !IsDigit(pSchedule[7]) || pSchedule[8] != ':'
      || !IsDigit(pSchedule[9]) || !IsDigit(pSchedule[10]) || pSchedule[11]) return false;
  int nBeginHour = (pSchedule[0] - '0') * 10 + pSchedule[1] - '0';
  int nBeginMinute = (pSchedule[3] - '0') * 10 + pSchedule[4] - '0';
  int nEndHour = (pSchedule[6] - '0') * 10 + pSchedule[7] - '0';
  int nEndMinute = (pSchedule[9] - '0') * 10 + pSchedule[10] - '0';
  return nBeginHour < 24 && nBeginMinute < 60 && nEndHour < 24 && nEndMinute < 60 && (nBeginHour < nEndHour || (nBeginHour == nEndHour && nBeginMinute < nEndMinute));
}

DOCTOR_TREE *CreateDoctorTree()
{
  DOCTOR_TREE *pTree = malloc(sizeof(DOCTOR_TREE));
  if (pTree)
  {
    pTree->pRoot = NULL;
  }
  return pTree;
}

DOCTOR_NODE *CreateDoctorNode(const DOCTOR *pDoctor)
{
  if (!pDoctor) return NULL;
  DOCTOR_NODE *pNode = malloc(sizeof(DOCTOR_NODE));
  if (pNode)
  {
    pNode->pLeft = NULL;
    pNode->pRight = NULL;
    pNode->nDepth = 1;
    pNode->Doctor = *pDoctor;
  }
  return pNode;
}

void DestroyDoctorNodes(DOCTOR_NODE *pNode)
{
  if (pNode)
  {
    DestroyDoctorNodes(pNode->pLeft);
    DestroyDoctorNodes(pNode->pRight);
    free(pNode);
  }
}

unsigned GetNodeDepth(const DOCTOR_NODE *pNode)
{
  return pNode ? pNode->nDepth : 0;
}

int GetNodeBalanceFactor(const DOCTOR_NODE *pNode)
{
  return pNode ? GetNodeDepth(pNode->pRight) - GetNodeDepth(pNode->pRight) : 0;
}

void UpdateNodeDepth(DOCTOR_NODE *pNode)
{
  if (pNode)
  {
    unsigned nLeftDepth = GetNodeDepth(pNode->pLeft);
    unsigned nRightDepth = GetNodeDepth(pNode->pRight);
    pNode->nDepth = (nLeftDepth > nRightDepth ? nLeftDepth : nRightDepth) + 1;
  }
}

DOCTOR_NODE *RotateNodeRight(DOCTOR_NODE *pNode)
{
  if (GetNodeDepth(pNode) < 2) return pNode;
  DOCTOR_NODE *pResult = pNode->pLeft;
  pNode->pLeft = pResult->pRight;
  pResult->pRight = pNode;
  UpdateNodeDepth(pNode);
  UpdateNodeDepth(pResult);
  return pResult;
}

DOCTOR_NODE *RotateNodeLeft(DOCTOR_NODE *pNode)
{
  if (GetNodeDepth(pNode) < 2) return pNode;
  DOCTOR_NODE *pResult = pNode->pRight;
  pNode->pRight = pResult->pLeft;
  pResult->pLeft = pNode;
  UpdateNodeDepth(pNode);
  UpdateNodeDepth(pResult);
  return pResult;
}

DOCTOR_NODE *BalanceNode(DOCTOR_NODE *pNode)
{
  UpdateNodeDepth(pNode);
  if (GetNodeBalanceFactor(pNode) == 2)
  {
    if (GetNodeBalanceFactor(pNode->pRight) < 0)
    {
      pNode->pRight = RotateNodeRight(pNode->pRight);
    }
    return RotateNodeLeft(pNode);
  }
  else if (GetNodeBalanceFactor(pNode) == -2)
  {
    if (GetNodeBalanceFactor(pNode->pLeft) > 0)
    {
      pNode->pLeft = RotateNodeLeft(pNode->pLeft);
    }
    return RotateNodeRight(pNode);
  }
  else return pNode;
}

void DestroyDoctorTree(DOCTOR_TREE *pTree)
{
  if (pTree)
  {
    DestroyDoctorNodes(pTree->pRoot);
    free(pTree);
  }
}

void ClearDoctorTree(DOCTOR_TREE *pTree)
{
  if (pTree)
  {
    DestroyDoctorNodes(pTree->pRoot);
    pTree->pRoot = NULL;
  }
}

bool GetDoctorCountCallback(const DOCTOR *pDoctor, void *pParams)
{
  return ++*(unsigned*)pParams;
}

unsigned GetDoctorCount(DOCTOR_TREE *pTree)
{
  unsigned nResult = 0;
  IterateDoctorTree(pTree, GetDoctorCountCallback, &nResult);
  return nResult;
}

DOCTOR_NODE *FindLeftMostNode(DOCTOR_NODE *pRoot)
{
  return pRoot->pLeft ? FindLeftMostNode(pRoot->pLeft) : pRoot;
}

DOCTOR_NODE *RemoveLeftMostNode(DOCTOR_NODE *pRoot)
{
  if (!pRoot->pLeft) return pRoot->pRight;
  pRoot->pLeft = RemoveLeftMostNode(pRoot->pLeft);
  return BalanceNode(pRoot);
}

DOCTOR_NODE *FindDoctor(DOCTOR_NODE *pRoot, const char *pFullName, bool bRemove, DOCTOR *pDoctor, bool *pbFound)
{
  if (!pRoot) return pRoot;
  int nDifference = strncmp(pFullName, pRoot->Doctor.szFullName, sizeof(pRoot->Doctor.szFullName));
  if (nDifference < 0)
  {
    pRoot->pLeft = FindDoctor(pRoot->pLeft, pFullName, bRemove, pDoctor, pbFound);
  }
  else if (nDifference > 0)
  {
    pRoot->pRight = FindDoctor(pRoot->pRight, pFullName, bRemove, pDoctor, pbFound);
  }
  else
  {
    *pbFound = true;
    if (pDoctor) *pDoctor = pRoot->Doctor;
    if (bRemove)
    {
      DOCTOR_NODE *pLeft = pRoot->pLeft;
      DOCTOR_NODE *pRight = pRoot->pRight;
      free(pRoot);
      if (!pRight) return pLeft;
      pRoot = FindLeftMostNode(pRight);
      pRoot->pRight = RemoveLeftMostNode(pRight);
      pRoot->pLeft = pLeft;
    }
  }
  return BalanceNode(pRoot);
}

bool GetDoctor(DOCTOR_TREE *pTree, const char *pFullName, bool bRemove, DOCTOR *pDoctor)
{
  if (!pTree) return false;
  bool bFound = false;
  pTree->pRoot = FindDoctor(pTree->pRoot, pFullName, bRemove, pDoctor, &bFound);
  return bFound;
}

DOCTOR_NODE *InsertNode(DOCTOR_NODE *pRoot, DOCTOR_NODE *pNode)
{
  if (!pRoot) return pNode;
  int nDifference = strncmp(pNode->Doctor.szFullName, pRoot->Doctor.szFullName, sizeof(pNode->Doctor.szFullName));
  if (nDifference < 0)
  {
    pRoot->pLeft = InsertNode(pRoot->pLeft, pNode);
  }
  else if (nDifference > 0)
  {
    pRoot->pRight = InsertNode(pRoot->pRight, pNode);
  }
  else
  {
    pRoot->Doctor = pNode->Doctor;
    free(pNode); /* A work around */
    return pRoot;
  }
  return BalanceNode(pRoot);
}

bool AddDoctor(DOCTOR_TREE *pTree, const DOCTOR *pDoctor)
{
  DOCTOR_NODE *pNode = CreateDoctorNode(pDoctor);
  if (!pTree || !pNode) return false;
  pTree->pRoot = InsertNode(pTree->pRoot, pNode);
  return true;
}

bool IterateDoctorNode(DOCTOR_NODE *pNode, DOCTOR_CALLBACK fnCallback, void *pParams)
{
  return !pNode || (IterateDoctorNode(pNode->pLeft, fnCallback, pParams) && fnCallback(&pNode->Doctor, pParams) && IterateDoctorNode(pNode->pRight, fnCallback, pParams));
}

bool IterateDoctorTree(DOCTOR_TREE *pTree, DOCTOR_CALLBACK fnCallback, void *pParams)
{
  return pTree && fnCallback && IterateDoctorNode(pTree->pRoot, fnCallback, pParams);
}
