#include "elf_reader.h"
#include "cafe_loader.h"

#include <idaldr.h>

static int idaapi
accept_file(qstring *fileformatname, qstring *processor, linput_t *li, const char * /*filename*/)
{
  elf_reader<elf32> elf(li);
  if (elf.verifyHeader())
  {
    if (elf.type() == ELF_FILETYPE_CAFE_RPL)
    {
      *processor = "ppc";
      *fileformatname = "WII U RPX/RPL";

      return ACCEPT_FIRST | 1;
    }
  }

  return 0;
}

static void idaapi
load_file(linput_t *li, ushort /*neflags*/, const char * /*fileformatname*/)
{
  set_processor_type("ppc", SETPROC_LOADER);

  elf_reader<elf32> elf(li);
  elf.read();

  cafe_loader ldr(&elf); ldr.apply();
}

#ifdef _WIN32
__declspec(dllexport)
#endif
loader_t LDSC =
{
  IDP_INTERFACE_VERSION,
  LDRF_REQ_PROC,
  accept_file,
  load_file,
  nullptr,
  nullptr,
  nullptr
};
