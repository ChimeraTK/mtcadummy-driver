#include "StructReaderWriter.h"

#include <unistd.h>
#include <fcntl.h>
#include "pciedev_io.h"

#include <iostream>
#include <cstdio>

// sorry, this is rather C-Style

StructReaderWriter::StructReaderWriter(int fileDescriptor)
  : ReaderWriter(fileDescriptor){
}

int32_t StructReaderWriter::readSingle(uint32_t offset, uint32_t bar){
  device_rw readInstruction;
  readInstruction.offset_rw = offset;
  readInstruction.data_rw = 0;
  readInstruction.mode_rw = RW_D32;
  readInstruction.barx_rw = bar;
  readInstruction.size_rw = sizeof(device_rw);
  readInstruction.rsrvd_rw  = 0;

  if ( read(_fileDescriptor, &readInstruction, sizeof(device_rw)) 
       != sizeof(device_rw) )
    {     
      std::cerr << "Error reading from device" << std::endl;
      throw DeviceIOException();
    }

  return readInstruction.data_rw;
}

void StructReaderWriter::readArea(uint32_t offset, uint32_t bar, uint32_t nWords,
	      int32_t * readBuffer){
  for (uint32_t i = 0; i < nWords; ++i){
    readBuffer[i] = readSingle(offset +i/sizeof(int32_t), bar);
  }
}

void StructReaderWriter::writeSingle(uint32_t offset, uint32_t bar, int32_t value){
  device_rw writeInstruction;
  writeInstruction.offset_rw = offset;
  writeInstruction.data_rw = value;
  writeInstruction.mode_rw = RW_D32;
  writeInstruction.barx_rw = bar;
  writeInstruction.size_rw = sizeof(device_rw);
  writeInstruction.rsrvd_rw  = 0;

  if ( write(_fileDescriptor, &writeInstruction, sizeof(device_rw)) 
       != sizeof(device_rw) )
    {     
      std::cerr << "Error writing to device" << std::endl;
      throw DeviceIOException();
    }
}
void StructReaderWriter::writeArea(uint32_t offset, uint32_t bar, uint32_t nWords,
	       int32_t * writeBuffer){
  for (uint32_t i = 0; i < nWords; ++i){
    writeSingle(offset +i/sizeof(int32_t), bar, writeBuffer[i]);
  }
}

