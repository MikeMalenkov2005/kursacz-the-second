#include <output.h>

#include <stdio.h>

bool OutputPatientData(const PATIENT *pPatient)
{
  if (!pPatient) return false;
  printf("%s:\n  - Full name: %s\n  - Birth year: %d\n  - Address: %s\n  - Work place: %s\n",
    pPatient->szRegNumber, pPatient->szFullName, pPatient->nBirthYear, pPatient->szAddress, pPatient->szWorkPlace);
  return true;
}

bool OutputPatientTableCallback(const PATIENT *pPatient, void *pParams)
{
  return OutputPatientData(pPatient);
}

bool OutputPatientTable(PATIENT_TABLE *pTable)
{
  return pTable && IteratePatientTable(pTable, OutputPatientTableCallback, NULL);
}
