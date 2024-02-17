#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <firebase/app.h>
#include <firebase/firestore.h>

using namespace firebase;
using namespace firebase::firestore;

namespace godot
{
    enum SetOptions : int
    {
        kOVERWRITE = 0,
        kMERGE_ALL = 1,
        kMERGE_SPECIFIC = 2
    };

    enum Direction
    {
        kASCENDING = 0,
        kDESCENDING = 1
    };

    enum Operation
    {
        kDELETE = 0,
        kGET = 1,
        kSET = 2,
        kUPDATE = 3
    };

    class FirebaseOptions
    {
    public:
        static bool LogDebug;
    };
}

VARIANT_ENUM_CAST(SetOptions);
VARIANT_ENUM_CAST(Direction);
VARIANT_ENUM_CAST(Operation);

#define GFB_CALLBACK0_H(m_class) /*****************/ \
    class m_class : public Object                    \
    {                                                \
        GDCLASS(m_class, Object);                    \
                                                     \
    protected:                                       \
        static void _bind_methods();                 \
        m_class *forward_to;                         \
                                                     \
    public:                                          \
        m_class();                                   \
        ~m_class();                                  \
                                                     \
        void completed();                            \
        void error(int error, String error_message); \
                                                     \
        void forward(m_class *to_task);              \
    };

