#include "bindings/luamatrix.h"
#include "moduleregistry.h"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

namespace wake
{
    namespace binding
    {

        ////
        // Vector Implementation
        ////

        template<typename VecType>
        struct VectorContainer
        {
            VecType* vector;
        };

        template<typename VecType>
        static void pushVector(lua_State* L, const VecType& value)
        {
            auto* container = (VectorContainer<VecType>*) lua_newuserdata(L, sizeof(VectorContainer<VecType>));
            container->vector = new VecType(value);
            luaL_getmetatable(L, VectorInfo<VecType>::metatable());
            lua_setmetatable(L, -2);
        }

        template<typename VecType>
        static VecType* checkVector(lua_State* L, int idx)
        {
            const int elements = VectorInfo<VecType>::elements();

            if (lua_istable(L, idx))
            {
                luaL_argcheck(L, lua_objlen(L, idx) == elements, 1, "table is of incorrect length");

                VecType vec;
                for (int i = 0; i < elements; ++i)
                {
                    lua_pushnumber(L, i + 1);
                    lua_gettable(L, idx);
                    float value = (float) lua_tonumber(L, -1);
                    lua_pop(L, 1);
                    vec[i] = value;
                }

                pushVector(L, vec);
                return checkVector<VecType>(L, -1);
            }

            void* data = luaL_checkudata(L, idx, VectorInfo<VecType>::metatable());
            luaL_argcheck(L, data != nullptr, idx, "'Vector' or 'table' expected");
            return ((VectorContainer<VecType>*) data)->vector;
        }

        template<typename VecType>
        static int vec_new(lua_State* L)
        {
            const int elements = VectorInfo<VecType>::elements();

            int argCount = lua_gettop(L);
            switch (argCount)
            {
                default:
                {
                    if (argCount != elements)
                        luaL_error(L, "wrong number of arguments for %s.new", VectorInfo<VecType>::type());

                    VecType vec;
                    for (int i = 0; i < elements; ++i)
                    {
                        float value = (float) luaL_checknumber(L, i + 1);
                        vec[i] = value;
                    }

                    pushVector<VecType>(L, vec);
                    return 1;
                }

                case 0:
                    pushVector<VecType>(L, VecType());
                    return 1;

                case 1:
                {
                    auto* vec = checkVector<VecType>(L, 1);
                    pushVector(L, *vec);
                    return 1;
                }
            }
        }

        template<typename VecType>
        static int vec_table(lua_State* L)
        {
            auto& vec = *checkVector<VecType>(L, 1);
            lua_newtable(L);
            for (int i = 0; i < VectorInfo<VecType>::elements(); ++i)
            {
                lua_pushnumber(L, i + 1);
                lua_pushnumber(L, vec[i]);
                lua_settable(L, -3);
            }

            return 1;
        }

        template<typename VecType>
        static int vec_get(lua_State* L)
        {
            auto& vec = *checkVector<VecType>(L, 1);
            int index = luaL_checkinteger(L, 2);
            luaL_argcheck(L, index >= 1 && index <= VectorInfo<VecType>::elements(), 2, "index out of range");
            lua_pushnumber(L, vec[index - 1]);
            return 1;
        }

        template<typename VecType>
        static int vec_set(lua_State* L)
        {
            auto& vec = *checkVector<VecType>(L, 1);
            int index = luaL_checkinteger(L, 2);
            luaL_argcheck(L, index >= 1 && index <= VectorInfo<VecType>::elements(), 2, "index out of range");
            float value = (float) luaL_checknumber(L, 3);
            vec[index - 1] = value;
            return 0;
        }

        template<typename VecType>
        static int vec_setAll(lua_State* L)
        {
            auto& vec = *checkVector<VecType>(L, 1);
            for (int i = 0; i < VectorInfo<VecType>::elements(); ++i)
            {
                vec[i] = (float) luaL_checknumber(L, i + 2);
            }

            return 0;
        }

        template<typename VecType>
        static int vec_dot(lua_State* L)
        {
            auto& vecA = *checkVector<VecType>(L, 1);
            auto& vecB = *checkVector<VecType>(L, 2);
            lua_pushnumber(L, glm::dot(vecA, vecB));
            return 1;
        }

        template<typename VecType>
        static int vec_distance(lua_State* L)
        {
            auto& vecA = *checkVector<VecType>(L, 1);
            auto& vecB = *checkVector<VecType>(L, 2);
            lua_pushnumber(L, glm::distance(vecA, vecB));
            return 1;
        }

        template<typename VecType>
        static int vec_length(lua_State* L)
        {
            auto& vec = *checkVector<VecType>(L, 1);
            lua_pushnumber(L, glm::length(vec));
            return 1;
        }

        template<typename VecType>
        static int vec_apply(lua_State* L)
        {
            auto& vec = *checkVector<VecType>(L, 1);
            luaL_argcheck(L, lua_isfunction(L, 2), 2, "'function' expected");

            VecType result;
            for (int i = 0; i < VectorInfo<VecType>::elements(); ++i)
            {
                lua_pushvalue(L, 2);
                lua_pushnumber(L, vec[i]);
                lua_pushnumber(L, i + 1);
                lua_call(L, 2, 1);

                result[i] = (float) lua_tonumber(L, -1);
                lua_pop(L, 1);
            }

            pushVector<VecType>(L, result);
            return 1;
        }

