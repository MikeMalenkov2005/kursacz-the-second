#include <winfix.h>
#include <input.h>
#include <output.h>
#include <utils.h>

#include <stdio.h>

bool FindPatientsCallback(const PATIENT *pPatient, void *pParam)
{
  PATIENT *pData = pParam;
  if (!~FindSubString(pPatient->szFullName, pData->szFullName)) return true;
  if (!pData->szRegNumber[0]) memcpy(pData->szRegNumber, pPatient->szRegNumber, sizeof(pData->szRegNumber));
  return OutputPatientData(pPatient);
}

bool FindDoctorsCallback(const DOCTOR *pDoctor, void *pParam)
{
  DOCTOR *pData = pParam;
  if (!~FindSubString(pDoctor->szJobTitle, pData->szJobTitle)) return true;
  if (!pData->szFullName[0]) memcpy(pData->szFullName, pDoctor->szFullName, sizeof(pData->szFullName));
  return OutputDoctorData(pDoctor);
}

int main(void)
{
  WINFIX(SetConsoleOutputCP(1251); SetConsoleCP(1251));
  PATIENT_TABLE *pTable = CreatePatientTable(0);
  DOCTOR_TREE *pTree = CreateDoctorTree();
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
        printf("  - add doctor\n");
        printf("  - get doctor\n");
        printf("  - find doctor\n");
        printf("  - remove doctor\n");
        printf("  - list doctors\n");
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
          if (GetPatient(pTable, Patient.szRegNumber, false, &Patient))
          {
            OutputPatientData(&Patient);
            printf("\nAppointments:\n");
            OutputPatientAppointments(pList, Patient.szRegNumber);
          }
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
        PATIENT Patient = { 0 };
        if (InputValidString("registration number", IsRegNumberValid, sizeof(Patient.szRegNumber), Patient.szRegNumber))
        {

          if (GetPatient(pTable, Patient.szRegNumber, true, &Patient))
          {
            if (RemoveAppointmentsByPatient(pList, Patient.szRegNumber)) printf("Success!\n");
            else printf("Warning: could not remove all appointments!\nSuccess!\n");
          }
          else printf("Error: record not found!\n");
        }
      }
      else if (!strcmp(cmd, "list patients")) OutputPatientTable(pTable);
      else if (!strcmp(cmd, "add doctor"))
      {
        DOCTOR Doctor = { 0 };
        if (InputDoctorData(&Doctor))
        {
          if (AddDoctor(pTree, &Doctor)) printf("Success!\n");
          else printf("Error: the tree is full!\n");
        }
      }
      else if (!strcmp(cmd, "get doctor"))
      {
        DOCTOR Doctor = { 0 };
        if (InputValidString("doctor's full name", NULL, sizeof(Doctor.szFullName), Doctor.szFullName))
        {
          if (GetDoctor(pTree, Doctor.szFullName, false, &Doctor))
          {
            OutputDoctorData(&Doctor);
            printf("\nAppointments:\n");
            OutputDoctorAppointments(pList, Doctor.szFullName);
          }
          else printf("Error: record not found!\n");
        }
      }
      else if (!strcmp(cmd, "find doctor"))
      {
        DOCTOR Doctor = { 0 };
        if (InputValidString("part of doctor's job title", NULL, sizeof(Doctor.szJobTitle), Doctor.szJobTitle))
        {
          IterateDoctorTree(pTree, FindDoctorsCallback, &Doctor);
          if (!Doctor.szFullName[0]) printf("Error: record not found!\n");
        }
      }
      else if (!strcmp(cmd, "remove doctor"))
      {
        DOCTOR Doctor;
        if (InputValidString("registration number", NULL, sizeof(Doctor.szFullName), Doctor.szFullName))
        {
          if (GetDoctor(pTree, Doctor.szFullName, true, &Doctor))
          {
            if (RemoveAppointmentsByDoctor(pList, Doctor.szFullName)) printf("Success!\n");
            else printf("Warning: could not remove all appointments!\nSuccess!\n");
          }
          else printf("Error: record not found!\n");
        }
      }
      else if (!strcmp(cmd, "list doctors")) OutputDoctorTree(pTree);
      else if (!strcmp(cmd, "add appointment"))
      {
        APPOINTMENT Appointment = { 0 };
        if (InputAppointmentData(&Appointment))
        {
          if (CheckIfAppointmentCanBeMade(pTable, pTree, pList, &Appointment))
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
  DestroyDoctorTree(pTree);
  DestroyPatientTable(pTable);
  printf("Bye!\n");
  return 0;
}
