#include <krnl/mem/pmm.h>
#include <krnl/boot/bootloaders/bootloader.h>
#include <krnl/debug/debug.h>
#include <krnl/mem/stack.h>

#ifndef PAGE_SIZE
#define PAGE_SIZE 0x1000
#endif

struct pmm_memory_region
{
    uint64_t base;
    uint64_t length;
};

struct pmm_memory_region pmm_main_memory_region = {
    .base = 0,
    .length = 0};

// Inicio una estructura stack de uint64_t donde metere las paginas libres
Stack *free_pages_stack;

void pmm_init(void)
{
    if (!stack_is_empty(free_pages_stack)){
        stack_destroy(free_pages_stack);
        free_pages_stack = stack_create();
    }
        

    uint64_t memmap_entries = get_memory_map_entries();
    kprintf("Memory map entries: Xllu\n", memmap_entries);

    for (uint64_t i = 0; i < memmap_entries; i++)
    {
        uint64_t base = get_memory_map_base(i);
        uint64_t length = get_memory_map_length(i);
        uint64_t type = get_memory_map_type(i);

        if (type == 0x0)
        {
            kprintf("USABLE MEMORY ENTRY Base: 0xXllx Length: 0xXllx Type: Xllu\n", base, length, type);
            if (length > pmm_main_memory_region.length)
            {
                pmm_main_memory_region.base = base;
                pmm_main_memory_region.length = length;
                uint64_t region_start = (base + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1); //alineamiento de pagina
                uint64_t region_end = base + length;

                for (uint64_t addr = region_start; addr + PAGE_SIZE <= region_end; addr += PAGE_SIZE)
                    stack_push(free_pages_stack, addr, i);
            }
        }
        kprintf("MEMORY ENTRY Base: 0xXllx Length: 0xXllx Type: Xllu\n", base, length, type);

        kprintf("Selected main memory region Base: 0xXllx Length: 0xXllx\n", pmm_main_memory_region.base, pmm_main_memory_region.length);
    }
}

void *pmm_alloc_page()
{
    if (stack_is_empty(free_pages_stack)) {
        kprintf("PMM: No hay páginas libres disponibles!\n");
        return NULL;
    }

    uint64_t addr = stack_pop(free_pages_stack);
    return (void *)addr;
}

void *pmm_alloc_pages(uint64_t num_pages)
{
    for (uint64_t i = 0; i < num_pages; i++)
        *pmm_alloc_page();
}


void pmm_free_pages(void *addr, uint64_t num_pages)
{
    stack_push(free_pages_stack, (uint64_t)addr, -1);
}