        template<typename VecType>
        static int vec_normalize(lua_State* L)
        {
            auto& vec = *checkVector<VecType>(L, 1);
            pushVector<VecType>(L, glm::normalize(vec));
            return 1;
        }

        template<typename VecType>
        static int vec_reflect(lua_State* L)
        {
            auto& vecI = *checkVector<VecType>(L, 1);
            auto& vecN = *checkVector<VecType>(L, 2);
            pushVector<VecType>(L, glm::reflect(vecI, vecN));
            return 1;
        }

        template<typename VecType>
        static int vec_refract(lua_State* L)
        {
            auto& vecI = *checkVector<VecType>(L, 1);
            auto& vecN = *checkVector<VecType>(L, 2);
            float eta = (float) luaL_checknumber(L, 3);
            pushVector<VecType>(L, glm::refract(vecI, vecN, eta));
            return 1;
        }

        template<typename VecType>
        static int vec_m_gc(lua_State* L)
        {
            delete checkVector<VecType>(L, 1);
            return 0;
        }

        template<typename VecType>
        static int vec_m_eq(lua_State* L)
        {
            auto& vecA = *checkVector<VecType>(L, 1);
            auto& vecB = *checkVector<VecType>(L, 2);
            lua_pushboolean(L, vecA == vecB);
            return 1;
        }

        template<typename VecType>
        static int vec_m_tostring(lua_State* L)
        {
            auto& vec = *checkVector<VecType>(L, 1);
            lua_pushstring(L, glm::to_string(vec).c_str());
            return 1;
        }

        template<typename VecType>
        static int vec_m_len(lua_State* L)
        {
            lua_pushnumber(L, VectorInfo<VecType>::elements());
            return 1;
        }

        template<typename VecType>
        static int vec_m_unm(lua_State* L)
        {
            auto& vec = *checkVector<VecType>(L, 1);
            pushVector<VecType>(L, -vec);
            return 1;
        }

        // specific to Vector3
        static int vec3_cross(lua_State* L)
        {
            auto& vecA = *luaW_checkvector3(L, 1);
            auto& vecB = *luaW_checkvector3(L, 2);
            pushValue(L, glm::cross(vecA, vecB));
            return 1;
        }

        ////
        // Matrix Implementation
        ////

        template<typename MatType>
        struct MatrixContainer
        {
            MatType* matrix;
        };

        template<typename MatType>
        static void pushMatrix(lua_State* L, const MatType& value)
        {
            auto* container = (MatrixContainer<MatType>*) lua_newuserdata(L, sizeof(MatrixContainer<MatType>));
            container->matrix = new MatType(value);
            luaL_getmetatable(L, MatrixInfo<MatType>::metatable());
            lua_setmetatable(L, -2);
        }

        template<typename MatType>
        static MatType* checkMatrix(lua_State* L, int idx)
        {
            const int elements = MatrixInfo<MatType>::elements();
            const int columns = MatrixInfo<MatType>::columns();

            if (lua_istable(L, idx))
            {
                luaL_argcheck(L, lua_objlen(L, idx) == elements, 1, "table is of incorrect length");

                MatType mat;
                for (int i = 0; i < elements; ++i)
                {
                    lua_pushnumber(L, i + 1);
                    lua_gettable(L, idx);
                    float value = (float) lua_tonumber(L, -1);
                    lua_pop(L, 1);
                    mat[i / columns][i % columns] = value;
                }

                pushMatrix<MatType>(L, mat);
                return checkMatrix<MatType>(L, -1);
            }

            void* data = luaL_checkudata(L, idx, MatrixInfo<MatType>::metatable());
            luaL_argcheck(L, data != nullptr, idx, "'Matrix' or 'table' expected");
            return ((MatrixContainer<MatType>*) data)->matrix;
        }

        template<typename MatType>
        static int mat_new(lua_State* L)
        {
            const int elements = MatrixInfo<MatType>::elements();
            const int columns = MatrixInfo<MatType>::columns();
            const int rows = MatrixInfo<MatType>::rows();

            int argCount = lua_gettop(L);
            switch (argCount)
            {
                default:
                {
                    if (argCount != elements)
                        luaL_error(L, "wrong number of elements for %s.new", MatrixInfo<MatType>::type());

                    MatType mat;
                    for (int i = 0; i < elements; ++i)
                    {
                        float value = (float) luaL_checknumber(L, i + 1);
                        mat[i / columns][i % columns] = value;
                    }

                    pushMatrix<MatType>(L, mat);
                    return 1;
                }

                case 0:
                    pushMatrix<MatType>(L, MatType());
                    return 1;

                case 1:
                {
                    auto* mat = checkMatrix<MatType>(L, 1);
                    pushMatrix<MatType>(L, *mat);
                    return 1;
                }
            }
        }

        template<typename MatType>
        static int mat_table(lua_State* L)
        {
            auto& mat = *checkMatrix<MatType>(L, 1);
            lua_newtable(L);
            for (int i = 0; i < MatrixInfo<MatType>::elements(); ++i)
            {
                lua_pushnumber(L, i + 1);
                lua_pushnumber(L, mat[i / MatrixInfo<MatType>::columns()][i % MatrixInfo<MatType>::columns()]);
                lua_settable(L, -3);
            }

            return 1;
        }

