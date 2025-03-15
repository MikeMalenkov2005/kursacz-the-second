#ifndef INPUT_H
#define INPUT_H

#include <patients.h>

typedef bool (*INTEGERVALIDATOR)(int);

typedef bool (*STRINGVALIDATOR)(const char *);

bool ClearInput(void);

bool InputString(size_t cbMaxLength, char *pString); // Returns true if the full line was read and false otherwise

bool InputValidInteger(const char *pTitle, INTEGERVALIDATOR fnIsValid, int *pInteger);

bool InputValidString(const char *pTitle, STRINGVALIDATOR fnIsValid, size_t cbMaxLength, char *pString);

bool InputPatientData(PATIENT *pPatient);

#endif
