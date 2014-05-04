#ifndef HIGHFLYERS_PROTOCOL_FRAME_BUILDER_H
#define HIGHFLYERS_PROTOCOL_FRAME_BUILDER_H

#include "frame_parser_helper.h"
#include "frames.h"

void* frame_builder_build_frame (byte* bytes, int size);
#endif