        template<typename MatType>
        static int mat_get(lua_State* L)
        {
            const int columns = MatrixInfo<MatType>::columns();
            const int rows = MatrixInfo<MatType>::rows();

            int argc = lua_gettop(L); // must be called before checkMatrix in case the stack is changed

            auto& mat = *checkMatrix<MatType>(L, 1);
            switch (argc)
            {
                default:
                    luaL_error(L, "expected 2 or 3 arguments");
                    return 0;

                case 2:
                {
                    int index = luaL_checkinteger(L, 2);
                    luaL_argcheck(L, index >= 1 && index <= rows, 2, "row index out of range");
                    pushValue(L, mat[index - 1]);
                    return 1;
                }

                case 3:
                {
                    int index1 = luaL_checkinteger(L, 2);
                    luaL_argcheck(L, index1 >= 1 && index1 <= rows, 2, "row index out of range");
                    int index2 = luaL_checkinteger(L, 3);
                    luaL_argcheck(L, index2 >= 1 && index2 <= columns, 3, "colun index out of range");
                    lua_pushnumber(L, mat[index1 - 1][index2 - 1]);
                    return 1;
                }
            }
        }

        template<typename MatType>
        static int mat_set(lua_State* L)
        {
            auto& mat = *checkMatrix<MatType>(L, 1);

            int index1 = luaL_checkinteger(L, 2);
            luaL_argcheck(L, index1 >= 1 && index1 <= MatrixInfo<MatType>::rows(), 2, "row index out of range");
            int index2 = luaL_checkinteger(L, 3);
            luaL_argcheck(L, index2 >= 1 && index2 <= MatrixInfo<MatType>::columns(), 3, "column index out of range");

            mat[index1 - 1][index2 - 1] = (float) luaL_checknumber(L, 4);
            return 0;
        }

        template<typename MatType>
        static int mat_setAll(lua_State* L)
        {
            auto& mat = *checkMatrix<MatType>(L, 1);
            for (int i = 0; i < MatrixInfo<MatType>::elements(); ++i)
            {
                mat[i / MatrixInfo<MatType>::columns()][i % MatrixInfo<MatType>::columns()] = (float) luaL_checknumber(
                        L, i + 2);
            }

            return 0;
        }

        template<typename MatType>
        static int mat_rows(lua_State* L)
        {
            lua_pushnumber(L, MatrixInfo<MatType>::rows());
            return 1;
        }

        template<typename MatType>
        static int mat_columns(lua_State* L)
        {
            lua_pushnumber(L, MatrixInfo<MatType>::columns());
            return 1;
        }

        template<typename MatType>
        static int mat_transpose(lua_State* L)
        {
            auto& mat = *checkMatrix<MatType>(L, 1);
            pushMatrix<typename MatType::transpose_type>(L, glm::transpose(mat));
            return 1;
        }

        // This only applies to matrices where #rows == #columns
        template<typename MatType>
        static int mat_determinant(lua_State* L)
        {
            auto& mat = *checkMatrix<MatType>(L, 1);
            lua_pushnumber(L, glm::determinant(mat));
            return 1;
        }

        // This only applies to matrices where #rows == #columns
        template<typename MatType>
        static int mat_inverse(lua_State* L)
        {
            auto& mat = *checkMatrix<MatType>(L, 1);
            pushMatrix<MatType>(L, glm::inverse(mat));
            return 1;
        }

        template<typename MatType>
        static int mat_apply(lua_State* L)
        {
            const int elements = MatrixInfo<MatType>::elements();
            const int columns = MatrixInfo<MatType>::columns();
            const int rows = MatrixInfo<MatType>::rows();

            auto& mat = *checkMatrix<MatType>(L, 1);
            luaL_argcheck(L, lua_isfunction(L, 2), 2, "'function' expected");

            MatType result;
            for (int i = 0; i < elements; ++i)
            {
                lua_pushvalue(L, 2);
                lua_pushnumber(L, mat[i / columns][i % columns]);
                lua_pushnumber(L, i + 1);
                lua_pushnumber(L, (i / columns) + 1);
                lua_pushnumber(L, (i % columns) + 1);
                lua_call(L, 4, 1);

                result[i / columns][i % columns] = (float) lua_tonumber(L, -1);
                lua_pop(L, 1);
            }

            pushMatrix<MatType>(L, result);
            return 1;
        }

        template<typename MatType>
        static int mat_m_gc(lua_State* L)
        {
            delete checkMatrix<MatType>(L, 1);
            return 0;
        }

        template<typename MatType>
        static int mat_m_eq(lua_State* L)
        {
            auto& matA = *checkMatrix<MatType>(L, 1);
            auto& matB = *checkMatrix<MatType>(L, 2);
            lua_pushboolean(L, matA == matB);
            return 1;
        }

        template<typename MatType>
        static int mat_m_tostring(lua_State* L)
        {
            auto& mat = *checkMatrix<MatType>(L, 1);
            lua_pushstring(L, glm::to_string(mat).c_str());
            return 1;
        }

        template<typename MatType>
        static int mat_m_len(lua_State* L)
        {
            lua_pushnumber(L, MatrixInfo<MatType>::elements());
            return 1;
        }

