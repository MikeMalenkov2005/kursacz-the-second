#include <input.h>

#include <stdio.h>

bool ClearInput(void)
{
  bool bResult = false;
  while (getchar() != '\n') bResult = true;
  return bResult;
}

bool InputString(size_t cbMaxLength, char *pString)
{
  if (!cbMaxLength || !pString) return false;
  for (size_t i = 0; i < cbMaxLength - 1; ++i)
  {
    int c = getchar();
    if (c == '\n' || c == 0 || c == EOF)
    {
      pString[i] = 0;
      return true;
    }
    else pString[i] = c;
  }
  pString[cbMaxLength - 1] = 0;
  return false;
}

bool InputValidInteger(const char *pTitle, INTEGERVALIDATOR fnIsValid, int *pInteger)
{
  if (!pTitle || !pInteger) return false;
  printf("Enter the %s: ", pTitle);
  while (scanf("%d", pInteger) != 1 || ClearInput() || (fnIsValid && !fnIsValid(*pInteger)))
  {
    printf("Invalid %s!\nDo you want to retry? (Y/n): ", pTitle);
    for (int c = getchar(); c != '\n' && (ClearInput() || (c != 'y' || c != 'Y')); c = getchar())
    {
      if (c == 'n' || c == 'N' || c == EOF || c == 0) return false;
      printf("Do you want to retry? (Y/n): ");
    }
    printf("Enter the %s: ", pTitle);
  }
  return true;
}

bool InputValidString(const char *pTitle, STRINGVALIDATOR fnIsValid, size_t cbMaxLength, char *pString)
{
  if (!pTitle || !cbMaxLength || !pString) return false;
  printf("Enter a %s: ", pTitle);
  while ((!InputString(cbMaxLength, pString) && ClearInput()) || (fnIsValid && !fnIsValid(pString)))
  {
    printf("Invalid %s!\nDo you want to retry? (Y/n): ", pTitle);
    for (int c = getchar(); c != '\n' && (ClearInput() || (c != 'y' || c != 'Y')); c = getchar())
    {
      if (c == 'n' || c == 'N' || c == EOF || c == 0) return false;
      printf("Do you want to retry? (Y/n): ");
    }
    printf("Enter a %s: ", pTitle);
  }
  return true;
}

bool InputPatientData(PATIENT *pPatient)
{
  return pPatient
    && InputValidString("registration number", IsValid, sizeof(pPatient->szRegNumber), pPatient->szRegNumber)
    && InputValidString("full name", NULL, sizeof(pPatient->szFullName), pPatient->szFullName)
    && InputValidInteger("birth year", NULL, &pPatient->nBirthYear)
    && InputValidString("address", NULL, sizeof(pPatient->szAddress), pPatient->szAddress)
    && InputValidString("work place", NULL, sizeof(pPatient->szWorkPlace), pPatient->szWorkPlace);
}
