#include "ext_v8js.h"
#include <v8.h>
#include <libplatform/libplatform.h>

namespace HPHP
{
	static void HHVM_METHOD(V8Js, __construct)
	{
        v8::V8::InitializeICU();
        v8::Platform* platform = v8::platform::CreateDefaultPlatform();
        v8::V8::InitializePlatform(platform);
        v8::V8::Initialize();
    }

    static Variant HHVM_METHOD(V8Js, executeString, const String& text)
    {
        
        v8::Isolate* isolate = v8::Isolate::New();
        v8::Isolate::Scope isolate_scope(isolate);

        // Create a stack-allocated handle scope.
        v8::HandleScope handle_scope(isolate);

        // Create a new context. JEB
        v8::Handle<v8::Context> context = v8::Context::New(isolate);

        // Enter the context for compiling and running the hello world script.
        v8::Context::Scope context_scope(context);

        // Create a string containing the JavaScript source code.
        v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, text.c_str());

        // Compile the source code.
        v8::Local<v8::Script> script = v8::Script::Compile(source);

        // Run it.
        v8::Local<v8::Value> result = script->Run();

        if (result->IsString())
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
    }

    void v8jsExtension::_initV8JsClass() {
        HHVM_ME(V8Js, __construct);
        HHVM_ME(V8Js, executeString);
    }
}
