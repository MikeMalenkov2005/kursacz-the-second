#ifndef STORAGE_H
#define STORAGE_H

#include <patients.h>
#include <doctors.h>
#include <appointments.h>

bool SaveMedicalData(const char *pFileName, PATIENT_TABLE *pTable, DOCTOR_TREE *pTree, APPOINTMENT_LIST *pList);

bool LoadMedicalData(const char *pFileName, PATIENT_TABLE *pTable, DOCTOR_TREE *pTree, APPOINTMENT_LIST *pList);

#endif
