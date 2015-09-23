#pragma once

#include <glm/fwd.hpp>

#include "luautil.h"

namespace wake
{
    namespace binding
    {
        ////
        // Vectors
        ////

        template<typename VecType>
        struct VectorInfo
        {
        };

        template<>
        struct VectorInfo<glm::vec2>
        {
            static inline int elements()
            {
                return 2;
            }

            static inline const char* metatable()
            {
                return "Wake.Vector2";
            }

            static inline const char* type()
            {
                return "Vector2";
            }
        };

        int luaopen_vector2(lua_State* L);

        template<>
        struct VectorInfo<glm::vec3>
        {
            static inline int elements()
            {
                return 3;
            }

            static inline const char* metatable()
            {
                return "Wake.Vector3";
            }

            static inline const char* type()
            {
                return "Vector3";
            }
        };

        int luaopen_vector3(lua_State* L);

        template<>
        struct VectorInfo<glm::vec4>
        {
            static inline int elements()
            {
                return 4;
            }

            static inline const char* metatable()
            {
                return "Wake.Vector4";
            }

            static inline const char* type()
            {
                return "Vector4";
            }
        };

        int luaopen_vector4(lua_State* L);

        ////
        // Matrices
        ////

        template<typename MatType>
        struct MatrixInfo
        {
        };

        template<>
        struct MatrixInfo<glm::mat2x2>
        {
            static inline int elements()
            {
                return 4;
            }

            static inline int columns()
            {
                return 2;
            }

            static inline int rows()
            {
                return 2;
            }

            static inline const char* metatable()
            {
                return "Wake.Matrix2x2";
            }

            static inline const char* type()
            {
                return "Matrix2x2";
            }
        };

        int luaopen_matrix2x2(lua_State* L);

        template<>
        struct MatrixInfo<glm::mat2x3>
        {
            static inline int elements()
            {
                return 6;
            }

            static inline int columns()
            {
                return 3;
            }

            static inline int rows()
            {
                return 2;
            }

            static inline const char* metatable()
            {
                return "Wake.Matrix2x3";
            }

            static inline const char* type()
            {
                return "Matrix2x3";
            }
        };

        int luaopen_matrix2x3(lua_State* L);

        template<>
        struct MatrixInfo<glm::mat2x4>
        {
            static inline int elements()
            {
                return 8;
            }

            static inline int columns()
            {
                return 4;
            }

            static inline int rows()
            {
                return 2;
            }

            static inline const char* metatable()
            {
                return "Wake.Matrix2x4";
            }

            static inline const char* type()
            {
                return "Matrix2x4";
            }
        };

        int luaopen_matrix2x4(lua_State* L);

        template<>
        struct MatrixInfo<glm::mat3x2>
        {
            static inline int elements()
            {
                return 6;
            }

            static inline int columns()
            {
                return 2;
            }

            static inline int rows()
            {
                return 3;
            }

            static inline const char* metatable()
            {
                return "Wake.Matrix3x2";
            }

            static inline const char* type()
            {
                return "Matrix3x2";
            }
        };

        int luaopen_matrix3x2(lua_State* L);

        template<>
        struct MatrixInfo<glm::mat3x3>
        {
            static inline int elements()
            {
                return 9;
            }

            static inline int columns()
            {
                return 3;
            }

            static inline int rows()
            {
                return 3;
            }

            static inline const char* metatable()
            {
                return "Wake.Matrix3x3";
            }

            static inline const char* type()
            {
                return "Matrix3x3";
            }
        };

        int luaopen_matrix3x3(lua_State* L);

        template<>
        struct MatrixInfo<glm::mat3x4>
        {
            static inline int elements()
            {
                return 12;
            }

            static inline int columns()
            {
                return 4;
            }

            static inline int rows()
            {
                return 3;
            }

            static inline const char* metatable()
            {
                return "Wake.Matrix3x4";
            }

            static inline const char* type()
            {
                return "Matrix3x4";
            }
        };

        int luaopen_matrix3x4(lua_State* L);

        template<>
        struct MatrixInfo<glm::mat4x2>
        {
            static inline int elements()
            {
                return 8;
            }

            static inline int columns()
            {
                return 2;
            }

            static inline int rows()
            {
                return 4;
            }

            static inline const char* metatable()
            {
                return "Wake.Matrix4x2";
            }

            static inline const char* type()
            {
                return "Matrix4x2";
            }
        };

        int luaopen_matrix4x2(lua_State* L);

        template<>
        struct MatrixInfo<glm::mat4x3>
        {
            static inline int elements()
            {
                return 12;
            }

            static inline int columns()
            {
                return 3;
            }

            static inline int rows()
            {
                return 4;
            }

            static inline const char* metatable()
            {
                return "Wake.Matrix4x3";
            }

            static inline const char* type()
            {
                return "Matrix4x3";
            }
        };

        int luaopen_matrix4x3(lua_State* L);

        template<>
        struct MatrixInfo<glm::mat4x4>
        {
            static inline int elements()
            {
                return 16;
            }

            static inline int columns()
            {
                return 4;
            }

            static inline int rows()
            {
                return 4;
            }

            static inline const char* metatable()
            {
                return "Wake.Matrix4x4";
            }

            static inline const char* type()
            {
                return "Matrix4x4";
            }
        };

        int luaopen_matrix4x4(lua_State* L);
    }

    void pushValue(lua_State* L, const glm::vec2& value);
    void pushValue(lua_State* L, const glm::vec3& value);
    void pushValue(lua_State* L, const glm::vec4& value);

    void pushValue(lua_State* L, const glm::mat2x2& value);
    void pushValue(lua_State* L, const glm::mat2x3& value);
    void pushValue(lua_State* L, const glm::mat2x4& value);

    void pushValue(lua_State* L, const glm::mat3x2& value);
    void pushValue(lua_State* L, const glm::mat3x3& value);
    void pushValue(lua_State* L, const glm::mat3x4& value);

    void pushValue(lua_State* L, const glm::mat4x2& value);
    void pushValue(lua_State* L, const glm::mat4x3& value);
    void pushValue(lua_State* L, const glm::mat4x4& value);

    glm::vec2* luaW_checkvector2(lua_State* L, int narg);
    glm::vec3* luaW_checkvector3(lua_State* L, int narg);
    glm::vec4* luaW_checkvector4(lua_State* L, int narg);

    glm::mat2x2* luaW_checkmatrix2x2(lua_State* L, int narg);
    glm::mat2x3* luaW_checkmatrix2x3(lua_State* L, int narg);
    glm::mat2x4* luaW_checkmatrix2x4(lua_State* L, int narg);

    glm::mat3x2* luaW_checkmatrix3x2(lua_State* L, int narg);
    glm::mat3x3* luaW_checkmatrix3x3(lua_State* L, int narg);
    glm::mat3x4* luaW_checkmatrix3x4(lua_State* L, int narg);

    glm::mat4x2* luaW_checkmatrix4x2(lua_State* L, int narg);
    glm::mat4x3* luaW_checkmatrix4x3(lua_State* L, int narg);
    glm::mat4x4* luaW_checkmatrix4x4(lua_State* L, int narg);
}