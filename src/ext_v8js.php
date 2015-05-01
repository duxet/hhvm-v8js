<?hh

class V8Js
{
    <<__Native>>
    public function __construct(): void;

    <<__Native>>
    public function executeString(string $script): mixed;
}
