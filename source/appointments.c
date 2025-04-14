#include <appointments.h>
#include <stdlib.h>

#define IsDigit(c)  (c >= '0' && c <= '9')

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

bool IsDateValid(const char *pDate)
{
  if (!IsDigit(pDate[0]) || !IsDigit(pDate[1]) || pDate[2] != '.'
      || !IsDigit(pDate[3]) || !IsDigit(pDate[4]) || pDate[5] != '.'
      || !IsDigit(pDate[6]) || !IsDigit(pDate[7]) || !IsDigit(pDate[8]) || !IsDigit(pDate[9]) || pDate[10]) return false;
  int nDay = (pDate[0] - '0') * 10 + pDate[1] - '0';
  int nMonth = (pDate[3] - '0') * 10 + pDate[4] - '0';
  int nYear = (pDate[6] - '0') * 1000 + (pDate[7] - '0') * 100 + (pDate[8] - '0') * 10 + pDate[9] - '0';
  if (nDay < 1 || nDay > 31 || nMonth < 1 || nMonth > 12) return false;
  if (nDay > 30 && (nMonth == 4 || nMonth == 6 || nMonth == 9 || nMonth == 11)) return false;
  if (nMonth == 2 && nDay > (nYear % 4 == 0 && (nYear % 100 != 0 || nYear % 400 == 0) ? 29 : 28)) return false;
  return true;
}

bool IsTimeValid(const char *pTime)
{
  if (!IsDigit(pTime[0]) || !IsDigit(pTime[1]) || pTime[2] != ':'
      || !IsDigit(pTime[3]) || !IsDigit(pTime[4]) || pTime[5]) return false;
  int nHour = (pTime[0] - '0') * 10 + pTime[1] - '0';
  int nMinute = (pTime[3] - '0') * 10 + pTime[4] - '0';
  return nHour < 24 && nMinute < 60;
}

APPOINTMENT_LIST *CreateAppointmentList()
{
  APPOINTMENT_LIST *pList = malloc(sizeof(APPOINTMENT_LIST));
  if (pList)
  {
    pList->pFirst = NULL;
    pList->pLast = NULL;
    pList->nSize = 0;
  }
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

void DestroyAppointmentList(APPOINTMENT_LIST *pList)
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
  pNode->Appointment = *pAppointment;
  pNode->pNext = NULL;
  if (pList->pFirst)
  {
    pList->pLast->pNext = pNode;
    pNode->pPrev = pList->pLast;
    pList->pLast = pNode;
  }
  else
  {
    pNode->pPrev = NULL;
    pList->pFirst = pNode;
    pList->pLast = pNode;
  }
  pList->nSize++;
  return true;
}

void SwapAppointmentNodes(APPOINTMENT_LIST *pList, APPOINTMENT_NODE **ppNode1, APPOINTMENT_NODE **ppNode2)
{
  if (!ppNode1 || !ppNode2 || !*ppNode1 || !*ppNode2 || ppNode1 == ppNode2 || *ppNode1 == *ppNode2) return;
  APPOINTMENT_NODE *pSwap;

  if (pList->pFirst == *ppNode1) pList->pFirst = *ppNode2;
  else if (pList->pFirst == *ppNode2) pList->pFirst = *ppNode1;
  if (pList->pLast == *ppNode1) pList->pLast = *ppNode2;
  else if (pList->pLast == *ppNode2) pList->pLast = *ppNode1;

  if ((*ppNode1)->pNext == *ppNode2)
  {
    (*ppNode1)->pNext = (*ppNode2)->pNext;
    (*ppNode2)->pPrev = (*ppNode1)->pPrev;
    (*ppNode1)->pPrev = *ppNode2;
    (*ppNode2)->pNext = *ppNode1;
  }
  else if ((*ppNode2)->pNext == *ppNode1)
  {
    (*ppNode2)->pNext = (*ppNode1)->pNext;
    (*ppNode1)->pPrev = (*ppNode2)->pPrev;
    (*ppNode2)->pPrev = *ppNode1;
    (*ppNode1)->pNext = *ppNode2;
  }
  else
  {
    pSwap = (*ppNode2)->pNext;
    (*ppNode2)->pNext = (*ppNode1)->pNext;
    (*ppNode1)->pNext = pSwap;

    pSwap = (*ppNode2)->pPrev;
    (*ppNode2)->pPrev = (*ppNode1)->pPrev;
    (*ppNode1)->pPrev = pSwap;
  }

  pSwap = *ppNode2;
  *ppNode2 = *ppNode1;
  *ppNode1 = pSwap;

  if ((*ppNode1)->pNext) (*ppNode1)->pNext->pPrev = *ppNode1;
  if ((*ppNode1)->pPrev) (*ppNode1)->pPrev->pNext = *ppNode1;
  if ((*ppNode2)->pNext) (*ppNode2)->pNext->pPrev = *ppNode2;
  if ((*ppNode2)->pPrev) (*ppNode2)->pPrev->pNext = *ppNode2;
}

APPOINTMENT_NODE *QuickSortPartitionAppointments(APPOINTMENT_LIST *pList, APPOINTMENT_NODE *pLow, APPOINTMENT_NODE *pHigh, APPOINTMENT_COMPARATOR fnCompare)
{
  APPOINTMENT_NODE *pNode1 = pLow;
  for (APPOINTMENT_NODE *pNode2 = pLow; pNode2 && pNode2 != pHigh; pNode2 = pNode2->pNext)
  {
    if (fnCompare(&pNode1->Appointment, &pHigh->Appointment) <= 0)
    {
      SwapAppointmentNodes(pList, &pNode1, &pNode2);
      pNode1 = pNode1->pNext;
    }
  }
  SwapAppointmentNodes(pList, &pNode1, &pHigh);
  return pNode1;
}

void QuickSortAppointments(APPOINTMENT_LIST *pList, APPOINTMENT_NODE *pLow, APPOINTMENT_NODE *pHigh, APPOINTMENT_COMPARATOR fnCompare)
{
  if (!pLow || !pHigh || pLow == pHigh || pLow->pPrev == pHigh) return;
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
