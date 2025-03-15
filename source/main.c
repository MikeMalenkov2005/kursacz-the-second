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

bool FindPatientsIterator(const PATIENT *pPatient, void *pParam)
{
  PATIENT *pData = pParam;
  if (!~FindSubString(pPatient->szFullName, pData->szFullName)) return false;
  if (!pData->szRegNumber[0]) memcpy(pData->szRegNumber, pPatient->szRegNumber, sizeof(pData->szRegNumber));
  return !OutputPatientData(pPatient);
}

int main(void)
{
  WINFIX(SetConsoleOutputCP(CP_UTF8); SetConsoleCP(CP_UTF8));
  PATIENTTABLE *pTable = CreatePatientTable(0);
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
        if (InputValidString("registration number", IsValid, sizeof(Patient.szRegNumber), Patient.szRegNumber))
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
          IteratePatientTable(pTable, FindPatientsIterator, &Patient);
          if (!Patient.szRegNumber[0]) printf("Error: record not found!\n");
        }
      }
      else if (!strcmp(cmd, "remove patient"))
      {
        char szRegNumber[10];
        if (InputValidString("registration number", IsValid, sizeof(szRegNumber), szRegNumber))
        {
          if (GetPatient(pTable, szRegNumber, true, NULL)) printf("Success!\n");
          else printf("Error: record not found!\n");
        }
      }
      else if (!strcmp(cmd, "list patients")) OutputPatientTable(pTable);
      else printf("Error: unknown command!\n");
    }
    else printf("Error: command has spaces or is too long!\n");
    printf("> ");
  }
  DestroyPatientTable(pTable);
  printf("Bye!\n");
  return 0;
}
