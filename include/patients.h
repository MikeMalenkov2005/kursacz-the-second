#ifndef PATIENTS_H
#define PATIENTS_H

#include <stdbool.h>

typedef struct PATIENT PATIENT;

struct PATIENT
{
  char szRegNumber[10]; // of form "MM-NNNNNN\0", '\0' = NULL-terminator
  char szFullName[64];
  int nBirthYear;
  char szAddress[64];
  char szWorkPlace[64];
};

typedef struct PATIENT_TABLE PATIENT_TABLE;

typedef bool (*PATIENT_ITERATOR)(const PATIENT *, void *); // returns true to stop iterating (signal that it found what it wanted)

bool IsValid(const char *pRegNumber);

PATIENT_TABLE *CreatePatientTable(int nSize);

void DestroyPatientTable(PATIENT_TABLE *pTable);

void ClearPatientTable(PATIENT_TABLE *pTable);

bool GetPatient(PATIENT_TABLE *pTable, const char *pRegNumber, bool bRemove, PATIENT *pPatient);

bool AddPatient(PATIENT_TABLE *pTable, const PATIENT *pPatient);

bool IteratePatientTable(PATIENT_TABLE *pTable, PATIENT_ITERATOR fnIterator, void *pParams);

#endif