        template<typename MatType>
        static int mat_m_unm(lua_State* L)
        {
            auto& mat = *checkMatrix<MatType>(L, 1);
            pushMatrix<MatType>(L, -mat);
            return 1;
        }

        ////
        // Shared Implementation
        ////

        static bool checkMetatable(lua_State* L, int idx, const char* name)
        {
            lua_getmetatable(L, idx);
            luaL_getmetatable(L, name);
            bool result = lua_equal(L, -1, -2) != 0;
            lua_pop(L, 2);
            return result;
        }

#define MATH_HELPER_EX(mode, type, exp) \
        if (checkMetatable(L, 2, mode##Info<type>::metatable())) \
        { \
            auto& v2 = *check##mode<type>(L, 2); \
            pushValue(L, (exp)); \
            return 1; \
        }

#define MATH_HELPER(mode, type, op) MATH_HELPER_EX(mode, type, (v1 op v2))

        static int shared_add(lua_State* L)
        {
            if (lua_isnumber(L, 1))
            {
                float v1 = (float) lua_tonumber(L, 1);

                MATH_HELPER(Vector, glm::vec2, +);
                MATH_HELPER(Vector, glm::vec3, +);
                MATH_HELPER(Vector, glm::vec4, +);

                MATH_HELPER(Matrix, glm::mat2x2, +);
                MATH_HELPER(Matrix, glm::mat3x3, +);
                MATH_HELPER(Matrix, glm::mat4x4, +);
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec2>::metatable()))
            {
                auto& v1 = *checkVector<glm::vec2>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 + v2);
                    return 1;
                }

                MATH_HELPER(Vector, glm::vec2, +);
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec3>::metatable()))
            {
                auto& v1 = *checkVector<glm::vec3>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 + v2);
                    return 1;
                }

                MATH_HELPER(Vector, glm::vec3, +);
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec4>::metatable()))
            {
                auto& v1 = *checkVector<glm::vec4>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 + v2);
                    return 1;
                }

                MATH_HELPER(Vector, glm::vec4, +);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat2x2>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat2x2>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 + v2);
                    return 1;
                }

                MATH_HELPER(Matrix, glm::mat2x2, +);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat2x3>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat2x3>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 + v2);
                    return 1;
                }

                MATH_HELPER(Matrix, glm::mat2x3, +);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat2x4>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat2x4>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 + v2);
                    return 1;
                }

                MATH_HELPER(Matrix, glm::mat2x4, +);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat3x2>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat3x2>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 + v2);
                    return 1;
                }

                MATH_HELPER(Matrix, glm::mat3x2, +);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat3x3>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat3x3>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 + v2);
                    return 1;
                }

                MATH_HELPER(Matrix, glm::mat3x3, +);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat3x4>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat3x4>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 + v2);
                    return 1;
                }

                MATH_HELPER(Matrix, glm::mat3x4, +);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat4x2>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat4x2>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 + v2);
                    return 1;
                }

                MATH_HELPER(Matrix, glm::mat4x2, +);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat4x3>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat4x3>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 + v2);
                    return 1;
                }

                MATH_HELPER(Matrix, glm::mat4x3, +);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat4x4>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat4x4>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 + v2);
                    return 1;
                }

                MATH_HELPER(Matrix, glm::mat4x4, +);
            }

            luaL_error(L, "Cannot apply operation to incompatible types!");
            return 0;
        }

        static int shared_sub(lua_State* L)
        {
            if (lua_isnumber(L, 1))
            {
                float v1 = (float) lua_tonumber(L, 1);

                MATH_HELPER(Vector, glm::vec2, -);
                MATH_HELPER(Vector, glm::vec3, -);
                MATH_HELPER(Vector, glm::vec4, -);

                MATH_HELPER(Matrix, glm::mat2x2, -);
                MATH_HELPER(Matrix, glm::mat3x3, -);
                MATH_HELPER(Matrix, glm::mat4x4, -);
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec2>::metatable()))
            {
                auto& v1 = *checkVector<glm::vec2>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 - v2);
                    return 1;
                }

                MATH_HELPER(Vector, glm::vec2, -);
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec3>::metatable()))
            {
                auto& v1 = *checkVector<glm::vec3>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 - v2);
                    return 1;
                }

                MATH_HELPER(Vector, glm::vec3, -);
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec4>::metatable()))
            {
                auto& v1 = *checkVector<glm::vec4>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 - v2);
                    return 1;
                }

                MATH_HELPER(Vector, glm::vec4, -);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat2x2>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat2x2>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 - v2);
                    return 1;
                }

                MATH_HELPER(Matrix, glm::mat2x2, -);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat2x3>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat2x3>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 - v2);
                    return 1;
                }

                MATH_HELPER(Matrix, glm::mat2x3, -);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat2x4>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat2x4>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 - v2);
                    return 1;
                }

                MATH_HELPER(Matrix, glm::mat2x4, -);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat3x2>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat3x2>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 - v2);
                    return 1;
                }

                MATH_HELPER(Matrix, glm::mat3x2, -);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat3x3>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat3x3>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 - v2);
                    return 1;
                }

                MATH_HELPER(Matrix, glm::mat3x3, -);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat3x4>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat3x4>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 - v2);
                    return 1;
                }

                MATH_HELPER(Matrix, glm::mat3x4, -);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat4x2>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat4x2>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 - v2);
                    return 1;
                }

                MATH_HELPER(Matrix, glm::mat4x2, -);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat4x3>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat4x3>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 - v2);
                    return 1;
                }

                MATH_HELPER(Matrix, glm::mat4x3, -);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat4x4>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat4x4>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 - v2);
                    return 1;
                }

                MATH_HELPER(Matrix, glm::mat4x4, -);
            }

            luaL_error(L, "Cannot apply operation to incompatible types!");
            return 0;
        }

        static int shared_mul(lua_State* L)
        {
            if (lua_isnumber(L, 1))
            {
                float v1 = (float) lua_tonumber(L, 1);

                MATH_HELPER(Vector, glm::vec2, *);
                MATH_HELPER(Vector, glm::vec3, *);
                MATH_HELPER(Vector, glm::vec4, *);

                MATH_HELPER(Matrix, glm::mat2x2, *);
                MATH_HELPER(Matrix, glm::mat2x3, *);
                MATH_HELPER(Matrix, glm::mat2x4, *);

                MATH_HELPER(Matrix, glm::mat3x2, *);
                MATH_HELPER(Matrix, glm::mat3x3, *);
                MATH_HELPER(Matrix, glm::mat3x4, *);

                MATH_HELPER(Matrix, glm::mat4x2, *);
                MATH_HELPER(Matrix, glm::mat4x3, *);
                MATH_HELPER(Matrix, glm::mat4x4, *);
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec2>::metatable()))
            {
                auto& v1 = *checkVector<glm::vec2>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 * v2);
                    return 1;
                }

                MATH_HELPER(Vector, glm::vec2, *);

                MATH_HELPER(Matrix, glm::mat2x2, *);
                MATH_HELPER(Matrix, glm::mat3x2, *);
                MATH_HELPER(Matrix, glm::mat4x2, *);
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec3>::metatable()))
            {
                auto& v1 = *checkVector<glm::vec3>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 * v2);
                    return 1;
                }

                MATH_HELPER(Vector, glm::vec3, *);

                MATH_HELPER(Matrix, glm::mat2x3, *);
                MATH_HELPER(Matrix, glm::mat3x3, *);
                MATH_HELPER(Matrix, glm::mat4x3, *);
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec4>::metatable()))
            {
                auto& v1 = *checkVector<glm::vec4>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 * v2);
                    return 1;
                }

                MATH_HELPER(Vector, glm::vec4, *);

                MATH_HELPER(Matrix, glm::mat2x4, *);
                MATH_HELPER(Matrix, glm::mat3x4, *);
                MATH_HELPER(Matrix, glm::mat4x4, *);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat2x2>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat2x2>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 * v2);
                    return 1;
                }

                MATH_HELPER(Vector, glm::vec2, *);

                MATH_HELPER(Matrix, glm::mat2x2, *);
                MATH_HELPER(Matrix, glm::mat3x2, *);
                MATH_HELPER(Matrix, glm::mat4x2, *);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat2x3>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat2x3>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 * v2);
                    return 1;
                }

                MATH_HELPER(Vector, glm::vec2, *);

                MATH_HELPER(Matrix, glm::mat2x2, *);
                MATH_HELPER(Matrix, glm::mat3x2, *);
                MATH_HELPER(Matrix, glm::mat4x2, *);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat2x4>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat2x4>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 * v2);
                    return 1;
                }

                MATH_HELPER(Vector, glm::vec2, *);

                MATH_HELPER(Matrix, glm::mat2x2, *);
                MATH_HELPER(Matrix, glm::mat3x2, *);
                MATH_HELPER(Matrix, glm::mat4x2, *);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat3x2>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat3x2>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 * v2);
                    return 1;
                }

                MATH_HELPER(Vector, glm::vec3, *);

                MATH_HELPER(Matrix, glm::mat2x3, *);
                MATH_HELPER(Matrix, glm::mat3x3, *);
                MATH_HELPER(Matrix, glm::mat4x3, *);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat3x3>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat3x3>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 * v2);
                    return 1;
                }

                MATH_HELPER(Vector, glm::vec3, *);

                MATH_HELPER(Matrix, glm::mat2x3, *);
                MATH_HELPER(Matrix, glm::mat3x3, *);
                MATH_HELPER(Matrix, glm::mat4x3, *);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat3x4>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat3x4>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 * v2);
                    return 1;
                }

                MATH_HELPER(Vector, glm::vec3, *);

                MATH_HELPER(Matrix, glm::mat2x3, *);
                MATH_HELPER(Matrix, glm::mat3x3, *);
                MATH_HELPER(Matrix, glm::mat4x3, *);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat4x2>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat4x2>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 * v2);
                    return 1;
                }

                MATH_HELPER(Vector, glm::vec4, *);

                MATH_HELPER(Matrix, glm::mat2x4, *);
                MATH_HELPER(Matrix, glm::mat3x4, *);
                MATH_HELPER(Matrix, glm::mat4x4, *);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat4x3>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat4x3>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 * v2);
                    return 1;
                }

                MATH_HELPER(Vector, glm::vec4, *);

                MATH_HELPER(Matrix, glm::mat2x4, *);
                MATH_HELPER(Matrix, glm::mat3x4, *);
                MATH_HELPER(Matrix, glm::mat4x4, *);
            }
            else if (checkMetatable(L, 1, MatrixInfo<glm::mat4x4>::metatable()))
            {
                auto& v1 = *checkMatrix<glm::mat4x4>(L, 1);

                if (lua_isnumber(L, 2))
                {
                    float v2 = (float) lua_tonumber(L, 2);
                    pushValue(L, v1 * v2);
                    return 1;
                }

                MATH_HELPER(Vector, glm::vec4, *);

                MATH_HELPER(Matrix, glm::mat2x4, *);
                MATH_HELPER(Matrix, glm::mat3x4, *);
                MATH_HELPER(Matrix, glm::mat4x4, *);
            }

            luaL_error(L, "Cannot apply operation to incompatible types!");
            return 0;
        }

