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

typedef bool (*PATIENT_CALLBACK)(const PATIENT *, void *); // returns false to stop iterating

bool IsRegNumberValid(const char *pRegNumber);

PATIENT_TABLE *CreatePatientTable(unsigned nSize);

void DestroyPatientTable(PATIENT_TABLE *pTable);

void ClearPatientTable(PATIENT_TABLE *pTable);

bool GetPatient(PATIENT_TABLE *pTable, const char *pRegNumber, bool bRemove, PATIENT *pPatient);

bool AddPatient(PATIENT_TABLE *pTable, const PATIENT *pPatient);

bool IteratePatientTable(PATIENT_TABLE *pTable, PATIENT_CALLBACK fnCallback, void *pParams);

#endif
