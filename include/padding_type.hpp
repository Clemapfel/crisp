// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

namespace crisp
{
    /// @brief enum that governs what values indices out of bounds will return
    enum PaddingType
    {
        /// let image be [1, 2, 3, 4] (size 4*1), then padding has the form:
        /// ...0, 0, [1, 2, 3, 4], 0, 0, 0...
        ZERO = 0,
        
        /// let image be [1, 2, 3, 4] (size 4*1), then padding has the form:
        // ...1, 1, [1, 2, 3, 4], 1, 1, 1...
        ONE = 1,
        
        /// let image be [1, 2, 3, 4] (size 4*1), then padding has the form:
        // ...3, 4, [1, 2, 3, 4], 1, 2, 3...
        REPEAT,
        
        /// let image be [1, 2, 3, 4] (size 4*1), then padding has the form:
        // ...2, 1, [1, 2, 3, 4], 4, 3, 2...
        MIRROR,
        
        /// let image be [1, 2, 3, 4] (size 4*1), then padding has the form:
        // ...1, 1, [1, 2, 3, 4], 4, 4, 4...
        STRETCH,
    };
}