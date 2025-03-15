#ifndef PATIENTS_H
#define PATIENTS_H

#include <stdbool.h>

typedef struct PATIENT
{
  char szRegNumber[10]; // of form "MM-NNNNNN\0", '\0' = NULL-terminator
  char szFullName[64];
  int nBirthYear;
  char szAddress[64];
  char szWorkPlace[64];
} PATIENT;

typedef struct PATIENTTABLE PATIENTTABLE;

typedef bool (*PATIENTITERATOR)(const PATIENT *, void *); // returns true to stop iterating (signal that it found what it wanted)

bool IsValid(const char *pRegNumber);

PATIENTTABLE *CreatePatientTable(int nSize);

void DestroyPatientTable(PATIENTTABLE *pTable);

void ClearPatientTable(PATIENTTABLE *pTable);

bool GetPatient(PATIENTTABLE *pTable, const char *pRegNumber, bool bRemove, PATIENT *pPatient);

bool AddPatient(PATIENTTABLE *pTable, const PATIENT *pPatient);

bool IteratePatientTable(PATIENTTABLE *pTable, PATIENTITERATOR fnIterator, void *pParams);

#endif
