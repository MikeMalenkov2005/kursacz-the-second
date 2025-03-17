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

APPOINTMENT_NODE *GetAppointmentListNode(APPOINTMENT_LIST *pList, unsigned index)
{
  if (!pList || index >= pList->nSize) return NULL;
  APPOINTMENT_NODE *pNode = pList->pFirst;
  while (index-- && pNode) pNode = pNode->pNext;
  return pNode;
}