#undef MATH_HELPER_EX
#undef MATH_HELPER

        ////
        // Vector Registration
        ////

#define VECTOR_LIB_F(name, type, ...) \
        static const luaL_reg name##_f[] = { \
            {"new", vec_new<type>}, \
            {"table", vec_table<type>}, \
            {"get", vec_get<type>}, \
            {"set", vec_set<type>}, \
            {"setAll", vec_setAll<type>}, \
            {"dot", vec_dot<type>}, \
            {"distance", vec_distance<type>}, \
            {"length", vec_length<type>}, \
            {"apply", vec_apply<type>}, \
            {"normalize", vec_normalize<type>}, \
            {"reflect", vec_reflect<type>}, \
            {"refract", vec_refract<type>}, \
            ##__VA_ARGS__, \
            {NULL, NULL} \
        }

#define VECTOR_LIB_M(name, type, ...) \
        static const luaL_reg name##_m[] = { \
            {"__gc", vec_m_gc<type>}, \
            {"__eq", vec_m_eq<type>}, \
            {"__tostring", vec_m_tostring<type>}, \
            {"__unm", vec_m_unm<type>}, \
            {"__len", vec_m_len<type>}, \
            {"__add", shared_add}, \
            {"__sub", shared_sub}, \
            {"__mul", shared_mul}, \
            {"table", vec_table<type>}, \
            {"get", vec_get<type>}, \
            {"set", vec_set<type>}, \
            {"setAll", vec_setAll<type>}, \
            {"dot", vec_dot<type>}, \
            {"distance", vec_distance<type>}, \
            {"length", vec_length<type>}, \
            {"apply", vec_apply<type>}, \
            {"normalize", vec_normalize<type>}, \
            {"reflect", vec_reflect<type>}, \
            {"refract", vec_refract<type>}, \
            ##__VA_ARGS__, \
            {NULL, NULL} \
        }

        VECTOR_LIB_F(vector2, glm::vec2);
        VECTOR_LIB_M(vector2, glm::vec2);

        int luaopen_vector2(lua_State* L)
        {
            luaL_newmetatable(L, VectorInfo<glm::vec2>::metatable());
            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);
            lua_settable(L, -3);
            luaL_register(L, NULL, vector2_m);

            luaL_register(L, VectorInfo<glm::vec2>::type(), vector2_f);

            return 1;
        }

        W_REGISTER_MODULE(luaopen_vector2);


        VECTOR_LIB_F(vector3, glm::vec3,
                     { "cross", vec3_cross }
        );

        VECTOR_LIB_M(vector3, glm::vec3,
                     { "cross", vec3_cross }
        );

        int luaopen_vector3(lua_State* L)
        {
            luaL_newmetatable(L, VectorInfo<glm::vec3>::metatable());
            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);
            lua_settable(L, -3);
            luaL_register(L, NULL, vector3_m);

            luaL_register(L, VectorInfo<glm::vec3>::type(), vector3_f);

            return 1;
        }

        W_REGISTER_MODULE(luaopen_vector3);

        VECTOR_LIB_F(vector4, glm::vec4);
        VECTOR_LIB_M(vector4, glm::vec4);

        int luaopen_vector4(lua_State* L)
        {
            luaL_newmetatable(L, VectorInfo<glm::vec4>::metatable());
            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);
            lua_settable(L, -3);
            luaL_register(L, NULL, vector4_m);

            luaL_register(L, VectorInfo<glm::vec4>::type(), vector4_f);

            return 1;
        }

        W_REGISTER_MODULE(luaopen_vector4);

