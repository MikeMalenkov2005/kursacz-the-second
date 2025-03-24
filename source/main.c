#include <winfix.h>
#include <input.h>
#include <output.h>

#include <stdio.h>

size_t FindSubString(const char *pString, const char *pSubString)
{
  size_t nStringLength = strlen(pString);
  size_t nSubStringLength = strlen(pSubString);
  for (size_t i = 0; i + nSubStringLength < nStringLength; ++i)
  {
    if (!strncmp(pString + i, pSubString, nSubStringLength)) return i;
  }
  return ~(size_t)0;
}

bool FindPatientsCallback(const PATIENT *pPatient, void *pParam)
{
  PATIENT *pData = pParam;
  if (!~FindSubString(pPatient->szFullName, pData->szFullName)) return true;
  if (!pData->szRegNumber[0]) memcpy(pData->szRegNumber, pPatient->szRegNumber, sizeof(pData->szRegNumber));
  return OutputPatientData(pPatient);
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

int main(void)
{
  WINFIX(SetConsoleOutputCP(CP_UTF8); SetConsoleCP(CP_UTF8));
  PATIENT_TABLE *pTable = CreatePatientTable(0);
  APPOINTMENT_LIST *pList = CreateAppointmentList();
  printf("> ");
  for (char cmd[32] = { 0 };;)
  {
    if (InputString(sizeof(cmd), cmd) || !ClearInput())
    {
      if (!strcmp(cmd, "exit") || !strcmp(cmd, "quit")) break;
      else if (!strcmp(cmd, "help"))
      {
        printf("Available commands:\n");
        printf("  - exit\n");
        printf("  - quit\n");
        printf("  - help\n");
        printf("  - add patient\n");
        printf("  - get patient\n");
        printf("  - find patient\n");
        printf("  - remove patient\n");
        printf("  - list patients\n");
        printf("  - add appointment\n");
        printf("  - get appointment\n");
        printf("  - remove appointment\n");
        printf("  - list appointments\n");
        printf("  - sort appointments\n");
      }
      else if (!strcmp(cmd, "add patient"))
      {
        PATIENT Patient = { 0 };
        if (InputPatientData(&Patient))
        {
          if (AddPatient(pTable, &Patient)) printf("Success!\n");
          else printf("Error: the table is full!\n");
        }
      }
      else if (!strcmp(cmd, "get patient"))
      {
        PATIENT Patient = { 0 };
        if (InputValidString("registration number", IsRegNumberValid, sizeof(Patient.szRegNumber), Patient.szRegNumber))
        {
          if (GetPatient(pTable, Patient.szRegNumber, false, &Patient)) OutputPatientData(&Patient);
          else printf("Error: record not found!\n");
        }
      }
      else if (!strcmp(cmd, "find patient"))
      {
        PATIENT Patient = { 0 };
        if (InputValidString("part of patient's name", NULL, sizeof(Patient.szFullName), Patient.szFullName))
        {
          IteratePatientTable(pTable, FindPatientsCallback, &Patient);
          if (!Patient.szRegNumber[0]) printf("Error: record not found!\n");
        }
      }
      else if (!strcmp(cmd, "remove patient"))
      {
        char szRegNumber[10];
        if (InputValidString("registration number", IsRegNumberValid, sizeof(szRegNumber), szRegNumber))
        {
          if (GetPatient(pTable, szRegNumber, true, NULL)) printf("Success!\n");
          else printf("Error: record not found!\n");
        }
      }
      else if (!strcmp(cmd, "list patients")) OutputPatientTable(pTable);
      else if (!strcmp(cmd, "add appointment"))
      {
        APPOINTMENT Appointment = { 0 };
        if (InputAppointmentData(&Appointment))
        {
          if (IterateAppointments(pList, CheckIfAppointmentCanBeMadeCallback, &Appointment))
          {
            if (AddAppointment(pList, &Appointment)) printf("Success!\n");
            else printf("Error: the list is full!\n");
          }
          else printf("Error: this appointment can not be made at specified time!\n");
        }
      }
      else if (!strcmp(cmd, "get appointment"))
      {
        unsigned nIndex;
        if (InputValidUnsignedInteger("one based index", IsOneBasedIndexValid, &nIndex))
        {
          APPOINTMENT Appointment = { 0 };
          if (GetAppointment(pList, nIndex - 1, false, &Appointment)) OutputAppointmentData(&Appointment);
          else printf("Error: index out of bounds!");
        }
      }
      else if (!strcmp(cmd, "remove appointment"))
      {
        unsigned nIndex;
        if (InputValidUnsignedInteger("one based index", IsOneBasedIndexValid, &nIndex))
        {
          if (GetAppointment(pList, nIndex, true, NULL)) printf("Success!\n");
          else printf("Error: index out of bounds!\n");
        }
      }
      else if (!strcmp(cmd, "list appointments")) OutputAppointmentList(pList);
      else if (!strcmp(cmd, "sort appointments"))
      {
        SortAppointments(pList, AppointmentByDoctorComparator);
        OutputAppointmentList(pList);
      }
      else printf("Error: unknown command!\n");
    }
    else printf("Error: command has spaces or is too long!\n");
    printf("> ");
  }
  DestroyAppointmentList(pList);
  DestroyPatientTable(pTable);
  printf("Bye!\n");
  return 0;
}
