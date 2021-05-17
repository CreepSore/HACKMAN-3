// @ts-nocheck
(function() {
    function Test(){
        this.a = 420;

        this.test = function() {
            var addr = util_findPattern("hitman3.exe", "\xf3\x44\x0f\x11\x87\x00\x00\x00\x00\x76", "xxxxx????x");
            if(addr === 0) {
                return null;
            }
            return readBytes(addr, 20);
        }
    }

    return new Test();
})();