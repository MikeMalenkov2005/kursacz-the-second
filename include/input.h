#ifndef INPUT_H
#define INPUT_H

#include <patients.h>
#include <appointments.h>

typedef bool (*INTEGER_VALIDATOR)(int);

typedef bool (*UNSIGNED_INTEGER_VALIDATOR)(unsigned);

typedef bool (*STRING_VALIDATOR)(const char *);

bool ClearInput(void);

bool InputString(size_t cbMaxLength, char *pString); // Returns true if the full line was read and false otherwise

bool InputValidInteger(const char *pTitle, INTEGER_VALIDATOR fnIsValid, int *pInteger);

bool InputValidUnsignedInteger(const char *pTitle, UNSIGNED_INTEGER_VALIDATOR fnIsValid, unsigned *pInteger);

bool InputValidString(const char *pTitle, STRING_VALIDATOR fnIsValid, size_t cbMaxLength, char *pString);

bool InputPatientData(PATIENT *pPatient);

bool InputAppointmentData(APPOINTMENT *pAppointment);

#endif
