#ifndef APPOINTMENTS_H
#define APPOINTMENTS_H

#include <stdbool.h>

typedef struct APPOINTMENT APPOINTMENT;

struct APPOINTMENT
{
  char szPatient[10];
  char szDoctor[26];
  char szDate[11];
  char szTime[6];
};

typedef struct APPOINTMENT_LIST APPOINTMENT_LIST;

typedef bool (*APPOINTMENT_CALLBACK)(const APPOINTMENT *, void *); // returns false to stop iterating

typedef int (*APPOINTMENT_COMPARATOR)(const APPOINTMENT *, const APPOINTMENT *);

APPOINTMENT_LIST *CreateAppointmentList();

void ClearAppointmentList(APPOINTMENT_LIST *pList);

void DeleteAppointmentList(APPOINTMENT_LIST *pList);

bool GetAppointment(APPOINTMENT_LIST *pList, unsigned nIndex, bool bRemove, APPOINTMENT *pAppointment);

bool AddAppointment(APPOINTMENT_LIST *pList, const APPOINTMENT *pAppointment);

bool IterateAppointments(APPOINTMENT_LIST *pList, APPOINTMENT_CALLBACK fnCallback, void *pParams);

#endif
