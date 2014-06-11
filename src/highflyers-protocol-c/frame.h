#ifndef HIGHFLYERS_PROTOCOL_FRAME_H
#define HIGHFLYERS_PROTOCOL_FRAME_H

#include "types.h"

void frame_preparse_data(const byte* data, bool* output, int field_count);
int frame_serialize(const byte* serialized_data, int size, byte* output);

#endif
