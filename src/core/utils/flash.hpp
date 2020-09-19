/*
 *  Copyright (c) 2020, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef FLASH_HPP_
#define FLASH_HPP_

#include "openthread-core-config.h"

#include <stdint.h>

#include <openthread/error.h>
#include <openthread/platform/toolchain.h>

#include "common/debug.hpp"
#include "common/locator.hpp"

namespace ot {

/**
 * This class implements the flash storage driver.
 *
 */
class Flash : public InstanceLocator
{
public:
    /**
     * Constructor.
     *
     */
    Flash(Instance &aInstance)
        : InstanceLocator(aInstance)
    {
    }

    /**
     * This method initializes the flash storage driver.
     *
     */
    void Init(void);

    /**
     * This method fetches the value identified by @p aKey.
     *
     * @param[in]     aKey          The key associated with the requested value.
     * @param[in]     aIndex        The index of the specific item to get.
     * @param[out]    aValue        A pointer to where the value of the setting should be written.
     *                              May be nullptr if just testing for the presence or length of a key.
     * @param[inout]  aValueLength  A pointer to the length of the value.
     *                              When called, this should point to an integer containing the maximum bytes that
     *                              can be written to @p aValue.
     *                              At return, the actual length of the setting is written.
     *                              May be nullptr if performing a presence check.
     *
     * @retval OT_ERROR_NONE        The value was fetched successfully.
     * @retval OT_ERROR_NOT_FOUND   The key was not found.
     *
     */
    otError Get(uint16_t aKey, int aIndex, uint8_t *aValue, uint16_t *aValueLength) const;

    /**
     * This method sets or replaces the value identified by @p aKey.
     *
     * If there was more than one value previously associated with @p aKey, then they are all deleted and replaced with
     * this single entry.
     *
     * @param[in]  aKey          The key associated with the value.
     * @param[in]  aValue        A pointer to where the new value of the setting should be read from.
     *                           MUST NOT be nullptr if @p aValueLength is non-zero.
     * @param[in]  aValueLength  The length of the data pointed to by @p aValue. May be zero.
     *
     * @retval OT_ERROR_NONE     The value was changed.
     * @retval OT_ERROR_NO_BUFS  Not enough space to store the value.
     *
     */
    inline otError Set(uint16_t aKey, const uint8_t *aValue, uint16_t aValueLength)
    {
        return Add(aKey, true, aValue, aValueLength);
    }

    /**
     * This method adds a value to @p aKey.
     *
     * @param[in]  aKey          The key associated with the value.
     * @param[in]  aValue        A pointer to where the new value of the setting should be read from.
     *                           MUST NOT be nullptr if @p aValueLength is non-zero.
     * @param[in]  aValueLength  The length of the data pointed to by @p aValue. May be zero.
     *
     * @retval OT_ERROR_NONE     The value was added.
     * @retval OT_ERROR_NO_BUFS  Not enough space to store the value.
     *
     */
    inline otError Add(uint16_t aKey, const uint8_t *aValue, uint16_t aValueLength)
    {
        return Add(aKey, false, aValue, aValueLength);
    }

    /**
     * This method removes a value from @p aKey.
     *
     *
     * @param[in] aKey    The key associated with the value.
     * @param[in] aIndex  The index of the value to be removed.
     *                    If set to -1, all values for @p aKey will be removed.
     *
     * @retval OT_ERROR_NONE       The given key and index was found and removed successfully.
     * @retval OT_ERROR_NOT_FOUND  The given key or index was not found.
     *
     */
    otError Delete(uint16_t aKey, int aIndex);

    /**
     * This method removes all values.
     *
     */
    void Wipe(void);

    /**
     * This method gets the erase counter for the flash swap areas.
     *
     * The counter is incremented only when erasing swap area 0.
     *
     * @returns The erase counter, clamped to 65535.
     *
     */
    uint16_t GetEraseCounter(void) const;

private:
    otError Add(uint16_t aKey, bool aInvalidatePrevious, const uint8_t *aValue, uint16_t aValueLength);
    bool    IsInvalidated(uint32_t aOffset, uint16_t aKey) const;
    void    SanitizeFreeSpace(void);
    void    Swap(void);

    uint32_t mSwapSize;
    uint32_t mSwapUsed;
    uint8_t  mSwapIndex;
    uint8_t  mSwapHeaderSize;
    uint8_t  mFormat;
};

} // namespace ot

#endif // FLASH_HPP_
