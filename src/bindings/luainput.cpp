#include "bindings/luainput.h"
#include "bindings/luaevent.h"
#include "moduleregistry.h"
#include "engine.h"

namespace wake
{
    namespace binding
    {
        inline static void add_constant(lua_State* L, const char* name, lua_Integer value)
        {
            lua_pushstring(L, name);
            lua_pushinteger(L, value);
            lua_settable(L, -3);
        }

        void push_action_constants(lua_State* L)
        {
            lua_newtable(L);
            add_constant(L, "Press", (lua_Integer) InputAction::Press);
            add_constant(L, "Release", (lua_Integer) InputAction::Release);
            add_constant(L, "Repeat", (lua_Integer) InputAction::Repeat);
        }

#define KB_AUTO(name) add_constant(L, #name, (lua_Integer) KeyboardInput::name)

        void push_keyboard_constants(lua_State* L)
        {
            lua_newtable(L);
            KB_AUTO(Unknown);
            KB_AUTO(Space);
            KB_AUTO(Apostrophe);
            KB_AUTO(Comma);
            KB_AUTO(Minus);
            KB_AUTO(Period);
            KB_AUTO(Slash);
            KB_AUTO(Num0);
            KB_AUTO(Num1);
            KB_AUTO(Num2);
            KB_AUTO(Num3);
            KB_AUTO(Num4);
            KB_AUTO(Num5);
            KB_AUTO(Num6);
            KB_AUTO(Num7);
            KB_AUTO(Num8);
            KB_AUTO(Num9);
            KB_AUTO(Semicolon);
            KB_AUTO(Equal);
            KB_AUTO(A);
            KB_AUTO(B);
            KB_AUTO(C);
            KB_AUTO(D);
            KB_AUTO(E);
            KB_AUTO(F);
            KB_AUTO(G);
            KB_AUTO(H);
            KB_AUTO(I);
            KB_AUTO(J);
            KB_AUTO(K);
            KB_AUTO(L);
            KB_AUTO(M);
            KB_AUTO(N);
            KB_AUTO(O);
            KB_AUTO(P);
            KB_AUTO(Q);
            KB_AUTO(R);
            KB_AUTO(S);
            KB_AUTO(T);
            KB_AUTO(U);
            KB_AUTO(V);
            KB_AUTO(W);
            KB_AUTO(X);
            KB_AUTO(Y);
            KB_AUTO(Z);
            KB_AUTO(LeftBracket);
            KB_AUTO(Backslash);
            KB_AUTO(RightBracket);
            KB_AUTO(Tilde);
            KB_AUTO(World1);
            KB_AUTO(World2);
            KB_AUTO(Escape);
            KB_AUTO(Enter);
            KB_AUTO(Tab);
            KB_AUTO(Backspace);
            KB_AUTO(Insert);
            KB_AUTO(Delete);
            KB_AUTO(Right);
            KB_AUTO(Left);
            KB_AUTO(Down);
            KB_AUTO(Up);
            KB_AUTO(PageUp);
            KB_AUTO(PageDown);
            KB_AUTO(Home);
            KB_AUTO(End);
            KB_AUTO(CapsLock);
            KB_AUTO(ScrollLock);
            KB_AUTO(NumLock);
            KB_AUTO(PrintScreen);
            KB_AUTO(Pause);
            KB_AUTO(F1);
            KB_AUTO(F2);
            KB_AUTO(F3);
            KB_AUTO(F4);
            KB_AUTO(F5);
            KB_AUTO(F6);
            KB_AUTO(F7);
            KB_AUTO(F8);
            KB_AUTO(F9);
            KB_AUTO(F10);
            KB_AUTO(F11);
            KB_AUTO(F12);
            KB_AUTO(F13);
            KB_AUTO(F14);
            KB_AUTO(F15);
            KB_AUTO(F16);
            KB_AUTO(F17);
            KB_AUTO(F18);
            KB_AUTO(F19);
            KB_AUTO(F20);
            KB_AUTO(F21);
            KB_AUTO(F22);
            KB_AUTO(F23);
            KB_AUTO(F24);
            KB_AUTO(F25);
            KB_AUTO(KP0);
            KB_AUTO(KP0);
            KB_AUTO(KP0);
            KB_AUTO(KP1);
            KB_AUTO(KP2);
            KB_AUTO(KP3);
            KB_AUTO(KP4);
            KB_AUTO(KP5);
            KB_AUTO(KP6);
            KB_AUTO(KP7);
            KB_AUTO(KP8);
            KB_AUTO(KP9);
            KB_AUTO(KPDecimal);
            KB_AUTO(KPDivide);
            KB_AUTO(KPMultiply);
            KB_AUTO(KPSubtract);
            KB_AUTO(KPAdd);
            KB_AUTO(KPEnter);
            KB_AUTO(LeftShift);
            KB_AUTO(LeftControl);
            KB_AUTO(LeftAlt);
            KB_AUTO(LeftSuper);
            KB_AUTO(RightShift);
            KB_AUTO(RightControl);
            KB_AUTO(RightAlt);
            KB_AUTO(RightSuper);
            KB_AUTO(Menu);
        }

#undef KB_AUTO

