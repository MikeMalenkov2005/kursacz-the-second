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

bool OutputDoctorData(const DOCTOR *pDoctor)
{
  if (!pDoctor) return false;
  printf("%s:\n  - Job title: %s\n  - Office number: %u\n  - Appointment schedule: %s",
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
  printf("%s:\n  - Patient: %s\n  - Date: %s\n  - Time: %s\n", pAppointment->szDoctor, pAppointment->szPatient, pAppointment->szDate, pAppointment->szTime);
  return true;
}

bool OutputAppointmentListCallback(const APPOINTMENT *pAppointment, void *pParams)
{
  printf("%u. ", ++*(unsigned*)pParams);
  return OutputAppointmentData(pAppointment);
}

bool OutputAppointmentList(APPOINTMENT_LIST *pList)
{
  unsigned nIndex = 0;
  return pList && IterateAppointments(pList, OutputAppointmentListCallback, &nIndex);
}