#define GFB_CALLBACK0_CPP(m_class, m_signal_name) /***************************************************************************************/   \
                                                                                                                                              \
    m_class::m_class()                                                                                                                        \
    {                                                                                                                                         \
        forward_to = nullptr;                                                                                                                 \
    }                                                                                                                                         \
    m_class::~m_class() {}                                                                                                                    \
    void m_class::completed()                                                                                                                 \
    {                                                                                                                                         \
        (forward_to == nullptr ? this : forward_to)->emit_signal(#m_signal_name "_completed");                                                \
    }                                                                                                                                         \
    void m_class::error(int error, String error_message)                                                                                      \
    {                                                                                                                                         \
        (forward_to == nullptr ? this : forward_to)->emit_signal(#m_signal_name "_error", error, error_message);                              \
    }                                                                                                                                         \
    void m_class::_bind_methods()                                                                                                             \
    {                                                                                                                                         \
        ADD_SIGNAL(MethodInfo(#m_signal_name "_completed"));                                                                                  \
        ADD_SIGNAL(MethodInfo(#m_signal_name "_error", PropertyInfo(Variant::INT, "error"), PropertyInfo(Variant::STRING, "error_message"))); \
    }                                                                                                                                         \
    void m_class::forward(m_class *to_task)                                                                                                   \
    {                                                                                                                                         \
        forward_to = to_task;                                                                                                                 \
    }

#define GFB_CALLBACK1_H(m_class, m_type1, m_arg1) /****/ \
    class m_class : public Object                        \
    {                                                    \
        GDCLASS(m_class, Object);                        \
                                                         \
    protected:                                           \
        static void _bind_methods();                     \
        m_class *forward_to;                             \
                                                         \
    public:                                              \
        m_class();                                       \
        ~m_class();                                      \
                                                         \
        void completed(m_type1 m_arg1);                  \
        void error(int error, String error_message);     \
                                                         \
        void forward(m_class *to_task);                  \
    };

#define GFB_CALLBACK1_CPP(m_class, m_signal_name, m_type1, m_variant1, m_arg1) /***********************************************************/  \
                                                                                                                                              \
    m_class::m_class()                                                                                                                        \
    {                                                                                                                                         \
        forward_to = nullptr;                                                                                                                 \
    }                                                                                                                                         \
    m_class::~m_class() {}                                                                                                                    \
    void m_class::completed(m_type1 m_arg1)                                                                                                   \
    {                                                                                                                                         \
        (forward_to == nullptr ? this : forward_to)->emit_signal(#m_signal_name "_completed", m_arg1);                                        \
    }                                                                                                                                         \
    void m_class::error(int error, String error_message)                                                                                      \
    {                                                                                                                                         \
        (forward_to == nullptr ? this : forward_to)->emit_signal(#m_signal_name "_error", error, error_message);                              \
    }                                                                                                                                         \
    void m_class::_bind_methods()                                                                                                             \
    {                                                                                                                                         \
        ADD_SIGNAL(MethodInfo(#m_signal_name "_completed", PropertyInfo(Variant::m_variant1, #m_arg1)));                                     \
        ADD_SIGNAL(MethodInfo(#m_signal_name "_error", PropertyInfo(Variant::INT, "error"), PropertyInfo(Variant::STRING, "error_message"))); \
    }                                                                                                                                         \
    void m_class::forward(m_class *to_task)                                                                                                   \
    {                                                                                                                                         \
        forward_to = to_task;                                                                                                                 \
    }

#define GFB_CALLBACK2_H(m_class, m_type1, m_arg1, m_type2, m_arg2) /***/ \
    class m_class : public Object                                        \
    {                                                                    \
        GDCLASS(m_class, Object);                                        \
                                                                         \
    protected:                                                           \
        static void _bind_methods();                                     \
        m_class *forward_to;                                             \
                                                                         \
    public:                                                              \
        m_class();                                                       \
        ~m_class();                                                      \
                                                                         \
        void completed(m_type1 m_arg1, m_type2 m_arg2);                  \
        void error(int error, String error_message);                     \
                                                                         \
        void forward(m_class *to_task);                                  \
    };

#define GFB_CALLBACK2_CPP(m_class, m_signal_name, m_type1, m_variant1, m_arg1, m_type2, m_variant2, m_arg2) /******************************************/ \
                                                                                                                                                         \
    m_class::m_class()                                                                                                                                   \
    {                                                                                                                                                    \
        forward_to = nullptr;                                                                                                                            \
    }                                                                                                                                                    \
    m_class::~m_class() {}                                                                                                                               \
    void m_class::completed(m_type1 m_arg1, m_type2 m_arg2)                                                                                              \
    {                                                                                                                                                    \
        (forward_to == nullptr ? this : forward_to)->emit_signal(#m_signal_name "_completed", m_arg1, m_arg2);                                           \
    }                                                                                                                                                    \
    void m_class::error(int error, String error_message)                                                                                                 \
    {                                                                                                                                                    \
        (forward_to == nullptr ? this : forward_to)->emit_signal(#m_signal_name "_error", error, error_message);                                         \
    }                                                                                                                                                    \
    void m_class::_bind_methods()                                                                                                                        \
    {                                                                                                                                                    \
        ADD_SIGNAL(MethodInfo(#m_signal_name "_completed", PropertyInfo(Variant::m_variant1, #m_arg1), PropertyInfo(Variant::m_variant2, #m_arg2)));   \
        ADD_SIGNAL(MethodInfo(#m_signal_name "_error", PropertyInfo(Variant::INT, "error"), PropertyInfo(Variant::STRING, "error_message")));            \
    }                                                                                                                                                    \
    void m_class::forward(m_class *to_task)                                                                                                              \
    {                                                                                                                                                    \
        forward_to = to_task;                                                                                                                            \
    }

#define GFB_CALLBACK3_H(m_class, m_type1, m_arg1, m_type2, m_arg2, m_type3, m_arg3) /**/ \
    class m_class : public Object                                                        \
    {                                                                                    \
        GDCLASS(m_class, Object);                                                        \
                                                                                         \
    protected:                                                                           \
        static void _bind_methods();                                                     \
        m_class *forward_to;                                                             \
                                                                                         \
    public:                                                                              \
        m_class();                                                                       \
        ~m_class();                                                                      \
                                                                                         \
        void completed(m_type1 m_arg1, m_type2 m_arg2, m_type3 m_arg3);                  \
        void error(int error, String error_message);                                     \
                                                                                         \
        void forward(m_class *to_task);                                                  \
    };

#define GFB_CALLBACK3_CPP(m_class, m_signal_name, m_type1, m_variant1, m_arg1, m_type2, m_variant2, m_arg2, m_type3, m_variant3, m_arg3) /*******************************************************/  \
                                                                                                                                                                                                    \
    m_class::m_class()                                                                                                                                                                              \
    {                                                                                                                                                                                               \
        forward_to = nullptr;                                                                                                                                                                       \
    }                                                                                                                                                                                               \
    m_class::~m_class() {}                                                                                                                                                                          \
    void m_class::completed(m_type1 m_arg1, m_type2 m_arg2, m_type3 m_arg3)                                                                                                                         \
    {                                                                                                                                                                                               \
        (forward_to == nullptr ? this : forward_to)->emit_signal(#m_signal_name "_completed", m_arg1, m_arg2, m_arg3);                                                                              \
    }                                                                                                                                                                                               \
    void m_class::error(int error, String error_message)                                                                                                                                            \
    {                                                                                                                                                                                               \
        (forward_to == nullptr ? this : forward_to)->emit_signal(#m_signal_name "_error", error, error_message);                                                                                    \
    }                                                                                                                                                                                               \
    void m_class::_bind_methods()                                                                                                                                                                   \
    {                                                                                                                                                                                               \
        ADD_SIGNAL(MethodInfo(#m_signal_name "_completed", PropertyInfo(Variant::m_variant1, #m_arg1), PropertyInfo(Variant::m_variant2, #m_arg2), PropertyInfo(Variant::m_variant3, #m_arg3))); \
        ADD_SIGNAL(MethodInfo(#m_signal_name "_error", PropertyInfo(Variant::INT, "error"), PropertyInfo(Variant::STRING, "error_message")));                                                       \
    }                                                                                                                                                                                               \
    void m_class::forward(m_class *to_task)                                                                                                                                                         \
    {                                                                                                                                                                                               \
        forward_to = to_task;                                                                                                                                                                       \
    }

#define GFB_CALLBACK4_H(m_class, m_type1, m_arg1, m_type2, m_arg2, m_type3, m_arg3, m_type4, m_arg4) /**/ \
    class m_class : public Object                                                                         \
    {                                                                                                     \
        GDCLASS(m_class, Object);                                                                         \
                                                                                                          \
    protected:                                                                                            \
        static void _bind_methods();                                                                      \
        m_class *forward_to;                                                                              \
                                                                                                          \
    public:                                                                                               \
        m_class();                                                                                        \
        ~m_class();                                                                                       \
                                                                                                          \
        void completed(m_type1 m_arg1, m_type2 m_arg2, m_type3 m_arg3, m_type4 m_arg4);                   \
        void error(int error, String error_message);                                                      \
                                                                                                          \
        void forward(m_class *to_task);                                                                   \
    };

#define GFB_CALLBACK4_CPP(m_class, m_signal_name, m_type1, m_variant1, m_arg1, m_type2, m_variant2, m_arg2, m_type3, m_variant3, m_arg3, m_type4, m_variant4, m_arg4) /*******************************************************/                  \
                                                                                                                                                                                                                                                 \
    m_class::m_class()                                                                                                                                                                                                                           \
    {                                                                                                                                                                                                                                            \
        forward_to = nullptr;                                                                                                                                                                                                                    \
    }                                                                                                                                                                                                                                            \
    m_class::~m_class() {}                                                                                                                                                                                                                       \
    void m_class::completed(m_type1 m_arg1, m_type2 m_arg2, m_type3 m_arg3, m_type4 m_arg4)                                                                                                                                                      \
    {                                                                                                                                                                                                                                            \
        (forward_to == nullptr ? this : forward_to)->emit_signal(#m_signal_name "_completed", m_arg1, m_arg2, m_arg3, m_arg4);                                                                                                                   \
    }                                                                                                                                                                                                                                            \
    void m_class::error(int error, String error_message)                                                                                                                                                                                         \
    {                                                                                                                                                                                                                                            \
        (forward_to == nullptr ? this : forward_to)->emit_signal(#m_signal_name "_error", error, error_message);                                                                                                                                 \
    }                                                                                                                                                                                                                                            \
    void m_class::_bind_methods()                                                                                                                                                                                                                \
    {                                                                                                                                                                                                                                            \
        ADD_SIGNAL(MethodInfo(#m_signal_name "_completed", PropertyInfo(Variant::m_variant1, #m_arg1), PropertyInfo(Variant::m_variant2, #m_arg2), PropertyInfo(Variant::m_variant3, #m_arg3), PropertyInfo(Variant::m_variant4, #m_arg4))); \
        ADD_SIGNAL(MethodInfo(#m_signal_name "_error", PropertyInfo(Variant::INT, "error"), PropertyInfo(Variant::STRING, "error_message")));                                                                                                    \
    }                                                                                                                                                                                                                                            \
    void m_class::forward(m_class *to_task)                                                                                                                                                                                                      \
    {                                                                                                                                                                                                                                            \
        forward_to = to_task;                                                                                                                                                                                                                    \
    }

#define GFB_STORAGE_CALLBACK1_H(m_class, m_type1, m_arg1) /****/ \
    class m_class : public Object, firebase::storage::Listener   \
    {                                                            \
        GDCLASS(m_class, Object);                                \
                                                                 \
    protected:                                                   \
        static void _bind_methods();                             \
        m_class *forward_to;                                     \
        Controller *controller;                                  \
                                                                 \
    public:                                                      \
        m_class();                                               \
        ~m_class();                                              \
                                                                 \
        Controller *getController();                             \
                                                                 \
        void completed(m_type1 m_arg1);                          \
        void error(int error, String error_message);             \
                                                                 \
        void forward(m_class *to_task);                          \
                                                                 \
        void OnPaused(Controller *controller) override;          \
        void OnProgress(Controller *controller) override;        \
                                                                 \
        int64_t BytesTransferred();                              \
        int64_t TotalByteCount();                                \
        bool Cancel();                                           \
        bool Pause();                                            \
        bool Resume();                                           \
    };

#define GFB_STORAGE_CALLBACK1_CPP(m_class, m_signal_name, m_type1, m_variant1, m_arg1) /***********************************************************/ \
                                                                                                                                                     \
    m_class::m_class()                                                                                                                               \
    {                                                                                                                                                \
        forward_to = nullptr;                                                                                                                        \
        controller = new Controller();                                                                                                               \
    }                                                                                                                                                \
    m_class::~m_class() {}                                                                                                                           \
    Controller *m_class::getController()                                                                                                             \
    {                                                                                                                                                \
        return controller;                                                                                                                           \
    }                                                                                                                                                \
    void m_class::completed(m_type1 m_arg1)                                                                                                          \
    {                                                                                                                                                \
        (forward_to == nullptr ? this : forward_to)->emit_signal(#m_signal_name "_completed", m_arg1);                                               \
    }                                                                                                                                                \
    void m_class::error(int error, String error_message)                                                                                             \
    {                                                                                                                                                \
        (forward_to == nullptr ? this : forward_to)->emit_signal(#m_signal_name "_error", error, error_message);                                     \
    }                                                                                                                                                \
    void m_class::_bind_methods()                                                                                                                    \
    {                                                                                                                                                \
        ClassDB::bind_method(D_METHOD("bytes_transferred"), &m_class::BytesTransferred);                                                             \
        ClassDB::bind_method(D_METHOD("total_byte_count"), &m_class::TotalByteCount);                                                                \
        ClassDB::bind_method(D_METHOD("cancel"), &m_class::Cancel);                                                                                  \
        ClassDB::bind_method(D_METHOD("pause"), &m_class::Pause);                                                                                    \
        ClassDB::bind_method(D_METHOD("resume"), &m_class::Resume);                                                                                  \
                                                                                                                                                     \
        ADD_SIGNAL(MethodInfo(#m_signal_name "_completed", PropertyInfo(Variant::m_variant1, #m_arg1)));                                            \
        ADD_SIGNAL(MethodInfo(#m_signal_name "_paused"));                                                                                            \
        ADD_SIGNAL(MethodInfo(#m_signal_name "_progress"));                                                                                          \
        ADD_SIGNAL(MethodInfo(#m_signal_name "_error", PropertyInfo(Variant::INT, "error"), PropertyInfo(Variant::STRING, "error_message")));        \
    }                                                                                                                                                \
    void m_class::forward(m_class *to_task)                                                                                                          \
    {                                                                                                                                                \
        forward_to = to_task;                                                                                                                        \
    }                                                                                                                                                \
    void m_class::OnPaused(Controller *_controller)                                                                                                  \
    {                                                                                                                                                \
        controller = _controller;                                                                                                                    \
        (forward_to == nullptr ? this : forward_to)->emit_signal(#m_signal_name "_paused");                                                          \
    }                                                                                                                                                \
    void m_class::OnProgress(Controller *_controller)                                                                                                \
    {                                                                                                                                                \
        controller = _controller;                                                                                                                    \
        (forward_to == nullptr ? this : forward_to)->emit_signal(#m_signal_name "_progress");                                                        \
    }                                                                                                                                                \
    int64_t m_class::BytesTransferred()                                                                                                              \
    {                                                                                                                                                \
        return controller->bytes_transferred();                                                                                                      \
    }                                                                                                                                                \
    int64_t m_class::TotalByteCount()                                                                                                                \
    {                                                                                                                                                \
        return controller->total_byte_count();                                                                                                       \
    }                                                                                                                                                \
    bool m_class::Cancel()                                                                                                                           \
    {                                                                                                                                                \
        return controller->Cancel();                                                                                                                 \
    }                                                                                                                                                \
    bool m_class::Pause()                                                                                                                            \
    {                                                                                                                                                \
        return controller->Pause();                                                                                                                  \
    }                                                                                                                                                \
    bool m_class::Resume()                                                                                                                           \
    {                                                                                                                                                \
        return controller->Resume();                                                                                                                 \
    }
