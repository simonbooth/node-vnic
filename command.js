
var vnic = require("./index.js")(process.argv[2]);
console.log("Listening to " + process.argv[2]);
//console.log(vnic.uplink);

vnic.uplink.pipe(process.stdout);

process.stdin.readline();