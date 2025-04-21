#include <storage.h>
#include <string.h>
#include <stdio.h>

typedef struct STORAGE_HELPER
{
  FILE *pFile;
  unsigned nCount;
  unsigned nIndex;
} STORAGE_HELPER;

unsigned ComputeCheckSum(unsigned nSeed, const void *pData, size_t nDataBytes)
{
  for (size_t i = 0; i < nDataBytes; ++i)
  {
    nSeed += (unsigned)((const unsigned char*)pData)[i];
  }
  return -nSeed;
}

bool VerifyCheckSum(unsigned nSeed, const void *pData, size_t nDataBytes, unsigned nCheckSum)
{
  for (size_t i = 0; i < nDataBytes; ++i)
  {
    nSeed += (unsigned)((const unsigned char*)pData)[i];
  }
  return !(nSeed + nCheckSum);
}

bool SavePatient(const PATIENT *pPatient, void *pParams)
{
  STORAGE_HELPER *pHelper = pParams;
  unsigned nCheckSum = ComputeCheckSum(pHelper->nIndex++, pPatient, sizeof(*pPatient));
  fwrite(pPatient, sizeof(*pPatient), 1, pHelper->pFile);
  fwrite(&nCheckSum, sizeof(nCheckSum), 1, pHelper->pFile);
  return true;
}

bool SaveDoctor(const DOCTOR *pDoctor, void *pParams)
{
  STORAGE_HELPER *pHelper = pParams;
  unsigned nCheckSum = ComputeCheckSum(pHelper->nIndex++, pDoctor, sizeof(*pDoctor));
  fwrite(pDoctor, sizeof(*pDoctor), 1, pHelper->pFile);
  fwrite(&nCheckSum, sizeof(nCheckSum), 1, pHelper->pFile);
  return true;
}

bool SaveAppointment(const APPOINTMENT *pAppointment, void *pParams)
{
  STORAGE_HELPER *pHelper = pParams;
  unsigned nCheckSum = ComputeCheckSum(pHelper->nIndex++, pAppointment, sizeof(*pAppointment));
  fwrite(pAppointment, sizeof(*pAppointment), 1, pHelper->pFile);
  fwrite(&nCheckSum, sizeof(nCheckSum), 1, pHelper->pFile);
  return true;
}

bool SaveMedicalData(const char *pFileName, PATIENT_TABLE *pTable, DOCTOR_TREE *pTree, APPOINTMENT_LIST *pList)
{
  STORAGE_HELPER Helper;
  if (!(Helper.pFile = fopen(pFileName, "wb"))) return false;

  fwrite("FMED", 1, 4, Helper.pFile);

  fwrite("PTBL", 1, 4, Helper.pFile);
  Helper.nCount = GetPatientCount(pTable);
  fwrite(&Helper.nCount, sizeof(unsigned), 1, Helper.pFile);
  Helper.nIndex = 0;
  IteratePatientTable(pTable, SavePatient, &Helper);

  fwrite("DTRE", 1, 4, Helper.pFile);
  Helper.nCount = GetDoctorCount(pTree);
  fwrite(&Helper.nCount, sizeof(unsigned), 1, Helper.pFile);
  Helper.nIndex = 0;
  IterateDoctorTree(pTree, SaveDoctor, &Helper);

  fwrite("ALST", 1, 4, Helper.pFile);
  Helper.nCount = GetAppointmentCount(pList);
  fwrite(&Helper.nCount, sizeof(unsigned), 1, Helper.pFile);
  Helper.nIndex = 0;
  IterateAppointments(pList, SaveAppointment, &Helper);

  fclose(Helper.pFile);
  return true;
}

bool LoadMedicalData(const char *pFileName, PATIENT_TABLE *pTable, DOCTOR_TREE *pTree, APPOINTMENT_LIST *pList)
{
  char aHeader[4];
  STORAGE_HELPER Helper;

  if (!(Helper.pFile = fopen(pFileName, "rb"))) return false;

  fread(aHeader, 1, 4, Helper.pFile);
  if (strncmp(aHeader, "FMED", 4)) return fclose(Helper.pFile), false;

  ClearPatientTable(pTable);
  ClearDoctorTree(pTree);
  ClearAppointmentList(pList);

  fread(aHeader, 1, 4, Helper.pFile);
  if (strncmp(aHeader, "PTBL", 4)) return fclose(Helper.pFile), false;
  fread(&Helper.nCount, sizeof(unsigned), 1, Helper.pFile);
  for (Helper.nIndex = 0; Helper.nIndex < Helper.nCount; ++Helper.nIndex)
  {
    PATIENT Patient;
    unsigned nCheckSum;
    fread(&Patient, sizeof(Patient), 1, Helper.pFile);
    fread(&nCheckSum, sizeof(nCheckSum), 1, Helper.pFile);
    if (!VerifyCheckSum(Helper.nIndex, &Patient, sizeof(Patient), nCheckSum)) return fclose(Helper.pFile), false;
  }

  fread(aHeader, 1, 4, Helper.pFile);
  if (strncmp(aHeader, "DTRE", 4)) return fclose(Helper.pFile), false;
  fread(&Helper.nCount, sizeof(unsigned), 1, Helper.pFile);
  for (Helper.nIndex = 0; Helper.nIndex < Helper.nCount; ++Helper.nIndex)
  {
    DOCTOR Doctor;
    unsigned nCheckSum;
    fread(&Doctor, sizeof(Doctor), 1, Helper.pFile);
    fread(&nCheckSum, sizeof(nCheckSum), 1, Helper.pFile);
    if (!VerifyCheckSum(Helper.nIndex, &Doctor, sizeof(Doctor), nCheckSum)) return fclose(Helper.pFile), false;
  }

  fread(aHeader, 1, 4, Helper.pFile);
  if (strncmp(aHeader, "ALST", 4)) return fclose(Helper.pFile), false;
  fread(&Helper.nCount, sizeof(unsigned), 1, Helper.pFile);
  for (Helper.nIndex = 0; Helper.nIndex < Helper.nCount; ++Helper.nIndex)
  {
    APPOINTMENT Appointment;
    unsigned nCheckSum;
    fread(&Appointment, sizeof(Appointment), 1, Helper.pFile);
    fread(&nCheckSum, sizeof(nCheckSum), 1, Helper.pFile);
    if (!VerifyCheckSum(Helper.nIndex, &Appointment, sizeof(Appointment), nCheckSum)) return fclose(Helper.pFile), false;
  }

  fclose(Helper.pFile);
  return true;
}
