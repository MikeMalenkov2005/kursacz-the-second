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

typedef struct STRING_UINT
{
  const char *pString;
  unsigned int nInt;
} STRING_UINT;

bool OutputPatientAppointmentsCallback(const APPOINTMENT *pAppointment, void *pParams)
{
  STRING_UINT *pInfo = pParams;
  return strncmp(pInfo->pString, pAppointment->szPatient, sizeof(pAppointment->szPatient)) || (printf("%u:\n", ++pInfo->nInt), OutputAppointmentData(pAppointment));
}

bool OutputPatientAppointments(APPOINTMENT_LIST *pList, const char *pPatientRegNumber)
{
  STRING_UINT Info = { pPatientRegNumber };
  return IterateAppointments(pList, OutputPatientAppointmentsCallback, &Info);
}

bool OutputDoctorAppointmentsCallback(const APPOINTMENT *pAppointment, void *pParams)
{
  STRING_UINT *pInfo = pParams;
  return strncmp(pInfo->pString, pAppointment->szDoctor, sizeof(pAppointment->szDoctor)) || (printf("%u:\n", ++pInfo->nInt), OutputAppointmentData(pAppointment));
}

bool OutputDoctorAppointments(APPOINTMENT_LIST *pList, const char *pDoctorFullName)
{
  STRING_UINT Info = { pDoctorFullName };
  return IterateAppointments(pList, OutputDoctorAppointmentsCallback, &Info);
}
