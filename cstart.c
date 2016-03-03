extern int __bss_start__;
extern int __bss_end__;
extern int _vectors;
extern int _vectors_end;

extern void main();

#define RPI_VECTOR_START 0x0

void _cstart() {
  // Zero out BSS
  int* bss = &__bss_start__;
  int* bss_end = &__bss_end__;
  while (bss < bss_end)
    *bss++ = 0;

  /*
   * Copy in interrupt vector vector and FIQ handler _vector and _vector_end are
   * symbols defined in the interrupt assembly file, at the beginning and end of
   * the vector and its embedded constants.
   */
  int *vectorsdst = (int *)RPI_VECTOR_START;
  int *vectors = &_vectors;
  int *vectors_end = &_vectors_end;
  while (vectors < vectors_end)
    *vectorsdst++ = *vectors++;

  main();
}
