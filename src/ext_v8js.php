<?hh

class V8Js
{
    const integer FLAG_NONE = 1;
    const integer FLAG_FORCE_ARRAY = 2;

    <<__Native>>
    public function __construct(): void;

    <<__Native>>
    public function executeString(string $script): mixed;
}

class V8JsException extends Exception {
    /* Properties */
    protected $JsFileName;
    protected $JsLineNumber;
    protected $JsSourceLine;
    protected $JsTrace;
}
