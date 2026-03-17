/* Ne pas modifier */


/*
 * This function copies a string from src to dst and return the number of chars copied
 * \param dst Destination pointer for the string to copy
 * \param src Pointer to the string to copy
 * \param [out] size Total number of character copied (included null terminator)
 */
void copy_string(char *dst, char *src, unsigned int *size) {
	unsigned int i = 0;
	do {
		dst[i] = src[i];
	} while (src[i++] != 0);

	*size = i;
}
