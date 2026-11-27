#ifndef OROGRAPH_OROGRAPH_H
#define OROGRAPH_OROGRAPH_H

#include <stdint.h>

#include "orograph_export.h"

#ifdef __cplusplus
extern "C" {
#define OROGRAPH_NOEXCEPT noexcept
#else
#define OROGRAPH_NOEXCEPT
#endif

OROGRAPH_EXPORT uint32_t og_add(uint32_t a, uint32_t b) OROGRAPH_NOEXCEPT;

#ifdef __cplusplus
}
#endif

#undef OROGRAPH_NOEXCEPT

#endif // OROGRAPH_OROGRAPH_H