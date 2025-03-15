#include <appointments.h>

typedef struct APPOINTMENT_RECORD APPOINTMENT_RECORD;

struct APPOINTMENT_RECORD
{
  APPOINTMENT_RECORD *pPrev;
  APPOINTMENT_RECORD *pNext;
  APPOINTMENT Appointment;
};

struct APPOINTMENT_LIST
{
  APPOINTMENT_RECORD *pFirst;
  APPOINTMENT_RECORD *pLast;
  int nSize;
};


