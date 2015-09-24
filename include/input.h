#pragma once

#include "util.h"
#include "glutil.h"

// The Ix_AUTO and Ix_MAN macros in this file are dirty little hacks to make it much quicker to enter
// enums. They are both defined and undefined in this file, so they cannot be used outside of here.

namespace wake
{
    enum class InputState : uint8
    {
        PRESS = GLFW_PRESS,
        RELEASE = GLFW_RELEASE
    };

#define IK_AUTO(n) n = GLFW_KEY_##n,
#define IK_MAN(n, v) n = GLFW_KEY_##v,
    enum class KeyboardInput : int16
    {
        IK_AUTO(UNKNOWN)
        IK_AUTO(SPACE)
        IK_AUTO(APOSTROPHE)
        IK_AUTO(COMMA)
        IK_AUTO(MINUS)
        IK_AUTO(PERIOD)
        IK_AUTO(SLASH)
        IK_MAN(NUM_0, 0)
        IK_MAN(NUM_1, 1)
        IK_MAN(NUM_2, 2)
        IK_MAN(NUM_3, 3)
        IK_MAN(NUM_4, 4)
        IK_MAN(NUM_5, 5)
        IK_MAN(NUM_6, 6)
        IK_MAN(NUM_7, 7)
        IK_MAN(NUM_8, 8)
        IK_MAN(NUM_9, 9)
        IK_AUTO(SEMICOLON)
        IK_AUTO(EQUAL)
        IK_AUTO(A)
        IK_AUTO(B)
        IK_AUTO(C)
        IK_AUTO(D)
        IK_AUTO(E)
        IK_AUTO(F)
        IK_AUTO(G)
        IK_AUTO(H)
        IK_AUTO(I)
        IK_AUTO(J)
        IK_AUTO(K)
        IK_AUTO(L)
        IK_AUTO(M)
        IK_AUTO(N)
        IK_AUTO(O)
        IK_AUTO(P)
        IK_AUTO(Q)
        IK_AUTO(R)
        IK_AUTO(S)
        IK_AUTO(T)
        IK_AUTO(U)
        IK_AUTO(V)
        IK_AUTO(W)
        IK_AUTO(X)
        IK_AUTO(Y)
        IK_AUTO(Z)
        IK_AUTO(LEFT_BRACKET)
        IK_AUTO(BACKSLASH)
        IK_AUTO(RIGHT_BRACKET)
        IK_AUTO(GRAVE_ACCENT)
        IK_AUTO(WORLD_1)
        IK_AUTO(WORLD_2)
        IK_AUTO(ESCAPE)
        IK_AUTO(ENTER)
        IK_AUTO(TAB)
        IK_AUTO(BACKSPACE)
        IK_AUTO(INSERT)
        IK_AUTO(DELETE)
        IK_AUTO(RIGHT)
        IK_AUTO(LEFT)
        IK_AUTO(DOWN)
        IK_AUTO(UP)
        IK_AUTO(PAGE_UP)
        IK_AUTO(PAGE_DOWN)
        IK_AUTO(HOME)
        IK_AUTO(END)
        IK_AUTO(CAPS_LOCK)
        IK_AUTO(SCROLL_LOCK)
        IK_AUTO(NUM_LOCK)
        IK_AUTO(PRINT_SCREEN)
        IK_AUTO(PAUSE)
        IK_AUTO(F1)
        IK_AUTO(F2)
        IK_AUTO(F3)
        IK_AUTO(F4)
        IK_AUTO(F5)
        IK_AUTO(F6)
        IK_AUTO(F7)
        IK_AUTO(F8)
        IK_AUTO(F9)
        IK_AUTO(F10)
        IK_AUTO(F11)
        IK_AUTO(F12)
        IK_AUTO(F13)
        IK_AUTO(F14)
        IK_AUTO(F15)
        IK_AUTO(F16)
        IK_AUTO(F17)
        IK_AUTO(F18)
        IK_AUTO(F19)
        IK_AUTO(F20)
        IK_AUTO(F21)
        IK_AUTO(F22)
        IK_AUTO(F23)
        IK_AUTO(F24)
        IK_AUTO(F25)
        IK_AUTO(KP_0)
        IK_AUTO(KP_1)
        IK_AUTO(KP_2)
        IK_AUTO(KP_3)
        IK_AUTO(KP_4)
        IK_AUTO(KP_5)
        IK_AUTO(KP_6)
        IK_AUTO(KP_7)
        IK_AUTO(KP_8)
        IK_AUTO(KP_9)
        IK_AUTO(KP_DECIMAL)
        IK_AUTO(KP_DIVIDE)
        IK_AUTO(KP_MULTIPLY)
        IK_AUTO(KP_SUBTRACT)
        IK_AUTO(KP_ADD)
        IK_AUTO(KP_ENTER)
        IK_AUTO(KP_EQUAL)
        IK_AUTO(LEFT_SHIFT)
        IK_AUTO(LEFT_CONTROL)
        IK_AUTO(LEFT_ALT)
        IK_AUTO(LEFT_SUPER)
        IK_AUTO(RIGHT_SHIFT)
        IK_AUTO(RIGHT_CONTROL)
        IK_AUTO(RIGHT_ALT)
        IK_AUTO(RIGHT_SUPER)
        IK_AUTO(MENU)
    };
#undef IK_AUTO
#undef IK_MAN

#define IM_AUTO(n) n = GLFW_MOUSE_##n,
    enum class MouseInput : uint8
    {
        IM_AUTO(BUTTON_1)
        IM_AUTO(BUTTON_2)
        IM_AUTO(BUTTON_3)
        IM_AUTO(BUTTON_4)
        IM_AUTO(BUTTON_5)
        IM_AUTO(BUTTON_6)
        IM_AUTO(BUTTON_7)
        IM_AUTO(BUTTON_8)
    };
#undef IM_AUTO

#define IJ_AUTO(n) J##n = GLFW_JOYSTICK_##n,
    enum class JoystickInput : uint8
    {
        IJ_AUTO(1)
        IJ_AUTO(2)
        IJ_AUTO(3)
        IJ_AUTO(4)
        IJ_AUTO(5)
        IJ_AUTO(6)
        IJ_AUTO(7)
        IJ_AUTO(8)
        IJ_AUTO(9)
        IJ_AUTO(10)
        IJ_AUTO(11)
        IJ_AUTO(12)
        IJ_AUTO(13)
        IJ_AUTO(14)
        IJ_AUTO(15)
        IJ_AUTO(16)
    };
#undef IJ_AUTO
}