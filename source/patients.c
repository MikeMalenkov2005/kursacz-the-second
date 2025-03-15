#include <patients.h>

#include <stdlib.h>

typedef struct PATIENT_RECORD PATIENT_RECORD;

struct PATIENT_RECORD
{
  int nBigHash;
  PATIENT Patient;
  bool bEmpty;
};

struct PATIENT_TABLE
{
  int nLoad;
  int nSize;
  PATIENT_RECORD *pRectords;
};

int BigHash(const char *pRegNumber)
{
  int i;
  int hash = 0;
  for (i = 0; i < 10 && pRegNumber[i]; ++i)
  {
    if (i != 2)
    {
      if (pRegNumber[i] < '0' || pRegNumber[i] > '9') return -1;
      hash = hash * 10 + pRegNumber[i] - '0';
    }
    else if (pRegNumber[i] != '-') return -1;
  }
  if (i != 9) return -1;
  return hash;
}

int FirstHash(int nTableSize, int nBigHash)
{
  return (((nBigHash >> 12) + (nBigHash & 0xFFF)) * 2 + 1) % nTableSize;
}

int SecondHash(int nTableSize, int nBigHash)
{
  return (((nBigHash >> 16) + (nBigHash & 0xFFFF)) * 2 + 1) % nTableSize;
}

PATIENT_RECORD *FindPatientRecord(PATIENT_TABLE *pTable, int nBigHash)
{
  if (!pTable || nBigHash < 0) return NULL;
  int nIndex = FirstHash(pTable->nSize, nBigHash);
  int nStep = SecondHash(pTable->nSize, nBigHash);
  for (int i = 0; i < pTable->nSize; ++i)
  {
    PATIENT_RECORD *pRecord = pTable->pRectords + nIndex;
    if (pRecord->bEmpty) return NULL;
    else if (pRecord->nBigHash == nBigHash) return pRecord;
    if ((nIndex += nStep) >= pTable->nSize) nIndex %= pTable->nSize;
  }
  return NULL;
}

PATIENT_RECORD *FindFreeRecord(PATIENT_TABLE *pTable, int nBigHash)
{
  if (!pTable || nBigHash < 0) return NULL;
  int nIndex = FirstHash(pTable->nSize, nBigHash);
  int nStep = SecondHash(pTable->nSize, nBigHash);
  for (int i = 0; i < pTable->nSize; ++i)
  {
    PATIENT_RECORD *pRecord = pTable->pRectords + nIndex;
    if (pRecord->bEmpty || pRecord->nBigHash < 0) return pRecord;
    if ((nIndex += nStep) >= pTable->nSize) nIndex %= pTable->nSize;
  }
  return NULL;
}

bool IsValid(const char *pRegNumber)
{
  return BigHash(pRegNumber) >= 0;
}

PATIENT_TABLE *CreatePatientTable(int nSize)
{
  if (nSize > 0)
  { // Round to the next power of 2 (from Big Twiddling Hacks)
    --nSize;
    nSize |= nSize >> 1;
    nSize |= nSize >> 2;
    nSize |= nSize >> 4;
    nSize |= nSize >> 8;
    nSize |= nSize >> 16;
    ++nSize;
  }
  else nSize = 128;
  PATIENT_TABLE *pTable = malloc(sizeof(PATIENT_TABLE));
  if (pTable)
  {
    if (!(pTable->pRectords = malloc(sizeof(PATIENT_RECORD) * nSize)))
    {
      free(pTable);
      return NULL;
    }
    pTable->nLoad = 0;
    pTable->nSize = nSize;
    ClearPatientTable(pTable);
  }
  return pTable;
}

void DestroyPatientTable(PATIENT_TABLE *pTable)
{
  if (pTable)
  {
    free(pTable->pRectords);
    free(pTable);
  }
}

void ClearPatientTable(PATIENT_TABLE *pTable)
{
  if (pTable)
  {
    for (int i = 0; i < pTable->nSize; ++i)
    {
      pTable->pRectords[i].bEmpty = true;
    }
  }
}

bool GetPatient(PATIENT_TABLE *pTable, const char *pRegNumber, bool bRemove, PATIENT *pPatient)
{
  int nBigHash = BigHash(pRegNumber);
  if (nBigHash < 0) return false;
  PATIENT_RECORD *pRecord = FindPatientRecord(pTable, nBigHash);
  if (!pRecord) return false;
  if (pPatient) *pPatient = pRecord->Patient;
  if (bRemove) pRecord->nBigHash = -1;
  return true;
}

bool AddPatient(PATIENT_TABLE *pTable, const PATIENT *pPatient)
{
  int nBigHash = BigHash(pPatient->szRegNumber);
  if (nBigHash < 0) return false;
  PATIENT_RECORD *pRecord = FindPatientRecord(pTable, nBigHash);
  if (!pRecord) pRecord = FindFreeRecord(pTable, nBigHash);
  if (!pRecord) return false;
  pRecord->nBigHash = nBigHash;
  pRecord->Patient = *pPatient;
  pRecord->bEmpty = false;
  if (pTable->nLoad++ >= ((3 * pTable->nSize) >> 2))
  { /* Try to resize the table */
    bool bFailed = true;
    int nLoad = pTable->nLoad;
    int nSize = pTable->nSize;
    PATIENT_RECORD *pRecords = pTable->pRectords;
    if ((pTable->nSize = nSize << 1) > 0 && (pTable->pRectords = malloc(sizeof(PATIENT_RECORD) * pTable->nSize)))
    {
      bFailed = false;
      pTable->nLoad = 0;
      for (int i = 0; i < nSize && !bFailed; ++i)
      {
        if (!pRecords[i].bEmpty && pRecords[i].nBigHash >= 0)
        {
          bFailed = !AddPatient(pTable, &pRecords[i].Patient);
        }
      }
    }
    if (bFailed)
    {
      pTable->nLoad = nLoad;
      pTable->nSize = nSize;
      pTable->pRectords = pRecords;
    }
    else free(pRecords);
  }
  return true;
}

bool IteratePatientTable(PATIENT_TABLE *pTable, PATIENT_ITERATOR fnIterator, void *pParams)
{
  if (!pTable) return false;
  for (int i = 0; i < pTable->nSize; ++i)
  {
    if (!pTable->pRectords[i].bEmpty && pTable->pRectords[i].nBigHash >= 0)
    {
      PATIENT Patient = pTable->pRectords[i].Patient;
      if (fnIterator(&Patient, pParams)) return true;
    }
  }
  return false;
}
