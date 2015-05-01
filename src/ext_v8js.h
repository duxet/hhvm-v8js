#ifndef EXT_V8JS_H_
#define EXT_V8JS_H_

#include "hphp/runtime/ext/extension.h"

namespace HPHP
{
    class v8jsExtension: public Extension
    {
        public:
            v8jsExtension(): Extension("v8js"){}
            virtual void moduleInit()
            {
                _initV8JsClass();
                loadSystemlib();            
            }
        private:
            void _initV8JsClass();
    };
}
#endif
