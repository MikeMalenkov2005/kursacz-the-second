#include <output.h>

#include <stdio.h>
#include <string.h>

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

bool OutputDoctorData(const DOCTOR *pDoctor)
{
  if (!pDoctor) return false;
  printf("%s:\n  - Job title: %s\n  - Office number: %u\n  - Appointment schedule: %s\n",
    pDoctor->szFullName, pDoctor->szJobTitle, pDoctor->nOfficeNumber, pDoctor->szSchedule);
  return true;
}

bool OutputDoctorTreeCallback(const DOCTOR *pDoctor, void *pParams)
{
  return OutputDoctorData(pDoctor);
}

bool OutputDoctorTree(DOCTOR_TREE *pTree)
{
  return pTree && IterateDoctorTree(pTree, OutputDoctorTreeCallback, NULL);
}

bool OutputAppointmentData(const APPOINTMENT *pAppointment)
{
  if (!pAppointment) return false;
  printf("  - Doctor: %s\n  - Patient: %s\n  - Date: %s\n  - Time: %s\n", pAppointment->szDoctor, pAppointment->szPatient, pAppointment->szDate, pAppointment->szTime);
  return true;
}

bool OutputAppointmentListCallback(const APPOINTMENT *pAppointment, void *pParams)
{
  printf("%u:\n", ++*(unsigned int*)pParams);
  return OutputAppointmentData(pAppointment);
}

bool OutputAppointmentList(APPOINTMENT_LIST *pList)
{
  unsigned nIndex = 0;
  return pList && IterateAppointments(pList, OutputAppointmentListCallback, &nIndex);
}

typedef struct APPOINTMENT_INFO
{
  const char *pPrimaryKey;
  unsigned int nIndex;
  void *pCollection;
} APPOINTMENT_INFO;

bool OutputPatientAppointmentsCallback(const APPOINTMENT *pAppointment, void *pParams)
{
  APPOINTMENT_INFO *pInfo = pParams;
  if (!strncmp(pInfo->pPrimaryKey, pAppointment->szPatient, sizeof(pAppointment->szPatient)))
  {
    printf("%u:\n  - Doctor: %s\n  - Date: %s\n  - Time: %s\n", ++pInfo->nIndex, pAppointment->szDoctor, pAppointment->szDate, pAppointment->szTime);
  }
  return true;
}

bool OutputPatientAppointments(APPOINTMENT_LIST *pList, const char *pPatientRegNumber)
{
  APPOINTMENT_INFO Info = { pPatientRegNumber };
  if (!pList || !pPatientRegNumber) return false;
  printf("\nAppointments:\n");
  return IterateAppointments(pList, OutputPatientAppointmentsCallback, &Info);
}

bool OutputDoctorAppointmentsCallback(const APPOINTMENT *pAppointment, void *pParams)
{
  PATIENT Patient;
  APPOINTMENT_INFO *pInfo = pParams;
  if (!strncmp(pInfo->pPrimaryKey, pAppointment->szDoctor, sizeof(pAppointment->szDoctor)) && GetPatient(pInfo->pCollection, pAppointment->szPatient, false, &Patient))
  {
    printf("%s:\n  - Full name: %s\n  - Date: %s\n  - Time: %s\n", Patient.szRegNumber, Patient.szFullName, pAppointment->szDate, pAppointment->szTime);
  }
  return true;
}

bool OutputDoctorAppointments(PATIENT_TABLE *pTable, APPOINTMENT_LIST *pList, const char *pDoctorFullName)
{
  APPOINTMENT_INFO Info = { pDoctorFullName };
  if (!pTable || !pList || !pDoctorFullName) return false;
  Info.pCollection = pTable;
  printf("\nPatients:\n");
  return IterateAppointments(pList, OutputDoctorAppointmentsCallback, &Info);
}
