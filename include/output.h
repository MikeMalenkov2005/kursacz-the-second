#ifndef OUTPUT_H
#define OUTPUT_H

#include <patients.h>
#include <appointments.h>

bool OutputPatientData(const PATIENT *pPatient);

bool OutputPatientTable(PATIENT_TABLE *pTable);

bool OutputAppointmentData(const APPOINTMENT *pAppointment);

bool OutputAppointmentList(APPOINTMENT_LIST *pList);

#endif
