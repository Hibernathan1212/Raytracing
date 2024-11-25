//
//  Input.hpp
//  Raytracing
//
//  Created by Nathan Thurber on 6/11/22.
//
#pragma once

#include "KeyCodes.hpp"

#include "glm.hpp"

class Input
{
public:
    static bool IsKeyDown(KeyCode keycode);
    static bool IsMouseButtonDown(MouseButton button);

    static glm::vec2 GetMousePosition();

    static void SetCursorMode(CursorMode mode);
};
