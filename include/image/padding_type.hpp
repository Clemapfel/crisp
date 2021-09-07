// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

// enum that governs what values indices out of bounds will return
namespace crisp
{
    enum PaddingType
    {
        // let image be [1, 2, 3, 4] (size 4*1), then padding has the form:
        ZERO = 0,   // ...0, 0, [1, 2, 3, 4], 0, 0, 0...
        ONE = 1,    // ...1, 1, [1, 2, 3, 4], 1, 1, 1...
        REPEAT,     // ...3, 4, [1, 2, 3, 4], 1, 2, 3...
        MIRROR,     // ...2, 1, [1, 2, 3, 4], 4, 3, 2...
        STRETCH,     // ...1, 1, [1, 2, 3, 4], 4, 4, 4...    // default
        // only x-direction padding shown, analogous in y-direction
    };
}