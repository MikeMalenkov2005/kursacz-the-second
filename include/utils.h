#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

#include <patients.h>
#include <doctors.h>
#include <appointments.h>

size_t FindSubString(const char *pString, const char *pSubString);

bool IsStringNotEmpy(const char *pString);

bool IsOneBasedIndexValid(unsigned nIndex);

int AppointmentByDoctorComparator(const APPOINTMENT *pLeft, const APPOINTMENT *pRight);

bool CheckIfAppointmentCanBeMade(PATIENT_TABLE *pTable, DOCTOR_TREE *pTree, APPOINTMENT_LIST *pList, const APPOINTMENT *pAppointment);

bool RemoveAppointmentsByPatient(APPOINTMENT_LIST *pList, const char *pPatient);

bool RemoveAppointmentsByDoctor(APPOINTMENT_LIST *pList, const char *pDoctor);

#endif
