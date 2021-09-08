// 
// Copyright 2021 Clemens Cords
// Created on 26.07.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SFML/Window/Keyboard.hpp>

namespace crisp
{
    // ID of keyboard and mouse keys
    // @note does not support all keys to assure portability and consistency across all keyboard and mouse types
    enum KeyID : int
    {
        UNKNOWN = sf::Keyboard::Key::Unknown, // any key not specifically mentioned below

        ESCAPE = sf::Keyboard::Key::Escape,         // escape key
        SPACE = sf::Keyboard::Key::Space,           // space bar
        ENTER = sf::Keyboard::Key::Enter,           // enter key
        BACKSPACE = sf::Keyboard::Key::BackSpace,   // backspace key

        UP = sf::Keyboard::Key::Up,           // up arrow
        DOWN = sf::Keyboard::Key::Down,       // down arrow
        LEFT = sf::Keyboard::Key::Left,       // left arrow
        RIGHT = sf::Keyboard::Key::Right,     // right arrow

        SHIFT = sf::Keyboard::Key::LShift,    // left shift key, including caps lock
        RSHIFT = sf::Keyboard::Key::RShift,   // right shift key, including caps lock
        CTRL = sf::Keyboard::Key::LControl,   // left control key
        RCTRL = sf::Keyboard::Key::RControl,  // right control key
        ALT = sf::Keyboard::Key::LAlt,        // left alt key
        RALT = sf::Keyboard::Key::RAlt,       // right alt key

        MOUSE_LEFT = -2,                      // left mouse button
        MOUSE_RIGHT = -3,                     // right mouse button
                                              // scrollwheel or side-buttons are not supported

        A = sf::Keyboard::Key::A,             // letter keys, umlaute and other special letters are not supported
        B = sf::Keyboard::Key::B,             // ...
        C = sf::Keyboard::Key::C,
        D = sf::Keyboard::Key::D,
        E = sf::Keyboard::Key::E,
        F = sf::Keyboard::Key::F,
        G = sf::Keyboard::Key::G,
        H = sf::Keyboard::Key::H,
        I = sf::Keyboard::Key::I,
        J = sf::Keyboard::Key::J,
        K = sf::Keyboard::Key::K,
        L = sf::Keyboard::Key::L,
        M = sf::Keyboard::Key::M,
        N = sf::Keyboard::Key::N,
        O = sf::Keyboard::Key::O,
        P = sf::Keyboard::Key::P,
        Q = sf::Keyboard::Key::Q,
        R = sf::Keyboard::Key::R,
        S = sf::Keyboard::Key::S,
        T = sf::Keyboard::Key::T,
        U = sf::Keyboard::Key::U,
        V = sf::Keyboard::Key::V,
        W = sf::Keyboard::Key::W,
        X = sf::Keyboard::Key::X,
        Y = sf::Keyboard::Key::Y,
        Z = sf::Keyboard::Key::Z,
        NUM_0 = sf::Keyboard::Key::Num0,    // number keys that are NOT the on the numpad, numpad keys are not supported
        NUM_1 = sf::Keyboard::Key::Num1,    // ...
        NUM_2 = sf::Keyboard::Key::Num2,
        NUM_3 = sf::Keyboard::Key::Num3,
        NUM_4 = sf::Keyboard::Key::Num4,
        NUM_5 = sf::Keyboard::Key::Num5,
        NUM_6 = sf::Keyboard::Key::Num6,
        NUM_7 = sf::Keyboard::Key::Num7,
        NUM_8 = sf::Keyboard::Key::Num8,
        NUM_9 = sf::Keyboard::Key::Num9,
        F1 = sf::Keyboard::Key::F1,         // FN keys, N > 12 is not supported
        F2 = sf::Keyboard::Key::F2,         // ...
        F3 = sf::Keyboard::Key::F3,
        F4 = sf::Keyboard::Key::F4,
        F5 = sf::Keyboard::Key::F5,
        F6 = sf::Keyboard::Key::F6,
        F7 = sf::Keyboard::Key::F7,
        F8 = sf::Keyboard::Key::F8,
        F9 = sf::Keyboard::Key::F9,
        F10 = sf::Keyboard::Key::F10,
        F11 = sf::Keyboard::Key::F11,
        F12 = sf::Keyboard::Key::F12
    };
}