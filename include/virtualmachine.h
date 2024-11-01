#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <QObject>
#include <QHash>
#include <QMap>

#include <functional>

#include <unicorn/unicorn.h>

#include <capstone/capstone.h>

#include <cstring>

#include <QDebug>

class VirtualMachine
{
public:
    explicit VirtualMachine(QObject *parent = nullptr)
        : m_uc(nullptr), m_debug(false)
    {
    }

    ~VirtualMachine();

    bool init();
    bool load(QString file);

    uint32_t push(void *data, int size);

    void read(uint32_t addr, void *buff, size_t size);
    void write(uint32_t addr, void *buff, size_t size);

    void reg(int regid, void *val);
    void setReg(int regid, void *val);

    void dumpStack();

    void setDebug(bool en){ m_debug = en; }
    bool debug(){ return m_debug; }

    template <typename T> void implement(QString function, T &&f)
    {
        _implement(function, std::function(std::move(f)));
    }

    template <typename ... T> void call(QString function, T&& ...  args)
    {
        _call(function, std::forward<T>(args)...);
    }

protected:
    template <typename R, typename ... T> void _implement(QString function, std::function<R(VirtualMachine *, T...)> impl)
    {
        m_implementations[function] = new implementation<R, T...>(impl);
    }

    template <typename ... T> void _call(QString function, T ... args)
    {
        QVector<uint8_t> argSizes = {alignTo4Byte(sizeof(args))...};

        int sum = 0;
        sum = std::accumulate(argSizes.begin(), argSizes.end(), sum);

        char *data = (char*) malloc(sum);
        std::tuple<T...> tuple{args ...};
        copy_args<T...>(std::make_index_sequence<sizeof...(T)>{}, tuple, data);
        callF(function, argSizes, data);

        free(data);
    }

    void callF(QString function, QVector<uint8_t> args, const char *data);

    static void hook_mem_invalid(uc_engine *uc, uc_mem_type type, uint64_t address, int size, int64_t value, void *user_data);
    static bool hook_code(uc_engine *uc, uint64_t address, uint32_t size, void *user_data);

    auto &importedSymbols(){ return m_importedSymbols; }
    auto &implementations(){ return m_implementations; }

    struct symbolImport
    {
        QString module;
        QString symbol;

        operator QString() const { return module + ":" + symbol; }
    };

    struct impl_base
    {
        virtual void call(VirtualMachine *machine, const char *args, char *ret = nullptr) = 0;
        virtual int size() = 0;
        virtual int arg(int num) = 0;
        virtual int numArgs() = 0;
        virtual bool hasReturn() = 0;
        virtual int returnSize() = 0;
    };

    static constexpr uint32_t alignTo4Byte(uint32_t size){ return (size + 3) & (~3); }

    template<typename...Args>
    static constexpr auto data_indices()
    {
        std::array is{(uint32_t)0, alignTo4Byte(sizeof(Args))...};
        for(std::size_t i=1;i<is.size();++i)
        {
            is[i]+=is[i-1];
        }
        return is;
    }

    template<typename...Args,std::size_t...Is>
    static auto get_tuple(std::index_sequence<Is...>, const char* data)
    {
        auto indices = data_indices<Args...>();
        std::tuple<Args...> tuple;

        (std::memcpy(&std::get<Is>(tuple), data+indices[Is], sizeof(Args)), ...);
        return tuple;
    }

    template<typename...Args,std::size_t...Is>
    static void copy_args(std::index_sequence<Is...>, const std::tuple<Args...> &in, char* data)
    {
        auto indices = data_indices<Args...>();

        (std::memcpy( data+indices[Is], &std::get<Is>(in), sizeof(Args)), ...);
    }

    template <typename R, typename ... T> struct implementation : impl_base
    {
        std::function<R(VirtualMachine *, T...)> impl;
        QList<uint8_t> args;

        static constexpr bool ret = !std::is_same<R, void>::value;

        implementation(std::function<R(VirtualMachine *, T...)> f)
            : impl(f), args({alignTo4Byte(sizeof(T))...})
        {}

        virtual void call(VirtualMachine *machine, const char *args, char *retptr = nullptr)
        {
            auto argTuple =  get_tuple<T...>(std::make_index_sequence<sizeof...(T)>{}, args);
            auto fullTuple = std::tuple_cat(std::make_tuple(machine), argTuple);

            if constexpr(ret)
            {
                auto retval = std::apply(impl, fullTuple);

                *reinterpret_cast<R*>(retptr) = retval;
            }
            else
            {
                std::apply(impl, fullTuple);
            }
        }

        virtual bool hasReturn()
        {
            return ret;
        }

        virtual int returnSize()
        {
            if constexpr(ret) return sizeof(R);
            else return 0;
        }

        virtual int size()
        {
            int sum = 0;
            sum = std::accumulate(args.begin(), args.end(), sum);
            return sum;
        }

        virtual int arg(int num)
        {
            return args[num];
        }

        virtual int numArgs()
        {
            return args.size();
        }
    };

    //std::map<std::string, uint32_t> m_functionMap;

    QHash<QString, uint32_t> m_functionMap;
    QMap<uint32_t, symbolImport> m_importedSymbols;

    QHash<QString, impl_base*> m_implementations;

    uc_engine* m_uc;
    bool m_debug;
};

#endif // VIRTUALMACHINE_H
