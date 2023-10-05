#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

long strtol(const char *nPtr, char **endPtr, int base)
{
	errno = 0;
	if (base < 0 || base > 36 || base == 1)
	{
		errno = EINVAL;
		return 0;
	}

	while (isspace(*nPtr))
	{
		nPtr++;
	}
	int isPositive = 1;

	if (*nPtr == '+')
	{
		nPtr++;
	}
	else if (*nPtr == '-')
	{
		nPtr++;
		isPositive = -1;
	}

	if (base == 0)
	{
		if (*nPtr == '0')
		{
			nPtr++;
			if (tolower(*nPtr) == 'x' && toupper(*(nPtr + 1)) - 'A' + 10 < 16 && *(nPtr + 1))
			{
				nPtr++;
				base = 16;
			}
			else
			{
				base = 8;
			}
		}
		else
		{
			base = 10;
		}
	}
	else if (base == 16)
	{
		if (*nPtr == '0')
		{
			if (tolower(*(nPtr + 1)) == 'x' && toupper(*(nPtr + 2)) - 'A' + 10 < base)
			{
				nPtr += 2;
			}
		}
	}

	int number = 0;
	long value = 0;

	if (isalnum(*nPtr))
	{

		while (isalnum(*nPtr))
		{
			if (isdigit(*nPtr))
			{
				number = *nPtr - '0';
			}
			else if (isalpha(*nPtr))
			{
				number = toupper(*nPtr) - 'A' + 10;
			}
			else
			{
				break;
			}

			if (number >= base)
			{
				break;
			}

			if ((value > (LONG_MAX - number) / base) || (value < (LONG_MIN + number) / base))
			{
				errno = ERANGE;

				if (endPtr != NULL)
				{
					while (isalnum(*nPtr))
					{
						nPtr++;
					}
					*endPtr = (char *)(nPtr);
				}

				return isPositive == 1 ? LONG_MAX : LONG_MIN;
			}

			value = value * base + (number * isPositive);
			nPtr++;
		}

		if (endPtr != NULL)
		{
			*endPtr = (char *)nPtr;
		}
	}

	return value;
}
