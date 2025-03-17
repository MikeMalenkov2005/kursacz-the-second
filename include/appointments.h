#ifndef APPOINTMENTS_H
#define APPOINTMENTS_H

typedef struct APPOINTMENT APPOINTMENT;

struct APPOINTMENT
{
  char szPatient[10];
  char szDoctor[26];
  char szDate[11];
  char szTime[6];
};

typedef struct APPOINTMENT_LIST APPOINTMENT_LIST;

APPOINTMENT_LIST *CreateAppointmentList();

void ClearAppointmentList(APPOINTMENT_LIST *pList);

void DeleteAppointmentList(APPOINTMENT_LIST *pList);

#endif