#undef VECTOR_LIB_F
#undef VECTOR_LIB_M

        ////
        // Matrix Registration
        ////

#define MATRIX_LIB_F(name, type, ...) \
        static const luaL_reg name##_f[] = { \
            {"new", mat_new<type>}, \
            {"table", mat_table<type>}, \
            {"get", mat_get<type>}, \
            {"set", mat_set<type>}, \
            {"setAll", mat_setAll<type>}, \
            {"rows", mat_rows<type>}, \
            {"columns", mat_columns<type>}, \
            {"transpose", mat_transpose<type>}, \
            {"apply", mat_apply<type>}, \
            ##__VA_ARGS__, \
            {NULL, NULL} \
        }

#define MATRIX_LIB_M(name, type, ...) \
        static const luaL_reg name##_m[] = { \
            {"__gc", mat_m_gc<type>}, \
            {"__eq", mat_m_eq<type>}, \
            {"__tostring", mat_m_tostring<type>}, \
            {"__unm", mat_m_unm<type>}, \
            {"__len", mat_m_len<type>}, \
            {"__add", shared_add}, \
            {"__sub", shared_sub}, \
            {"__mul", shared_mul}, \
            {"table", mat_table<type>}, \
            {"get", mat_get<type>}, \
            {"set", mat_set<type>}, \
            {"setAll", mat_setAll<type>}, \
            {"rows", mat_rows<type>}, \
            {"columns", mat_columns<type>}, \
            {"transpose", mat_transpose<type>}, \
            {"apply", mat_apply<type>}, \
            ##__VA_ARGS__, \
            {NULL, NULL} \
        }

        MATRIX_LIB_F(matrix2x2, glm::mat2x2,
             {"determinant", mat_determinant<glm::mat2x2>},
             {"inverse", mat_inverse<glm::mat2x2>}
        );
        MATRIX_LIB_M(matrix2x2, glm::mat2x2,
             {"determinant", mat_determinant<glm::mat2x2>},
             {"inverse", mat_inverse<glm::mat2x2>}
        );

        int luaopen_matrix2x2(lua_State* L)
        {
            luaL_newmetatable(L, MatrixInfo<glm::mat2x2>::metatable());
            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);
            lua_settable(L, -3);
            luaL_register(L, NULL, matrix2x2_m);

            luaL_register(L, MatrixInfo<glm::mat2x2>::type(), matrix2x2_f);

            return 1;
        }

        W_REGISTER_MODULE(luaopen_matrix2x2);

        MATRIX_LIB_F(matrix2x3, glm::mat2x3);
        MATRIX_LIB_M(matrix2x3, glm::mat2x3);

        int luaopen_matrix2x3(lua_State* L)
        {
            luaL_newmetatable(L, MatrixInfo<glm::mat2x3>::metatable());
            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);
            lua_settable(L, -3);
            luaL_register(L, NULL, matrix2x3_m);

            luaL_register(L, MatrixInfo<glm::mat2x3>::type(), matrix2x3_f);

            return 1;
        }

        W_REGISTER_MODULE(luaopen_matrix2x3);

        MATRIX_LIB_F(matrix2x4, glm::mat2x4);
        MATRIX_LIB_M(matrix2x4, glm::mat2x4);

        int luaopen_matrix2x4(lua_State* L)
        {
            luaL_newmetatable(L, MatrixInfo<glm::mat2x4>::metatable());
            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);
            lua_settable(L, -3);
            luaL_register(L, NULL, matrix2x4_m);

            luaL_register(L, MatrixInfo<glm::mat2x4>::type(), matrix2x4_f);

            return 1;
        }

        W_REGISTER_MODULE(luaopen_matrix2x4);

        MATRIX_LIB_F(matrix3x2, glm::mat3x2);
        MATRIX_LIB_M(matrix3x2, glm::mat3x2);

        int luaopen_matrix3x2(lua_State* L)
        {
            luaL_newmetatable(L, MatrixInfo<glm::mat3x2>::metatable());
            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);
            lua_settable(L, -3);
            luaL_register(L, NULL, matrix3x2_m);

            luaL_register(L, MatrixInfo<glm::mat3x2>::type(), matrix3x2_f);

            return 1;
        }

        W_REGISTER_MODULE(luaopen_matrix3x2);

        MATRIX_LIB_F(matrix3x3, glm::mat3x3,
             {"determinant", mat_determinant<glm::mat3x3>},
             {"inverse", mat_inverse<glm::mat3x3>}
        );
        MATRIX_LIB_M(matrix3x3, glm::mat3x3,
             {"determinant", mat_determinant<glm::mat3x3>},
             {"inverse", mat_inverse<glm::mat3x3>}
        );

        int luaopen_matrix3x3(lua_State* L)
        {
            luaL_newmetatable(L, MatrixInfo<glm::mat3x3>::metatable());
            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);
            lua_settable(L, -3);
            luaL_register(L, NULL, matrix3x3_m);

            luaL_register(L, MatrixInfo<glm::mat3x3>::type(), matrix3x3_f);

            return 1;
        }

        W_REGISTER_MODULE(luaopen_matrix3x3);

        MATRIX_LIB_F(matrix3x4, glm::mat3x4);
        MATRIX_LIB_M(matrix3x4, glm::mat3x4);

        int luaopen_matrix3x4(lua_State* L)
        {
            luaL_newmetatable(L, MatrixInfo<glm::mat3x4>::metatable());
            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);
            lua_settable(L, -3);
            luaL_register(L, NULL, matrix3x4_m);

            luaL_register(L, MatrixInfo<glm::mat3x4>::type(), matrix3x4_f);

            return 1;
        }

        W_REGISTER_MODULE(luaopen_matrix3x4);

        MATRIX_LIB_F(matrix4x2, glm::mat4x2);
        MATRIX_LIB_M(matrix4x2, glm::mat4x2);

        int luaopen_matrix4x2(lua_State* L)
        {
            luaL_newmetatable(L, MatrixInfo<glm::mat4x2>::metatable());
            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);
            lua_settable(L, -3);
            luaL_register(L, NULL, matrix4x2_m);

            luaL_register(L, MatrixInfo<glm::mat4x2>::type(), matrix4x2_f);

            return 1;
        }

        W_REGISTER_MODULE(luaopen_matrix4x2);

        MATRIX_LIB_F(matrix4x3, glm::mat4x3);
        MATRIX_LIB_M(matrix4x3, glm::mat4x3);

        int luaopen_matrix4x3(lua_State* L)
        {
            luaL_newmetatable(L, MatrixInfo<glm::mat4x3>::metatable());
            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);
            lua_settable(L, -3);
            luaL_register(L, NULL, matrix4x3_m);

            luaL_register(L, MatrixInfo<glm::mat4x3>::type(), matrix4x3_f);

            return 1;
        }

        W_REGISTER_MODULE(luaopen_matrix4x3);

        MATRIX_LIB_F(matrix4x4, glm::mat4x4,
             {"determinant", mat_determinant<glm::mat4x4>},
             {"inverse", mat_inverse<glm::mat4x4>}
        );
        MATRIX_LIB_M(matrix4x4, glm::mat4x4,
             {"determinant", mat_determinant<glm::mat4x4>},
             {"inverse", mat_inverse<glm::mat4x4>}
        );

        int luaopen_matrix4x4(lua_State* L)
        {
            luaL_newmetatable(L, MatrixInfo<glm::mat4x4>::metatable());
            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);
            lua_settable(L, -3);
            luaL_register(L, NULL, matrix4x4_m);

            luaL_register(L, MatrixInfo<glm::mat4x4>::type(), matrix4x4_f);

            return 1;
        }

        W_REGISTER_MODULE(luaopen_matrix4x4);

