#pragma once

#include "elf_reader.h"
#include "cafe.h"

#include <vector>

class cafe_loader {
  elf_reader<elf32> *m_elf;
  uint32 m_relocAddr;

  ea_t m_externStart;
  ea_t m_externEnd;

  struct import {
    ea_t addr;
    ea_t orig;
    const char *name;
  };

  std::vector<import> m_imports;
  
public:
  cafe_loader(elf_reader<elf32> *elf);
  
  void apply();
  
private:
  void applySegments();
  void applySegment(uint32 sel,
                    const char *data,
                    uint32 addr,
                    uint32 size,
                    const char *name,
                    const char *sclass,
                    uchar perm,
                    uchar align,
                    bool load);

  void applyRelocations();

  void processImports();
  void processExports();

  void swapSymbols();
  void applySymbols();
};
