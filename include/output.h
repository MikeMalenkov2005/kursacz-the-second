#ifndef OUTPUT_H
#define OUTPUT_H

#include <patients.h>
#include <doctors.h>
#include <appointments.h>

bool OutputPatientData(const PATIENT *pPatient);

bool OutputPatientTable(PATIENT_TABLE *pTable);

bool OutputDoctorData(const DOCTOR *pPatient);

bool OutputDoctorTree(DOCTOR_TREE *pTable);

bool OutputAppointmentData(const APPOINTMENT *pAppointment);

bool OutputAppointmentList(APPOINTMENT_LIST *pList);

bool OutputPatientAppointments(APPOINTMENT_LIST *pList, const char *pPatientRegNumber);

bool OutputDoctorAppointments(PATIENT_TABLE *pTable, APPOINTMENT_LIST *pList, const char *pDoctorFullName);

#endif
