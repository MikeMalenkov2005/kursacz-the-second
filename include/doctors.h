#ifndef DOCTORS_H
#define DOCTORS_H

typedef struct DOCTOR
{
  char szFullName[26]; // 26 = 25 + 1 because it is NULL-terminated
  char szJobTitle[64];
  int nOfficeNumber;
} DOCTOR;

#endif
