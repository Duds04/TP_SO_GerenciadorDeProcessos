#ifndef PAGINAS_H
#define PAGINAS_H

#include <stdbool.h>

// Informações sobre as páginas de um processo
typedef struct {
    int paginaInicial; // índice da sua primeira página
    int numPaginas;    // quantidade de páginas
    bool noDisco;      // as páginas do processo estão no disco?
} ProcessoPagInfo;

#define PAGINA_NULA (ProcessoPagInfo){ -1, -1, false }

#endif // PAGINAS_H
