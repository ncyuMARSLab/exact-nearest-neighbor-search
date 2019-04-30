#pragma once

#define white_space(c) ((c) == ' ' || (c) == '\t')
#define valid_digit(c) ((c) >= '0' && (c) <= '9')

int CharToIntOnly(char **p) {//faster
	int value;
	for (value = 0; valid_digit(**p); *p += 1) {
		value = value * 10 + (**p - '0');
	}
	*p += 1;
	return value;
}
int CharToIntOnlyAutoStop(char **p, int *num) {//faster
	int value;
	bool empty = true;
	for (value = 0; valid_digit(**p); *p += 1) {
		value = value * 10 + (**p - '0');
		empty = false;
	}
	*p += 1;
	if (**p == '\0' || empty) {
		return 0;
	}
	*num = value;

	return 1;
}

char *Strsep(char **stringp, char delim)
{
	char *s;
	int c;
	char *tok;

	if ((s = *stringp) == NULL)
		return (NULL);
	for (tok = s;;) {
		c = *s++;
		if (c == delim || c == '\0') {
			if (c == 0)
				s = NULL;
			else
				s[-1] = 0;
			*stringp = s;
			return (tok);
		}
	}
}


double CharToDouble(char **saveptr)
{
	char *token = Strsep(saveptr, ' ');
	char **p = &token;

	int frac;
	double sign, value, scale;

	// Skip leading white space, if any.

	/*while (white_space(**p)) {
		p += 1;
	}*/

	// Get sign, if any.

	sign = 1.0;
	if (**p == '-') {
		sign = -1.0;
		*p += 1;

	}
	else if (**p == '+') {
		*p += 1;
	}

	// Get digits before decimal point or exponent, if any.

	for (value = 0.0; valid_digit(**p); *p += 1) {
		value = value * 10.0 + (**p - '0');
	}

	// Get digits after decimal point, if any.

	if (**p == '.') {
		double pow10 = 10.0;
		*p += 1;
		while (valid_digit(**p)) {
			value += (**p - '0') / pow10;
			pow10 *= 10.0;
			*p += 1;
		}
	}

	// Handle exponent, if any.

	frac = 0;
	scale = 1.0;
	if ((**p == 'e') || (**p == 'E')) {
		unsigned int expon;

		// Get sign of exponent, if any.

		*p += 1;
		if (**p == '-') {
			frac = 1;
			*p += 1;

		}
		else if (**p == '+') {
			*p += 1;
		}

		// Get digits of exponent, if any.

		for (expon = 0; valid_digit(**p); *p += 1) {
			expon = expon * 10 + (**p - '0');
		}
		if (expon > 308) expon = 308;

		// Calculate scaling factor.

		while (expon >= 50) { scale *= 1E50; expon -= 50; }
		while (expon >= 8) { scale *= 1E8;  expon -= 8; }
		while (expon >   0) { scale *= 10.0; expon -= 1; }
	}

	// Return signed and scaled floating point result.

	return sign * (frac ? (value / scale) : (value * scale));
}