        void push_mouse_constants(lua_State* L)
        {
            lua_newtable(L);
            add_constant(L, "Button1", (lua_Integer) MouseInput::Button1);
            add_constant(L, "Button2", (lua_Integer) MouseInput::Button2);
            add_constant(L, "Button3", (lua_Integer) MouseInput::Button3);
            add_constant(L, "Button4", (lua_Integer) MouseInput::Button4);
            add_constant(L, "Button5", (lua_Integer) MouseInput::Button5);
            add_constant(L, "Button6", (lua_Integer) MouseInput::Button6);
            add_constant(L, "Button7", (lua_Integer) MouseInput::Button7);
            add_constant(L, "Button8", (lua_Integer) MouseInput::Button8);
            add_constant(L, "Left", (lua_Integer) MouseInput::Left);
            add_constant(L, "Right", (lua_Integer) MouseInput::Right);
            add_constant(L, "Middle", (lua_Integer) MouseInput::Middle);
        }

        static int getKey(lua_State* L)
        {
            int key = luaL_checkinteger(L, 1);
            InputAction state = W_INPUT.getKey((KeyboardInput) key);
            pushValue(L, state);
            return 1;
        }

        static int getMouseButton(lua_State* L)
        {
            int button = luaL_checkinteger(L, 1);
            InputAction state = W_INPUT.getMouseButton((MouseInput) button);
            pushValue(L, state);
            return 1;
        }

        static int getCursorPos(lua_State* L)
        {
            double x, y;
            W_INPUT.getCursorPosition(&x, &y);

            lua_pushnumber(L, (lua_Number) x);
            lua_pushnumber(L, (lua_Number) y);
            return 2;
        }

        static const struct luaL_reg inputlib_f[] = {
                {"getKey",         getKey},
                {"getMouseButton", getMouseButton},
                {"getCursorPos",   getCursorPos},
                {NULL, NULL}
        };

        int luaopen_input(lua_State* L)
        {
            luaL_register(L, "input", inputlib_f);

            // event table
            lua_pushstring(L, "event");

            lua_newtable(L);

            lua_pushstring(L, "key");
            pushValue(L, W_INPUT.KeyEvent);
            lua_settable(L, -3);

            lua_pushstring(L, "mouseButton");
            pushValue(L, W_INPUT.MouseButtonEvent);
            lua_settable(L, -3);

            lua_pushstring(L, "cursorPos");
            pushValue(L, W_INPUT.CursorPositionEvent);
            lua_settable(L, -3);

            lua_settable(L, -3);
            // end event table

            lua_pushstring(L, "action");
            push_action_constants(L);
            lua_settable(L, -3);

            lua_pushstring(L, "key");
            push_keyboard_constants(L);
            lua_settable(L, -3);

            lua_pushstring(L, "mouse");
            push_mouse_constants(L);
            lua_settable(L, -3);

            return 1;
        }

        W_REGISTER_MODULE(luaopen_input);
    }

    void pushValue(lua_State* L, InputAction value)
    {
        lua_pushinteger(L, (lua_Integer) value);
    }

    void pushValue(lua_State* L, KeyboardInput value)
    {
        lua_pushinteger(L, (lua_Integer) value);
    }

    void pushValue(lua_State* L, MouseInput value)
    {
        lua_pushinteger(L, (lua_Integer) value);
    }
}