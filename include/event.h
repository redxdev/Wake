#pragma once

#include <list>
#include <assert.h>

namespace wake
{
    template<typename... Arguments>
    class Delegate
    {
    public:
        virtual ~Delegate()
        {
        }

        virtual void call(Arguments... args) = 0;

        virtual bool equals(Delegate<Arguments...>* other) const;
    };

    template<typename... Arguments>
    class StaticDelegate : public Delegate<Arguments...>
    {
    public:
        typedef void (* FuncPtr)(Arguments...);

        StaticDelegate(FuncPtr f)
        {
            function = f;
        }

        virtual void call(Arguments... args) override
        {
            if (!function)
                return;

            function(args...);
        }

        virtual bool equals(Delegate<Arguments...>* other) const override
        {
            auto otherPtr = dynamic_cast<StaticDelegate<Arguments...>*>(other);
            if (!otherPtr)
                return false;

            return function == otherPtr->function;
        }

    private:
        FuncPtr function;
    };

    template<typename T, typename... Arguments>
    class InstancedDelegate : public Delegate<Arguments...>
    {
    public:
        typedef void (T::*FuncPtr)(Arguments...);

        InstancedDelegate(T* inst, FuncPtr f)
        {
            instance = inst;
            function = f;
        }

        virtual void call(Arguments... args) override
        {
            if (!instance || !function)
                return;

            (instance->*function)(args...);
        }

        virtual bool equals(Delegate<Arguments...>* other) const override
        {
            auto otherPtr = dynamic_cast<InstancedDelegate<T, Arguments...>*>(&other);
            if (!otherPtr)
                return false;

            return instance == otherPtr->instance && function == otherPtr->function;
        }

    private:
        T* instance;
        FuncPtr function;
    };

    template<typename... Arguments>
    class Event
    {
    public:
        Event()
        {
            currentItr = delegates.end();
        }

        void bind(Delegate<Arguments...>* delegate)
        {
            delegates.push_back(delegate);
        }

        void bind(void (* f)(Arguments...))
        {
            bind(new StaticDelegate<Arguments...>(f));
        }

        template<typename T>
        void bind(T* inst, void (T::*f)(Arguments...))
        {
            bind(new InstancedDelegate<T, Arguments...>(inst, f));
        }

        void unbind(Delegate<Arguments...>* delegate)
        {
            auto itr = delegates.begin();
            while (itr != delegates.end())
            {
                if ((*itr)->equals(delegate))
                {
                    delete *itr;

                    if (itr == currentItr)
                    {
                        itr = delegates.erase(itr);
                        currentItr = itr;
                        if (currentItr != delegates.begin())
                        {
                            --currentItr;
                        }
                    }
                    else
                    {
                        itr = delegates.erase(itr);
                    }
                }
                else
                {
                    ++itr;
                }
            }
        }

        void unbind(void (* f)(Arguments...))
        {
            auto delegate = new StaticDelegate<Arguments...>(f);
            unbind(delegate);
            delete delegate;
        }

        template<typename T>
        void unbind(T* inst, void (T::*f)(Arguments...))
        {
            auto delegate = new InstancedDelegate<T, Arguments...>(inst, f);
            unbind(delegate);
            delete delegate;
        }

        void clear()
        {
            if (currentItr != delegates.end())
                currentItr = delegates.end();

            for (auto delegate : delegates)
            {
                delete delegate;
            }

            delegates.clear();
        }

        void call(Arguments... args)
        {
            assert(currentItr == delegates.end() && "Cannot call an Event recursively");

            currentItr = delegates.begin();
            while (currentItr != delegates.end())
            {
                (*currentItr)->call(args...);
            }
        }

    private:
        std::list<Delegate<Arguments...>*> delegates;
        typename std::list<Delegate<Arguments...>*>::iterator currentItr;
    };
}