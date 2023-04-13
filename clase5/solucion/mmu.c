/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"

#include "kassert.h"

static pd_entry_t* kpd = (pd_entry_t*)KERNEL_PAGE_DIR;
static pt_entry_t* kpt = (pt_entry_t*)KERNEL_PAGE_TABLE_0;

//static const uint32_t identity_mapping_end = 0x003FFFFF;
//static const uint32_t user_memory_pool_end = 0x02FFFFFF;

static paddr_t next_free_kernel_page = 0x100000;
static paddr_t next_free_user_page = 0x400000;

/**
 * kmemset asigna el valor c a un rango de memoria interpretado 
 * como un rango de bytes de largo n que comienza en s 
 * @param s es el puntero al comienzo del rango de memoria
 * @param c es el valor a asignar en cada byte de s[0..n-1]
 * @param n es el tamaño en bytes a asignar
 * @return devuelve el puntero al rango modificado (alias de s)
*/
static inline void* kmemset(void* s, int c, size_t n) {
  uint8_t* dst = (uint8_t*)s;
  for (size_t i = 0; i < n; i++) {
    dst[i] = c;
  }
  return dst;
}

/**
 * zero_page limpia el contenido de una página que comienza en addr
 * @param addr es la dirección del comienzo de la pagina a limpiar
*/
static inline void zero_page(paddr_t addr) {
  kmemset((void*)addr, 0x00,PAGE_SIZE);
}


void mmu_init(void) {}


/**
 * mmu_next_free_kernel_page devuelve la dirección de la próxima página de kernel disponible
 * @return devuelve la dirección de memoria de comienzo de la próxima página libre de kernel
 */
paddr_t mmu_next_free_kernel_page(void) {
  paddr_t nfkp = next_free_kernel_page;
  next_free_kernel_page += 0x1000;
  return nfkp;
}

/**
 * mmu_next_free_user_page devuelve la dirección de la próxima página de usuarix disponible
 * @return devuelve la dirección de memoria de comienzo de la próxima página libre de usuarix
 */
paddr_t mmu_next_free_user_page(void) {
  paddr_t nfup = next_free_user_page;
  next_free_user_page += 0x1000;
  return nfup;
}

/**
 * mmu_init_kernel_dir inicializa las estructuras de paginación vinculadas al kernel y
 * realiza el identity mapping
 * @return devuelve la dirección de memoria de la página donde se encuentra el directorio 
 * de páginas usado por el kernel
 */
paddr_t mmu_init_kernel_dir(void) {
  for(int i = 0; i < 1024; i++){
    kpd[i].pt = 0;
    kpd[i].attrs = 0;
    kpt[i].page = 0;
    kpt[i].attrs = 0;
  }
  kpd[0].pt = KERNEL_PAGE_TABLE_0 >> 12;
  kpd[0].attrs = 0x003;
  for(int i = 0; i < 1024; i++){
    kpt[i].page = i;
    kpt[i].attrs = 0x003;
  }
  tlbflush();
  return KERNEL_PAGE_DIR;
}

// /**
//  * mmu_map_page agrega las entradas necesarias a las estructuras de paginación de modo de que
//  * la dirección virtual virt se traduzca en la dirección física phy con los atributos definidos en attrs
//  * @param cr3 el contenido que se ha de cargar en un registro CR3 al realizar la traducción
//  * @param virt la dirección virtual que se ha de traducir en phy
//  * @param phy la dirección física que debe ser accedida (dirección de destino)
//  * @param attrs los atributos a asignar en la entrada de la tabla de páginas
//  */
void mmu_map_page(uint32_t cr3, vaddr_t virt, paddr_t phy, uint32_t attrs) {
  pd_entry_t *pd = (pd_entry_t *) (cr3 & ~0xFFF);
  uint32_t directoryIdx = virt >> 22; //10 bits +signif de virt
  uint32_t tableIdx = (virt >> 12) & 0x3FF; //siguientes 10 bits de virt
  if ((pd[directoryIdx].attrs & 0x1) != 0x1){ //si bit presente != 1
    pt_entry_t* newPT = (pt_entry_t *) mmu_next_free_kernel_page();
    for(int i = 0; i < 1024; ++i){
      newPT[i] = (pt_entry_t){0};
    }
    pd[directoryIdx].attrs = attrs | 1;
    pd[directoryIdx].pt = (uint32_t) newPT >> 12;
  }
  pt_entry_t* pt = (pt_entry_t *) (pd[directoryIdx].pt << 12); //
  pt[tableIdx].attrs = attrs | 1;
  pt[tableIdx].page = phy >> 12;

  tlbflush();
}
// 
// /**
//  * mmu_unmap_page elimina la entrada vinculada a la dirección virt en la tabla de páginas correspondiente
//  * @param virt la dirección virtual que se ha de desvincular
//  * @return la dirección física de la página desvinculada
// */
paddr_t mmu_unmap_page(uint32_t cr3, vaddr_t virt) {
  pd_entry_t *pd = (pd_entry_t*) (cr3 & ~0xFFF);
  uint32_t directoryIdx = virt >> 22;
  uint32_t tableIdx = (virt >> 12) & 0x3FF;
  paddr_t r = 0x00000000;
  if ((pd[directoryIdx].attrs & 0x1) != 0x0){ //Si esta presente...
    pt_entry_t *pt = (pt_entry_t*) (pd[directoryIdx].pt << 12);
    r = (pt[tableIdx].page) << 12; //Copiamos la pagina en el physical address
    pt[tableIdx] = (pt_entry_t){0}; //seteamos toda la tabla en 0
  }
  tlbflush();
  return r; //devolvemos la pagina.
}

