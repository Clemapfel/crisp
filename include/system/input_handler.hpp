// 
// Copyright 2021 Clemens Cords
// Created on 26.07.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <system/key_id.hpp>
#include <vector.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <string>
#include <map>

namespace crisp
{
    class RenderWindow;

    // static handler that allows accesing the keyboard/mouse state
    class InputHandler
    {
        friend class RenderWindow;

        public:
            // @brief InputHandler cannot be constructed, all access happens through static functions
            InputHandler() = delete;

            // @brief does the current state show that the key is pressed?
            // @param key_id: id of key
            // @returns true if down, false otherwise
            static bool is_key_down(KeyID key_id);

            // @brief is the key state this cycle different than last cycle
            // @param key_id: id of key
            // @returns true if state is different, false otherwise
            static bool has_state_changed(KeyID key_id);

            // @brief is key down this cycle, not down last cycle
            // @param key_id: id of key
            // @returns true if key is down this cycle and not down last cycle, false otherwise
            static bool was_key_pressed(KeyID key_id);

            // @brief is key not down this cycle, down last cycle
            // @param key_id: id of key
            // @returns true if key is up this cycle and down last cycle, false otherwise
            static bool was_key_released(KeyID key_id);

            // @brief how long was the key in a pressed state
            // @param key_id: id of key
            // @returns time since the key was first pressed
            // @note the duration only updates after each render cycles because it is tied to the operating systems window updating
            static sf::Time get_hold_duration(KeyID key_id);

            // @brief get position of mouse cursor relative to the windows origin
            // @returns {0,0} if the cursor is in the top left of the window, {resolution.x, resolution.y} if in the bottom right, outside those bounds if the cursor is outside of the window
            static Vector2f cursor_position();

        protected:
            static void set_window(RenderWindow*);
            static void update(sf::Time);

        private:
            static void process_event(sf::Event&);
            static KeyID sf_key_to_id(sf::Keyboard::Key);

            static inline RenderWindow* _window = nullptr;

            static inline Vector2f _cursor_position = Vector2f(0,0);
            static inline std::map<KeyID, sf::Time> _hold_duration = {};

            static inline size_t _current_state_index = 1;
            static inline std::array<std::map<KeyID, bool>, 2> _detail_states = std::array<std::map<KeyID, bool>, 2>();
            static bool states_at(size_t i, KeyID id);
    };
}

#include ".src/input_handler.inl"