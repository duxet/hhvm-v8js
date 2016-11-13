#include "ext_v8js.h"
#include "v8js_alloc.cpp"

namespace HPHP
{
    const StaticString
        s_V8Js("V8Js"),
        s_V8_VERSION("V8_VERSION");

    Class *V8Js::s_class = nullptr;
    const StaticString V8Js::s_className("V8Js");

    Class *V8Js::getClass() {
        if (s_class == nullptr) {
            s_class = Unit::lookupClass(s_className.get());
            assert(s_class);
        }

        return s_class;
    }

    V8Js::V8Js() : m_isolate(nullptr) {
    }

    V8Js::~V8Js() {
        if (m_isolate) {
            m_isolate->Dispose();
        }
    }

	static void HHVM_METHOD(V8Js, __construct)
	{
        auto *data = Native::data<V8Js>(this_);
        data->m_isolate = v8::Isolate::New();
    }

    static Variant HHVM_METHOD(V8Js, executeString, const String& text)
    {
        auto *data = Native::data<V8Js>(this_);

        v8::Isolate *isolate = data->m_isolate;
        v8::Isolate::Scope isolate_scope(isolate);

        // Create a stack-allocated handle scope.
        v8::HandleScope handle_scope(isolate);

        // Create a new context.
        v8::Local<v8::Context> context = v8::Context::New(isolate);

        // Enter the context for compiling and running the script.
        v8::Context::Scope context_scope(context);

        // Create a string containing the JavaScript source code.
        v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, text.c_str());

        // Compile the source code.
        v8::Local<v8::Script> script = v8::Script::Compile(source);

        // Try to catch exceptions
        v8::TryCatch trycatch(isolate);

        // Run it.
        v8::Local<v8::Value> result = script->Run();

        if (result.IsEmpty())
        {
            v8::Local<v8::Value> exception = trycatch.Exception();
            v8::String::Utf8Value exception_str(exception);

            throw Exception(std::string(*exception_str));
        }
        else if (result->IsString())
        {
            v8::String::Utf8Value strResult(result->ToString());
            return std::string(*strResult);
        }
        else if (result->IsBoolean())
        {
            return result->BooleanValue();
        }
        else if (result->IsInt32() || result->IsUint32())
        {
            return result->IntegerValue();
        }
        else if (result->IsNumber())
        {
            return result->NumberValue();
        }
        throw Exception(std::string("NoExecuteStringResultException"));
    }

    void v8jsExtension::_initV8JsClass() {
        v8::V8::InitializeICU();
        v8::Platform* platform = v8::platform::CreateDefaultPlatform();
        v8::V8::InitializePlatform(platform);
        v8::V8::Initialize();

        v8::V8::SetArrayBufferAllocator(new MallocArrayBufferAllocator);

        HHVM_ME(V8Js, __construct);
        HHVM_ME(V8Js, executeString);

        Native::registerClassConstant<KindOfPersistentString>(
                s_V8Js.get(), s_V8_VERSION.get(), String::FromCStr(v8::V8::GetVersion()).get()
        );
    }
}
