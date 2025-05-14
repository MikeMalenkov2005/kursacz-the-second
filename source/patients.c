#include <patients.h>
#include <stdlib.h>

#define INVALID_HASH  -1

typedef struct PATIENT_RECORD PATIENT_RECORD;

struct PATIENT_RECORD
{
  unsigned nBigHash;
  PATIENT Patient;
  bool bEmpty;
};

struct PATIENT_TABLE
{
  unsigned nLoad;
  unsigned nSize;
  PATIENT_RECORD *pRectords;
};

unsigned BigHash(const char *pRegNumber)
{
  unsigned i;
  unsigned hash = 0;
  for (i = 0; i < 10 && pRegNumber[i]; ++i)
  {
    if (i != 2)
    {
      if (pRegNumber[i] < '0' || pRegNumber[i] > '9') return INVALID_HASH;
      hash = hash * 10 + pRegNumber[i] - '0';
    }
    else if (pRegNumber[i] != '-') return INVALID_HASH;
  }
  if (i != 9) return INVALID_HASH;
  return hash;
}

unsigned FirstHash(unsigned nTableSize, unsigned nBigHash)
{
  return (((nBigHash >> 12) + (nBigHash & 0xFFF)) * 2 + 1) % nTableSize;
}

unsigned SecondHash(unsigned nTableSize, unsigned nBigHash)
{
  return (((nBigHash >> 16) + (nBigHash & 0xFFFF)) * 2 + 1) % nTableSize;
}

PATIENT_RECORD *FindPatientRecord(PATIENT_TABLE *pTable, unsigned nBigHash)
{
  if (!pTable || nBigHash == INVALID_HASH) return NULL;
  unsigned nIndex = FirstHash(pTable->nSize, nBigHash);
  unsigned nStep = SecondHash(pTable->nSize, nBigHash);
  for (unsigned i = 0; i < pTable->nSize; ++i)
  {
    PATIENT_RECORD *pRecord = pTable->pRectords + nIndex;
    if (pRecord->bEmpty) return NULL;
    else if (pRecord->nBigHash == nBigHash) return pRecord;
    if ((nIndex += nStep) >= pTable->nSize) nIndex %= pTable->nSize;
  }
  return NULL;
}

PATIENT_RECORD *FindFreeRecord(PATIENT_TABLE *pTable, unsigned nBigHash)
{
  if (!pTable || nBigHash == INVALID_HASH) return NULL;
  unsigned nIndex = FirstHash(pTable->nSize, nBigHash);
  unsigned nStep = SecondHash(pTable->nSize, nBigHash);
  for (unsigned i = 0; i < pTable->nSize; ++i)
  {
    PATIENT_RECORD *pRecord = pTable->pRectords + nIndex;
    if (pRecord->bEmpty || pRecord->nBigHash == INVALID_HASH) return pRecord;
    if ((nIndex += nStep) >= pTable->nSize) nIndex %= pTable->nSize;
  }
  return NULL;
}

bool IsRegNumberValid(const char *pRegNumber)
{
  return BigHash(pRegNumber) != INVALID_HASH;
}

PATIENT_TABLE *CreatePatientTable(unsigned nSize)
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
    for (unsigned i = 0; i < pTable->nSize; ++i)
    {
      pTable->pRectords[i].bEmpty = true;
    }
  }
}

unsigned GetPatientCount(PATIENT_TABLE *pTable)
{
  return pTable->nLoad;
}

bool GetPatient(PATIENT_TABLE *pTable, const char *pRegNumber, bool bRemove, PATIENT *pPatient)
{
  unsigned nBigHash = BigHash(pRegNumber);
  if (nBigHash == INVALID_HASH) return false;
  PATIENT_RECORD *pRecord = FindPatientRecord(pTable, nBigHash);
  if (!pRecord) return false;
  if (pPatient) *pPatient = pRecord->Patient;
  if (bRemove)
  {
    pRecord->nBigHash = INVALID_HASH;
    pTable->nLoad--;
  }
  return true;
}

bool AddPatient(PATIENT_TABLE *pTable, const PATIENT *pPatient)
{
  unsigned nBigHash = BigHash(pPatient->szRegNumber);
  if (nBigHash == INVALID_HASH) return false;
  PATIENT_RECORD *pRecord = FindPatientRecord(pTable, nBigHash);
  if (!pRecord) pRecord = FindFreeRecord(pTable, nBigHash);
  if (!pRecord) return false;
  pRecord->nBigHash = nBigHash;
  pRecord->Patient = *pPatient;
  pRecord->bEmpty = false;
  if (pTable->nLoad++ >= ((3 * pTable->nSize) >> 2))
  { /* Try to resize the table */
    bool bFailed = true;
    unsigned nLoad = pTable->nLoad;
    unsigned nSize = pTable->nSize;
    PATIENT_RECORD *pRecords = pTable->pRectords;
    pTable->nSize = nSize << 1;
    if ((pTable->pRectords = malloc(sizeof(PATIENT_RECORD) * pTable->nSize)))
    {
      bFailed = false;
      pTable->nLoad = 0;
      for (unsigned i = 0; i < nSize && !bFailed; ++i)
      {
        if (!pRecords[i].bEmpty && pRecords[i].nBigHash != INVALID_HASH)
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

bool IteratePatientTable(PATIENT_TABLE *pTable, PATIENT_CALLBACK fnCallback, void *pParams)
{
  if (!pTable) return false;
  for (unsigned i = 0; i < pTable->nSize; ++i)
  {
    if (!pTable->pRectords[i].bEmpty && pTable->pRectords[i].nBigHash != INVALID_HASH)
    {
      if (!fnCallback(&pTable->pRectords[i].Patient, pParams)) return false;
    }
  }
  return true;
}