#undef MATRIX_LIB_F
#undef MATRIX_LIB_M
    }

    void pushValue(lua_State* L, const glm::vec2& value)
    {
        binding::pushVector<glm::vec2>(L, value);
    }

    void pushValue(lua_State* L, const glm::vec3& value)
    {
        binding::pushVector<glm::vec3>(L, value);
    }

    void pushValue(lua_State* L, const glm::vec4& value)
    {
        binding::pushVector<glm::vec4>(L, value);
    }

    void pushValue(lua_State* L, const glm::mat2x2& value)
    {
        binding::pushMatrix<glm::mat2x2>(L, value);
    }

    void pushValue(lua_State* L, const glm::mat2x3& value)
    {
        binding::pushMatrix<glm::mat2x3>(L, value);
    }

    void pushValue(lua_State* L, const glm::mat2x4& value)
    {
        binding::pushMatrix<glm::mat2x4>(L, value);
    }

    void pushValue(lua_State* L, const glm::mat3x2& value)
    {
        binding::pushMatrix<glm::mat3x2>(L, value);
    }

    void pushValue(lua_State* L, const glm::mat3x3& value)
    {
        binding::pushMatrix<glm::mat3x3>(L, value);
    }

    void pushValue(lua_State* L, const glm::mat3x4& value)
    {
        binding::pushMatrix<glm::mat3x4>(L, value);
    }

    void pushValue(lua_State* L, const glm::mat4x2& value)
    {
        binding::pushMatrix<glm::mat4x2>(L, value);
    }

    void pushValue(lua_State* L, const glm::mat4x3& value)
    {
        binding::pushMatrix<glm::mat4x3>(L, value);
    }

    void pushValue(lua_State* L, const glm::mat4x4& value)
    {
        binding::pushMatrix<glm::mat4x4>(L, value);
    }

    glm::vec2* luaW_checkvector2(lua_State* L, int narg)
    {
        return binding::checkVector<glm::vec2>(L, narg);
    }

    glm::vec3* luaW_checkvector3(lua_State* L, int narg)
    {
        return binding::checkVector<glm::vec3>(L, narg);
    }

    glm::vec4* luaW_checkvector4(lua_State* L, int narg)
    {
        return binding::checkVector<glm::vec4>(L, narg);
    }

    glm::mat2x2* luaW_checkmatrix2x2(lua_State* L, int narg)
    {
        return binding::checkMatrix<glm::mat2x2>(L, narg);
    }

    glm::mat2x3* luaW_checkmatrix2x3(lua_State* L, int narg)
    {
        return binding::checkMatrix<glm::mat2x3>(L, narg);
    }

    glm::mat2x4* luaW_checkmatrix2x4(lua_State* L, int narg)
    {
        return binding::checkMatrix<glm::mat2x4>(L, narg);
    }

    glm::mat3x2* luaW_checkmatrix3x2(lua_State* L, int narg)
    {
        return binding::checkMatrix<glm::mat3x2>(L, narg);
    }

    glm::mat3x3* luaW_checkmatrix3x3(lua_State* L, int narg)
    {
        return binding::checkMatrix<glm::mat3x3>(L, narg);
    }

    glm::mat3x4* luaW_checkmatrix3x4(lua_State* L, int narg)
    {
        return binding::checkMatrix<glm::mat3x4>(L, narg);
    }

    glm::mat4x2* luaW_checkmatrix4x2(lua_State* L, int narg)
    {
        return binding::checkMatrix<glm::mat4x2>(L, narg);
    }

    glm::mat4x3* luaW_checkmatrix4x3(lua_State* L, int narg)
    {
        return binding::checkMatrix<glm::mat4x3>(L, narg);
    }

    glm::mat4x4* luaW_checkmatrix4x4(lua_State* L, int narg)
    {
        return binding::checkMatrix<glm::mat4x4>(L, narg);
    }
}