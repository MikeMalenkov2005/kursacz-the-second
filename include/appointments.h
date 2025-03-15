#ifndef APPOINTMENTS_H
#define APPOINTMENTS_H

typedef struct APPOINTMENT APPOINTMENT;

struct APPOINTMENT
{
  int nPatient;
  char szDoctor[26];
  char szDate[11];
  char szTime[6];
};

typedef struct APPOINTMENT_LIST APPOINTMENT_LIST;

#endif
