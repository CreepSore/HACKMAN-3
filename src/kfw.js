// @ts-nocheck
(function() {
    function Hackman3(){
        this.ips = 0;
        this.lastPrint = getTickCount();

        this.init = function() {
            log("Initialized", "JS");
        };

        this.loop = function() {
            var tc = getTickCount();
            if(tc - this.lastPrint > 1000) {
                log("Instructions per Second: " + this.ips, "JS");
                this.lastPrint = tc;
                this.ips = 0;
            }
            this.ips++;
        };
    }

    return new Hackman3();
})();