#define DST_VIRT_PAGE 0xA00000
#define SRC_VIRT_PAGE 0xB00000

// /**
//  * copy_page copia el contenido de la página física localizada en la dirección src_addr a la página física ubicada en dst_addr
//  * @param dst_addr la dirección a cuya página queremos copiar el contenido
//  * @param src_addr la dirección de la página cuyo contenido queremos copiar
//  * 
//  * Esta función mapea ambas páginas a las direcciones SRC_VIRT_PAGE y DST_VIRT_PAGE, respectivamente, realiza
//  * la copia y luego desmapea las páginas. Usar la función rcr3 definida en i386.h para obtener el cr3 actual
//  */
static inline void copy_page(paddr_t dst_addr, paddr_t src_addr) {
  uint32_t cr3 = rcr3();
  
  //Aca vamos a mapear las direcciones
  mmu_map_page(cr3, SRC_VIRT_PAGE, src_addr, 0x3);
  mmu_map_page(cr3, DST_VIRT_PAGE, dst_addr, 0x3);

  //casteo las direcciones virtuales
  uint8_t* dst = (uint8_t*) DST_VIRT_PAGE;
  uint8_t* src = (uint8_t*) SRC_VIRT_PAGE;

  //Copio
  for (int i = 0; i < 4096; i++) {
    dst[i] = src[i];
  }

  //Desmapeo
  mmu_unmap_page(cr3, SRC_VIRT_PAGE);
  mmu_unmap_page(cr3, DST_VIRT_PAGE);

}

//  /**
//  * mmu_init_task_dir inicializa las estructuras de paginación vinculadas a una tarea cuyo código se encuentra en la dirección phy_start
//  * @pararm phy_start es la dirección donde comienzan las dos páginas de código de la tarea asociada a esta llamada
//  * @return el contenido que se ha de cargar en un registro CR3 para la tarea asociada a esta llamada
//  */
paddr_t mmu_init_task_dir(paddr_t phy_start) {
  pd_entry_t *task_pd = (pd_entry_t*) mmu_next_free_kernel_page();
  paddr_t free_stack_page = mmu_next_free_user_page();
  for (int i = 0; i < 1024; i++) {
    task_pd[i] = (pd_entry_t) {0};
  }
  for (int i = 0; i < 1024; i++) {
    mmu_map_page((uint32_t)task_pd, i*PAGE_SIZE, i*PAGE_SIZE, 0x3); //Identity Mapping
  }

  mmu_map_page((uint32_t)task_pd, TASK_CODE_VIRTUAL, phy_start, 0x5);
  mmu_map_page((uint32_t)task_pd, TASK_CODE_VIRTUAL + PAGE_SIZE, phy_start + PAGE_SIZE, 0x5);
  mmu_map_page((uint32_t)task_pd, TASK_STACK_BASE, free_stack_page, 0x7);
  return (paddr_t) task_pd;  
}


  // uint32_t cr3 = rcr3();
  // mmu_map_page(cr3, TASK_CODE_VIRTUAL, phy_start, 0x5);
  // mmu_map_page(cr3, TASK_CODE_VIRTUAL + 0x1000, phy_start + 0x1000, 0x5); //0x1000 refiere a la siguiente pagina
  // mmu_map_page(cr3, TASK_STACK_BASE, free_page, 0x7);
  // return ????;
