#include <appointments.h>
#include <stdlib.h>

typedef struct APPOINTMENT_NODE APPOINTMENT_NODE;

struct APPOINTMENT_NODE
{
  APPOINTMENT_NODE *pPrev;
  APPOINTMENT_NODE *pNext;
  APPOINTMENT Appointment;
};

struct APPOINTMENT_LIST
{
  APPOINTMENT_NODE *pFirst;
  APPOINTMENT_NODE *pLast;
  unsigned nSize;
};

APPOINTMENT_LIST *CreateAppointmentList()
{
  APPOINTMENT_LIST *pList = malloc(sizeof(APPOINTMENT_LIST));
  pList->pFirst = NULL;
  pList->pLast = NULL;
  pList->nSize = 0;
  return pList;
}

void ClearAppointmentList(APPOINTMENT_LIST *pList)
{
  if (pList && pList->pFirst)
  {
    for (APPOINTMENT_NODE *pNode = pList->pFirst->pNext; pNode; pNode = pNode->pNext)
    {
      free(pNode->pPrev);
    }
    free(pList->pLast);
    pList->pFirst = NULL;
    pList->pLast = NULL;
    pList->nSize = 0;
  }
}

void DeleteAppointmentList(APPOINTMENT_LIST *pList)
{
  if (pList)
  {
    ClearAppointmentList(pList);
    free(pList);
  }
}

APPOINTMENT_NODE *GetAppointmentListNode(APPOINTMENT_LIST *pList, unsigned nIndex)
{
  if (!pList || nIndex >= pList->nSize) return NULL;
  APPOINTMENT_NODE *pNode = pList->pFirst;
  while (nIndex-- && pNode) pNode = pNode->pNext;
  return pNode;
}

bool GetAppointment(APPOINTMENT_LIST *pList, unsigned nIndex, bool bRemove, APPOINTMENT *pAppointment)
{
  APPOINTMENT_NODE *pNode = GetAppointmentListNode(pList, nIndex);
  if (!pNode) return false;
  if (pAppointment) *pAppointment = pNode->Appointment;
  if (bRemove)
  {
    if (pNode->pPrev) pNode->pPrev->pNext = pNode->pNext;
    else pList->pFirst = pNode->pNext;
    if (pNode->pNext) pNode->pNext->pPrev = pNode->pPrev;
    else pList->pLast = pNode->pPrev;
    pList->nSize--;
  }
  return true;
}

bool AddAppointment(APPOINTMENT_LIST *pList, const APPOINTMENT *pAppointment)
{
  if (!pList || !pAppointment) return false;
  APPOINTMENT_NODE *pNode = malloc(sizeof(APPOINTMENT_NODE));
  if (!pNode) return false;
  pList->pLast->pNext = pNode;
  pNode->pPrev = pList->pLast;
  pList->pLast = pNode;
  pList->nSize++;
  return true;
}

void SwapAppointmentNodes(APPOINTMENT_NODE **ppNode1, APPOINTMENT_NODE **ppNode2)
{
  if (!ppNode1 || !ppNode2 || !*ppNode1 || !*ppNode2) return;
  APPOINTMENT_NODE *pSwap;

  pSwap = (*ppNode2)->pNext;
  (*ppNode2)->pNext = (*ppNode1)->pNext;
  (*ppNode1)->pNext = pSwap;

  pSwap = (*ppNode2)->pPrev;
  (*ppNode2)->pPrev = (*ppNode1)->pPrev;
  (*ppNode1)->pPrev = pSwap;

  pSwap = *ppNode2;
  *ppNode2 = *ppNode1;
  *ppNode1 = pSwap;
}

APPOINTMENT_NODE *QuickSortPartitionAppointments(APPOINTMENT_LIST *pList, APPOINTMENT_NODE *pLow, APPOINTMENT_NODE *pHigh, APPOINTMENT_COMPARATOR fnCompare)
{
  APPOINTMENT_NODE *pNode1 = pLow;
  for (APPOINTMENT_NODE *pNode2 = pLow; pNode2 != pHigh; pNode2 = pNode2->pNext)
  {
    if (fnCompare(&pNode1->Appointment, &pHigh->Appointment) <= 0)
    {
      SwapAppointmentNodes(&pNode1, &pNode2);
      pNode1 = pNode1->pNext;
    }
  }
  SwapAppointmentNodes(&pNode1, &pHigh);
  return pNode1;
}

void QuickSortAppointments(APPOINTMENT_LIST *pList, APPOINTMENT_NODE *pLow, APPOINTMENT_NODE *pHigh, APPOINTMENT_COMPARATOR fnCompare)
{
  if (!pList || !pList || !pHigh || pLow == pHigh || pLow->pPrev == pHigh) return;
  APPOINTMENT_NODE *pPivot = QuickSortPartitionAppointments(pList, pLow, pHigh, fnCompare);
  QuickSortAppointments(pList, pLow, pPivot->pPrev, fnCompare);
  QuickSortAppointments(pList, pPivot->pNext, pHigh, fnCompare);
}

bool SortAppointments(APPOINTMENT_LIST *pList, APPOINTMENT_COMPARATOR fnCompare)
{
  if (!pList) return false;
  QuickSortAppointments(pList, pList->pFirst, pList->pLast, fnCompare);
  return true;
}

bool IterateAppointments(APPOINTMENT_LIST *pList, APPOINTMENT_CALLBACK fnCallback, void *pParams)
{
  if (!pList || !fnCallback) return false;
  for (APPOINTMENT_NODE *pNode = pList->pFirst; pNode; pNode = pNode->pNext)
  {
    if (!fnCallback(&pNode->Appointment, pParams)) return false;
  }
  return true;
}
