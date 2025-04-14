#include <utils.h>

#include <string.h>

size_t FindSubString(const char *pString, const char *pSubString)
{
  size_t nStringLength = strlen(pString);
  size_t nSubStringLength = strlen(pSubString);
  for (size_t i = 0; i + nSubStringLength <= nStringLength; ++i)
  {
    if (!strncmp(pString + i, pSubString, nSubStringLength)) return i;
  }
  return ~(size_t)0;
}

bool IsStringNotEmpy(const char *pString)
{
  return pString && *pString;
}

bool IsOneBasedIndexValid(unsigned nIndex)
{
  return nIndex;
}

int AppointmentByDoctorComparator(const APPOINTMENT *pLeft, const APPOINTMENT *pRight)
{
  return strncmp(pLeft->szDoctor, pRight->szDoctor, sizeof(pLeft->szDoctor));
}

bool CheckIfAppointmentCanBeMadeCallback(const APPOINTMENT *pListEntry, void *pParams)
{
  const APPOINTMENT *pAppointment = pParams;
  return (strncmp(pListEntry->szDoctor, pAppointment->szDoctor, sizeof(pAppointment->szDoctor))
      && strncmp(pListEntry->szPatient, pAppointment->szPatient, sizeof(pAppointment->szPatient)))
    || strncmp(pListEntry->szDate, pAppointment->szDate, sizeof(pAppointment->szDate))
    || strncmp(pListEntry->szTime, pAppointment->szTime, sizeof(pAppointment->szTime));
}

bool CheckIfAppointmentCanBeMade(PATIENT_TABLE *pTable, DOCTOR_TREE *pTree, APPOINTMENT_LIST *pList, const APPOINTMENT *pAppointment)
{
  PATIENT Patient;
  DOCTOR Doctor;
  return GetPatient(pTable, pAppointment->szPatient, false, &Patient)
    && GetDoctor(pTree, pAppointment->szDoctor, false, &Doctor)
    && strncmp(Doctor.szSchedule, pAppointment->szTime, 5) <= 0
    && strncmp(Doctor.szSchedule + 6, pAppointment->szTime, 5) >= 0
    && IterateAppointments(pList, CheckIfAppointmentCanBeMadeCallback, (void*)pAppointment);
}

typedef struct STRING_UINT
{
  const char *pString;
  unsigned int nInt;
} STRING_UINT;

bool RemoveAppointmentsByPatientCallback(const APPOINTMENT *pAppointment, void *pParams)
{
  STRING_UINT *pInfo = pParams;
  pInfo->nInt++;
  return strncmp(pInfo->pString, pAppointment->szPatient, sizeof(pAppointment->szPatient));
}

bool RemoveAppointmentsByPatient(APPOINTMENT_LIST *pList, const char *pPatient)
{
  STRING_UINT Info = { pPatient };
  while (!IterateAppointments(pList, RemoveAppointmentsByPatientCallback, &Info))
  {
    if (!GetAppointment(pList, Info.nInt - 1, true, NULL)) return false;
    Info.nInt = 0;
  }
  return true;
}

bool RemoveAppointmentsByDoctorCallback(const APPOINTMENT *pAppointment, void *pParams)
{
  STRING_UINT *pInfo = pParams;
  pInfo->nInt++;
  return strncmp(pInfo->pString, pAppointment->szDoctor, sizeof(pAppointment->szDoctor));
}

bool RemoveAppointmentsByDoctor(APPOINTMENT_LIST *pList, const char *pDoctor)
{
  STRING_UINT Info = { pDoctor };
  while (!IterateAppointments(pList, RemoveAppointmentsByDoctorCallback, &Info))
  {
    if (!GetAppointment(pList, Info.nInt - 1, true, NULL)) return false;
    Info.nInt = 0;
  }
  return true;
}
