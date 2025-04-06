#ifndef DOCTORS_H
#define DOCTORS_H

#include <stdbool.h>

typedef struct DOCTOR DOCTOR;

struct DOCTOR
{
  char szFullName[26]; // 26 = 25 + 1 because it is NULL-terminated
  char szJobTitle[64];
  unsigned nOfficeNumber;
  char szSchedule[12]; // HH:MM-HH:MM
};

typedef bool (*DOCTOR_CALLBACK)(const DOCTOR *, void *); // returns false to stop iterating

typedef struct DOCTOR_TREE DOCTOR_TREE;

bool IsValidSchedule(const char *pSchedule);

DOCTOR_TREE *CreateDoctorTree();

void ClearDoctorTree(DOCTOR_TREE *pTree);

void DestroyDoctorTree(DOCTOR_TREE *pTree);

bool GetDoctor(DOCTOR_TREE *pTree, bool bRemove, DOCTOR *pDoctor);

bool AddDoctor(DOCTOR_TREE *pTree, const DOCTOR *pDoctor);

bool IterateDoctorTree(DOCTOR_TREE *pTree, DOCTOR_CALLBACK fnCallback, void *pParams);

#endif
